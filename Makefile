CC=gcc
CFLAGS=-Wall -I.

all: uts_clone uts_unshare uts_setns pidns_clone pidns_unshare \
	pidns_unshare_fork pidns_init orphan pidns_setns
	
uts_clone: uts_clone.o
	$(CC) -o uts_clone uts_clone.o $(CFLAGS)

uts_unshare: uts_unshare.o
	$(CC) -o uts_unshare uts_unshare.o $(CFLAGS)

uts_setns: uts_setns.o
	$(CC) -o uts_setns uts_setns.o $(CFLAGS)

pidns_clone: pidns_clone.o
	$(CC) -o pidns_clone pidns_clone.o $(CFLAGS)

pidns_unshare: pidns_unshare.o
	$(CC) -o pidns_unshare pidns_unshare.o $(CFLAGS)

pidns_unshare_clone: pidns_unshare_fork.o
	$(CC) -o pidns_unshare_fork pidns_unshare_fork.o $(FLAGS)

pidns_init: pidns_init.o 
	$(CC) -o pidns_init pidns_init.o $(FLAGS)

common.o: common.c
	$(CC) -c common.c $(FLAGS)

orphan: orphan.o
	$(CC) -o orphan orphan.o $(FLAGS)

clean:
	rm -f uts_clone uts_unshare uts_setns pidns_clone pidns_unshare \
		uts_clone.o uts_unshare.o uts_setns.o pidns_clone.o pidns_unshare.o \
		pidns_unshare_fork pidns_unshare_fork.o pidns_init pidns_init.o common.o \
		orphan.o orphan pidns_setns.o pidns_setns
