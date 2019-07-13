#pragma once

#include "Nucleus/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Nucleus {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_windowHandle;
	};

}