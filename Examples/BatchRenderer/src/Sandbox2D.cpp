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

	m_FlowerTexture = Marble::Texture2D::Create("assets/textures/transparentFlower.png");
	m_CheckerboardTexture = Marble::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	NC_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Marble::TimeStep ts)
{
	NC_PROFILE_FUNCTION();
	// Update
	m_CameraController.OnUpdate(ts);
	
	// Render
	{
		NC_PROFILE_SCOPE("Render Prep");

		Marble::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Marble::RenderCommand::Clear();
	}
	{
		NC_PROFILE_SCOPE("Render Draw");

		Marble::Renderer2D::BeginScene(m_CameraController.GetCamera());
		
		for (int y = 0; y < 5; y++) {
			for (int x = 0; x < 5; x++) {
				Marble::Ref<Marble::Texture2D> texture = (x + y) % 2 == 0 ? m_FlowerTexture : m_CheckerboardTexture;
				Marble::Renderer2D::DrawQuad({ x, y }, { 1.f, 1.f }, texture);
			}
		}

		for (float y = -10.0f; y < 10.0f; y += 0.25f) {
			for (float x = -10.0f; x < 10.0f; x += 0.25f) {
				glm::vec4 color = { (x + 10) / 20.0f, 0.2f, (y + 10) / 20.0f, 1.0f };
				Marble::Renderer2D::DrawQuad({ x, y }, { 0.15f, 0.15f }, color);
			}
		}

		
		
		Marble::Renderer2D::EndScene();

		Marble::Renderer2D::BeginScene(m_CameraController.GetCamera());
		
		//Marble::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		//Marble::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		/*Marble::Renderer2D::DrawRotatedQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f }, -45.0f);
		Marble::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_TextureShader, -45.0f, 10.0f);
		*/
		Marble::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	NC_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Marble::Event& e)
{
	m_CameraController.OnEvent(e);
}

