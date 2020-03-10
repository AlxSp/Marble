#pragma once

#include "ncpch.h"

class SimplexNoise {
	public:
		static float Noise(float x);
		static float Noise(float x, float y);
		static float Noise(float x, float y, float z);
};