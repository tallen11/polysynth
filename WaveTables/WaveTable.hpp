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
	~WaveTable();

	void generateSineTable();
	void generateSquareTable();
	void generateSawtoothTable();
	void normalizeTable();
	float& operator[](const int index);

	float *samples;
	int sampleCount;

	typedef void (WaveTable::*tableGeneratorFunc)();
	tableGeneratorFunc generatorFuncs[3];
};