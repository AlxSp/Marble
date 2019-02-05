#include "ncpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include "Nucleus/Application.h"

//TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Nucleus {

	ImGuiLayer::ImGuiLayer() :Layer("ImGuiLayer")
	{
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}
	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// Temporary: should eventually use Hazel key codes
		io.KeyMap[ImGuiKey_Tab] = NC_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = NC_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = NC_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = NC_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = NC_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = NC_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = NC_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = NC_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = NC_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = NC_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = NC_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = NC_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = NC_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = NC_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = NC_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = NC_KEY_A;
		io.KeyMap[ImGuiKey_C] = NC_KEY_C;
		io.KeyMap[ImGuiKey_V] = NC_KEY_V;
		io.KeyMap[ImGuiKey_X] = NC_KEY_X;
		io.KeyMap[ImGuiKey_Y] = NC_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = NC_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImGuiLayer::OnDetach()
	{
		
	}
	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.getWindow().GetWidth(), app.getWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void ImGuiLayer::OnEvent(Event & event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(NC_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(NC_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(NC_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(NC_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(NC_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(NC_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(NC_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(NC_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));

	}
	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;
		return false;
	}
	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;
		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.MousePos = ImVec2(e.GetX(), e.GetY());
		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();
		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.KeysDown[e.GetKeyCode()] = true;

		// Modifiers are not reliable across systems
		io.KeyCtrl = io.KeysDown[NC_KEY_LEFT_CONTROL] || io.KeysDown[NC_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[NC_KEY_LEFT_SHIFT] || io.KeysDown[NC_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[NC_KEY_LEFT_ALT] || io.KeysDown[NC_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[NC_KEY_LEFT_SUPER] || io.KeysDown[NC_KEY_RIGHT_SUPER];
		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}
	
	bool  ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());
		return true;
	}
}