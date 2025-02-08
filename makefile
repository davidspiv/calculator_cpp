CC=g++
CFLAGS= -c -g -Wall -std=c++17 -fpermissive
EXENAME= main

default: build/main.o build/io.o build/taylorSeries.o build/_math.o build/lexer.o build/shuntingYard.o build/evalRpnNotation.o
	$(CC) build/main.o build/io.o build/taylorSeries.o build/_math.o build/lexer.o build/shuntingYard.o build/evalRpnNotation.o -o $(EXENAME)

#order-only-prerequisite for build dir
build/main.o: src/main.cpp include/io.h include/taylorSeries.h include/token.h include/_math.h include/lexer.h include/shuntingYard.h include/evalRpnNotation.h
	$(CC) $(CFLAGS) src/main.cpp -o build/main.o

build/io.o: src/io.cpp include/io.h | build
	$(CC) $(CFLAGS) src/io.cpp -o build/io.o

build/_math.o: src/_math.cpp include/_math.h | build
	$(CC) $(CFLAGS) src/_math.cpp -o build/_math.o

build/taylorSeries.o: src/taylorSeries.cpp include/taylorSeries.h | build
	$(CC) $(CFLAGS) src/taylorSeries.cpp -o build/taylorSeries.o

build/lexer.o: src/lexer.cpp include/lexer.h | build
	$(CC) $(CFLAGS) src/lexer.cpp -o build/lexer.o

build/shuntingYard.o: src/shuntingYard.cpp include/shuntingYard.h | build
	$(CC) $(CFLAGS) src/shuntingYard.cpp -o build/shuntingYard.o

build/evalRpnNotation.o: src/evalRpnNotation.cpp include/evalRpnNotation.h | build
	$(CC) $(CFLAGS) src/evalRpnNotation.cpp -o build/evalRpnNotation.o

build:
	mkdir -p $@

clean:
	rm build/*.o $(EXENAME)

run:
	./$(EXENAME)
