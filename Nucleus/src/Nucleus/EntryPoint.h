#pragma once

#ifdef NC_PLATFORM_WINDOWS

extern Nucleus::Application* Nucleus::CreateApplication();

int main(int argc, char** argv) {

	Nucleus::Log::Init();
	NC_CORE_WARNING("Initialized Log!");
	int a = 0;
	NC_INFO("Hello World {0}!", a);


	auto app = Nucleus::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif //NC_PLATFORM_WINDOWS