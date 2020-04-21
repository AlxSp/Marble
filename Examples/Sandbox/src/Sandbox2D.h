#pragma once

#include "Marble.h"

class Sandbox2D : public Marble::Layer {
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Marble::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Marble::Event& e) override;

private:
	Marble::OrthographicCameraController m_CameraController;


	/* Temporary*/
	Marble::Ref<Marble::VertexArray> m_QuadVertexArray;
	Marble::Ref<Marble::Shader> m_FlatColorShader;

	Marble::Ref<Marble::Texture2D> m_FlowerTexture;
	Marble::Ref<Marble::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	struct ProfileResult 
	{
		const char* Name;
		float Time;
	};

};