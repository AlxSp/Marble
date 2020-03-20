#pragma once



namespace Nucleus {

	class SimplexNoise {
	public:
		SimplexNoise(float frequency = 1.0f, float amplitude = 1.0f, float lacunarity = 2.0f, float persistence = 0.5f) :
			m_Frequency(frequency), m_Amplitude(amplitude), m_Lacunarity(lacunarity), m_Persistence(persistence) {}

		static float Noise(float x);
		static float Noise(float x, float y);
		static float Noise(float x, float y, float z);
		static float Noise(float x, float y, float z, float w);

		static float extrapolate(float x, float y, const size_t octaves, float frequency, float amplitude, const float lacunarity, const float persistence);


		float extrapolate(float x, float y, size_t octaves);

	private:
		float m_Frequency;
		float m_Amplitude;
		float m_Lacunarity;
		float m_Persistence;
	};

}