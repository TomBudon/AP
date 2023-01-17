CC=gcc

CFLAGS=-g3

OFLAGS=-march=native -O3

VFLAGS=-DBASELINE=1

all: sobel

sobel: sobel.c
	$(CC) $(VFLAGS) $(CFLAGS) $(OFLAGS) $< kernel.c common.c -o $@ -lm

clean:
	rm -Rf *~ sobel
