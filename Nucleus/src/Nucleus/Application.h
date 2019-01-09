#pragma once
#include "Core.h"

namespace Nucleus {
	class NUCLEUS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();

}