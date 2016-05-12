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
	// Oscillator();
	Oscillator(WaveTable *leftTable, WaveTable *rightTable);
	~Oscillator();
	double getNextSample();

	Parameter* getFrequencyParameter();
	Parameter* getTableParameter();

private:
	WaveTable *_table;
	WaveTable *_table2;
	double _currentTableIndex;

	Parameter *frequencyParameter;
	Parameter *phaseParameter;
	Parameter *tableParameter;
};

inline double convertToFrequencyValue(double frequency)
{
	return convertRanges(frequency, MIN_FREQUENCY, MAX_FREQUENCY, 0.0, 1.0);
}
