CC=g++
CFLAGS= -c -g -Wall -std=c++17 -fpermissive
EXENAME= main

default: build/main.o build/io.o build/taylorSeries.o build/math.o
	$(CC) build/main.o build/io.o build/taylorSeries.o build/math.o -o $(EXENAME)

#order-only-prerequisite for build dir
build/main.o: src/main.cpp include/io.h include/taylorSeries.h include/math.h | build
	$(CC) $(CFLAGS) src/main.cpp -o build/main.o

build/io.o: src/io.cpp include/io.h | build
	$(CC) $(CFLAGS) src/io.cpp -o build/io.o

build/math.o: src/math.cpp include/math.h include/taylorSeries.h | build
	$(CC) $(CFLAGS) src/math.cpp -o build/math.o

build/taylorSeries.o: src/taylorSeries.cpp include/taylorSeries.h include/math.h | build
	$(CC) $(CFLAGS) src/taylorSeries.cpp -o build/taylorSeries.o

build:
	mkdir -p $@

clean:
	rm build/*.o $(EXENAME)

run:
	./$(EXENAME)
