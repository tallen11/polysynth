#include "EffectDelay.hpp"

EffectDelay::EffectDelay()
{
	delayLengthParameter = new Parameter(1.0, 0.1, 0.25);
	attenuationCoefficientParameter = new Parameter(0.9, 0.1, 0.4);
}

EffectDelay::~EffectDelay()
{
	delete delayLengthParameter;
	delete attenuationCoefficientParameter;
}

void EffectDelay::processBuffer(std::vector<double> &samples, int bufferLength)
{
	for (int i = 0; i < bufferLength; ++i) {
		
	}
}
