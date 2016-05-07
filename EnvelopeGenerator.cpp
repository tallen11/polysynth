#include "EnvelopeGenerator.hpp"
#include "Constants.hpp"
#include <iostream>

EnvelopeGenerator::EnvelopeGenerator()
{
	state = esWaiting;
	stateCounter = 0;
	progressCounter = 0;
	attack = SAMPLE_RATE;
	decay = SAMPLE_RATE;
	sustain = 1.0;
	release = SAMPLE_RATE;
	lastMultiplier = 0.0;
}

EnvelopeGenerator::EnvelopeGenerator(double attack, double decay, double sustain, double release)
{
	state = esWaiting;
	stateCounter = 0;
	progressCounter = 0;
	this->attack = SAMPLE_RATE * attack;
	this->decay = SAMPLE_RATE * decay;
	this->sustain = sustain;
	this->release = SAMPLE_RATE * release;
	lastMultiplier = 0.0;
}

EnvelopeGenerator::~EnvelopeGenerator()
{

}

void EnvelopeGenerator::update()
{
	switch (state) {
		case esWaiting: {
			parameter->multiplyValue(0.0);
			break;
		}

		case esAttacking: {
			stateCounter++;
			progressCounter++;
			if (stateCounter > attack) {
				state = esDecaying;
				break;
			}

			double multiplier = (1.0 / attack) * stateCounter;
			parameter->multiplyValue(multiplier);
			lastMultiplier = multiplier;
			break;
		}

		case esDecaying: {
			stateCounter++;
			progressCounter++;
			if (stateCounter > attack + decay) {
				state = esSustaining;
				break;
			}

			double multiplier = ((sustain - 1.0) / decay) * (stateCounter - attack) + 1.0;
			parameter->multiplyValue(multiplier);
			lastMultiplier = multiplier;
			break;
		}

		case esSustaining: {
			stateCounter++;
			progressCounter++;
			parameter->multiplyValue(sustain);
			lastMultiplier = sustain;
			break;
		}

		case esReleasing: {
			stateCounter++;
			if (stateCounter > release + progressCounter) {
				state = esWaiting;
				break;
			}

			double multiplier = (-lastMultiplier / release) * (stateCounter - progressCounter) + lastMultiplier;
			parameter->multiplyValue(multiplier);
			break;
		}
	}
}

void EnvelopeGenerator::notePressed()
{
	state = esAttacking;
	stateCounter = 0;
	progressCounter = 0;
	lastMultiplier = 0.0;
}

void EnvelopeGenerator::noteReleased()
{
	state = esReleasing;
}

void EnvelopeGenerator::setAttack(double attack)
{
	// attack is measured in seconds and converted to samples
	this->attack = SAMPLE_RATE * attack;
}

void EnvelopeGenerator::setDecay(double decay)
{
	// attack is measured in seconds and converted to samples
	this->decay = SAMPLE_RATE * decay;
}

void EnvelopeGenerator::setSustain(double sustain)
{
	// This should be between 0.0 and 1.0
	this->sustain = sustain;
}

void EnvelopeGenerator::setRelease(double release)
{
	// attack is measured in seconds and converted to samples
	this->release = SAMPLE_RATE * release;
}

void EnvelopeGenerator::setParameter(Parameter *parameter)
{
	this->parameter = parameter;
}
