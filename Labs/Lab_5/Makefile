all: part1 iobound

iobound: iobound.c
	gcc -o iobound iobound.c

cpubound: cpubound.c
	gcc -o cpubound cpubound.c

part1: part1.c
	gcc -o part1 part1.c

clean:
	rm -f core *.o iobound cpubound part1