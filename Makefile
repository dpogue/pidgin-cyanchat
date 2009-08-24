#Customisable stuff here
LINUX32_COMPILER = gcc

LIBPURPLE_CFLAGS = -I/usr/include/libpurple -DPURPLE_PLUGINS -DENABLE_NLS
GLIB_CFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/lib64/glib-2.0/include -I/usr/include

LIBCYANCHAT_SOURCES =	libcyanchat.c \
						cyanchat.c \
						cc_account.c \
						cc_callback.c \
						cc_member.c \
						cc_parser.c \
						cc_utils.c

#By default, 'make' compiles X11 version on local platform
all: ${LIBCYANCHAT_SOURCES}
	gcc ${LIBPURPLE_CFLAGS} -Wall -pthread ${GLIB_CFLAGS} -I. -g -pipe ${LIBCYANCHAT_SOURCES} -o libcyanchat.so -shared -fPIC -DPIC

install:
	cp libcyanchat.so /usr/lib/purple-2/
