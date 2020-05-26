#pragma once

extern Marble::Application* Marble::CreateApplication();

int main(int argc, char** argv) {

	Marble::Log::Init();
	MBL_PROFILE_BEGIN_SESSION("Startup", "MarbleProfile-Startup.json");
	auto app = Marble::CreateApplication();
	MBL_PROFILE_END_SESSION();

	MBL_PROFILE_BEGIN_SESSION("Runtime", "MarbleProfile-Runtime.json");
	app->Run();
	MBL_PROFILE_END_SESSION();

	MBL_PROFILE_BEGIN_SESSION("Shutdown", "MarbleProfile-Shutdown.json");
	delete app;
	MBL_PROFILE_END_SESSION();

	return 0;
}