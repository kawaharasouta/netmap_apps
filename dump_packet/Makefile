CC = gcc

OBJS = dump_packet.o
SRCS = $(OBJS:.o=%.c)
CFLAGS = -g
CFLAGS += -I $(NETMAP_DIR)/sys
LDLIBS = 
TARGET = netmap_hexdump

#MK_MAN = no
#
#.include <bsd.prog.mk>

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS) 
