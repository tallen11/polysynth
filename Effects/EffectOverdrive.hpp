#pragma once

#include "Effect.hpp"
#include "../Parameter.hpp"

class EffectOverdrive : public Effect
{
public:
	EffectOverdrive();
	~EffectOverdrive();
	void processBuffer(std::vector<double> &samples, int bufferLength) override;
	Parameter *getGainParameter();

private:
	Parameter *gainParameter;
};