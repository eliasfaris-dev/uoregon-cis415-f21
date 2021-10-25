all: iobound

iobound: main.o iobound.o
	gcc -o iobound main.o iobound.o

main.o: main.c
	gcc -c main.c

iobound.o: iobound.c
	gcc -c iobound.c

clean:
	rm -f core *.o iobound