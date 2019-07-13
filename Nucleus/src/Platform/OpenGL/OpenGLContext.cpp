#include "ncpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Nucleus {
	OpenGLContext::OpenGLContext(GLFWwindow * windowHandle) : m_windowHandle(windowHandle)
	{
		NC_CORE_ASSERT(windowHandle, "Window handle is null!")
	}
	OpenGLContext::~OpenGLContext()
	{
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		NC_CORE_ASSERT(status, "failed to initialize glad");
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}
}