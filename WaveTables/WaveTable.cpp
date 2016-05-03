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
	currentIndex = 0;

	(*this.*generatorFuncs[type])();
}

WaveTable::~WaveTable()
{
	delete [] samples;
}

float WaveTable::getNextSample()
{
	float sample = samples[currentIndex];
	currentIndex += 1;
	if (currentIndex >= sampleCount) {
		currentIndex = 0;
	}

	return sample;
}

void WaveTable::generateSineTable()
{
	int tableLength = static_cast<int>(floor(SAMPLE_RATE / BASE_FREQUENCY));
	samples = new float[tableLength];
	sampleCount = tableLength;
	for (int i = 0; i < tableLength; ++i) {
		float x = static_cast<float>(i) / SAMPLE_RATE;
		samples[i] = sin(2.0f * M_PI * BASE_FREQUENCY * x);
	}
}

void WaveTable::generateSquareTable()
{

}

void WaveTable::generateSawtoothTable()
{
	int tableLength = static_cast<int>(floor(SAMPLE_RATE / BASE_FREQUENCY));
	samples = new float[tableLength];
	sampleCount = tableLength;
	for (int i = 0; i < tableLength; ++i) {
		float x = static_cast<float>(i) / SAMPLE_RATE;
		samples[i] = -sin(2.0f * M_PI * BASE_FREQUENCY * x);
		for (int j = 2; j * BASE_FREQUENCY < SAMPLE_RATE / 2; ++j) {
			samples[i] += pow(-1.0f, j) * (sin(2.0f * M_PI * BASE_FREQUENCY * j * x) / j);
		}

		samples[i] = 0.5 - (1.0f / M_PI) * samples[i];
	}

	// normalizeTable();
}

void WaveTable::normalizeTable()
{
	float magnitude = 0.0f;
	for (int i = 0; i < sampleCount; ++i) {
		magnitude += samples[i] * samples[i];
	}

	magnitude = sqrt(magnitude);
	for (int i = 0; i < sampleCount; ++i) {
		samples[i] /= magnitude;
	}
}