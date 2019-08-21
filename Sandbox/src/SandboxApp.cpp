#include <Nucleus.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Nucleus::Layer {
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f) {

		m_VertexArray.reset(Nucleus::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f,  0.2f, 0.3f, 0.8f, 1.0f,
			0.0f, 0.5f, 0.0f,  0.8f, 0.8f, 0.2f, 1.0f,
		};



		std::shared_ptr<Nucleus::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(Nucleus::VertexBuffer::Create(vertices, sizeof(vertices)));


		Nucleus::BufferLayout layout = {
			{ Nucleus::ShaderDataType::Float3, "a_Position"},
			{ Nucleus::ShaderDataType::Float4, "a_Color"},
		};


		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);


		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Nucleus::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Nucleus::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);


		float Squarevertices[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};

		m_SquareVertexArray.reset(Nucleus::VertexArray::Create());
		std::shared_ptr<Nucleus::VertexBuffer> m_SquareVertexBuffer;
		m_SquareVertexBuffer.reset(Nucleus::VertexBuffer::Create(Squarevertices, sizeof(Squarevertices)));
		//m_SquareVertexBuffer.reset(VertexBuffer::Create(Squarevertices, sizeof(Squarevertices)));


		Nucleus::BufferLayout m_Squarelayout = {
			{ Nucleus::ShaderDataType::Float3, "a_Position"},
		};


		m_SquareVertexBuffer->SetLayout(m_Squarelayout);
		m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);


		uint32_t m_Squareindices[6] = { 0, 1, 2, 2, 3, 0 };
		//m_SquareIndexBuffer.reset(IndexBuffer::Create(m_Squareindices, sizeof(m_Squareindices) / sizeof(uint32_t)));
		std::shared_ptr<Nucleus::IndexBuffer> m_SquareIndexBuffer;
		m_SquareIndexBuffer.reset(Nucleus::IndexBuffer::Create(m_Squareindices, sizeof(m_Squareindices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(m_SquareIndexBuffer);
		//m_SquareVertexArray->SetIndexBuffer(m_SquareIndexBuffer);




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

		std::string fragmentSrc2 = R"(
			#version 410 core

			layout(location = 0) out vec4 a_Color;

			in vec3 v_Position;

			void main(){
				a_Color = vec4(0.2, 0.3, 0.8, 1.0); 
			}

		)";

		m_BlueShader.reset(Nucleus::Shader::Create(vertexSrc2, fragmentSrc2));
	}

	void OnUpdate(Nucleus::TimeStep ts) override {
		NC_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		if (Nucleus::Input::IsKeyPressed(NC_KEY_LEFT)) {
			m_CameraPosition.x -= m_CameraMovementSpeed * ts;
		}
		else if (Nucleus::Input::IsKeyPressed(NC_KEY_RIGHT) ) {
			m_CameraPosition.x += m_CameraMovementSpeed * ts;
		}

		if (Nucleus::Input::IsKeyPressed(NC_KEY_UP) ) {
			m_CameraPosition.y += m_CameraMovementSpeed * ts;
		}
		else if (Nucleus::Input::IsKeyPressed(NC_KEY_DOWN)) {
			m_CameraPosition.y -= m_CameraMovementSpeed * ts;
		}

		if (Nucleus::Input::IsKeyPressed(NC_KEY_A)) {
			m_CameraRotation += m_CameraRotationSpeed * ts;
		}
		else if (Nucleus::Input::IsKeyPressed(NC_KEY_D)) {
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		}


		Nucleus::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Nucleus::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Nucleus::Renderer::BeginScene(m_Camera);

		
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Nucleus::Renderer::Submit(m_BlueShader, m_SquareVertexArray, transform);
			}
		}
		

		Nucleus::Renderer::Submit(m_Shader, m_VertexArray);

		Nucleus::Renderer::EndScene();

		//Renderer::Flush();
	}

	virtual void OnImGuiRender() override {

	}

	void OnEvent(Nucleus::Event& event) override {
	}

private:
	std::shared_ptr<Nucleus::Shader> m_Shader;
	std::shared_ptr<Nucleus::VertexArray> m_VertexArray;


	std::shared_ptr<Nucleus::Shader> m_BlueShader;
	std::shared_ptr<Nucleus::VertexArray>	m_SquareVertexArray;

	Nucleus::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMovementSpeed = 5.0f;

	float m_CameraRotationSpeed = 90.0f;
	float m_CameraRotation = 0.0f;
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