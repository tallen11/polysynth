#pragma once

#include "Parameter.hpp"
#include <stdint.h>

enum EnvelopeState {
	esWaiting = 0,
	esAttacking,
	esDecaying,
	esSustaining,
	esReleasing
};

class EnvelopeGenerator
{
public:
	EnvelopeGenerator();
	EnvelopeGenerator(double attack, double decay, double sustain, double release);
	~EnvelopeGenerator();
	double getNextMultiplier();
	void notePressed();
	void noteReleased();
	void setAttack(double attack);
	void setDecay(double decay);
	void setSustain(double sustain);
	void setRelease(double release);
	bool isEnabled();
	void setEnabled(bool setting);
	// void setParameter(Parameter *parameter);

private:
	double attack;
	double decay;
	double sustain;
	double release;
	double lastMultiplier;
	double startMultiplier;
	// Parameter *parameter;
	EnvelopeState state;
	uint64_t stateCounter;
	uint64_t progressCounter;
	bool enabled;
};
