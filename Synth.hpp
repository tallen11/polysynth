#pragma once

#include "Oscillator.hpp"
#include "EnvelopeGenerator.hpp"
#include "VolumeModule.hpp"
#include <vector>

class Synth
{
public:
	Synth();
	~Synth();
	double getNextSample();
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