#include "Synth.hpp"
#include "Constants.hpp"
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

	for (auto envelope : envelopes) {
		delete envelope;
	}
}

double Synth::getNextSample()
{
	// Update envelopes
	for (auto envelope : envelopes) {
		envelope->update();
	}

	// Mix all oscillator signals
	double total = 0.0;
	for (auto oscillator : oscillators) {
		total += oscillator->getNextSample();
	}
	total /= oscillators.size();

	return total;
}

void Synth::getNextBuffer(std::vector<double> &samples, int bufferLength)
{
	for (int i = 0; i < bufferLength; ++i) {
		
	}
}

void Synth::setMasterVolume(double volume)
{
	for (auto oscillator : oscillators) {
		oscillator->getVolumeParameter()->setValue(volume);
	}
}

void Synth::keyPressed(int midiKey)
{
	int key = -(REFERENCE_MIDI - midiKey);
	double frequencyValue = convertToFrequencyValue(pow(2.0, (double)key / 12.0) * REFERENCE_FREQUENCY);

	for (auto oscillator : oscillators) {
		oscillator->getFrequencyParameter()->setValue(frequencyValue);
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

void Synth::pitchBend(double amount)
{
	for (auto oscillator : oscillators) {
		oscillator->getFrequencyParameter()->multiplyValue(amount);
	}
}
