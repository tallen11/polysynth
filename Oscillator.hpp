#pragma once

#include "WaveTables/WaveTable.hpp"
#include "Parameter.hpp"
#include "Util.hpp"

// #define TABLE_COUNT 32
#define MAX_FREQUENCY 20000.0
#define MIN_FREQUENCY 20.0
#define OSCILLATOR_DESIRED_BASE_FREQUENCY 440.0

class Oscillator
{
public:
	Oscillator();
	~Oscillator();
	double getNextSample();
	void setFrequencyValue(double value);
	double getFrequencyValue() const;

	Parameter* getVolumeParameter();
	Parameter* getVolumeEnvelopeParameter();

private:
	double _frequencyValue;
	// double phaseValue;
	// WaveTable **_tables;
	WaveTable _table;
	double _currentTableIndex;
	double _tableIndexIncrement;

	Parameter *volumeParameter;
	Parameter *volumeEnvelopeParameter;
};

inline double convertToFrequencyValue(double frequency)
{
	return convertRanges(frequency, MIN_FREQUENCY, MAX_FREQUENCY, 0.0, 1.0);
}
