#include <Nucleus.h>

class Sandbox : public Nucleus::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

Nucleus::Application* Nucleus::CreateApplication() {
	return new Sandbox();
}