#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() 
	: Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f, true)
{

}

void Sandbox2D::OnAttach()
{
	

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Nucleus::TimeStep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Nucleus::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Nucleus::RenderCommand::Clear();

	Nucleus::Renderer2D::BeginScene(m_CameraController.GetCamera());

	//std::dynamic_pointer_cast<Nucleus::OpenGLShader>(m_FlatColorShader)->Bind();
	//std::dynamic_pointer_cast<Nucleus::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	//Nucleus::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Nucleus::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });

	Nucleus::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Nucleus::Event& e)
{
	m_CameraController.OnEvent(e);
}

