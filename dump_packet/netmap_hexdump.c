#include <poll.h>
//#include <libutil.h>
#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>

struct nm_desc *nm_desc;

void 
hexdump(uint8_t *str, int len, void *arg, int times) {
	printf("---hexdump---");
	for (int i = 0; i < len; i++) {
		printf("%x ", str + i);
		if ((i + 1) % 8 == 0) {
			printf(" ");
				if ((i + 1) % 16 == 0) {
					printf("\n");
				}
		}
	}
	printf("---end hexdump---");
}

int
main(int argc, char *argv[])
{
	unsigned int cur, n, i, num, ret;
	struct netmap_ring *rxring;
	struct pollfd pollfd[1];
	struct netmap_slot *slot;
	char *rxbuf;

	nm_desc = nm_open("netmap:enp1s0f1", NULL, 0, NULL);
	if (!nm_desc)
		exit(1);

	pollfd[0].fd = nm_desc->fd;
	pollfd[0].events = POLLIN;
	for (;;) {
		ret = poll(pollfd, 1, 100);
		if (ret < 0) {
			fprintf(stderr, "poll error\n");
			exit(1);
		}
		else if (ret == 0) {
			continue;
		}

		//! netmap_ring addr
		rxring = NETMAP_RXRING(nm_desc->nifp, i);
		//! num of ring(packet?)
		num = nm_ring_space(rxring);
		cur = rxring->cur;
		
		while (n-- > 0) {
			slot = &rxring->slot[cur];
			printf("packet len: %d bytes\n", slot->len);

			cur = nm_ring_next(rxring, cur);
		}


		//for (i = nm_desc->first_rx_ring; i <= nm_desc->last_rx_ring; i++) {
		//	cur = rxring->cur;
		//	for (n = nm_ring_space(rxring); n > 0; n--, cur = nm_ring_next(rxring, cur)) {
		//		hexdump(NETMAP_BUF(rxring, rxring->slot[cur].buf_idx), rxring->slot[cur].len, NULL, 0);
		//	}
		//	rxring->head = rxring->cur = cur;
		//}

	}
}
