#pragma once

#include "Marble.h"
#include "Player.h"
#include "Container2D.h"
#include "ObjectContainerManager2D.h"

class Noise2D : public Marble::Layer {
public:
	Noise2D();
	virtual ~Noise2D() { delete ocm; }
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Marble::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Marble::Event& e) override;

private:
	Marble::OrthographicCameraController m_CameraController;
	ObjectContainerManager2D* ocm;


	float perspective_width  = 0.0f;
	float perspective_height = 0.0f;

	float ZoomDifference = .1f;

	Marble::Ref<Player> m_Player;



	Marble::Ref<Marble::Texture2D> texture;
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