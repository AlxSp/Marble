#include "ncpch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Nucleus {

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