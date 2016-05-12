#include "Filter.hpp"
#include "Constants.hpp"
#include "Util.hpp"
#include <cmath>
#include <iostream>

Filter::Filter()
{
	f = 0; 
	pc = 0; 
	q = 0;
    bf0 = 0; 
    bf1 = 0; 
    bf2 = 0; 
    bf3 = 0; 
    bf4 = 0;
    t1 = 0; 
    t2 = 0;

	frequencyCutoffParameter = new Parameter(MIN_FREQUENCY, MAX_FREQUENCY, MAX_FREQUENCY, true);
	resonanceParameter = new Parameter(1.0, 0.0, 0.5, true);
	frequencyCutoffEnvelope = nullptr;
	filterLFO = nullptr;
}

Filter::~Filter()
{
	delete frequencyCutoffParameter;
	delete resonanceParameter;
}

void Filter::processBuffer(std::vector<double> &samples, int bufferLength)
{
	for (int i = 0; i < bufferLength; i++) {
		/* Calculate parameters */
		double fcp = frequencyCutoffParameter->getValue();
		double lfo = filterLFO == nullptr ? 0.0 : filterLFO->getMultiplier();
		double lfoAmp = filterLFO == nullptr ? 0.0 : filterLFO->getAmplitudeParameter()->getValue();
		double lfoFreq = convertRanges(lfo, -1.0, 1.0, MIN_FREQUENCY, fcp);
		double env = frequencyCutoffEnvelope == nullptr ? 1.0 : frequencyCutoffEnvelope->getNextMultiplier();
		double frequency = env * (fcp - lfoFreq * lfoAmp + 20.0) / (0.5 * SAMPLE_RATE);
	    double resonance = resonanceParameter->getValue();

	    if (frequency < 0)
	    	frequency = 0;
	    if (frequency > 0.6) 
	    	frequency = 0.6;

	    q = 1.0 - frequency;
	    pc = frequency + 0.8 * frequency * q;
	    f = pc + pc - 1.0;
	    q = resonance * (1.0 + 0.5 * q * (1.0 - q + 5.6 * q * q));

	    /* Do filtering */ 
        double in = samples[i];

        in -= q * bf4; //feedback
        t1 = bf1;  bf1 = (in + bf0) * pc - bf1 * f;
        t2 = bf2;  bf2 = (bf1 + t1) * pc - bf2 * f;
        t1 = bf3;  bf3 = (bf2 + t2) * pc - bf3 * f;
        bf4 = (bf3 + t1) * pc - bf4 * f;
        bf4 = bf4 - bf4 * bf4 * bf4 * 0.16666666666666;    //clipping
        bf0 = in;

        // Lowpass  output:  bf4
        // Highpass output:  in - bf4;
        // Bandpass output:  3.0f * (bf3 - bf4);
        samples[i] = bf4;

        filterLFO->stepLFO();
    }
}

void Filter::setFrequencyCutoffEnvelope(EnvelopeGenerator *envelope)
{
	frequencyCutoffEnvelope = envelope;
}

void Filter::setFilterLFO(LFO *lfo)
{
	filterLFO = lfo;
}

EnvelopeGenerator* Filter::getFrequencyCutoffEnvelope()
{
	return frequencyCutoffEnvelope;
}

Parameter* Filter::getFrequencyCutoffParameter()
{
	return frequencyCutoffParameter;
}

Parameter* Filter::getResonanceParameter()
{
	return resonanceParameter;
}

LFO* Filter::getFilterLFO()
{
	return filterLFO;
}
