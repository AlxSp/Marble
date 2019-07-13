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

		NC_CORE_INFO("OpenGL Info");
		NC_CORE_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		NC_CORE_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		NC_CORE_INFO("Version: {0}", glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}
}