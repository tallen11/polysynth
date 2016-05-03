#pragma once

enum WaveTableType
{
	wtSine = 0,
	wtSquare = 1,
	wtSawtooth = 2
};

class WaveTable
{
public:
	WaveTable(WaveTableType type);
	~WaveTable();
	float getNextSample();

private:
	void generateSineTable();
	void generateSquareTable();
	void generateSawtoothTable();
	void normalizeTable();

	float *samples;
	int sampleCount;
	int currentIndex;

	typedef void (WaveTable::*tableGeneratorFunc)();
	tableGeneratorFunc generatorFuncs[3];
};