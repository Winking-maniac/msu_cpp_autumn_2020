COMP = g++
CFLAGS = -std=c++11 -Wall
LFLAGS = -lparser
OBJECTS =

all: test

test: test.out
	./test.out

test.out: /usr/lib/libparser.so test.o
	$(COMP) test.o $(CFLAGS) $(LFLAGS) -o $@

test.o: test.cpp
	$(COMP) $(CFLAGS) -c test.cpp -o $@

/usr/lib/libparser.so: parser.cpp
	$(COMP) $(CFLAGS) -fPIC parser.cpp -c -o parser.o
	$(COMP) -shared parser.o -o /usr/lib/libparser.so
	ldconfig
clean:
	rm -rf *.o
