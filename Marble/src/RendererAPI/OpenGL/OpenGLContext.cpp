#include "mblpch.h"

#include "RendererAPI/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Marble {
	OpenGLContext::OpenGLContext(GLFWwindow * windowHandle) : m_windowHandle(windowHandle)
	{
		MBL_CORE_ASSERT(windowHandle, "Window handle is null!")
	}
	OpenGLContext::~OpenGLContext()
	{
	}
	void OpenGLContext::Init()
	{
		MBL_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MBL_CORE_ASSERT(status, "failed to initialize glad");

		MBL_CORE_INFO("OpenGL Info");
		MBL_CORE_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		MBL_CORE_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		MBL_CORE_INFO("Version: {0}", glGetString(GL_VERSION));

		#ifdef MBL_ENABLE_ASSERTS
				int versionMajor;
				int versionMinor;
				glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
				glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

				MBL_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Marble requires at least OpenGL version 4.5!");
		#endif
	}
	void OpenGLContext::SwapBuffers()
	{
		MBL_PROFILE_FUNCTION();

		glfwSwapBuffers(m_windowHandle);
	}
}