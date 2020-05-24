#include <Marble.h>
#include <Marble/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public Marble::Application {
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox() {

	}
};

Marble::Application* Marble::CreateApplication() {
	return new Sandbox();
}