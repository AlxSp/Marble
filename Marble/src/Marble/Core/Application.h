#pragma once

#include "Marble/Core/Core.h"

#include "Marble/Core/Window.h"
#include "Marble/Core/LayerStack.h"
#include "Marble/Events/Event.h"
#include "Marble/Events/ApplicationEvent.h"

#include "Marble/Core/TimeStep.h"

#include "Marble/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Marble {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	//To be defined in Client
	Application* CreateApplication();

}