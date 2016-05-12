#pragma once

#include "Parameter.hpp"
#include "EnvelopeGenerator.hpp"
#include "LFO.hpp"
#include <vector>

/* Based on Moog filter filter */

class Filter
{
public:
	Filter();
	~Filter();
	void processBuffer(std::vector<double> &samples, int bufferLength);
	void setFrequencyCutoffEnvelope(EnvelopeGenerator *envelope);
	void setFilterLFO(LFO *lfo);
	EnvelopeGenerator* getFrequencyCutoffEnvelope();
	Parameter* getFrequencyCutoffParameter();
	Parameter* getResonanceParameter();
	LFO* getFilterLFO();

private:
	double f;
	double pc;
	double q;
    double bf0;
    double bf1;
    double bf2;
    double bf3;
    double bf4;
    double t1;
    double t2;

	Parameter *frequencyCutoffParameter;
	Parameter *resonanceParameter;
	EnvelopeGenerator *frequencyCutoffEnvelope;
	LFO *filterLFO;
	// EnvelopeGenerator *resonanceEnvelope;
};