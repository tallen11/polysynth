#pragma once

#include "WaveTables/WaveTable.hpp"
#include "Parameter.hpp"
#include "Util.hpp"
#include "Constants.hpp"

// #define TABLE_COUNT 32
#define OSCILLATOR_DESIRED_BASE_FREQUENCY 440.0

class Oscillator
{
public:
	Oscillator();
	~Oscillator();
	double getNextSample();

	Parameter* getFrequencyParameter();

private:
	WaveTable _table;
	double _currentTableIndex;

	Parameter *frequencyParameter;
	Parameter *phaseParameter;
};

inline double convertToFrequencyValue(double frequency)
{
	return convertRanges(frequency, MIN_FREQUENCY, MAX_FREQUENCY, 0.0, 1.0);
}
