#pragma once

#include "WaveTables/WaveTable.hpp"
#include "Parameter.hpp"
#include "Util.hpp"
#include "Constants.hpp"
#include <vector>

// #define TABLE_COUNT 32
#define OSCILLATOR_DESIRED_BASE_FREQUENCY 440.0

class Oscillator
{
public:
	Oscillator(WaveTable *leftTable, WaveTable *rightTable);
	~Oscillator();
	double getNextSample();
	void resetOscillator();
    void toggleOscillator();
    bool isEnabled();
    void setLeftWavetable(WaveTable *wavtable);
    void setRightWavetable(WaveTable *wavetable);

	Parameter* getFrequencyParameter();
	Parameter* getPhaseParameter();
	Parameter* getTableParameter();

	void setVoiceCount(int count);
	Parameter* getVoiceDetuneFactorParameter();

private:
	WaveTable *_table;
	WaveTable *_table2;
	double _currentTableIndex;

	Parameter *frequencyParameter;
	Parameter *phaseParameter;
	Parameter *tableParameter;

	int voiceCount;
	Parameter *voiceDetuneFactorParameter;
	std::vector<double> voiceIndices;
    
    bool enabled;
};

inline double convertToFrequencyValue(double frequency)
{
	return convertRanges(frequency, MIN_FREQUENCY, MAX_FREQUENCY, 0.0, 1.0);
}
