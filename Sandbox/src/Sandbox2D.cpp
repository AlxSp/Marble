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
	m_SquareVertexArray = Nucleus::VertexArray::Create();

	float Squarevertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 
		 0.5f, -0.5f, 0.0f, 
		 0.5f,  0.5f, 0.0f, 
		-0.5f,  0.5f, 0.0f 
	};


	Nucleus::Ref<Nucleus::VertexBuffer> m_SquareVertexBuffer;
	m_SquareVertexBuffer.reset(Nucleus::VertexBuffer::Create(Squarevertices, sizeof(Squarevertices)));

	m_SquareVertexBuffer->SetLayout({
		{ Nucleus::ShaderDataType::Float3, "a_Position"}
		});
	m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);


	uint32_t m_Squareindices[6] = { 0, 1, 2, 2, 3, 0 };
	Nucleus::Ref<Nucleus::IndexBuffer> m_SquareIndexBuffer;
	m_SquareIndexBuffer.reset(Nucleus::IndexBuffer::Create(m_Squareindices, sizeof(m_Squareindices) / sizeof(uint32_t)));
	m_SquareVertexArray->SetIndexBuffer(m_SquareIndexBuffer);


	m_FlatColorShader = Nucleus::Shader::Create("assets/shaders/FlatColor.glsl");

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

	Nucleus::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Nucleus::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Nucleus::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Nucleus::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Nucleus::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Nucleus::Event& e)
{
	m_CameraController.OnEvent(e);
}

