#pragma once

#include "Parameter.hpp"
#include "EnvelopeGenerator.hpp"
#include "LFO.hpp"

class VolumeModule
{
public:
	VolumeModule();
	VolumeModule(EnvelopeGenerator *envelope);
	~VolumeModule();
	double processSample(double sample);
	void setVolumeEnvelope(EnvelopeGenerator *envelope);
	void setVolumeLFO(LFO *lfo);
	EnvelopeGenerator* getVolumeEnvelope();
	Parameter* getVolumeParameter();
	// Parameter* getVolumeEnvelopeParameter();

private:
	Parameter *volumeParameter;
	EnvelopeGenerator *volumeEnvelope;
	LFO *volumeLFO;
	// Parameter *volumeEnvelopeParameter;
};