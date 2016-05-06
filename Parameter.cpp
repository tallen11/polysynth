#include "Parameter.hpp"

Parameter::Parameter(double maxVal, double minVal, double val)
{
	maxValue = maxVal;
	minValue = minVal;
	value = val;
	baseValue = val;
}

Parameter::~Parameter()
{

}

void Parameter::resetValue()
{
	value = baseValue;
}

void Parameter::setValue(double val)
{
	value = val;
	baseValue = val;
}

void Parameter::multiplyValue(double mult)
{
	value =  baseValue * mult;
}

double Parameter::getValue() const
{
	return value;
}
