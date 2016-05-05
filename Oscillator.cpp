#include "Oscillator.hpp"
#include "Util.hpp"
#include <cmath>

Oscillator::Oscillator()
{
	_tables = new WaveTable*[TABLE_COUNT];
	for (int i = 0; i < TABLE_COUNT; ++i) {
		double hfreq = convertRanges((double)i, 0.0, (double)TABLE_COUNT, (double)MIN_FREQUENCY, (double)MAX_FREQUENCY);
		_tables[i] = new WaveTable(wtSawtooth, hfreq);
	}

	_frequencyValue = convertToFrequencyValue(OSCILLATOR_DESIRED_BASE_FREQUENCY);
	_currentTableIndex = 0.0f;
	// _tableIndex = 0.0f;
	_tableIndexIncrement = _frequencyValue / convertToFrequencyValue(OSCILLATOR_BASE_FREQUENCY);
}

Oscillator::~Oscillator()
{
	for (int i = 0; i < TABLE_COUNT; ++i) {
		delete _tables[i];
	}

	delete [] _tables;
}

double Oscillator::getNextSample()
{
	double ti = convertRanges(_frequencyValue, 0.0, 1.0, 0.0, (double)TABLE_COUNT);

	int tableIndex1 = static_cast<int>(floor(ti));
	int tableIndex2 = tableIndex1 + 1 >= TABLE_COUNT ? -1 : tableIndex1 + 1;
	double tableWeight1 = ti - tableIndex1;
	double tableWeight2 = tableWeight1 == -1 ? 0.0 : 1.0 - tableWeight1;
	if (tableIndex2 == -1) {
		tableIndex2 = 0;	// This will not affect anything. Only here to prevent reading out of bounds
	}

	int sampleCount = _tables[0]->sampleCount;

	/* Get next sample using interpolation */
	int baseIndex = floor(_currentTableIndex);
	int nextIndex = baseIndex + _tableIndexIncrement >= sampleCount ? sampleCount - (baseIndex + _tableIndexIncrement) : baseIndex + _tableIndexIncrement;
	double nextPercentage = _currentTableIndex - baseIndex;
	double basePercentage = 1.0 - nextPercentage;
	double sample1 = tableWeight1 * (basePercentage * _tables[tableIndex1]->samples[baseIndex] + nextPercentage * _tables[tableIndex1]->samples[nextIndex]);
	double sample2 = tableWeight2 * (basePercentage * _tables[tableIndex2]->samples[baseIndex] + nextPercentage * _tables[tableIndex2]->samples[nextIndex]);
	double finalSample = sample1 + sample2;

	/* Increment and check index */
	_currentTableIndex += _tableIndexIncrement;
	if (_currentTableIndex >= sampleCount) {
		_currentTableIndex = _currentTableIndex - (double)sampleCount;
	}

	return finalSample;
}

void Oscillator::setFrequencyValue(double value)
{
	if (value > 1.0f)
		value = 1.0f;
	else if (value < 0.0f)
		value = 0.0f;

	_frequencyValue = value;
	_tableIndexIncrement = _frequencyValue / convertToFrequencyValue(OSCILLATOR_BASE_FREQUENCY);
}

double Oscillator::getFrequencyValue()
{
	return _frequencyValue;
}
