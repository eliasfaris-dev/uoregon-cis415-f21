all: part1 part2 iobound cpubound

iobound: iobound.c
	gcc -o iobound iobound.c

cpubound: cpubound.c
	gcc -o cpubound cpubound.c

part1: part1.c
	gcc -o part1 part1.c

part2: part2.c
	gcc -o part2 part2.c

clean:
	rm -f core *.o iobound cpubound part1