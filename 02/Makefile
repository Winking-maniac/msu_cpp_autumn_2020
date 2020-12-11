COMP = g++
CFLAGS = -std=c++17 -Wall
LFLAGS = 

all: test

test: test.out
	./test.out

test.out: test.o parser.o
	$(COMP) $(CFLAGS) $(LFLAGS) $^ -o $@

%.o: %.cpp
	$(COMP) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o
	rm -rf *.out
