#include "ncpch.h"
#include "Application.h"

#include "Nucleus/Events/ApplicationEvent.h"
#include "Nucleus/Log.h"

namespace Nucleus {
	Application::Application()
	{
	}


	Application::~Application()
	{
	}

	void Application::Run(){
		WindowResizeEvent e(1280, 720);
		NC_TRACE(e);

		while (true);
	}
}