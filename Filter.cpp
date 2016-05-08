#include "Filter.hpp"
#include "Constants.hpp"
#include <cmath>

Filter::Filter()
{
	v0z = 0.0;
	v1 = 0.0;
	v2 = 0.0;

	frequencyCutoffParameter = new Parameter(MIN_FREQUENCY, MAX_FREQUENCY, MIN_FREQUENCY);
	resonanceParameter = new Parameter(1.0, 0.0, 0.0);
}

Filter::~Filter()
{
	delete frequencyCutoffParameter;
	delete resonanceParameter;
}

void Filter::processBuffer(std::vector<double> &samples, int bufferLength)
{
	/* Process coefficients */
	double g = tan((M_PI * frequencyCutoffParameter->getValue()) / SAMPLE_RATE);
	k = 1.0 - (0.99 * resonanceParameter->getValue());
	double ginv = g / (1.0 + g * (g + k));
	g1 = ginv;
	g2 = 2.0 * (g + k) * ginv;
	g3 = g * ginv;
	g4 = 2.0 * ginv;

	/* May need separate buffers for input/output */
	for (int i = 0; i < bufferLength; ++i) {
		double v0 = samples[i];
		double v1z = v1;
		double v2z = v2;
		double v3 = v0 + v0z - 2.0 * v2z;

		v1 += g1 * v3 - g2 * v1z;
		v2 += g3 * v3 + g4 * v1z;
		v0z = v0;

		samples[i] = v2;
	}
}
