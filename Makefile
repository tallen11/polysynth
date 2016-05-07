CXX=clang++
DEBUG=-g
LFLAGS=-lportaudio -framework CoreFoundation -framework CoreAudio -framework CoreMidi
CFLAGS=-Wall -pedantic -Wunused-parameter -Wold-style-cast -Wunreachable-code -std=c++14 $(DEBUG)
SRC=Parameter.o WaveTable.o Oscillator.o EnvelopeGenerator.o Synth.o RtMidi.o main.o

all: $(SRC)
	$(CXX) $(SRC) $(LFLAGS) -o bin/synth

Parameter.o: Parameter.cpp Parameter.hpp
	$(CXX) Parameter.cpp $(CFLAGS) -c

WaveTable.o: WaveTables/WaveTable.cpp WaveTables/WaveTable.hpp
	$(CXX) WaveTables/WaveTable.cpp $(CFLAGS) -c

Oscillator.o: Oscillator.cpp Oscillator.hpp
	$(CXX) Oscillator.cpp $(CFLAGS) -c

EnvelopeGenerator.o: EnvelopeGenerator.cpp EnvelopeGenerator.hpp
	$(CXX) EnvelopeGenerator.cpp $(CFLAGS) -c

Synth.o: Synth.cpp Synth.hpp
	$(CXX) Synth.cpp $(CFLAGS) -c

RtMidi.o: Libraries/RtMidi/RtMidi.cpp Libraries/RtMidi/RtMidi.h
	$(CXX) Libraries/RtMidi/RtMidi.cpp $(CFLAGS) -c

main.o: main.cpp Constants.hpp Util.hpp Notes.hpp
	$(CXX) main.cpp $(CFLAGS) -c

clean:
	rm -rf bin/synth
	rm -rf *.o
	rm -rf *.dSYM
