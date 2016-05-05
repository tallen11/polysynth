#pragma once

inline float convertRanges(float value, float oldMin, float oldMax, float newMin, float newMax)
{
	return (((value - oldMin) * (newMax - newMin)) / (oldMax - oldMin)) + newMin;
}