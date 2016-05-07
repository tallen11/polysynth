#include "Synth.hpp"
#include <cmath>
#include <iostream>

Synth::Synth()
{
	auto osc = new Oscillator();
	auto env = new EnvelopeGenerator(0.02, 0.1, 0.9, 0.02);

	env->setParameter(osc->getVolumeEnvelopeParameter());

	oscillators.push_back(osc);
	envelopes.push_back(env);
}

Synth::~Synth()
{
	for (auto oscillator : oscillators) {
		delete oscillator;
	}
}

double Synth::getNextSample()
{
	// Mix all oscillator signals here. Do basic summing for now
	for (auto envelope : envelopes) {
		envelope->update();
	}

	double total = 0.0;
	for (auto oscillator : oscillators) {
		total += oscillator->getNextSample();
	}

	total /= oscillators.size();

	return total;
}

void Synth::keyPressed(int midiKey)
{
	int key = -(69 - midiKey);
	double frequencyValue = convertToFrequencyValue(pow(2.0, (double)key / 12.0) * 440.0);

	for (auto oscillator : oscillators) {
		oscillator->setFrequencyValue(frequencyValue);
	}

	for (auto envelope : envelopes) {
		envelope->notePressed();
	}
}

void Synth::keyReleased(int midiKey)
{	
	for (auto envelope : envelopes) {
		envelope->noteReleased();
	}
}
