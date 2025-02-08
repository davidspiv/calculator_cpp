CC=g++
CFLAGS= -c -g -Wall -std=c++17 -fpermissive
EXENAME= main

default: build/main.o build/io.o build/taylorSeries.o build/_math.o build/helpers.o
	$(CC) build/main.o build/io.o build/taylorSeries.o build/_math.o build/helpers.o -o $(EXENAME)

#order-only-prerequisite for build dir
build/main.o: src/main.cpp include/io.h include/taylorSeries.h include/token.h include/_math.h include/helpers.h include/operators.h
	$(CC) $(CFLAGS) src/main.cpp -o build/main.o

build/io.o: src/io.cpp include/io.h | build
	$(CC) $(CFLAGS) src/io.cpp -o build/io.o

build/_math.o: src/_math.cpp include/_math.h | build
	$(CC) $(CFLAGS) src/_math.cpp -o build/_math.o

build/taylorSeries.o: src/taylorSeries.cpp include/taylorSeries.h | build
	$(CC) $(CFLAGS) src/taylorSeries.cpp -o build/taylorSeries.o

build/helpers.o: src/helpers.cpp include/helpers.h | build
	$(CC) $(CFLAGS) src/helpers.cpp -o build/helpers.o

build:
	mkdir -p $@

clean:
	rm build/*.o $(EXENAME)

run:
	./$(EXENAME)
