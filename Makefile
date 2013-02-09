CC=gcc
CFLAGS=-std=c99

all: bench-lookup-perf

rtbstorage.o: rtbstorage.h rtbstorage.c
	${CC} ${CFLAGS} -c rtbstorage.c

rtbbench.o: rtbstorage.o rtbbench.h rtbbench.c
	${CC} ${CFLAGS} -c rtbbench.c

bench-lookup-perf: rtbstorage.o rtbbench.o bench-lookup-perf.c
	${CC} ${CFLAGS} rtbstorage.o rtbbench.o bench-lookup-perf.c -o bench-lookup-perf

clean:
	rm rtbstorage.o rtbbench.o bench-lookup-perf
