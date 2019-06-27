all: scanner

scanner: p1.o scanner.o testScanner.o
	g++ -g -std=c++11 p1.o scanner.o testScanner.o -o scanner
	
p1.o: p1.cpp testScanner.h
	g++ -g -std=c++11 -c p1.cpp -o p1.o
	
scanner.o: scanner.c sc.h
	g++ -g -std=c++11 -c scheduler2.c -lpthread -lrt -o scheduler2.o

help.o: help.c help.h
	g++ -g -std=c++11 -c help.c -o help.o
	
clean:
	rm *.o
