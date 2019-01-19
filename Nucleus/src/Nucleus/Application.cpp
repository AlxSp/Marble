#include "ncpch.h"
#include "Application.h"

#include "Nucleus/Log.h"

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Nucleus {
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}


	Application::~Application()
	{
	}

	void Application::OnEvent(Event &e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		NC_CORE_TRACE("{0}", e);
	}

	void Application::Run() {
		while (m_Running) {
			m_Window-> OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}