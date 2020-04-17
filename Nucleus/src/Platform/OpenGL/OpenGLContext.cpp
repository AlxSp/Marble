#include "ncpch.h"

#include "Platform/OpenGL/OpenGLContext.h"

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
		NC_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		NC_CORE_ASSERT(status, "failed to initialize glad");

		NC_CORE_INFO("OpenGL Info");
		NC_CORE_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		NC_CORE_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		NC_CORE_INFO("Version: {0}", glGetString(GL_VERSION));

		#ifdef NC_ENABLE_ASSERTS
				int versionMajor;
				int versionMinor;
				glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
				glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

				NC_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Nucleus requires at least OpenGL version 4.5!");
		#endif
	}
	void OpenGLContext::SwapBuffers()
	{
		NC_PROFILE_FUNCTION();

		glfwSwapBuffers(m_windowHandle);
	}
}