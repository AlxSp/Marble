#include "ncpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"


namespace Nucleus {

	struct Renderer2DStorage
	{
		Ref<VertexArray> SquareVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage;

		s_Data->SquareVertexArray = VertexArray::Create();

		float Squarevertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};


		Ref<VertexBuffer> m_SquareVertexBuffer;
		m_SquareVertexBuffer.reset(VertexBuffer::Create(Squarevertices, sizeof(Squarevertices)));

		m_SquareVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"},
			 {ShaderDataType::Float2, "a_TexCoord"}
			});
		s_Data->SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);


		uint32_t m_Squareindices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> m_SquareIndexBuffer;
		m_SquareIndexBuffer.reset(IndexBuffer::Create(m_Squareindices, sizeof(m_Squareindices) / sizeof(uint32_t)));
		s_Data->SquareVertexArray->SetIndexBuffer(m_SquareIndexBuffer);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color, const float rotation)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color, const float rotation)
	{
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();
		// Bind white texture 

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->SquareVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->SquareVertexArray);
	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float rotation)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float rotation)
	{
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		s_Data->TextureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		texture->Bind();

		s_Data->SquareVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->SquareVertexArray);
	}

}
