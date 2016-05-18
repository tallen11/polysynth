#include "Oscillator.hpp"
#include <cmath>
#include <iostream>

// Oscillator::Oscillator()
// {
// 	_table = new WaveTable(wtSawtooth);
// 	_table2 = new WaveTable(wtSquare);

// 	frequencyParameter = new Parameter(0.0, 1.0, OSCILLATOR_DESIRED_BASE_FREQUENCY);
// 	phaseParameter = new Parameter(2.0 * M_PI, 0.0, M_PI);
// 	tableParameter = new Parameter(1.0, 0.0, 0.0);

// 	_currentTableIndex = 0.0;
// }

Oscillator::Oscillator(WaveTable *leftTable, WaveTable *rightTable)
{
	_table = leftTable;
	_table2 = rightTable;

	frequencyParameter = new Parameter(0.0, 1.0, OSCILLATOR_DESIRED_BASE_FREQUENCY, true);
	phaseParameter = new Parameter(1.0, 0.0, 0.0, true);
	tableParameter = new Parameter(1.0, 0.0, 0.0, true);

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
	int nextIndex = baseIndex + 1 >= _table->sampleCount ? 0 : baseIndex + 1;
	double nextPercentage = _currentTableIndex - static_cast<double>(baseIndex);
	double basePercentage = 1.0 - nextPercentage;

	double sample1 = basePercentage * _table->samples[baseIndex] + nextPercentage * _table->samples[nextIndex];
	double sample2 = basePercentage * _table2->samples[baseIndex] + nextPercentage * _table2->samples[nextIndex];

	// double phaseMult = phaseParameter->getValue() * _table->sampleCount;
	// double phaseIndex = _currentTableIndex + phaseMult >= _table->sampleCount ? _currentTableIndex + phaseMult - _table->sampleCount : _currentTableIndex + phaseMult;
	// int phaseBaseIndex = static_cast<int>(floor(phaseIndex));
	// int nextPhaseIndex = phaseBaseIndex + 1 >= _table->sampleCount ? 0 : phaseBaseIndex + 1;
	// double nextPhasePercentage = phaseIndex - static_cast<double>(phaseBaseIndex);
	// double basePhasePercentage = 1.0 - nextPhasePercentage;

	// double phaseSample1 = basePhasePercentage * _table->samples[phaseBaseIndex] + nextPhasePercentage * _table->samples[nextPhaseIndex]; 
	// double phaseSample2 = basePhasePercentage * _table2->samples[phaseBaseIndex] + nextPhasePercentage * _table2->samples[nextPhaseIndex]; 

	// sample1 -= phaseSample1;
	// sample2 -= phaseSample2;

	_currentTableIndex += frequencyParameter->getValue() / BASE_FREQUENCY;
	if (_currentTableIndex >= _table->sampleCount) {
		_currentTableIndex = _currentTableIndex - static_cast<double>(_table->sampleCount);
	}

	double tableFade = tableParameter->getValue();

	return tableFade * sample2 + (1.0 - tableFade) * sample1; // sample;
}

Parameter* Oscillator::getFrequencyParameter()
{
	return frequencyParameter;
}

Parameter* Oscillator::getPhaseParameter()
{
	return phaseParameter;
}

Parameter* Oscillator::getTableParameter()
{
	return tableParameter;
}
