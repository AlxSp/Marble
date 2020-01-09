#pragma once

#include "OrthographicCamera.h"
#include "Nucleus/Renderer/Texture.h"

namespace Nucleus {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));



		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, glm::vec4 color, const float rotation = 0.0f);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, glm::vec4 color, const float rotation = 0.0f);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float rotation = 0.0f, const float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float rotation = 0.0f, const float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

	};
}