#include "EffectsLoop.hpp"

EffectsLoop::EffectsLoop()
{

}

EffectsLoop::~EffectsLoop()
{

}

void EffectsLoop::addEffect(Effect *effect)
{
	effects.push_back(effect);
}

void EffectsLoop::processBuffer(std::vector<double> &samples, int bufferLength)
{
	for (auto effect : effects) {
		if (effect->isEnabled()) {
			effect->processBuffer(samples, bufferLength);
		}
	}
}
