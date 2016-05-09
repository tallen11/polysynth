#pragma once

#include <vector>

class Effect
{
public:
	Effect();
	~Effect();
	virtual void processBuffer(std::vector<double> &samples, int bufferLength) = 0;	
};