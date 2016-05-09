#include "EffectOverdrive.hpp"
#include <cmath>

EffectOverdrive::EffectOverdrive()
{
	gainParameter = new Parameter(4.0, 1.0, 1.0);
}

EffectOverdrive::~EffectOverdrive()
{
	delete gainParameter;
}

void EffectOverdrive::processBuffer(std::vector<double> &samples, int bufferLength)
{
	for (int i = 0; i < bufferLength; ++i) {
		double gain = gainParameter->getValue();
		double sample = samples[i];
		double gainedSample = fabs(sample * gain);

		if (gainedSample > 1.0) {
			samples[i] = 1.0;
		} else {
			samples[i] = sample * gain;
		}
	}
}

Parameter* EffectOverdrive::getGainParameter()
{
	return gainParameter;
}
