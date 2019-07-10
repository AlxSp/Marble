#include <Nucleus.h>

class ExampleLayer : public Nucleus::Layer {
public:
	ExampleLayer() : Layer("Example") {
		
	}

	void OnUpdate() override {
		NC_INFO("ExampleLayer::Update");

		if (Nucleus::Input::isKeyPressed(NC_KEY_TAB))
			NC_TRACE("Tab key pressed");
	}

	void OnEvent(Nucleus::Event& event) override {
		//NC_TRACE("{0}", event);
	}
};

class Sandbox : public Nucleus::Application {
public:
	Sandbox() {
		
		PushLayer(new ExampleLayer());
		PushOverlay(new Nucleus::ImGuiLayer());
	}

	~Sandbox() {

	}
};

Nucleus::Application* Nucleus::CreateApplication() {
	return new Sandbox();
}