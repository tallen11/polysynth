#pragma once

#include "Oscillator.hpp"
#include "EnvelopeGenerator.hpp"
#include "VolumeModule.hpp"
#include "Filter.hpp"
#include "Effects/EffectsLoop.hpp"
#include "WaveTables/WaveTable.hpp"
#include "LFO.hpp"
#include <vector>

struct OscillatorGroup
{
	int midiKey = -1;
	std::vector<Oscillator*> oscillators;
	VolumeModule volumeModule;
	Filter filter;
	std::vector<double> sampleBuffer;

	OscillatorGroup() : sampleBuffer(BUFFER_SIZE, 0.0)
	{
		volumeModule.getVolumeParameter()->setInterpolates(false);
	}

	double getNextSample()
	{
		double sample = 0.0;
        int size = 0;
		for (auto oscillator : oscillators) {
            if (oscillator->isEnabled()) {
                sample += oscillator->getNextSample();
                size++;
            }
		}

		sample /= static_cast<double>(size);

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
    
    /* Modify the Synth */
    void setMainFilterPitchCutoff(double cutoff);
    void setMainFilterResonance(double resonance);
    
    void setVolumeLFOFrequency(double frequency);
    void setVolumeLFOAmplitude(double amplitude);
    void setFilterLFOFrequency(double frequency);
    void setFilterLFOAmplitude(double amplitude);
    
    void setNoteAttackDuration(double duration);
    void setNoteDecayDuration(double duration);
    void setNoteSustainLevel(double level);
    void setNoteReleaseDuration(double duration);
    
    void toggleOscillator2();
    
    void setOscillatorWavetable(int oscillatorID, int wavetableID, int wavetable);
    void setTableFadePercentage(int oscillatorID, double percentage);

private:
	OscillatorGroup* getNextOscillatorGroup();

	std::vector<OscillatorGroup*> oscillatorGroups;
	std::vector<EnvelopeGenerator*> envelopes;
	std::vector<double> sampleBuffer;
	EffectsLoop effectsLoop;
	VolumeModule masterVolumeModule;
	LFO *volumeLFO;
	int oscillatorGroupsIndex;
    WaveTable *sawtoothWavetable;
    WaveTable *squareWavetable;
    WaveTable *sineWavetable;
    
//	WaveTable *leftWaveTable;
//	WaveTable *rightWaveTable;
};