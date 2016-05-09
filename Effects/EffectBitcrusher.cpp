#include "EffectBitcrusher.hpp"
#include <cmath>

EffectBitcrusher::EffectBitcrusher()
{
	bitsParameter = new Parameter(63.0, 0.0, 0.0);
	rateParameter = new Parameter(10.0, 1.0, 1.0);
	rateCounter = 0;
}

EffectBitcrusher::~EffectBitcrusher()
{
	delete bitsParameter;
	delete rateParameter;
}

void EffectBitcrusher::processBuffer(std::vector<double> &samples, int bufferLength)
{
	for (int i = 0; i < bufferLength; ++i) {
		int rate = static_cast<int>(floor(rateParameter->getValue()));
		if (rateCounter >= rate) {
			rateCounter = 0;
			double sample = samples[i];
			int bits = static_cast<int>(floor(bitsParameter->getValue()));
			int64_t iSample = static_cast<int64_t>(fmin(sample * INT64_MAX, INT64_MAX));
			int64_t crushedISample = (iSample >> bits) << bits;
			double crushedSample = static_cast<double>(static_cast<double>(crushedISample) / static_cast<double>(INT64_MAX));
			samples[i] = crushedSample;
		} else {
			rateCounter++;
		}
	}
}
