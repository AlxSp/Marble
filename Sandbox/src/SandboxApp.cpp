#include <Marble.h>
#include <Marble/Core/EntryPoint.h>

#include "Editor.h"
#include "Sandbox2D.h"

class Sandbox : public Marble::Application {
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());
		PushLayer(new Editor());
	}

	~Sandbox() {

	}
};

Marble::Application* Marble::CreateApplication() {
	return new Sandbox();
}