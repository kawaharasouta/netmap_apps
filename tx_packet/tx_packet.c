#include<stdio.h>
#include<stdlib.h>
#include<sys/poll.h>
#define NETMAP_WITH_LIBS
#include<net/netmap_user.h>

#include<netinet/if_ether.h>
#include<netinet/ip.h>
#include<netinet/udp.h>

struct nm_desc *nm_desc;

static unsigned int make_packet(char *pkt_buf, char *payload, unsigned int payload_len)
{
	struct ether_header *eh;
	struct ip *ip;
	struct udphdr *udp;
	char *udp_payload;

	/* Destination MAC address */
	uint8_t dst_mac[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	/* Source MAC address */
	uint8_t src_mac[6] = { 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

	/* Destination IP address */
	uint32_t dst_ip = (10 << 24) + (0 << 16) + (0 << 8) + (1 << 0) ;
	/* Source IP address */
	uint32_t src_ip = (10 << 24) + (0 << 16) + (0 << 8) + (2 << 0) ;

	/* Ethernet header */
	eh = (struct ether_header *) pkt_buf;
	eh->ether_type = htons(ETHERTYPE_IP);
	memcpy(eh->ether_dhost, dst_mac, 6);
	memcpy(eh->ether_shost, src_mac, 6);

	/* IP header */
	ip = (struct ip *) ((unsigned long) eh + sizeof(*eh));
	ip->ip_v = IPVERSION;
	ip->ip_hl = sizeof(*ip) >> 2;
	ip->ip_id = 0;
	ip->ip_tos = IPTOS_LOWDELAY;
	ip->ip_len = htons(payload_len + sizeof(*udp) + sizeof(*ip));
	ip->ip_id = 0;
	ip->ip_off = htons(IP_DF); /* Don't fragment */
	ip->ip_ttl = IPDEFTTL;
	ip->ip_p = IPPROTO_UDP;
	ip->ip_dst.s_addr = htonl(dst_ip);
	ip->ip_src.s_addr = htonl(src_ip);
	//ip->ip_sum = wrapsum(checksum(&ip, sizeof(*ip), 0));
	ip->ip_sum = 0;

	/* UDP header */
	udp = (struct udphdr *) ((unsigned long) ip + sizeof(*ip));

	/* Copy payload to the packet buffer */
	udp_payload = (char *) ((unsigned long) udp + sizeof(*udp));
	memcpy(udp_payload, payload, payload_len);

	/* UDP header setup */
	udp->uh_sport = htons(10000);
	udp->uh_dport = htons(10001);
	udp->uh_ulen = htons(payload_len);
	//udp->uh_sum = wrapsum(
	//    checksum(&udp, sizeof(*udp),
  //          checksum(udp_payload, payload_len,
	//    checksum(&ip->ip_src, 2 * sizeof(ip->ip_src),
	//	IPPROTO_UDP + (u_int32_t)ntohs(udp->uh_ulen)))));
	udp->uh_sum = 0;

	return payload_len + sizeof(*udp) + sizeof(*ip) + sizeof(*eh);
}

int
main(int argc, char *argv[])
{
	unsigned int cur, i, num;
	struct netmap_ring *txring;
	struct netmap_slot *slot;
	char *txbuf;

	nm_desc = nm_open("netmap:enp1s0f1", NULL, 0, NULL);
	if (!nm_desc)
		exit(1);

	printf("translate packet using netmap.\n");

	txring = NETMAP_TXRING(nm_desc->nifp, 0);
	cur = txring->cur;

	slot = &txring->slot[cur];
	txbuf = NETMAP_BUF(txring, slot->buf_idx);

	uint8_t payload[128];
	snprintf(payload, sizeof(payload), "tx packet using netmap.\n");

	unsigned int len = make_packet(txbuf, payload, strlen(payload));


	printf("sending packet: %d bytes.\n", len);
	//printf("send 3 packets.\n")
	slot->len = len;

	cur = nm_ring_next(txring, cur);
	txring->head = txring->cur = cur;

	ioctl(nm_desc->fd, NIOCTXSYNC, NULL);
}
