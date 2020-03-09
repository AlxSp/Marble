#pragma once

#include "Nucleus.h"

class Sandbox2D : public Nucleus::Layer {
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Nucleus::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Nucleus::Event& e) override;

private:
	Nucleus::OrthographicCameraController m_CameraController;


	/* Temporary*/
	Nucleus::Ref<Nucleus::VertexArray> m_QuadVertexArray;
	Nucleus::Ref<Nucleus::Shader> m_FlatColorShader;

	Nucleus::Ref<Nucleus::Texture2D> m_FlowerTexture;
	Nucleus::Ref<Nucleus::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	struct ProfileResult 
	{
		const char* Name;
		float Time;
	};

};