#include "ncpch.h"
#include "Shader.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Nucleus {

	Shader * Shader::Create(const std::string & vertexSrc, const std::string & fragmentSrc) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::None:
			NC_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		NC_CORE_ASSERT(false, "Unknown error!");
		return nullptr;
	}
}
