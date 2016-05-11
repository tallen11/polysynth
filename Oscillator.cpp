#include "Oscillator.hpp"
#include <cmath>
#include <iostream>

Oscillator::Oscillator() : _table(wtSawtooth), _table2(wtSine)
{
	frequencyParameter = new Parameter(0.0, 1.0, OSCILLATOR_DESIRED_BASE_FREQUENCY);
	phaseParameter = new Parameter(2.0 * M_PI, 0.0, M_PI);
	tableParameter = new Parameter(1.0, 0.0, 0.0);

	_currentTableIndex = 0.0;
}

Oscillator::Oscillator(WaveTable *leftTable, WaveTable *rightTable)
{
	frequencyParameter = new Parameter(0.0, 1.0, OSCILLATOR_DESIRED_BASE_FREQUENCY);
	phaseParameter = new Parameter(2.0 * M_PI, 0.0, M_PI);
	tableParameter = new Parameter(1.0, 0.0, 0.0);

	_currentTableIndex = 0.0;
}

Oscillator::~Oscillator()
{
	delete frequencyParameter;
	delete phaseParameter;
	delete tableParameter;
}

double Oscillator::getNextSample()
{
	int baseIndex = static_cast<int>(floor(_currentTableIndex));
	int nextIndex = baseIndex + 1 >= _table.sampleCount ? 0 : baseIndex + 1;
	double nextPercentage = _currentTableIndex - static_cast<double>(baseIndex);
	double basePercentage = 1.0 - nextPercentage;

	double sample1 = basePercentage * _table.samples[baseIndex] + nextPercentage * _table.samples[nextIndex];
	double sample2 = basePercentage * _table2.samples[baseIndex] + nextPercentage * _table2.samples[nextIndex];

	double tableFade = tableParameter->getValue();

	double frequency = frequencyParameter->getValue();
	_currentTableIndex += frequency / BASE_FREQUENCY;
	if (_currentTableIndex >= _table.sampleCount) {
		_currentTableIndex = _currentTableIndex - static_cast<double>(_table.sampleCount);
	}

	return tableFade * sample2 + (1.0 - tableFade) * sample1; // sample;
}

Parameter* Oscillator::getFrequencyParameter()
{
	return frequencyParameter;
}

Parameter* Oscillator::getTableParameter()
{
	return tableParameter;
}
