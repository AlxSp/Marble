#pragma once
#include "Core.h"

#include "Window.h"
#include "Nucleus/LayerStack.h"
#include "Nucleus/Events/Event.h"
#include "Nucleus/Events/ApplicationEvent.h"



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

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();

}