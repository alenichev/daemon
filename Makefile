# $Surreal: Makefile,v 1.2 2009/08/26 07:45:27 mitya Exp $

PROG=		daemon
MAN=		daemon.8

CFLAGS+=	-Wall -Werror -Wstrict-prototypes
CFLAGS+=	-Wmissing-prototypes -Wmissing-declarations
CFLAGS+=	-Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+=	-Wsign-compare
CLEANFILES+=	daemon.cat8

BINDIR?=	/usr/local/sbin

.include <bsd.prog.mk>
