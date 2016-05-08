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
		// auto o2 = new Oscillator();
		auto o3 = new Oscillator();
		o1->getFrequencyParameter()->setValue(440.0);
		// o2->getFrequencyParameter()->setValue(442.0);
		o3->getFrequencyParameter()->setValue(444.0);

		oGroup->oscillators.push_back(o1);
		// oGroup->oscillators.push_back(o2);
		oGroup->oscillators.push_back(o3);

		auto env = new EnvelopeGenerator(0.02, 0.1, 0.9, 0.02);
		env->setParameter(oGroup->volumeModule.getVolumeEnvelopeParameter());
		envelopes.push_back(env);
		oGroup->volumeEnvelope = env;

		oscillatorGroups.push_back(oGroup);
	}

	// auto osc = new Oscillator();
	// auto osc2 = new Oscillator();
	// osc2->getFrequencyParameter()->setValue(444.0);

	// auto env = new EnvelopeGenerator(0.5, 0.1, 0.9, 0.1);
	// env->setParameter(masterVolumeModule.getVolumeEnvelopeParameter());

	// oscillators.push_back(osc);
	// oscillators.push_back(osc2);
	// envelopes.push_back(env);
}

Synth::~Synth()
{
	// for (auto oscillator : oscillators) {
	// 	delete oscillator;
	// }

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
			// if (subSample != 0.0) {
				sample += subSample;
				++sampleCount;
			// }
		}

		sample /= static_cast<double>(sampleCount);
		// =========== Mixer Goes Here =================

		sampleBuffer[i] = masterVolumeModule.processSample(sample);
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





	// for (auto oscillator : oscillators) {
	// 	oscillator->getFrequencyParameter()->multiplyValue(multiplier);
	// }

	// for (auto envelope : envelopes) {
	// 	envelope->notePressed();
	// }
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

	// for (auto envelope : envelopes) {
	// 	envelope->noteReleased();
	// }
}

void Synth::pitchBend(double amount)
{
	// for (auto oscillator : oscillators) {
	// 	oscillator->getFrequencyParameter()->multiplyValue(amount);
	// }
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
