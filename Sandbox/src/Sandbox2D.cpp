#include "Sandbox2D.h"
#include "imgui/imgui.h"
//#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <chrono>


Sandbox2D::Sandbox2D() 
	: Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f, true)
{

}

void Sandbox2D::OnAttach()
{
	NC_PROFILE_FUNCTION();

	m_FlowerTexture = Nucleus::Texture2D::Create("assets/textures/transparentFlower.png");
	m_CheckerboardTexture = Nucleus::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	NC_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Nucleus::TimeStep ts)
{
	NC_PROFILE_FUNCTION();
	// Update
	m_CameraController.OnUpdate(ts);
	
	// Render
	{
		NC_PROFILE_SCOPE("Render Prep");

		Nucleus::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Nucleus::RenderCommand::Clear();
	}
	{
		NC_PROFILE_SCOPE("Render Draw");
		Nucleus::Renderer2D::ResetStats();

		Nucleus::Renderer2D::BeginScene(m_CameraController.GetCamera());
		
		for (int y = 0; y < 5; y++) {
			for (int x = 0; x < 5; x++) {
				Nucleus::Ref<Nucleus::Texture2D> texture = (x + y) % 2 == 0 ? m_FlowerTexture : m_CheckerboardTexture;
				Nucleus::Renderer2D::DrawQuad({ x, y, -0.1 }, { 1.f, 1.f }, texture);
			}
		}
		Nucleus::Renderer2D::EndScene();

		Nucleus::Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (float y = -10.0f; y < 10.0f; y += 0.25f) {
			for (float x = -10.0f; x < 10.0f; x += 0.25f) {
				glm::vec4 color = { (x + 10) / 20.0f, 0.2f, (y + 10) / 20.0f, .5f };
				Nucleus::Renderer2D::DrawQuad({ x, y }, { 0.15f, 0.15f }, color);
			}
		}

		Nucleus::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	NC_PROFILE_FUNCTION();

	ImGui::Begin("Renderer2D Stats");
	auto stats = Nucleus::Renderer2D::GetStats();
	ImGui::Text("Draw Calls: %i", stats.DrawCalls);
	ImGui::Text("Quad Count: %i", stats.QuadCount);
	ImGui::Text("Vertices: %i", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %i", stats.GetTotalIndexCount());
	ImGui::End();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Nucleus::Event& e)
{
	m_CameraController.OnEvent(e);
}

