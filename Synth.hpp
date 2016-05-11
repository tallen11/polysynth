#pragma once

#include "Oscillator.hpp"
#include "EnvelopeGenerator.hpp"
#include "VolumeModule.hpp"
#include "Filter.hpp"
#include "Effects/EffectsLoop.hpp"
#include <vector>

struct OscillatorGroup
{
	int midiKey = -1;
	std::vector<Oscillator*> oscillators;
	VolumeModule volumeModule;
	Filter filter;
	std::vector<double> sampleBuffer;

	OscillatorGroup() : sampleBuffer(BUFFER_SIZE, 0.0)
	{ }

	double getNextSample()
	{
		double sample = 0.0;
		for (auto oscillator : oscillators) {
			sample += oscillator->getNextSample();
		}

		sample /= static_cast<double>(oscillators.size());

		return sample; // volumeModule.processSample(sample);
	}

	std::vector<double>& getNextBuffer(int bufferLength)
	{
		for (int i = 0; i < bufferLength; ++i) {
			sampleBuffer[i] = getNextSample();
		}

		filter.processBuffer(sampleBuffer, bufferLength);

		for (int i = 0; i < bufferLength; ++i) {
			sampleBuffer[i] = volumeModule.processSample(sampleBuffer[i]);
		}

		return sampleBuffer;
	}
};

class Synth
{
public:
	Synth();
	~Synth();
	std::vector<double>& getNextBuffer(int bufferLength);
	void setMasterVolume(double volume);
	void keyPressed(int midiKey, double velocity);
	void keyReleased(int midiKey);
	void pitchBend(double amount);

private:
	OscillatorGroup* getNextOscillatorGroup();

	std::vector<OscillatorGroup*> oscillatorGroups;
	int oscillatorGroupsIndex;
	std::vector<EnvelopeGenerator*> envelopes;
	std::vector<double> sampleBuffer;
	EffectsLoop effectsLoop;
	VolumeModule masterVolumeModule;
};