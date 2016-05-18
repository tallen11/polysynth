#include "Parameter.hpp"

#define P_CONST 0.0005
#define I_CONST 0.01
#define D_CONST 0.001

Parameter::Parameter(double maxVal, double minVal, double defaultVal, bool smoothInterpolation)
{
	maxValue = maxVal;
	minValue = minVal;
	value = defaultVal;
	baseValue = defaultVal;
	seekValue = defaultVal;
	interpolates = smoothInterpolation;
	lastSeekValue = 0.0;
	errorSum = 0.0;
}

Parameter::~Parameter()
{

}

void Parameter::resetValue()
{
	value = baseValue;
	seekValue = baseValue;
}

void Parameter::setValue(double val)
{
	value = val;
	baseValue = val;
	seekValue = val;
}

void Parameter::multiplyValue(double mult)
{
	value = baseValue * mult;
	seekValue = value;
}

void Parameter::multiplySeekValue(double mult)
{
	seekValue = value * mult;
}

double Parameter::getValue()
{
	if (interpolates) {
		double v = value;

		/* Update "PID" loop */
		double p = P_CONST * (seekValue - v);
		// double i = I_CONST * errorSum;


		value += p; // + i;

		// errorSum += I_CONST * p;

		return v;
	}

	return value;
}

void Parameter::setSeekValue(double val)
{
	seekValue = val;
}

void Parameter::setInterpolates(bool smoothInterpolation)
{
	interpolates = smoothInterpolation;
}
