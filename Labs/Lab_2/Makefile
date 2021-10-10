all : lab2.exe


lab2.exe: main.o 
	gcc -o lab2.exe main.o


main.o : main.c
	gcc -c main.o

clean:
	rm -f core *.o lab2.exe