CXX=clang++
DEBUG=-g
LFLAGS=-lportaudio -framework CoreFoundation -framework CoreAudio -framework CoreMidi
CFLAGS=-Wall -pedantic -Wunused-parameter -Wunreachable-code -std=c++14 $(DEBUG)
SRC=Parameter.o WaveTable.o Oscillator.o Filter.o EnvelopeGenerator.o VolumeModule.o Effect.o EffectsLoop.o EffectOverdrive.o EffectBitcrusher.o EffectDelay.o Synth.o RtMidi.o main.o

all: $(SRC)
	$(CXX) $(SRC) $(LFLAGS) -o bin/synth

Parameter.o: Parameter.cpp Parameter.hpp
	$(CXX) Parameter.cpp $(CFLAGS) -c

WaveTable.o: WaveTables/WaveTable.cpp WaveTables/WaveTable.hpp
	$(CXX) WaveTables/WaveTable.cpp $(CFLAGS) -c

Oscillator.o: Oscillator.cpp Oscillator.hpp
	$(CXX) Oscillator.cpp $(CFLAGS) -c

Filter.o: Filter.cpp Filter.hpp
	$(CXX) Filter.cpp $(CFLAGS) -c

EnvelopeGenerator.o: EnvelopeGenerator.cpp EnvelopeGenerator.hpp
	$(CXX) EnvelopeGenerator.cpp $(CFLAGS) -c

VolumeModule.o: VolumeModule.cpp VolumeModule.hpp
	$(CXX) VolumeModule.cpp $(CFLAGS) -c

Effect.o: Effects/Effect.cpp Effects/Effect.hpp
	$(CXX) Effects/Effect.cpp $(CFLAGS) -c

EffectsLoop.o: Effects/EffectsLoop.cpp Effects/EffectsLoop.hpp
	$(CXX) Effects/EffectsLoop.cpp $(CFLAGS) -c

EffectOverdrive.o: Effects/EffectOverdrive.cpp Effects/EffectOverdrive.hpp
	$(CXX) Effects/EffectOverdrive.cpp $(CFLAGS) -c

EffectBitcrusher.o: Effects/EffectBitcrusher.cpp Effects/EffectBitcrusher.hpp
	$(CXX) Effects/EffectBitcrusher.cpp $(CFLAGS) -c

EffectDelay.o: Effects/EffectDelay.cpp Effects/EffectDelay.hpp
	$(CXX) Effects/EffectDelay.cpp $(CFLAGS) -c

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
