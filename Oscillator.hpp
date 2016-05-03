#pragma once

#include "WaveTables/WaveTable.hpp"

#define WAVE_TABLE_COUNT 32

class Oscillator
{
public:
	Oscillator();
	~Oscillator();

private:
	float frequencyValue;
	float phaseValue;
	WaveTable 
};