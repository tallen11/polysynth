#include "Oscillator.hpp"
#include <cmath>

Oscillator::Oscillator() : _table(wtSawtooth)
{
	_frequencyValue = convertToFrequencyValue(OSCILLATOR_DESIRED_BASE_FREQUENCY);
	_tableIndex = 0.0f;
	_tableIndexIncrement = _frequencyValue / convertToFrequencyValue(OSCILLATOR_BASE_FREQUENCY);
}

Oscillator::~Oscillator()
{

}

float Oscillator::getNextSample()
{
	/* Get next sample using interpolation */
	int baseIndex = floor(_tableIndex);
	int nextIndex = baseIndex + _tableIndexIncrement >= _table.sampleCount ? _table.sampleCount - (baseIndex + _tableIndexIncrement) : baseIndex + _tableIndexIncrement;
	float nextPercentage = _tableIndex - baseIndex;
	float basePercentage = 1.0f - nextPercentage;

	float sample = basePercentage * _table[baseIndex] + nextPercentage * _table[nextIndex];

	/* Increment and check index */
	_tableIndex += _tableIndexIncrement;
	if (_tableIndex >= _table.sampleCount) {
		_tableIndex = _tableIndex - (float)_table.sampleCount;
	}

	return sample;
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
