#include <Marble.h>
#include <Marble/Core/EntryPoint.h>

#include "Editor.h"

class EditorApp : public Marble::Application {
public:
	EditorApp() {
		PushLayer(new Editor());
	}

	~EditorApp() {

	}
};

Marble::Application* Marble::CreateApplication() {
	return new EditorApp();
}