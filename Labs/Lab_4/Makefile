all: main iobound

iobound: iobound.c
	gcc -o iobound iobound.c

main: main.c
	gcc -o main main.c

clean:
	rm -f core *.o iobound main