#include "Oscillator.hpp"
#include "Constants.hpp"
#include <cmath>
#include <iostream>

Oscillator::Oscillator() : _table(wtSawtooth)
{
	// _tables = new WaveTable*[TABLE_COUNT];
	// // double hfreq = MIN_FREQUENCY;
	// for (int i = 0; i < TABLE_COUNT; ++i) {
	// 	double hfreq = convertRanges((double)i, 0.0, (double)TABLE_COUNT, MIN_FREQUENCY, MAX_FREQUENCY);
	// 	_tables[i] = new WaveTable(wtSawtooth, hfreq);
	// 	// hfreq = hfreq * 2.0;
	// }

	setFrequencyValue(convertToFrequencyValue(OSCILLATOR_DESIRED_BASE_FREQUENCY));
	_currentTableIndex = 0.0;

	volumeParameter = new Parameter(1.0, 0.0, 1.0);
	volumeEnvelopeParameter = new Parameter(1.0, 0.0, 1.0);
	phaseParameter = new Parameter(2.0 * M_PI, 0.0, M_PI);
}

Oscillator::~Oscillator()
{
	delete volumeParameter;
	delete volumeEnvelopeParameter;

	// for (int i = 0; i < TABLE_COUNT; ++i) {
	// 	delete _tables[i];
	// }

	// delete [] _tables;
}

double Oscillator::getNextSample()
{
	// double ti = convertRanges(_frequencyValue, 0.0, 1.0, 0.0, (double)TABLE_COUNT);

	// int tableIndex1 = static_cast<int>(floor(ti));
	// int tableIndex2 = tableIndex1 + 1 >= TABLE_COUNT ? -1 : tableIndex1 + 1;
	// double tableWeight1 = ti - tableIndex1;
	// double tableWeight2 = tableWeight1 == -1 ? 0.0 : 1.0 - tableWeight1;
	// if (tableIndex2 == -1) {
	// 	tableIndex2 = 0;	// This will not affect anything. Only here to prevent reading out of bounds
	// }

	// int sampleCount = _tables[0]->sampleCount;

	// /* Get next sample using interpolation */
	// int baseIndex = floor(_currentTableIndex);
	// int nextIndex = baseIndex + _tableIndexIncrement >= sampleCount ? sampleCount - (baseIndex + _tableIndexIncrement) : baseIndex + _tableIndexIncrement;
	// double nextPercentage = _currentTableIndex - baseIndex;
	// double basePercentage = 1.0 - nextPercentage;
	// double sample1 = tableWeight1 * (basePercentage * _tables[tableIndex1]->samples[baseIndex] + nextPercentage * _tables[tableIndex1]->samples[nextIndex]);
	// double sample2 = tableWeight2 * (basePercentage * _tables[tableIndex2]->samples[baseIndex] + nextPercentage * _tables[tableIndex2]->samples[nextIndex]);
	// double finalSample = sample1 + sample2;

	int baseIndex = static_cast<int>(floor(_currentTableIndex));
	int nextIndex = baseIndex + 1 >= _table.sampleCount ? 0 : baseIndex + 1;
	double nextPercentage = _currentTableIndex - static_cast<double>(baseIndex);
	double basePercentage = 1.0 - nextPercentage;

	double sample = volumeParameter->getValue() * 
					volumeEnvelopeParameter->getValue() * 
					(basePercentage * _table.samples[baseIndex] + nextPercentage * _table.samples[nextIndex]);

	_currentTableIndex += _tableIndexIncrement;
	if (_currentTableIndex >= _table.sampleCount) {
		_currentTableIndex = _currentTableIndex - static_cast<double>(_table.sampleCount);
	}

	return sample;
}

void Oscillator::setFrequencyValue(double value)
{
	if (value > 1.0)
		value = 1.0;
	else if (value < 0.0)
		value = 0.0;

	_frequencyValue = value;

	double frequency = convertRanges(_frequencyValue, 0.0, 1.0, MIN_FREQUENCY, MAX_FREQUENCY);
	_tableIndexIncrement = frequency / BASE_FREQUENCY; // (_tableIndexIncrement + frequency) / BASE_FREQUENCY;
}

double Oscillator::getFrequencyValue() const
{
	return _frequencyValue;
}

Parameter* Oscillator::getVolumeParameter()
{
	return volumeParameter;
}

Parameter* Oscillator::getVolumeEnvelopeParameter()
{
	return volumeEnvelopeParameter;
}
