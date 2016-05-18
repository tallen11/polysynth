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
	startMultiplier = 0.0;
	enabled = true;
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
	startMultiplier = 0.0;
	enabled = true;
}

EnvelopeGenerator::~EnvelopeGenerator()
{

}

double EnvelopeGenerator::getNextMultiplier()
{
	double mult = 0.0;
	switch (state) {
		case esWaiting: {
			// parameter->multiplyValue(0.0);
			mult = 0.0;

			lastMultiplier = 0.0;
			startMultiplier = 0.0;
			break;
		}

		case esAttacking: {
			stateCounter++;
			progressCounter++;
			if (stateCounter > attack) {
				state = esDecaying;
				mult = lastMultiplier;
				break;
			}

			double multiplier = ((1.0 - startMultiplier) / attack) * stateCounter + startMultiplier;
			mult = multiplier;
			// parameter->multiplyValue(multiplier);

			lastMultiplier = multiplier;
			break;
		}

		case esDecaying: {
			stateCounter++;
			progressCounter++;
			if (stateCounter > attack + decay) {
				state = esSustaining;
				mult = lastMultiplier;
				break;
			}

			double multiplier = ((sustain - 1.0) / decay) * (stateCounter - attack) + 1.0;
			mult = multiplier;
			// parameter->multiplyValue(multiplier);

			lastMultiplier = multiplier;
			startMultiplier = multiplier;
			break;
		}

		case esSustaining: {
			stateCounter++;
			progressCounter++;

			mult = sustain;
			// parameter->multiplyValue(sustain);

			lastMultiplier = sustain;
			startMultiplier = sustain;
			break;
		}

		case esReleasing: {
			stateCounter++;
			if (stateCounter > release + progressCounter) {
				state = esWaiting;
				lastMultiplier = 0.0;
				break;
			}

			double multiplier = (-lastMultiplier / release) * (stateCounter - progressCounter) + lastMultiplier;
			mult = multiplier;
			// parameter->multiplyValue(multiplier);
			
			startMultiplier = multiplier;
			break;
		}
	}

	return mult;
}

void EnvelopeGenerator::notePressed()
{
	state = esAttacking;
	stateCounter = 0;
	progressCounter = 0;
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

bool EnvelopeGenerator::isEnabled()
{
	return enabled;
}

void EnvelopeGenerator::setEnabled(bool setting)
{
	enabled = setting;
}

// void EnvelopeGenerator::setParameter(Parameter *parameter)
// {
// 	this->parameter = parameter;
// }
