#pragma once

#include "Effect.hpp"
#include <vector>

class EffectsLoop
{
public:
	EffectsLoop();
	~EffectsLoop();
	void addEffect(Effect *effect);
	void processBuffer(std::vector<double> &samples, int bufferLength);

private:
	std::vector<Effect*> effects;
};