#include <Marble.h>
#include <Marble/Core/EntryPoint.h>
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"
#include "Noise2D.h"

class Sandbox : public Marble::Application {
public:
	Sandbox() {
		//PushLayer(new Noise2D());
		PushLayer(new Sandbox2D());
	}

	~Sandbox() {

	}
};

Marble::Application* Marble::CreateApplication() {
	return new Sandbox();
}