#pragma once

#include "Oscillator.hpp"
#include "EnvelopeGenerator.hpp"
#include "VolumeModule.hpp"
#include <vector>

struct OscillatorGroup
{
	std::vector<Oscillator*> oscillators
};

class Synth
{
public:
	Synth();
	~Synth();
	std::vector<double>& getNextBuffer(int bufferLength);
	void setMasterVolume(double volume);
	void keyPressed(int midiKey);
	void keyReleased(int midiKey);
	void pitchBend(double amount);

private:
	std::vector<Oscillator*> oscillators;
	std::vector<EnvelopeGenerator*> envelopes;
	
	std::vector<double> sampleBuffer;
	
	VolumeModule volumeModule;
};