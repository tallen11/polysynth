#pragma once

class Parameter
{
public:
	Parameter(double maxVal, double minVal, double val = 0.0);
	~Parameter();
	void resetValue();
	void setValue(double val);
	void multiplyValue(double mult);
	double getValue() const;

private:	
	double value;
	double baseValue;
	double maxValue;
	double minValue;
};