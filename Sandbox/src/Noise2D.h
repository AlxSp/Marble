#pragma once

#include "Nucleus.h"

class Noise2D : public Nucleus::Layer {
public:
	Noise2D();
	virtual ~Noise2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Nucleus::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Nucleus::Event& e) override;

private:
	Nucleus::OrthographicCameraController m_CameraController;

	int m_Width = 128;
	int m_Height = 128;

	uint8_t NoiseMap[128][128][4];
	//std::array<std::array<uint8_t, 3 * 4>, 3> NoiseMap;
	float addXoffseet = 0.0f;
	Nucleus::Ref<Nucleus::Texture2D> m_NoiseTexture;

	Nucleus::SimplexNoise* simplexNoise;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	int count = 0; 

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

private:
	float m_Frequency = .1f;
	float m_Amplitude = 1.0f;
	float m_Lacunarity = 2.0f;
	float m_Persistence = 0.5f;
	int m_Octaves = 1.0f;

};