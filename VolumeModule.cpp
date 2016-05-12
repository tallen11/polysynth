#include "VolumeModule.hpp"

VolumeModule::VolumeModule(EnvelopeGenerator *envelope)
{
	volumeParameter = new Parameter(1.0, 0.0, 1.0, true);
	volumeEnvelope = envelope;
	volumeLFO = nullptr;
}

VolumeModule::VolumeModule()
{
	volumeParameter = new Parameter(1.0, 0.0, 1.0, true);
	volumeEnvelope = nullptr;
	volumeLFO = nullptr;
}

VolumeModule::~VolumeModule()
{
	delete volumeParameter;
}

double VolumeModule::processSample(double sample)
{
	double env = volumeEnvelope == nullptr ? 1.0 : volumeEnvelope->getNextMultiplier();
	double lfo = volumeLFO == nullptr ? 0.0 : volumeLFO->getMultiplier();
	double lfoAmp = volumeLFO == nullptr ? 0.0 : volumeLFO->getAmplitudeParameter()->getValue();
	return volumeParameter->getValue() * env * sample - (lfo * lfoAmp * sample);
}

void VolumeModule::setVolumeEnvelope(EnvelopeGenerator *envelope)
{
	volumeEnvelope = envelope;
}

void VolumeModule::setVolumeLFO(LFO *lfo)
{
	volumeLFO = lfo;
}

EnvelopeGenerator* VolumeModule::getVolumeEnvelope()
{
	return volumeEnvelope;
}

Parameter* VolumeModule::getVolumeParameter()
{
	return volumeParameter;
}
