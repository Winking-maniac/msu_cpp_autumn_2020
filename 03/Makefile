COMP = g++
CFLAGS = -std=c++17 -Wall 
OBJECTS = array.o test.o

all: test

test: test.out
	./test.out silent

test.out: $(OBJECTS)
	$(COMP) $(CFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(COMP) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o
