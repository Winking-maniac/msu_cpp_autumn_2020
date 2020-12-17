COMP = g++
CFLAGS = -std=c++17 -Wall
LFLAGS = -pthread
OBJECTS = test.o


all: test

test: test.out
	./test.out 

test.out: $(OBJECTS)
	$(COMP) $(CFLAGS) $(LFLAGS) $(OBJECTS) -o $@

%.o: %.cpp pool.h
	$(COMP) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o
	rm -rf *.out
