#pragma once

#include "Nucleus/Renderer/RendererAPI.h"

namespace Nucleus {

	class OpenGLRendererAPI : public RendererAPI {

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}