#pragma once

#include "Marble.h"

class Noise2D : public Marble::Layer {
public:
	Noise2D();
	virtual ~Noise2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Marble::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Marble::Event& e) override;

private:
	Marble::OrthographicCameraController m_CameraController;

	int m_Width = 64;
	int m_Height = 64;

	uint8_t NoiseMap[64][64][4];
	//std::array<std::array<uint8_t, 3 * 4>, 3> NoiseMap;

	Marble::Ref<Marble::Texture2D> m_NoiseTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	int count = 0; 

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

};