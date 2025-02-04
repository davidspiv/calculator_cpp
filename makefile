CC=g++
CFLAGS= -c -g -Wall -std=c++17 -fpermissive
EXENAME= main

default: build/main.o build/io.o
	$(CC) build/main.o build/io.o -o $(EXENAME)

#order-only-prerequisite for build dir
build/main.o: src/main.cpp include/io.h | build
	$(CC) $(CFLAGS) src/main.cpp -o build/main.o

build/io.o: src/io.cpp include/io.h | build
	$(CC) $(CFLAGS) src/io.cpp -o build/io.o

build:
	mkdir -p $@

clean:
	rm build/*.o $(EXENAME)

run:
	./$(EXENAME)
