#pragma once

#include "Parameter.hpp"
#include "EnvelopeGenerator.hpp"
#include <vector>

/* Based on linear trapezoidal integrated state variable filter */

class Filter
{
public:
	Filter();
	~Filter();
	void processBuffer(std::vector<double> &samples, int bufferLength);
	void setFrequencyCutoffEnvelope(EnvelopeGenerator *envelope);

private:
	double v0z;
	double v1;
	double v2;
	
	double k;
	double g1;
	double g2;
	double g3;
	double g4;

	Parameter *frequencyCutoffParameter;
	Parameter *resonanceParameter;
	EnvelopeGenerator *frequencyCutoffEnvelope;
	// EnvelopeGenerator *resonanceEnvelope;
};