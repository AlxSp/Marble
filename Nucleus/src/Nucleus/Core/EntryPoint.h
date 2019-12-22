#pragma once

#ifdef NC_PLATFORM_WINDOWS

extern Nucleus::Application* Nucleus::CreateApplication();

int main(int argc, char** argv) {

	Nucleus::Log::Init();
	NC_PROFILE_BEGIN_SESSION("Startup", "NucleusProfile-Startup.json");
	auto app = Nucleus::CreateApplication();
	NC_PROFILE_END_SESSION();

	NC_PROFILE_BEGIN_SESSION("Runtime", "NucleusProfile-Runtime.json");
	app->Run();
	NC_PROFILE_END_SESSION();

	NC_PROFILE_BEGIN_SESSION("Shutdown", "NucleusProfile-Shutdown.json");
	delete app;
	NC_PROFILE_END_SESSION();

	return 0;
}

#endif //NC_PLATFORM_WINDOWS