#include "WaveTable.hpp"
#include "../Constants.hpp"
#include <cmath>
#include <iostream>

WaveTable::WaveTable(WaveTableType type)
{
	generatorFuncs[0] = &WaveTable::generateSineTable;
	generatorFuncs[1] = &WaveTable::generateSquareTable;
	generatorFuncs[2] = &WaveTable::generateSawtoothTable;
	generatorFuncs[3] = &WaveTable::generateTriangleTable;
	generatorFuncs[4] = &WaveTable::generateExperimentalTable;

	samples = nullptr;
	sampleCount = 0;
	harmonicFrequency = BASE_FREQUENCY;
	phaseShift = 0.0;

	(*this.*generatorFuncs[type])();
}

WaveTable::WaveTable(WaveTableType type, double shift)
{
	generatorFuncs[0] = &WaveTable::generateSineTable;
	generatorFuncs[1] = &WaveTable::generateSquareTable;
	generatorFuncs[2] = &WaveTable::generateSawtoothTable;
	generatorFuncs[3] = &WaveTable::generateTriangleTable;
	generatorFuncs[4] = &WaveTable::generateExperimentalTable;

	samples = nullptr;
	sampleCount = 0;
	harmonicFrequency = BASE_FREQUENCY;
	phaseShift = shift;

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
		samples[i] = sin(2.0 * M_PI * BASE_FREQUENCY * x + phaseShift);
	}
}

void WaveTable::generateSquareTable()
{
	int tableLength = static_cast<int>(floor(SAMPLE_RATE / BASE_FREQUENCY));
	samples = new double[tableLength];
	sampleCount = tableLength;
	for (int i = 0; i < tableLength; ++i) {
		double x = static_cast<double>(i) / SAMPLE_RATE;
		samples[i] = sin(2.0 * M_PI * BASE_FREQUENCY * x);
		for (int j = 2; j * harmonicFrequency < SAMPLE_RATE / 2; ++j) {
			samples[i] += sin(2.0 * M_PI * (2.0 * j - 1.0) * BASE_FREQUENCY * x) / (2.0 * j - 1.0);
		}

		samples[i] = (4.0 / M_PI) * samples[i];
	}
}

void WaveTable::generateSawtoothTable()
{
	int tableLength = static_cast<int>(floor(SAMPLE_RATE / BASE_FREQUENCY));
	samples = new double[tableLength];
	sampleCount = tableLength;
	for (int i = 0; i < tableLength; ++i) {
		double x = static_cast<double>(i) / SAMPLE_RATE;
		samples[i] = -sin(2.0 * M_PI * BASE_FREQUENCY * x);
		for (int j = 2; j * harmonicFrequency < SAMPLE_RATE / 2; ++j) {
			samples[i] += pow(-1.0, j) * (sin(2.0 * M_PI * BASE_FREQUENCY * j * x + 2.0 * j * phaseShift) / j);			
		}

		samples[i] = 0.5 - (1.0 / M_PI) * samples[i];
	}
}

void WaveTable::generateTriangleTable()
{
	int tableLength = static_cast<int>(floor(SAMPLE_RATE / BASE_FREQUENCY));
	samples = new double[tableLength];
	sampleCount = tableLength;
	for (int i = 0; i < tableLength; ++i) {
		double x = static_cast<double>(i) / SAMPLE_RATE;
		samples[i] = -(sin(6.0 * M_PI * BASE_FREQUENCY * x) / 9.0);
		for (int j = 2; j * harmonicFrequency < SAMPLE_RATE / 2; ++j) {
			samples[i] += pow(-1.0, j) * (sin(2.0 * M_PI * (2.0 * j + 1.0) * BASE_FREQUENCY * x) / ((2.0 * j + 1.0) * (2.0 * j + 1.0)));
		}

		samples[i] = (8.0 / (M_PI * M_PI)) * samples[i];
	}
}

void WaveTable::generateExperimentalTable()
{
	generateSquareTable();

	int tableLength = static_cast<int>(floor(SAMPLE_RATE / BASE_FREQUENCY));
	samples = new double[tableLength];
	sampleCount = tableLength;
	for (int i = 0; i < tableLength; ++i) {
		double x = static_cast<double>(i) / SAMPLE_RATE;
		samples[i] = sin(2.0 * M_PI * BASE_FREQUENCY * x + sin(2.0 * M_PI * 3.0 * x));
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
