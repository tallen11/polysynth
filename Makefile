CXX=clang++
DEBUG=-g
LFLAGS=-lportaudio -lncurses
CFLAGS=-Wall -pedantic -std=c++14
SRC=WaveTable.o Oscillator.o main.o

all: $(SRC)
	$(CXX) $(SRC) $(LFLAGS) -o bin/synth

WaveTable.o: WaveTables/WaveTable.cpp WaveTables/WaveTable.hpp
	$(CXX) WaveTables/WaveTable.cpp $(CFLAGS) -c

Oscillator.o: Oscillator.cpp Oscillator.hpp
	$(CXX) Oscillator.cpp $(CFLAGS) -c

main.o: main.cpp Constants.hpp Util.hpp
	$(CXX) main.cpp $(CFLAGS) -c

clean:
	rm -rf bin/synth
	rm -rf *.o
	rm -rf *.dSYM
