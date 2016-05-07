#include "WaveTable.hpp"
#include "../Constants.hpp"
#include <cmath>
#include <iostream>

WaveTable::WaveTable(WaveTableType type)
{
	generatorFuncs[0] = &WaveTable::generateSineTable;
	generatorFuncs[1] = &WaveTable::generateSquareTable;
	generatorFuncs[2] = &WaveTable::generateSawtoothTable;

	samples = nullptr;
	sampleCount = 0;
	harmonicFrequency = BASE_FREQUENCY;

	(*this.*generatorFuncs[type])();
}

WaveTable::WaveTable(WaveTableType type, double harmonicFreq)
{
	generatorFuncs[0] = &WaveTable::generateSineTable;
	generatorFuncs[1] = &WaveTable::generateSquareTable;
	generatorFuncs[2] = &WaveTable::generateSawtoothTable;

	samples = nullptr;
	sampleCount = 0;
	harmonicFrequency = harmonicFreq;

	(*this.*generatorFuncs[type])();
}

WaveTable::~WaveTable()
{
	delete [] samples;
}

void WaveTable::generateSineTable()
{
	int tableLength = static_cast<int>(floor(SAMPLE_RATE / BASE_FREQUENCY));
	samples = new double[tableLength];
	sampleCount = tableLength;
	for (int i = 0; i < tableLength; ++i) {
		double x = static_cast<double>(i) / SAMPLE_RATE;
		samples[i] = sin(2.0 * M_PI * BASE_FREQUENCY * x);
	}
}

void WaveTable::generateSquareTable()
{

}

void WaveTable::generateSawtoothTable()
{
	int tableLength = static_cast<int>(floor(SAMPLE_RATE / BASE_FREQUENCY)) + 1;
	samples = new double[tableLength];
	sampleCount = tableLength;
	for (int i = 0; i < tableLength; ++i) {
		double x = static_cast<double>(i) / SAMPLE_RATE;
		samples[i] = -sin(2.0 * M_PI * BASE_FREQUENCY * x);
		for (int j = 2; j * harmonicFrequency < SAMPLE_RATE / 2; ++j) {
			samples[i] += pow(-1.0, j) * (sin(2.0 * M_PI * BASE_FREQUENCY * j * x) / j);			
		}

		samples[i] = 0.5 - (1.0 / M_PI) * samples[i];

		// double gibbs = pow(cos((2.0) - 1.0), 2.0) * (M_PI / (2.0 * (SAMPLE_RATE / harmonicFrequency)));
		// samples[i] *= gibbs;
	}
}

void WaveTable::normalizeTable()
{
	double magnitude = 0.0;
	for (int i = 0; i < sampleCount; ++i) {
		magnitude += samples[i] * samples[i];
	}

	magnitude = sqrt(magnitude);
	for (int i = 0; i < sampleCount; ++i) {
		samples[i] /= magnitude;
	}
}
