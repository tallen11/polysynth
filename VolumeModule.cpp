#include "VolumeModule.hpp"

VolumeModule::VolumeModule()
{
	volumeParameter = new Parameter(1.0, 0.0, 1.0);
	volumeEnvelopeParameter = new Parameter(1.0, 0.0, 1.0);
}

VolumeModule::~VolumeModule()
{
	delete volumeParameter;
	delete volumeEnvelopeParameter;
}

double VolumeModule::processSample(double sample)
{
	return volumeParameter->getValue() * volumeEnvelopeParameter->getValue() * sample;
}

Parameter* VolumeModule::getVolumeParameter()
{
	return volumeParameter;
}

Parameter* VolumeModule::getVolumeEnvelopeParameter()
{
	return volumeEnvelopeParameter;
}
