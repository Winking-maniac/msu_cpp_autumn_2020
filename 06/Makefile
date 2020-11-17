COMP = g++
CFLAGS = -std=c++17 -Wall 
OBJECTS = format.o

all: test

test: test.out
	./test.out 

test.out: $(OBJECTS)
	$(COMP) $(CFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(COMP) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o
	rm -rf *.out
