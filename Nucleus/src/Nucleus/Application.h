#pragma once

#include "Core.h"

#include "Window.h"
#include "Nucleus/LayerStack.h"
#include "Nucleus/Events/Event.h"
#include "Nucleus/Events/ApplicationEvent.h"

#include "Nucleus/ImGui/ImGuiLayer.h"

#include "Nucleus/Renderer/Shader.h"
#include "Nucleus/Renderer/Buffer.h"
#include "Nucleus/Renderer/VertexArray.h"

#include "Nucleus/Renderer/OrthographicCamera.h"

namespace Nucleus {
	class NUCLEUS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;


		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;


		std::shared_ptr<Shader> m_Shader2;
		std::shared_ptr<VertexArray>	m_SquareVertexArray;
		
		OrthographicCamera m_Camera;

	private:
		static Application* s_Instance;
	};

	//To be defined in Client
	Application* CreateApplication();

}