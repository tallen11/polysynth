#include "Parameter.hpp"

#define P_CONST 0.001

Parameter::Parameter(double maxVal, double minVal, double defaultVal, bool smoothInterpolation)
{
	maxValue = maxVal;
	minValue = minVal;
	value = defaultVal;
	baseValue = defaultVal;
	seekValue = defaultVal;
	interpolates = smoothInterpolation;
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

double Parameter::getValue()
{
	if (interpolates) {
		double v = value;

		/* Update "PID" loop */
		double p = P_CONST * (seekValue - v);
		value += p;

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
