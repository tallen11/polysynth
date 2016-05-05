#pragma once

inline double convertRanges(double value, double oldMin, double oldMax, double newMin, double newMax)
{
	return (((value - oldMin) * (newMax - newMin)) / (oldMax - oldMin)) + newMin;
}
