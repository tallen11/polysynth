#include "Filter.hpp"
#include "Constants.hpp"
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

	frequencyCutoffParameter = new Parameter(MIN_FREQUENCY, MAX_FREQUENCY, MAX_FREQUENCY);
	resonanceParameter = new Parameter(1.0, 0.0, 0.0);
	frequencyCutoffEnvelope = nullptr;
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
		double frequency = frequencyCutoffParameter->getValue() / (0.5 * SAMPLE_RATE);
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
    }






	// /* May need separate buffers for input/output */
	// for (int i = 0; i < bufferLength; ++i) {

	// 	/* Process coefficients */
	// 	double env = frequencyCutoffEnvelope == nullptr ? 1.0 : frequencyCutoffEnvelope->getNextMultiplier();
	// 	double fcp = frequencyCutoffParameter->getValue() * env;

	// 	double g = tan((M_PI * fcp) / SAMPLE_RATE);
	// 	k = 1.0 - (0.99 * resonanceParameter->getValue());
	// 	double ginv = g / (1.0 + g * (g + k));
	// 	g1 = ginv;
	// 	g2 = 2.0 * (g + k) * ginv;
	// 	g3 = g * ginv;
	// 	g4 = 2.0 * ginv;

	// 	double v0 = samples[i];
	// 	double v1z = v1;
	// 	double v2z = v2;
	// 	double v3 = v0 + v0z - 2.0 * v2z;

	// 	v1 += g1 * v3 - g2 * v1z;
	// 	v2 += g3 * v3 + g4 * v1z;
	// 	v0z = v0;

	// 	samples[i] = v2;

	// 	// std::cout << env << std::endl;
	// }
}

void Filter::setFrequencyCutoffEnvelope(EnvelopeGenerator *envelope)
{
	frequencyCutoffEnvelope = envelope;
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

