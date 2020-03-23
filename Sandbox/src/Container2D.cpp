#include "Container2D.h"

NoiseContainer2D::NoiseContainer2D(const glm::vec2& postion, const  glm::vec2& size, const uint32_t& detail)
    : VisibleContainer2D(postion, size), Detail(detail), TexturePixels(new uint8_t[detail * detail * Channels]), NoiseTexture(Nucleus::Texture2D::Create(detail, detail))
{
	StepSize.x = Size.x / detail;
	StepSize.y = Size.y / detail;
    NoiseTexture->SetData(TexturePixels, sizeof(uint8_t) * Detail * Detail * Channels);
}

void NoiseContainer2D::FillPixels(float xOffset, float yOffset)
{
	int rowPtr, columnPtr;

	for (int i = 0; i < Detail; i++) {
		for (int j = 0; j < Detail; j++) {
			float value = (Nucleus::SimplexNoise::extrapolate(i * StepSize.x + xOffset, j * StepSize.y + yOffset, m_Octaves, m_Frequency, m_Amplitude, m_Lacunarity, m_Persistence));
			value = (value + 1) / 2 * 255;

			rowPtr = i * Channels;
			columnPtr = j * Detail * Channels;

			TexturePixels[rowPtr + columnPtr + 0] = (uint8_t)value;
			TexturePixels[rowPtr + columnPtr + 1] = (uint8_t)value;
			TexturePixels[rowPtr + columnPtr + 2] = (uint8_t)value;
			TexturePixels[rowPtr + columnPtr + 3] = (uint8_t)255;
		}
	}

	NoiseTexture->SetData(TexturePixels, sizeof(uint8_t) * Detail * Detail * 4);
}

void NoiseContainer2D::FillPixels(glm::vec2& offset)
{
	int rowPtr, columnPtr;

	for (int i = 0; i < Detail; i++) {
		for (int j = 0; j < Detail; j++) {
			float value = (Nucleus::SimplexNoise::extrapolate(i * StepSize.x + offset.x, j * StepSize.y + offset.y, m_Octaves, m_Frequency, m_Amplitude, m_Lacunarity, m_Persistence));
			value = (value + 1) / 2 * 255;

			rowPtr = i * Channels;
			columnPtr = j * Detail * Channels;

			TexturePixels[rowPtr + columnPtr + 0] = (uint8_t)value;
			TexturePixels[rowPtr + columnPtr + 1] = (uint8_t)value;
			TexturePixels[rowPtr + columnPtr + 2] = (uint8_t)value;
			TexturePixels[rowPtr + columnPtr + 3] = (uint8_t)255;
		}
	}

	NoiseTexture->SetData(TexturePixels, sizeof(uint8_t) * Detail * Detail * 4);
}

void NoiseContainer2D::FillChannel(int& channel, float xOffset, float yOffset)
{
	int rowPtr, columnPtr;

	for (int i = 0; i < Detail; i++) {
		for (int j = 0; j < Detail; j++) {
			float value = (Nucleus::SimplexNoise::extrapolate(i * StepSize.x + xOffset, j * StepSize.y + yOffset, m_Octaves, m_Frequency, m_Amplitude, m_Lacunarity, m_Persistence));
			value = (value + 1) / 2 * 255;

			rowPtr = i * Channels;
			columnPtr = j * Detail * Channels;

			TexturePixels[rowPtr + columnPtr + channel] = (uint8_t)value;
		}
	}

	NoiseTexture->SetData(TexturePixels, sizeof(uint8_t) * Detail * Detail * 4);
}
