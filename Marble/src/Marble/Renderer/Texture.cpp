#include "mblpch.h"

#include "Marble/Renderer/Texture.h"
#include "Marble/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Marble {
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: 	MBL_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL: 	return CreateRef<OpenGLTexture2D>(width, height);
		}

		MBL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path) 
	{
		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::None:	MBL_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(path);
		}

		MBL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}