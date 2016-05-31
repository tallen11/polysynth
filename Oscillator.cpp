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
	voiceDetuneFactorParameter = new Parameter(1.0, 0.0, 0.0, true);

	_currentTableIndex = 0.0;
    
    enabled = true;

	setVoiceCount(1);
}

Oscillator::~Oscillator()
{
	delete frequencyParameter;
	delete phaseParameter;
	delete tableParameter;
	delete voiceDetuneFactorParameter;
}

double Oscillator::getNextSample()
{
	double tableFade = tableParameter->getValue();
	double currentFrequency = frequencyParameter->getValue();
	double voiceDetuneFactor = voiceDetuneFactorParameter->getValue();
	double sample = 0.0;
    
	for (int i = 0; i < voiceCount; ++i) {
		double tableIndex = voiceIndices[i];

		int baseIndex = static_cast<int>(floor(tableIndex));
		int nextIndex = baseIndex + 1 >= _table->sampleCount ? 0 : baseIndex + 1;
		double nextPercentage = tableIndex - static_cast<double>(baseIndex);
		double basePercentage = 1.0 - nextPercentage;
        
		double sample1 = basePercentage * _table->samples[baseIndex] + nextPercentage * _table->samples[nextIndex];
		double sample2 = basePercentage * _table2->samples[baseIndex] + nextPercentage * _table2->samples[nextIndex];

		tableIndex += (currentFrequency + voiceDetuneFactor * i * currentFrequency) / BASE_FREQUENCY;
		if (tableIndex >= _table->sampleCount) {
			tableIndex = tableIndex - static_cast<double>(_table->sampleCount);
		}

		voiceIndices[i] = tableIndex;

		sample += tableFade * sample2 + (1.0 - tableFade) * sample1;




		// int baseIndex = static_cast<int>(floor(tableIndex));
		// int nextIndex = baseIndex + 1 >= _table->sampleCount ? 0 : baseIndex + 1;
		// double nextPercentage = tableIndex - static_cast<double>(baseIndex);
		// double basePercentage = 1.0 - nextPercentage;

		// double sample1 = basePercentage * _table->samples[baseIndex] + nextPercentage * _table->samples[nextIndex];
		// double sample2 = basePercentage * _table2->samples[baseIndex] + nextPercentage * _table2->samples[nextIndex];

		// double phaseMult = phaseParameter->getValue() * _table->sampleCount;
		// double phaseIndex = tableIndex + phaseMult >= _table->sampleCount ? tableIndex + phaseMult - _table->sampleCount : _currentTableIndex + phaseMult;
		// int phaseBaseIndex = static_cast<int>(floor(phaseIndex));
		// int nextPhaseIndex = phaseBaseIndex + 1 >= _table->sampleCount ? 0 : phaseBaseIndex + 1;
		// double nextPhasePercentage = phaseIndex - static_cast<double>(phaseBaseIndex);
		// double basePhasePercentage = 1.0 - nextPhasePercentage;

		// double phaseSample1 = basePhasePercentage * _table->samples[phaseBaseIndex] + nextPhasePercentage * _table->samples[nextPhaseIndex]; 
		// double phaseSample2 = basePhasePercentage * _table2->samples[phaseBaseIndex] + nextPhasePercentage * _table2->samples[nextPhaseIndex]; 

		// sample1 -= phaseSample1;
		// sample2 -= phaseSample2;

		// tableIndex += (currentFrequency + voiceDetuneFactor * i * currentFrequency) / BASE_FREQUENCY;
		// if (tableIndex >= _table->sampleCount) {
		// 	tableIndex = tableIndex - static_cast<double>(_table->sampleCount);
		// }

		// voiceIndices[i] = tableIndex;

		// double tableFade = tableParameter->getValue();

		// // sample += tableFade * sample2 + (1.0 - tableFade) * sample1;
		// return tableFade * sample2 + (1.0 - tableFade) * sample1; // sample;
	}

	sample /= static_cast<double>(voiceCount);
	return sample;
}

void Oscillator::resetOscillator()
{
	for (int i = 0; i < voiceCount; ++i) {
		voiceIndices[i] = 0.0;
	}
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

Parameter* Oscillator::getVoiceDetuneFactorParameter()
{
	return voiceDetuneFactorParameter;
}

void Oscillator::setVoiceCount(int count)
{
	voiceIndices.clear();
    voiceCount = count;

	for (int i = 0; i < count; ++i) {
		voiceIndices.push_back(0.0);
	}
}

void Oscillator::toggleOscillator()
{
    enabled = !enabled;
}

bool Oscillator::isEnabled()
{
    return enabled;
}

void Oscillator::setLeftWavetable(WaveTable *wavtable)
{
    _table = wavtable;
}

void Oscillator::setRightWavetable(WaveTable *wavetable)
{
    _table2 = wavetable;
}
