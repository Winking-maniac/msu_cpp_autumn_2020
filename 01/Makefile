COMP = g++
CFLAGS = -std=c++11 -Wall 
OBJECTS = allocator.o test.o

all: test

test: test.out
	./test.out

test.out: $(OBJECTS)
	$(COMP) $(CFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(COMP) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o
