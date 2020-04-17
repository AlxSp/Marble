#include "ncpch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Nucleus {

	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userrParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:			NC_CORE_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:			NC_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:				NC_CORE_WARNING(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION:	NC_CORE_TRACE(message); return;
		}

		NC_CORE_ASSERT(false, "Unknown OpenGL severity level!")
	}

	static GLenum DrawModeToOpenGLDrawMode(DrawMode mode) {
		switch (mode) {
			case DrawMode::Points:		return GL_POINTS;
			case DrawMode::Lines:		return GL_LINES;
			case DrawMode::Triangles:	return GL_TRIANGLES;
		}
		NC_CORE_ASSERT(false, "Unknown BufferType!");
		return 0;
	}

	void OpenGLRendererAPI::Init()
	{
		NC_PROFILE_FUNCTION();

		#ifdef NC_DEBUG
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLMessageCallback, nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4 & color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void OpenGLRendererAPI::DrawIndexed(const DrawMode& mode, const uint32_t& count)
	{
		glDrawElements(DrawModeToOpenGLDrawMode(mode), count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}