#pragma once

#include "WaveTables/WaveTable.hpp"

#define TABLE_COUNT 32
#define MAX_FREQUENCY 20000.0f
#define MIN_FREQUENCY 20.0f
#define OSCILLATOR_BASE_FREQUENCY 441.0f
#define OSCILLATOR_DESIRED_BASE_FREQUENCY 440.0f

class Oscillator
{
public:
	Oscillator();
	~Oscillator();
	float getNextSample();
	void setFrequencyValue(float value);
	float getFrequencyValue();

private:
	float _frequencyValue;
	// float phaseValue;
	WaveTable **_tables;
	float _currentTableIndex;
	float _tableIndexIncrement;
};

inline float convertToFrequencyValue(float frequency)
{
	return (((frequency - MIN_FREQUENCY) * (1.0f - 0.0f)) / (MAX_FREQUENCY - MIN_FREQUENCY)) + 0.0f;
}
