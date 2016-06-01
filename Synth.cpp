#include "Synth.hpp"
#include "Constants.hpp"
#include "Effects/EffectOverdrive.hpp"
#include "Effects/EffectBitcrusher.hpp"
#include <cmath>
#include <iostream>

Synth::Synth() : sampleBuffer(BUFFER_SIZE, 0.0)
{
	/* Create the starting wavetables */
//	leftWaveTable = new WaveTable(wtSawtooth);
//	rightWaveTable = new WaveTable(wtExperimental);
    sawtoothWavetable = new WaveTable(wtSawtooth);
    squareWavetable = new WaveTable(wtSquare);
    sineWavetable = new WaveTable(wtSine);

	/* Setup LFOs */
	volumeLFO = new LFO();
	volumeLFO->getFrequencyParameter()->setValue(0.0);
	volumeLFO->getAmplitudeParameter()->setValue(0.1);

	/* Create the oscillators and their envelopes, filters, and filter envelopes */
	for (int i = 0; i < 32; ++i) {
		auto oGroup = new OscillatorGroup;

		auto o1 = new Oscillator(sawtoothWavetable, squareWavetable);
        auto o2 = new Oscillator(sawtoothWavetable, squareWavetable);
		// auto o3 = new Oscillator(leftWaveTable, rightWaveTable);
		// auto o4 = new Oscillator(leftWaveTable, rightWaveTable);

		o1->getFrequencyParameter()->setValue(440.0);
        o2->getFrequencyParameter()->setValue(440.0);
		// o3->getFrequencyParameter()->setValue(442.0);
		// o4->getFrequencyParameter()->setValue(220.0);
        
        o1->setVoiceCount(3);
        o1->getVoiceDetuneFactorParameter()->setValue(0.00454545);
        
        o2->setVoiceCount(1);
        o2->getVoiceDetuneFactorParameter()->setValue(0.0);

		o1->getTableParameter()->setValue(0.0);
        o2->getTableParameter()->setValue(0.0);
        // o3->getTableParameter()->setValue(0.0);
		// o4->getTableParameter()->setValue(0.0);
        
        o2->toggleOscillator();

		oGroup->oscillators.push_back(o1);
        oGroup->oscillators.push_back(o2);
		// oGroup->oscillators.push_back(o3);
		// oGroup->oscillators.push_back(o4);

		auto env = new EnvelopeGenerator(0.1, 0.01, 0.9, 0.25);
		oGroup->volumeModule.setVolumeEnvelope(env);
		envelopes.push_back(env);

		auto fEnv = new EnvelopeGenerator(0.5, 0.5, 0.5, 0.5);
		fEnv->setEnabled(false);
		oGroup->filter.setFrequencyCutoffEnvelope(fEnv);
		envelopes.push_back(fEnv);

		auto fLFO = new LFO();
		fLFO->getFrequencyParameter()->setValue(0.0);
		fLFO->getAmplitudeParameter()->setValue(0.0);
		oGroup->filter.setFilterLFO(fLFO);

		oscillatorGroups.push_back(oGroup);
	}

	masterVolumeModule.setVolumeLFO(volumeLFO);

	/* Create the effects loop */
	// auto overdrive = new EffectOverdrive();
	// auto bitcrusher = new EffectBitcrusher();
	// overdrive->setEnabled(false);
	// bitcrusher->setEnabled(false);
	// effectsLoop.addEffect(overdrive);
	// effectsLoop.addEffect(bitcrusher);

	oscillatorGroupsIndex = 0;
}

Synth::~Synth()
{
	for (auto oGroup : oscillatorGroups) {
		for (auto oscillator : oGroup->oscillators) {
			delete oscillator;
		}

		delete oGroup->filter.getFilterLFO();
		delete oGroup;
	}

	for (auto envelope : envelopes) {
		delete envelope;
	}

    delete sawtoothWavetable;
    delete squareWavetable;
    delete sineWavetable;
    delete volumeLFO;
//	delete leftWaveTable;
//	delete rightWaveTable;
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
		volumeLFO->stepLFO();
	}

	return sampleBuffer;
}

void Synth::setMasterVolume(double volume)
{
	// masterVolumeModule.getVolumeParameter()->setSeekValue(volume);
	for (auto oGroup : oscillatorGroups) {
		for (auto oscillator : oGroup->oscillators) {
			oscillator->getTableParameter()->setSeekValue(volume);
		}
	}
}

void Synth::keyPressed(int midiKey, double velocity)
{
	int key = -(REFERENCE_MIDI - midiKey);
	double multiplier = pow(2.0, static_cast<double>(key) / 12.0);

	auto oGroup = getNextOscillatorGroup();
	for (auto oscillator : oGroup->oscillators) {
		oscillator->resetOscillator();
		oscillator->getFrequencyParameter()->multiplyValue(multiplier);
		// oscillator->getFrequencyParameter()->setValue(multiplier * REFERENCE_FREQUENCY);
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

// Modify Synth Parameter methods here

void Synth::pitchBend(double amount)
{
	// for (auto oGroup : oscillatorGroups) {
	// 	for (auto oscillator : oGroup->oscillators) {
	// 		// if (amount == 0.0) {
	// 			// oscillator->getFrequencyParameter()->resetValue();
	// 		// } else {
	// 			oscillator->getFrequencyParameter()->multiplySeekValue(amount);
	// 		// }
	// 	}
	// }
}

void Synth::setMainFilterPitchCutoff(double cutoff)
{
    double adjusted = pow(pow(10.0, cutoff), 0.1);
    double conversion = convertRanges(adjusted, 1.0, pow(10.0, 0.1), 20.0, 20000.0);
    for (auto oGroup : oscillatorGroups) {
        oGroup->filter.getFrequencyCutoffParameter()->setSeekValue(conversion);
    }
}

void Synth::setMainFilterResonance(double resonance)
{
    for (auto oGroup : oscillatorGroups) {
        oGroup->filter.getResonanceParameter()->setSeekValue(resonance);
    }
}

void Synth::setVolumeLFOFrequency(double frequency)
{
    volumeLFO->getFrequencyParameter()->setSeekValue(frequency);
}

void Synth::setVolumeLFOAmplitude(double amplitude)
{
    volumeLFO->getAmplitudeParameter()->setSeekValue(amplitude);
}

void Synth::setFilterLFOFrequency(double frequency)
{
    for (auto oGroup : oscillatorGroups) {
        oGroup->filter.getFilterLFO()->getFrequencyParameter()->setSeekValue(frequency);
    }
}

void Synth::setFilterLFOAmplitude(double amplitude)
{
    for (auto oGroup : oscillatorGroups) {
        oGroup->filter.getFilterLFO()->getAmplitudeParameter()->setSeekValue(amplitude);
    }
}

void Synth::setNoteAttackDuration(double duration)
{
    for (auto oGroup : oscillatorGroups) {
        oGroup->volumeModule.getVolumeEnvelope()->setAttack(duration);
    }
}

void Synth::setNoteDecayDuration(double duration)
{
    for (auto oGroup : oscillatorGroups) {
        oGroup->volumeModule.getVolumeEnvelope()->setDecay(duration);
    }
}

void Synth::setNoteSustainLevel(double level)
{
    for (auto oGroup : oscillatorGroups) {
        oGroup->volumeModule.getVolumeEnvelope()->setSustain(level);
    }
}

void Synth::setNoteReleaseDuration(double duration)
{
    for (auto oGroup : oscillatorGroups) {
        oGroup->volumeModule.getVolumeEnvelope()->setRelease(duration);
    }
}

void Synth::toggleOscillator2()
{
    for (auto oGroup : oscillatorGroups) {
        oGroup->oscillators[1]->toggleOscillator();
    }
}

void Synth::setOscillatorWavetable(int oscillatorID, int wavetableID, int wavetable)
{
    WaveTable *table = nullptr;
    switch (wavetable) {
        case 1:
            table = sawtoothWavetable;
            break;
        case 2:
            table = squareWavetable;
            break;
        case 3:
            table = sineWavetable;
            break;
        default:
            break;
    }
        
    if (wavetableID == 0) {
        for (auto oGroup : oscillatorGroups) {
            oGroup->oscillators[oscillatorID]->resetOscillator();
            oGroup->oscillators[oscillatorID]->setLeftWavetable(table);
        }
    } else {
        for (auto oGroup : oscillatorGroups) {
            oGroup->oscillators[oscillatorID]->resetOscillator();
            oGroup->oscillators[oscillatorID]->setRightWavetable(table);
        }
    }
}

void Synth::setTableFadePercentage(int oscillatorID, double percentage)
{
    for (auto oGroup : oscillatorGroups) {
        oGroup->oscillators[oscillatorID]->getTableParameter()->setValue(percentage);
    }
}

void Synth::setOscillatorVoiceCount(int oscillatorID, int count)
{
    for (auto oGroup : oscillatorGroups) {
        oGroup->oscillators[oscillatorID]->setVoiceCount(count);
    }
}

void Synth::setFilterType(FilterType type)
{
    for (auto oGroup : oscillatorGroups) {
        oGroup->filter.setFilterType(type);
    }
}

// Modify Synth Parameter methods here


inline OscillatorGroup* Synth::getNextOscillatorGroup()
{
	// OscillatorGroup *oGroup = oscillatorGroups[oscillatorGroupsIndex];
	// if (oGroup->midiKey != -1) {
	// 	for (int i = 0; i < oscillatorGroups.size(); ++i) {
	// 		auto g = oscillatorGroups[i];
	// 		if (g->midiKey != -1) {
	// 			oGroup = g;
	// 			break;
	// 		}

	// 		oGroup = nullptr;
	// 	}
	// }

	// if (oGroup == nullptr) {
	// 	oGroup = oscillatorGroups[oscillatorGroupsIndex];
	// }

	// oscillatorGroupsIndex++;
	auto oGroup = oscillatorGroups[oscillatorGroupsIndex];
	oscillatorGroupsIndex++;
	if (oscillatorGroupsIndex >= oscillatorGroups.size()) {
		oscillatorGroupsIndex = 0;
	}

	return oGroup;
}
