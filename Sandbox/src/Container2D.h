#pragma once
#include "Nucleus.h"

class Container2D {
public: 
	Container2D(const glm::vec2& postion, const glm::vec2& size) : Position(postion), Size(size) {}
	~Container2D() = default;
protected:
	glm::vec2 Position;
	glm::vec2 Size;
};

class VisibleContainer2D : public Container2D {
public:
	VisibleContainer2D(const glm::vec2& postion, const glm::vec2& size) : Container2D(postion, size) {}
	~VisibleContainer2D() = default;

	//glm::vec2 GetPostion() { return Position; }
	virtual void OnRender() = 0;
	//virtual 
};

class ColorContainer2D : public VisibleContainer2D {
public:
	ColorContainer2D(const glm::vec2& postion, const glm::vec2& size, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }) : VisibleContainer2D(postion, size), m_Color(color) {}
	~ColorContainer2D() = default;

	void SetColor(const glm::vec4 color) { m_Color = color; }
	void OnRender() override { Nucleus::BatchRenderer2D::DrawQuad(Position - Size / 2.0f, Size, m_Color); }

private:
	glm::vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
};



class NoiseContainer2D : public VisibleContainer2D {
public:
	NoiseContainer2D(const glm::vec2& postion, const glm::vec2& size, const uint32_t& detail);
	~NoiseContainer2D() { delete TexturePixels; }

	void SetSimplexNoiseValues(const uint8_t& octaves, const float& frequency, const float& amplitude, const float& lacunarity, const float& persistence) { 
		m_Octaves = octaves;
		m_Frequency = frequency;
		m_Amplitude = amplitude;
		m_Lacunarity = lacunarity;
		m_Persistence = persistence;
	}

	glm::vec2 GetPostion() { return Position; }
	
	void FillPixels(float xOffset = 0.0f, float yOffset = 0.0f);
	void FillPixels(glm::vec2& offset);
	void FillChannel(int& channel, float xOffset = 0.0f, float yOffset = 0.0f);

	void OnUpdate(Nucleus::TimeStep ts) {}
	void OnRender() override { Nucleus::BatchRenderer2D::DrawQuad(Position - Size / 2.0f, Size, NoiseTexture); }

private:
	uint8_t	m_Octaves = 4;
	float m_Frequency = .1f;
	float m_Amplitude = 1.0f;
	float m_Lacunarity = 2.0f;
	float m_Persistence = 0.5f;
private:
	glm::vec2 StepSize = { 1.0f, 1.0f };
	uint8_t* TexturePixels;
	uint32_t Detail;
	uint8_t Channels = 4;
	Nucleus::Ref<Nucleus::Texture2D> NoiseTexture;
};
