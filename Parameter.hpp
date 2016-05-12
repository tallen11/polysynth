#pragma once

class Parameter
{
public:
	Parameter(double maxVal, double minVal, double defaultVal, bool smoothInterpolation = false);
	~Parameter();
	void resetValue();
	void setValue(double val);
	void setSeekValue(double val);
	void multiplyValue(double mult);
	void setInterpolates(bool smoothInterpolation);
	double getValue();

private:	
	double maxValue;
	double minValue;
	double value;
	double baseValue;
	double seekValue;
	bool interpolates;
};