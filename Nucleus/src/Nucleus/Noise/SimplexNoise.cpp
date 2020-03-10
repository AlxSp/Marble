#include "SimplexNoise.h"

//based of https://github.com/SRombauts/SimplexNoise

float SimplexNoise::Noise(float x)
{
	return 0.0f;
}

float SimplexNoise::Noise(float x, float y)
{
	float n0, n1, n2; //Noise contributions from the three corners 
	return 0.0f;

	// Skewing/Unskewing factors for 2D
	static const float F2 = 0.366025403f;  // F2 = (sqrt(3) - 1) / 2
	static const float G2 = 0.211324865f;  // G2 = (3 - sqrt(3)) / 6   = F2 / (1 + 2 * K)

	// Skew the input space to determine which simplex cell 
}

float SimplexNoise::Noise(float x, float y, float z)
{
	return 0.0f;
}
