#pragma once

#include "Oscillator.hpp"
#include "EnvelopeGenerator.hpp"
#include <vector>

class Synth
{
public:
	Synth();
	~Synth();
	double getNextSample();
	void setMasterVolume(double volume);
	void keyPressed(int midiKey);
	void keyReleased(int midiKey);

private:
	std::vector<Oscillator*> oscillators;
	std::vector<EnvelopeGenerator*> envelopes;
};