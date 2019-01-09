#pragma once

#ifdef NC_PLATFORM_WINDOWS

extern Nucleus::Application* Nucleus::CreateApplication();

int main(int argc, char** argv) {
	auto app = Nucleus::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif //NC_PLATFORM_WINDOWS