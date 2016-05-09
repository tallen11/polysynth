#pragma once

#include "Effect.hpp"
#include "../Parameter.hpp"

class EffectBitcrusher : public Effect
{
public:
	EffectBitcrusher();
	~EffectBitcrusher();
	void processBuffer(std::vector<double> &samples, int bufferLength) override;

private:
	Parameter *bitsParameter;
	Parameter *rateParameter;
	int rateCounter;
};