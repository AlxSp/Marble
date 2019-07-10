#include "ncpch.h"
#include "Application.h"

#include "Nucleus/Log.h"
//MAy not be needed
#include <glad/glad.h>

#include "Input.h"

namespace Nucleus {

//#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		NC_CORE_ASSERT(!s_Instance, "Application Already Exists!!");
		s_Instance = this; 
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(NC_BIND_EVENT_FN(Application::OnEvent));
	}


	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
		
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event &e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(NC_BIND_EVENT_FN(Application::OnWindowClose));

		//NC_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() {
		while (m_Running) {
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}	

			auto[x, y] = Input::GetMousePosition();
			NC_CORE_TRACE("{0}, {1}", x, y);
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}