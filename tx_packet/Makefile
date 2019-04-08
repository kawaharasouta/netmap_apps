CC = gcc

OBJS = tx_packet.o
SRCS = $(OBJS:.o=%.c)
CFLAGS = -g
CFLAGS += -I $(NETMAP_DIR)/sys
LDLIBS = 
TARGET = tx_packet

#MK_MAN = no
#
#.include <bsd.prog.mk>

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS) 
