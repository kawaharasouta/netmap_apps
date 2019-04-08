CC = gcc

OBJS = netmap_hexdump.o
SRCS = $(OBJS:.o=%.c)
CFLAGS = -g
CFLAGS += -I $(NETMAP_DIR)/sys
LDLIBS = -lutil
TARGET = netmap_hexdump

#MK_MAN = no
#
#.include <bsd.prog.mk>

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS) 
