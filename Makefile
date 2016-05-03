CXX=clang++
DEBUG=-g
LFLAGS=-lportaudio
CFLAGS=-Wall -pedantic -std=c++14
SRC=main.o

all: $(SRC)
	$(CXX) $(SRC) $(LFLAGS) -o bin/synth

main.o: main.cpp
	$(CXX) main.cpp $(CFLAGS) -c

clean:
	rm -rf bin/synth
	rm -rf *.o
	rm -rf *.dSYM
