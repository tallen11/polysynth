#include "LFO.hpp"
#include "Constants.hpp"
#include <cmath>

LFO::LFO()
{
	frequencyParameter = new Parameter(20.0, 0.1, 1.0, true);
	amplitudeParameter = new Parameter(1.0, 0.0, 1.0, true);
	currentSample = 0;
}

LFO::~LFO()
{
	delete frequencyParameter;
	delete amplitudeParameter;
}

double LFO::getMultiplier()
{
	double x = static_cast<double>(currentSample) / SAMPLE_RATE;
	return sin(2.0 * M_PI * frequencyParameter->getValue() * x);
}

void LFO::stepLFO()
{
	++currentSample;
}

Parameter* LFO::getFrequencyParameter()
{
	return frequencyParameter;
}

Parameter* LFO::getAmplitudeParameter()
{
	return amplitudeParameter;
}
