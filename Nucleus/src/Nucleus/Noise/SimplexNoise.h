#pragma once



namespace Nucleus {

	class SimplexNoise {
	public:
		static float Noise(float x);
		static float Noise(float x, float y);
		static float Noise(float x, float y, float z);
	};

}