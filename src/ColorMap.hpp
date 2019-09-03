#ifndef COLORMAP_INCLUDED
#define COLORMAP_INCLUDED

#pragma once

#include <math.h>

/*
	map normalized value [0 , 1] to r, g, b values [0 , 255]
*/

//inline void ColorMap(float value, int* r, int* g, int* b)
inline void ColorMap(float value, int* r, int* g, int* b)
{
	if (value > 1)
		value = 1;
	if (value <= 0.105714)
	{
		float increment = (value - 0.000000) / (0.105714 - 0.000000);
		*r = 255 + (167 - 255) * increment;
		*g = 255 + (22 - 255) * increment;
		*b = 150 + (255 - 150) * increment;

		return;
	}
	if (value <= 0.602857)
	{
		float increment = (value - 0.105714) / (0.602857 - 0.105714);
		*r = 167 + (28 - 167) * increment;
		*g = 22 + (255 - 22) * increment;
		*b = 255 + (255 - 255) * increment;

		return;
	}
	if (value <= 0.731429)
	{
		float increment = (value - 0.602857) / (0.731429 - 0.602857);
		*r = 28 + (53 - 28) * increment;
		*g = 255 + (155 - 255) * increment;
		*b = 255 + (255 - 255) * increment;

		return;
	}
	//if (value <= 0.834286)
	else
	{
		float increment = (value - 0.731429) / (0.834286 - 0.731429);
		*r = 53 + (255 - 53) * increment;
		*g = 155 + (65 - 155) * increment;
		*b = 255 + (155 - 255) * increment;

		return;
	}
	/*
	else
	{
		float increment = (value - 0.834286) / (1.000000 - 0.834286);
		*r = 255 + (153 - 255) * increment;
		*g = 65 + (0 - 65) * increment;
		*b = 155 + (0 - 155) * increment;

		return;
	}
	*/
}


#endif