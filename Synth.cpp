#include "Synth.hpp"
#include "Constants.hpp"
#include "Effects/EffectOverdrive.hpp"
#include "Effects/EffectBitcrusher.hpp"
#include <cmath>
#include <iostream>

Synth::Synth() : sampleBuffer(BUFFER_SIZE, 0.0)
{
	oscillatorGroupsIndex = 0;

	auto overdrive = new EffectOverdrive();
	// auto bitcrusher = new EffectBitcrusher();
	effectsLoop.addEffect(overdrive);
	// effectsLoop.addEffect(bitcrusher);

	for (int i = 0; i < 8; ++i) {
		auto oGroup = new OscillatorGroup;

		auto o1 = new Oscillator();
		auto o2 = new Oscillator();
		auto o3 = new Oscillator();
		auto o4 = new Oscillator();

		o1->getFrequencyParameter()->setValue(440.0);
		o2->getFrequencyParameter()->setValue(442.0);
		o3->getFrequencyParameter()->setValue(444.0);
		o4->getFrequencyParameter()->setValue(220.0);

		o1->getTableParameter()->setValue(0.0);
		o2->getTableParameter()->setValue(0.0);
		o3->getTableParameter()->setValue(0.0);

		oGroup->oscillators.push_back(o1);
		oGroup->oscillators.push_back(o2);
		oGroup->oscillators.push_back(o3);
		oGroup->oscillators.push_back(o4);

		auto env = new EnvelopeGenerator(0.05, 0.01, 0.9, 0.05);
		oGroup->volumeModule.setVolumeEnvelope(env);
		envelopes.push_back(env);

		auto fEnv = new EnvelopeGenerator(0.1, 0.1, 1.0, 1.0);
		oGroup->filter.setFrequencyCutoffEnvelope(fEnv);
		envelopes.push_back(fEnv);

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
	auto oGroup1 = oscillatorGroups[0];
	auto subBuffer1 = oGroup1->getNextBuffer(bufferLength);
	for (int i = 0; i < bufferLength; ++i) {
		sampleBuffer[i] = subBuffer1[i];
	}

	for (int i = 1; i < oscillatorGroups.size(); ++i) {
		auto oGroup = oscillatorGroups[i];
		auto subBuffer = oGroup->getNextBuffer(bufferLength);
		for (int i = 0; i < bufferLength; ++i) {
			sampleBuffer[i] += subBuffer[i];
		}
	}

	for (int i = 0; i < bufferLength; ++i) {
		sampleBuffer[i] /= oscillatorGroups.size();
	}

	// effectsLoop.processBuffer(sampleBuffer, bufferLength);

	for (int i = 0; i < bufferLength; ++i) {
		sampleBuffer[i] = masterVolumeModule.processSample(sampleBuffer[i]);
	}


	// for (int i = 0; i < bufferLength; ++i) {
	// 	// for (auto envelope : envelopes) {
	// 	// 	envelope->update();
	// 	// }

	// 	double sample = 0.0;
	// 	int sampleCount = 0;
	// 	// =========== Mixer Goes Here =================
	// 	for (auto oGroup : oscillatorGroups) {
	// 		double subSample = oGroup->getNextSample();
	// 		sample += subSample;
	// 		++sampleCount;
	// 	}

	// 	sample /= static_cast<double>(sampleCount);
	// 	// =========== Mixer Goes Here =================

	// 	sampleBuffer[i] = masterVolumeModule.processSample(sample);
	// }



	return sampleBuffer;
}

void Synth::setMasterVolume(double volume)
{
	masterVolumeModule.getVolumeParameter()->setValue(volume);
}

void Synth::keyPressed(int midiKey, double velocity)
{
	int key = -(REFERENCE_MIDI - midiKey);
	double multiplier = pow(2.0, static_cast<double>(key) / 12.0);

	auto oGroup = getNextOscillatorGroup();
	for (auto oscillator : oGroup->oscillators) {
		oscillator->getFrequencyParameter()->multiplyValue(multiplier);
	}

	// oGroup->volumeModule.getVolumeParameter()->setValue(velocity);
	oGroup->volumeModule.getVolumeEnvelope()->notePressed();
	oGroup->filter.getFrequencyCutoffEnvelope()->notePressed();
	oGroup->midiKey = midiKey;
}

void Synth::keyReleased(int midiKey)
{	
	for (auto oGroup : oscillatorGroups) {
		if (oGroup->midiKey == midiKey) {
			oGroup->volumeModule.getVolumeEnvelope()->noteReleased();
			oGroup->filter.getFrequencyCutoffEnvelope()->noteReleased();
			oGroup->midiKey = -1;
			break;
		}
	}
}

void Synth::pitchBend(double amount)
{
	
}

inline OscillatorGroup* Synth::getNextOscillatorGroup()
{
	auto oGroup = oscillatorGroups[oscillatorGroupsIndex];
	oscillatorGroupsIndex++;
	if (oscillatorGroupsIndex >= oscillatorGroups.size()) {
		oscillatorGroupsIndex = 0;
	}

	return oGroup;
}
