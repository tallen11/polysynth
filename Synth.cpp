#include "Synth.hpp"
#include "Constants.hpp"
#include <cmath>
#include <iostream>

Synth::Synth() : sampleBuffer(BUFFER_SIZE, 0.0)
{
	oscillatorGroupsIndex = 0;

	for (int i = 0; i < 8; ++i) {
		auto oGroup = new OscillatorGroup;

		auto o1 = new Oscillator();
		auto o2 = new Oscillator();
		auto o3 = new Oscillator();
		o1->getFrequencyParameter()->setValue(440.0);
		o2->getFrequencyParameter()->setValue(220.0);
		o3->getFrequencyParameter()->setValue(224.0);

		oGroup->oscillators.push_back(o1);
		oGroup->oscillators.push_back(o2);
		oGroup->oscillators.push_back(o3);

		auto env = new EnvelopeGenerator(0.01, 0.05, 0.9, 0.5);
		env->setParameter(oGroup->volumeModule.getVolumeEnvelopeParameter());
		envelopes.push_back(env);
		oGroup->volumeEnvelope = env;

		oscillatorGroups.push_back(oGroup);
	}
}

Synth::~Synth()
{
	for (auto oGroup : oscillatorGroups) {
		for (auto oscillator : oGroup->oscillators) {
			delete oscillator;
		}

		delete oGroup;
	}

	for (auto envelope : envelopes) {
		delete envelope;
	}
}

std::vector<double>& Synth::getNextBuffer(int bufferLength)
{
	for (int i = 0; i < bufferLength; ++i) {
		for (auto envelope : envelopes) {
			envelope->update();
		}

		double sample = 0.0;
		int sampleCount = 0;
		// =========== Mixer Goes Here =================
		for (auto oGroup : oscillatorGroups) {
			double subSample = oGroup->getNextSample();
			sample += subSample;
			++sampleCount;
		}

		sample /= static_cast<double>(sampleCount);
		// =========== Mixer Goes Here =================

		sampleBuffer[i] = masterVolumeModule.processSample(sample);
	}



	return sampleBuffer;
}

void Synth::setMasterVolume(double volume)
{
	masterVolumeModule.getVolumeParameter()->setValue(volume);
}

void Synth::keyPressed(int midiKey)
{
	int key = -(REFERENCE_MIDI - midiKey);
	double multiplier = pow(2.0, static_cast<double>(key) / 12.0);

	auto oGroup = getNextOscillatorGroup();
	for (auto oscillator : oGroup->oscillators) {
		oscillator->getFrequencyParameter()->multiplyValue(multiplier);
	}

	oGroup->volumeEnvelope->notePressed();
	// Trigger filter envelope too
	oGroup->midiKey = midiKey;
}

void Synth::keyReleased(int midiKey)
{	
	for (auto oGroup : oscillatorGroups) {
		if (oGroup->midiKey == midiKey) {
			oGroup->volumeEnvelope->noteReleased();
			// Trigger filter envelope too
			oGroup->midiKey = -1;
			break;
		}
	}
}

void Synth::pitchBend(double amount)
{
	
}

OscillatorGroup* Synth::getNextOscillatorGroup()
{
	auto oGroup = oscillatorGroups[oscillatorGroupsIndex];
	oscillatorGroupsIndex++;
	if (oscillatorGroupsIndex >= oscillatorGroups.size()) {
		oscillatorGroupsIndex = 0;
	}

	return oGroup;
}
