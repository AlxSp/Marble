#pragma once

#include "OrthographicCamera.h"
#include "VertexArray.h"
#include "Nucleus/Renderer/Texture.h"

namespace Nucleus {



	class BatchRenderer2D 
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void BeginBatch();
		static void EndBatch();
		static void Flush();


		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));
	
	private:
		//static void DrawVertex();
	
	};
}