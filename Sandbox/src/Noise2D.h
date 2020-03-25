#pragma once

#include "Nucleus.h"
#include "Container2D.h"
#include "ObjectContainerManager2D.h"

class Noise2D : public Nucleus::Layer {
public:
	Noise2D();
	virtual ~Noise2D() { delete ocm; }
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Nucleus::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Nucleus::Event& e) override;

private:
	Nucleus::OrthographicCameraController m_CameraController;

	float perspective_width  = 0.0f;
	float perspective_height = 0.0f;

	float ZoomDifference = 1.0f;

	ObjectContainerManager2D* ocm;

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