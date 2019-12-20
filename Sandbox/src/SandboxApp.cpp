#include <Nucleus.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class ExampleLayer : public Nucleus::Layer {
public:
	ExampleLayer() : Layer("Example"), m_CameraController(1920.0f / 1080.0f, true) {

		m_VertexArray.reset(Nucleus::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f,  0.2f, 0.3f, 0.8f, 1.0f,
			0.0f, 0.5f, 0.0f,  0.8f, 0.8f, 0.2f, 1.0f,
		};



		Nucleus::Ref<Nucleus::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(Nucleus::VertexBuffer::Create(vertices, sizeof(vertices)));


		Nucleus::BufferLayout layout = {
			{ Nucleus::ShaderDataType::Float3, "a_Position"},
			{ Nucleus::ShaderDataType::Float4, "a_Color"},
		};


		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);


		uint32_t indices[3] = { 0, 1, 2 };
		Nucleus::Ref<Nucleus::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Nucleus::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		m_SquareVertexArray.reset(Nucleus::VertexArray::Create());

		float Squarevertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		
		Nucleus::Ref<Nucleus::VertexBuffer> m_SquareVertexBuffer;
		m_SquareVertexBuffer.reset(Nucleus::VertexBuffer::Create(Squarevertices, sizeof(Squarevertices)));

		m_SquareVertexBuffer->SetLayout({
			{ Nucleus::ShaderDataType::Float3, "a_Position"},
			{ Nucleus::ShaderDataType::Float2, "a_TexCoord"}
			});
		m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);


		uint32_t m_Squareindices[6] = { 0, 1, 2, 2, 3, 0 };
		Nucleus::Ref<Nucleus::IndexBuffer> m_SquareIndexBuffer;
		m_SquareIndexBuffer.reset(Nucleus::IndexBuffer::Create(m_Squareindices, sizeof(m_Squareindices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(m_SquareIndexBuffer);

		std::string vertexSrc = R"(
			#version 410 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main(){
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSrc = R"(
			#version 410 core

			layout(location = 0) out vec4 a_Color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main(){
				a_Color = v_Color; 
			}

		)";

		m_Shader.reset(Nucleus::Shader::Create(vertexSrc, fragmentSrc));

		std::string vertexSrc2 = R"(
			#version 410 core

			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec3 v_Position;

			void main(){
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 410 core

			layout(location = 0) out vec4 a_Color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main(){
				a_Color = vec4(u_Color, 1.0); 
			}

		)";

		flatColorShader.reset(Nucleus::Shader::Create(vertexSrc2, flatColorShaderFragmentSrc));

		m_TextureShader.reset(Nucleus::Shader::Create("assets/shaders/Texture.glsl"));

		m_Texture = Nucleus::Texture2D::Create("assets/textures/Checkerboard.png");
		m_FlowerTexture = Nucleus::Texture2D::Create("assets/textures/transparentFlower.png");

		std::dynamic_pointer_cast<Nucleus::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Nucleus::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Nucleus::TimeStep ts) override {
		//Update
		m_CameraController.OnUpdate(ts);

		//Render
		Nucleus::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Nucleus::RenderCommand::Clear();

		Nucleus::Renderer::BeginScene(m_CameraController.GetCamera());
		
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Nucleus::OpenGLShader>(flatColorShader)->Bind();
		std::dynamic_pointer_cast<Nucleus::OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);


		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Nucleus::Renderer::Submit(flatColorShader, m_SquareVertexArray, transform);
			}
		}
		
		//Triangle
		//Nucleus::Renderer::Submit(m_Shader, m_VertexArray);
		m_Texture->Bind();
		Nucleus::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_FlowerTexture->Bind();
		Nucleus::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Nucleus::Renderer::EndScene();

		//Renderer::Flush();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Nucleus::Event& e) override {
		m_CameraController.OnEvent(e);
	}

private:
	Nucleus::Ref<Nucleus::Shader> m_Shader;
	Nucleus::Ref<Nucleus::VertexArray> m_VertexArray;

	Nucleus::Ref<Nucleus::Shader> flatColorShader, m_TextureShader;
	Nucleus::Ref<Nucleus::VertexArray>	m_SquareVertexArray;

	Nucleus::Ref<Nucleus::Texture2D> m_Texture, m_FlowerTexture;

	Nucleus::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Nucleus::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Nucleus::Application* Nucleus::CreateApplication() {
	return new Sandbox();
}