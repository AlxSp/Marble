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

	int m_Width = 64;
	int m_Height = 64;

	uint8_t NoiseMap[64][64][4];
	//std::array<std::array<uint8_t, 3 * 4>, 3> NoiseMap;

	Nucleus::Ref<Nucleus::Texture2D> m_NoiseTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	int count = 0; 

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

};