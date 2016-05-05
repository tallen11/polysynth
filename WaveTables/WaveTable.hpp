#pragma once

enum WaveTableType
{
	wtSine = 0,
	wtSquare = 1,
	wtSawtooth = 2
};

struct WaveTable
{
	WaveTable(WaveTableType type);
	WaveTable(WaveTableType type, float harmonicFreq);
	~WaveTable();

	void generateSineTable();
	void generateSquareTable();
	void generateSawtoothTable();
	void normalizeTable();

	float *samples;
	int sampleCount;
	float harmonicFrequency;

	typedef void (WaveTable::*tableGeneratorFunc)();
	tableGeneratorFunc generatorFuncs[3];
};