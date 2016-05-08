#include "Oscillator.hpp"
#include <cmath>
#include <iostream>

Oscillator::Oscillator() : _table(wtSawtooth)
{
	frequencyParameter = new Parameter(0.0, 1.0, OSCILLATOR_DESIRED_BASE_FREQUENCY);
	phaseParameter = new Parameter(2.0 * M_PI, 0.0, M_PI);

	_currentTableIndex = 0.0;
}

Oscillator::~Oscillator()
{
	delete frequencyParameter;
	delete phaseParameter;
}

double Oscillator::getNextSample()
{
	int baseIndex = static_cast<int>(floor(_currentTableIndex));
	int nextIndex = baseIndex + 1 >= _table.sampleCount ? 0 : baseIndex + 1;
	double nextPercentage = _currentTableIndex - static_cast<double>(baseIndex);
	double basePercentage = 1.0 - nextPercentage;

	double sample = basePercentage * _table.samples[baseIndex] + nextPercentage * _table.samples[nextIndex];

	double frequency = frequencyParameter->getValue();
	_currentTableIndex += frequency / BASE_FREQUENCY;
	if (_currentTableIndex >= _table.sampleCount) {
		_currentTableIndex = _currentTableIndex - static_cast<double>(_table.sampleCount);
	}

	return sample;
}

Parameter* Oscillator::getFrequencyParameter()
{
	return frequencyParameter;
}
