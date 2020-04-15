#pragma once
#include "Nucleus.h"

enum ContainerLevelofDetail {
	L0, L1, L2
};

struct ContainerLODSetting {
	ContainerLevelofDetail lod;
	float maxDistance;
};

struct ContainerLOD {
	std::array<ContainerLODSetting, 3> lods = { { {L0, 5.0f}, { L1, 10.f}, { L2, 15.f} } };
};

static ContainerLOD containerLOD;

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

	virtual void OnRender() = 0; 
};

class ColorContainer2D : public VisibleContainer2D {
public:
	ColorContainer2D(const glm::vec2& postion, const glm::vec2& size, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }) : VisibleContainer2D(postion, size), m_Color(color) {}
	~ColorContainer2D() = default;

	void SetColor(const glm::vec4 color) { m_Color = color; }

	virtual void OnUpdate(float distance) {
		for (int i = 0; i < containerLOD.lods.size(); i++) {
			if (distance < containerLOD.lods[i].maxDistance) {
				SetLODColor(containerLOD.lods[i].lod);
				break;
			}
		}
	}
	virtual void OnRender() override { Nucleus::Renderer2D::DrawQuad(Position, Size, m_Color); }

private:
	void SetLODColor(ContainerLevelofDetail& level) {
		switch (level) {
		case L0: m_Color = { 0.0f, .9f, 0.4f, 1.0f }; break;
		case L1: m_Color = { 1.0f, 0.6f, 0.1f, 1.0f }; break;
		case L2: m_Color = { 1.0f, 0.0f, 0.0f, 1.0f }; break;
		default: m_Color =  { 0.0f, 0.0f, 0.0f, 1.0f };
		}
	}

	glm::vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
};



class NoiseContainer2D : public VisibleContainer2D {
public:
	NoiseContainer2D(const glm::vec2& postion, const glm::vec2& size, const uint32_t& detail = 128);
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
	void FillPixels(const glm::vec2& offset);
	void FillChannel(int& channel, float xOffset = 0.0f, float yOffset = 0.0f);

	void UpdateTexture();

	void OnUpdate(Nucleus::TimeStep ts) {}
	void OnRender() override { Nucleus::Renderer2D::DrawQuad(Position, Size, NoiseTexture); }

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
