#include <Nucleus.h>

class ExampleLayer : public Nucleus::Layer {
public:
	ExampleLayer() : Layer("Example") {
		
	}

	void OnUpdate() override {
		NC_INFO("ExampleLayer::Update");
	}

	void OnEvent(Nucleus::Event& event) override {
		NC_TRACE("{0}", event);
	}
};

class Sandbox : public Nucleus::Application {
public:
	Sandbox() {
		
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Nucleus::Application* Nucleus::CreateApplication() {
	return new Sandbox();
}