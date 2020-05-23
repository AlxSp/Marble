#include "mblpch.h"
#include "Marble/Core/Application.h"

#include "Marble/Core/Log.h"
#include "Marble/Renderer/Renderer.h"

#include "Marble/Core/Input.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Marble {

	Application* Application::s_Instance = nullptr;

	Application::Application() 
	{
		MBL_PROFILE_FUNCTION();

		MBL_CORE_ASSERT(!s_Instance, "Application Already Exists!!");
		s_Instance = this; 
		m_Window = Window::Create();
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	}

	Application::~Application() 
	{
		MBL_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer) {
		MBL_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		MBL_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event &e) {
		MBL_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		//MBL_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() {

		MBL_PROFILE_FUNCTION();

		while (m_Running) {
			MBL_PROFILE_SCOPE("RunLoop");


			float time = (float)glfwGetTime(); //Platform::GetTime
			TimeStep timeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized) {
				{
					MBL_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack) {
						layer->OnUpdate(timeStep);
					}
				}
				m_ImGuiLayer->Begin();
				{
					MBL_PROFILE_SCOPE("LayerStack onImGuiRender");

					for (Layer* layer : m_LayerStack) {
						layer->OnImGuiRender();
					}
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent & e)
	{
		MBL_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}