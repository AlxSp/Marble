#include "ncpch.h"
#include "Application.h"

#include "Nucleus/Events/ApplicationEvent.h"
#include "Nucleus/Log.h"

namespace Nucleus {
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}


	Application::~Application()
	{
	}

	void Application::Run() {
		while (m_Running) {
			m_Window-> OnUpdate();
		}
	}
}