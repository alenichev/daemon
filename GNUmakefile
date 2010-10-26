CFLAGS+=        -Wall -Werror -Wstrict-prototypes
CFLAGS+=        -Wmissing-prototypes -Wmissing-declarations
CFLAGS+=        -Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+=        -Wsign-compare

all:		daemon

daemon:		daemon.c
	$(CC) $(CFLAGS) -o daemon daemon.c

clean cleandir:
	rm -f *.o daemon *.core core
