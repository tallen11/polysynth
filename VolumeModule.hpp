#pragma once

#include "Parameter.hpp"
#include "EnvelopeGenerator.hpp"

class VolumeModule
{
public:
	VolumeModule();
	VolumeModule(EnvelopeGenerator *envelope);
	~VolumeModule();
	double processSample(double sample);
	void setVolumeEnvelope(EnvelopeGenerator *envelope);
	EnvelopeGenerator* getVolumeEnvelope();
	Parameter* getVolumeParameter();
	// Parameter* getVolumeEnvelopeParameter();

private:
	Parameter *volumeParameter;
	EnvelopeGenerator *volumeEnvelope;
	// Parameter *volumeEnvelopeParameter;
};