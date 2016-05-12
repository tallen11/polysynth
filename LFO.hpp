#pragma once

#include "Parameter.hpp"
#include <stdint.h>

class LFO
{
public:
	LFO();
	~LFO();
	double getMultiplier();
	void stepLFO();
	Parameter* getFrequencyParameter();
	Parameter* getAmplitudeParameter();

private:
	Parameter *frequencyParameter;
	Parameter *amplitudeParameter;
	uint64_t currentSample;
};