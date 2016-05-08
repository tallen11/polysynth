#pragma once

#include "Parameter.hpp"

class VolumeModule
{
public:
	VolumeModule();
	~VolumeModule();
	double processSample(double sample);
	Parameter* getVolumeParameter();
	Parameter* getVolumeEnvelopeParameter();

private:
	Parameter *volumeParameter;
	Parameter *volumeEnvelopeParameter;
};