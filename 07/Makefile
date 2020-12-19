COMP = g++
CFLAGS = -std=c++17 -Wall 
OBJECTS = test.o

all: test

test: test.out
	./test.out 

test.out: $(OBJECTS)
	$(COMP) $(CFLAGS) $(OBJECTS) -o $@

%.o: %.cpp my_vector.h
	$(COMP) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o
	rm -rf *.out
