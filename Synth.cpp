#include "Synth.hpp"
#include "Constants.hpp"
#include <cmath>
#include <iostream>

Synth::Synth() : sampleBuffer(BUFFER_SIZE, 0.0)
{
	auto osc = new Oscillator();
	auto env = new EnvelopeGenerator(0.5, 0.1, 0.9, 0.1);

	env->setParameter(volumeModule.getVolumeEnvelopeParameter());

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

	total = volumeModule.processSample(total);

	return total;
}

std::vector<double>& Synth::getNextBuffer(int bufferLength)
{
	for (int i = 0; i < bufferLength; ++i) {
		for (auto envelope : envelopes) {
			envelope->update();
		}

		double sample = 0.0;
		// =========== Mixer Goes Here =================
		for (auto oscillator : oscillators) {
			sample += oscillator->getNextSample();
		}
		sample /= oscillators.size();
		// =========== Mixer Goes Here =================

		sampleBuffer[i] = volumeModule.processSample(sample);
	}

	// =========== FX Loop Goes Here =================
	// 		Pass buffer through effects
	// =========== FX Loop Goes Here =================

	// ============ Filter Goes Here =================
	// 		Pass buffer through filter
	// ============ Filter Goes Here =================

	// for (int i = 0; i < bufferLength; ++i) {
	// 	sampleBuffer[i] = volumeModule.processSample(sampleBuffer[i]);
	// }

	return sampleBuffer;
}

void Synth::setMasterVolume(double volume)
{
	for (auto oscillator : oscillators) {
		volumeModule.getVolumeParameter()->setValue(volume);
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
