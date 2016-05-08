#pragma once

#include "Oscillator.hpp"
#include "EnvelopeGenerator.hpp"
#include "VolumeModule.hpp"
#include <vector>

struct OscillatorGroup
{
	int midiKey = -1;
	std::vector<Oscillator*> oscillators;
	VolumeModule volumeModule;
	EnvelopeGenerator *volumeEnvelope;
	// Filter
	// Filter envelope
	// FX Somehow???

	double getNextSample()
	{
		double sample = 0.0;
		for (auto oscillator : oscillators) {
			sample += oscillator->getNextSample();
		}

		sample /= static_cast<double>(oscillators.size());

		return volumeModule.processSample(sample);
	}
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
	OscillatorGroup* getNextOscillatorGroup();

	std::vector<OscillatorGroup*> oscillatorGroups;
	int oscillatorGroupsIndex;
	std::vector<EnvelopeGenerator*> envelopes;
	
	std::vector<double> sampleBuffer;
	
	VolumeModule masterVolumeModule;
};