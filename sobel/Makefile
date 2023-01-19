CC=gcc

CFLAGS=-g3

OFLAGS=-O1

OPFLAGS=-march=native -ftree-vectorize

VFLAGS=-DBASELINE=1

all: sobel

sobel: sobel.c
	$(CC) $(VFLAGS) $(CFLAGS) $(OFLAGS) $(OPFLAGS) $< kernel.c common.c -o $@ -lm

clean:
	rm -Rf *~ sobel
