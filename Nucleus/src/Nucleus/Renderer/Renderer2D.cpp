#include "ncpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Nucleus {

	struct Renderer2DStorage
	{
		Ref<VertexArray> SquareVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage;

		s_Data->SquareVertexArray = VertexArray::Create();

		float Squarevertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};


		Ref<VertexBuffer> m_SquareVertexBuffer;
		m_SquareVertexBuffer.reset(VertexBuffer::Create(Squarevertices, sizeof(Squarevertices)));

		m_SquareVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"}
			});
		s_Data->SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);


		uint32_t m_Squareindices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> m_SquareIndexBuffer;
		m_SquareIndexBuffer.reset(IndexBuffer::Create(m_Squareindices, sizeof(m_Squareindices) / sizeof(uint32_t)));
		s_Data->SquareVertexArray->SetIndexBuffer(m_SquareIndexBuffer);


		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformFloat4("u_Color", color);

		s_Data->SquareVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->SquareVertexArray);
	}

}
