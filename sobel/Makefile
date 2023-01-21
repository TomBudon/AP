CC=gcc

CFLAGS=-g3

OFLAGS=-O1

OPFLAGS=-march=native -finline-functions -funroll-loops

VFLAGS=-DBASELINE=1

all: sobel

sobel: sobel.c
ifeq ($(CC),icc)
	$(CC) $(VFLAGS) $(CFLAGS) $(OFLAGS) $(OPFLAGS) -vec $< kernel.c common.c -o $@ -lm -qopenmp -diag-disable=10441
else
ifeq ($(CC),icx)
	$(CC) $(VFLAGS) $(CFLAGS) $(OFLAGS) $(OPFLAGS) -vec $< kernel.c common.c -o $@ -lm -qopenmp
else
ifeq ($(CC),gcc)
	$(CC) $(VFLAGS) $(CFLAGS) $(OFLAGS) $(OPFLAGS) -ftree-vectorize $< kernel.c common.c -o $@ -lm -fopenmp
else
ifeq ($(CC),clang-14)
	$(CC) $(VFLAGS) $(CFLAGS) $(OFLAGS) $(OPFLAGS) -ftree-vectorize $< kernel.c common.c -o $@ -lm
else
	@echo "ERROR: no compiler specified using CC. Possible values for CC: gcc, icc, icx, clang-14"
endif
endif
endif
endif


clean:
	rm -Rf *~ sobel
