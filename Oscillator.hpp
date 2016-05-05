#pragma once

#include "WaveTables/WaveTable.hpp"

#define TABLE_COUNT 32
#define MAX_FREQUENCY 20000.0
#define MIN_FREQUENCY 20.0
#define OSCILLATOR_BASE_FREQUENCY 441.0
#define OSCILLATOR_DESIRED_BASE_FREQUENCY 440.0

class Oscillator
{
public:
	Oscillator();
	~Oscillator();
	double getNextSample();
	void setFrequencyValue(double value);
	double getFrequencyValue();

private:
	double _frequencyValue;
	// float phaseValue;
	WaveTable **_tables;
	double _currentTableIndex;
	double _tableIndexIncrement;
};

inline double convertToFrequencyValue(float frequency)
{
	return (((frequency - MIN_FREQUENCY) * (1.0 - 0.0)) / (MAX_FREQUENCY - MIN_FREQUENCY)) + 0.0;
}
