#include "VolumeModule.hpp"

VolumeModule::VolumeModule(EnvelopeGenerator *envelope)
{
	volumeParameter = new Parameter(1.0, 0.0, 1.0);
	// volumeEnvelopeParameter = new Parameter(1.0, 0.0, 1.0);
	volumeEnvelope = envelope;
}

VolumeModule::VolumeModule()
{
	volumeParameter = new Parameter(1.0, 0.0, 1.0);
	volumeEnvelope = nullptr;
}

VolumeModule::~VolumeModule()
{
	delete volumeParameter;
	// delete volumeEnvelopeParameter;
}

double VolumeModule::processSample(double sample)
{
	// return volumeParameter->getValue() * volumeEnvelopeParameter->getValue() * sample;
	double env = volumeEnvelope == nullptr ? 1.0 : volumeEnvelope->getNextMultiplier();
	return volumeParameter->getValue() * env * sample;
}

void VolumeModule::setVolumeEnvelope(EnvelopeGenerator *envelope)
{
	volumeEnvelope = envelope;
}

EnvelopeGenerator* VolumeModule::getVolumeEnvelope()
{
	return volumeEnvelope;
}

Parameter* VolumeModule::getVolumeParameter()
{
	return volumeParameter;
}

// Parameter* VolumeModule::getVolumeEnvelopeParameter()
// {
// 	return volumeEnvelopeParameter;
// }
