#include "Oscillator.hpp"
#include "Util.hpp"
#include <cmath>

Oscillator::Oscillator()
{
	_tables = new WaveTable*[TABLE_COUNT];
	for (int i = 0; i < TABLE_COUNT; ++i) {
		float hfreq = convertRanges((float)i, 0.0f, (float)TABLE_COUNT, (float)MIN_FREQUENCY, (float)MAX_FREQUENCY);
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

float Oscillator::getNextSample()
{
	float ti = convertRanges(_frequencyValue, 0.0f, 1.0f, 0.0f, (float)TABLE_COUNT);

	int tableIndex1 = static_cast<int>(floor(ti));
	int tableIndex2 = tableIndex1 + 1 >= TABLE_COUNT ? -1 : tableIndex1 + 1;
	float tableWeight1 = ti - tableIndex1;
	float tableWeight2 = tableWeight1 == -1 ? 0.0f : 1.0f - tableWeight1;
	if (tableIndex2 == -1) {
		tableIndex2 = 0;	// This will not affect anything. Only here to prevent reading out of bounds
	}

	int sampleCount = _tables[0]->sampleCount;

	/* Get next sample using interpolation */
	int baseIndex = floor(_currentTableIndex);
	int nextIndex = baseIndex + _tableIndexIncrement >= sampleCount ? sampleCount - (baseIndex + _tableIndexIncrement) : baseIndex + _tableIndexIncrement;
	float nextPercentage = _currentTableIndex - baseIndex;
	float basePercentage = 1.0f - nextPercentage;
	float sample1 = tableWeight1 * (basePercentage * _tables[tableIndex1]->samples[baseIndex] + nextPercentage * _tables[tableIndex1]->samples[nextIndex]);
	float sample2 = tableWeight2 * (basePercentage * _tables[tableIndex2]->samples[baseIndex] + nextPercentage * _tables[tableIndex2]->samples[nextIndex]);
	float finalSample = sample1 + sample2;

	/* Increment and check index */
	_currentTableIndex += _tableIndexIncrement;
	if (_currentTableIndex >= sampleCount) {
		_currentTableIndex = _currentTableIndex - (float)sampleCount;
	}

	return finalSample;
}

void Oscillator::setFrequencyValue(float value)
{
	if (value > 1.0f)
		value = 1.0f;
	else if (value < 0.0f)
		value = 0.0f;

	_frequencyValue = value;
	_tableIndexIncrement = _frequencyValue / convertToFrequencyValue(OSCILLATOR_BASE_FREQUENCY);
}

float Oscillator::getFrequencyValue()
{
	return _frequencyValue;
}
