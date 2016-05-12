#include "Effect.hpp"

Effect::Effect() {
	enabled = true;
}

Effect::~Effect() { }

bool Effect::isEnabled()
{
	return enabled;
}

void Effect::setEnabled(bool setting)
{
	enabled = setting;
}
