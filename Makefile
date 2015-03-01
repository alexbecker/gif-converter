CC=gcc
CFLAGS=-std=gnu99 -O3
LDFLAGS=-lm

all: encoding.c encoding.h giffer.c degiffer.c
all:
	mkdir -p bin
	$(CC) $(CFLAGS) giffer.c encoding.c -o bin/giffer $(LDFLAGS)
	$(CC) $(CFLAGS) degiffer.c encoding.c -o bin/degiffer $(LDFLAGS)

test: CFLAGS+=-DTEST_ENCODING
test: encoding.c encoding.h
test:
	mkdir -p bin
	$(CC) $(CFLAGS) encoding.c -o bin/encoding
	bin/encoding

.PHONY: clean
clean:
	rm -r bin
