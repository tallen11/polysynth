#pragma once

#include "Effect.hpp"
#include "../Parameter.hpp"
#include <vector>

class EffectDelay : public Effect
{
public:
	EffectDelay();
	~EffectDelay();
	void processBuffer(std::vector<double> &samples, int bufferLength) override;

private:
	Parameter *delayLengthParameter;
	Parameter *attenuationCoefficientParameter;
	std::vector<double> delayedSamples;
};