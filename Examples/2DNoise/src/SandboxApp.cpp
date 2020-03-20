#include <Nucleus.h>
#include <Nucleus/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"
#include "Noise2D.h"

class Sandbox : public Nucleus::Application {
public:
	Sandbox() {
		PushLayer(new Noise2D());
		//PushLayer(new Sandbox2D());
	}

	~Sandbox() {

	}
};

Nucleus::Application* Nucleus::CreateApplication() {
	return new Sandbox();
}