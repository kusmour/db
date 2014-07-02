PROGRAM = db
CC = g++
CFLAG = -g -Wall
CXXF = $(CC) $(CFLAG)

$(PROGRAM): main.o Load.o Retrieve.o externalSort.o Compress.o Join.o
	$(CXXF) bin/main.o bin/Load.o bin/Retrieve.o bin/externalSort.o bin/Compress.o bin/Join.o -o bin/$(PROGRAM)

main.o: src/main.cpp
	$(CXXF) -o bin/main.o -c src/main.cpp
Load.o:
	$(CXXF) -o bin/Load.o -c src/Load.cpp
Retrieve.o:
	$(CXXF) -o bin/Retrieve.o -c src/Retrieve.cpp
externalSort.o:
	$(CXXF) -o bin/externalSort.o -c src/externalSort.cpp
Compress.o:
	$(CXXF) -o bin/Compress.o -c src/Compress.cpp
Join.o:
	$(CXXF) -o bin/Join.o -c src/Join.cpp

clean:
	rm -f bin/*.o
	rm -f src/*.h.gch
	rm -f bin/db
