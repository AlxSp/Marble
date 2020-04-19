#include "mblpch.h"

#include "Marble/Renderer/Renderer2D.h"
#include "Marble/Renderer/Shader.h"
#include "Marble/Renderer/RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Marble {

	static const Marble::BufferLayout Render2DQuadStandardLayout {
		{ ShaderDataType::Float3, "a_Position"},
		{ ShaderDataType::Float4, "a_Color" },
		{ ShaderDataType::Float2, "a_TexCoord" },
		{ ShaderDataType::Float,  "TexIndex" },
		{ ShaderDataType::Float,  "TilingFactor" }
	};

	struct Renderer2DData {
		static const uint32_t MaxQuadCount = 10000;
		static const uint32_t MaxVertexCount = MaxQuadCount * 4;
		static const uint32_t MaxIndexCount = MaxQuadCount * 6;
		static const uint32_t MaxBufferSize = MaxVertexCount * 11;
		static const uint32_t MaxTextures = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		
		float* QuadBuffer = nullptr;
		float* QuadBufferPtr = nullptr;
		uint32_t QuadIndexCount = 0;

		glm::vec4 QuadVertexPositions[4];
		
		std::array<Ref<Marble::Texture2D>, MaxTextures> TextureSlots;
		uint32_t WhiteTextureSlot = 0;
		uint32_t TextureSlotIndex = 1;

		Ref<Shader> TextureShader;

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		MBL_PROFILE_FUNCTION();

		s_Data.QuadBuffer = new float[s_Data.MaxBufferSize];
		
		s_Data.QuadVertexBuffer = VertexBuffer::Create(sizeof(float) * s_Data.MaxBufferSize, BufferType::Dynamic);
		s_Data.QuadVertexBuffer->SetLayout(Render2DQuadStandardLayout);

		s_Data.QuadVertexArray = VertexArray::Create();
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndexCount];

		uint32_t offset = 0;
		for (int i = 0; i < s_Data.MaxIndexCount; i += 6) {
			quadIndices[i + 0] = 0 + offset;
			quadIndices[i + 1] = 1 + offset;
			quadIndices[i + 2] = 2 + offset;
			quadIndices[i + 3] = 2 + offset;
			quadIndices[i + 4] = 3 + offset;
			quadIndices[i + 5] = 0 + offset;

			offset += 4;
		}
		
		Ref<IndexBuffer> m_QuadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data.MaxIndexCount);
		s_Data.QuadVertexArray->SetIndexBuffer(m_QuadIndexBuffer);
		delete[] quadIndices;

		s_Data.TextureSlots[s_Data.WhiteTextureSlot] = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.TextureSlots[s_Data.WhiteTextureSlot]->SetData(&whiteTextureData, sizeof(uint32_t));
		
		int samplers[s_Data.MaxTextures];
		for (int i = 0; i < s_Data.MaxTextures; i++)
			samplers[i] = i;

		s_Data.TextureShader = Shader::Create("assets/shaders/BatchShader.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextures);

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		MBL_PROFILE_FUNCTION();

		delete[] s_Data.QuadBuffer;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		MBL_PROFILE_FUNCTION();

		s_Data.QuadBufferPtr = s_Data.QuadBuffer;
		s_Data.QuadIndexCount = 0;
		
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		MBL_PROFILE_FUNCTION();
		
		EndBatch();
		Flush();
	}

	void Renderer2D::BeginBatch()
	{
		MBL_PROFILE_FUNCTION();

		s_Data.QuadBufferPtr = s_Data.QuadBuffer;
		s_Data.QuadIndexCount = 0;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndBatch()
	{
		MBL_PROFILE_FUNCTION();

		uint32_t size = (uint32_t*)s_Data.QuadBufferPtr - (uint32_t*)s_Data.QuadBuffer;
		if (size > 0)
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadBuffer, size * sizeof(float));
	}

	void Renderer2D::Flush()
	{
		MBL_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++){
			s_Data.TextureSlots[i]->Bind(i);
		}

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(DrawMode::Triangles, s_Data.QuadIndexCount);

		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::SetStandardLayout()
	{
		EndBatch();
		Flush();

		s_Data.QuadVertexBuffer->SetLayout(Render2DQuadStandardLayout);
		BeginBatch();
	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		MBL_PROFILE_FUNCTION();

		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		MBL_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndexCount ) {
			EndBatch();
			Flush();
			BeginBatch();
		}

		const float textureIndex = 0.0f; //White Texture Slot Index
		const float tilingFactor = 1.0f; //White Texture Slot Index

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawVertex(transform * s_Data.QuadVertexPositions[0], color, { 0.0f, 0.0f }, textureIndex, tilingFactor);
		DrawVertex(transform * s_Data.QuadVertexPositions[1], color, { 1.0f, 0.0f }, textureIndex, tilingFactor);
		DrawVertex(transform * s_Data.QuadVertexPositions[2], color, { 1.0f, 1.0f }, textureIndex, tilingFactor);
		DrawVertex(transform * s_Data.QuadVertexPositions[3], color, { 0.0f, 1.0f }, textureIndex, tilingFactor);

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float& tilingFactor, const glm::vec4& tintColor)
	{
		MBL_PROFILE_FUNCTION();

		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float& tilingFactor, const glm::vec4& tintColor)
	{
		MBL_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndexCount) {
			EndBatch();
			Flush();
			BeginBatch();
		}

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (s_Data.TextureSlotIndex >= s_Data.MaxTextures) {
				EndBatch();
				Flush();
				BeginBatch();
			}

			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawVertex(transform * s_Data.QuadVertexPositions[0], tintColor, { 0.0f, 0.0f }, textureIndex, tilingFactor);
		DrawVertex(transform * s_Data.QuadVertexPositions[1], tintColor, { 1.0f, 0.0f }, textureIndex, tilingFactor);
		DrawVertex(transform * s_Data.QuadVertexPositions[2], tintColor, { 1.0f, 1.0f }, textureIndex, tilingFactor);
		DrawVertex(transform * s_Data.QuadVertexPositions[3], tintColor, { 0.0f, 1.0f }, textureIndex, tilingFactor);

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, glm::vec4 color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, glm::vec4 color)
	{
		MBL_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndexCount ) {
			EndBatch();
			Flush();
			BeginBatch();
		}

		const float textureIndex = 0.0f; //White Texture Slot Index
		const float tilingFactor = 1.0f; //White Texture Slot Index

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawVertex(transform * s_Data.QuadVertexPositions[0], color, { 0.0f, 0.0f }, textureIndex, tilingFactor);
		DrawVertex(transform * s_Data.QuadVertexPositions[1], color, { 1.0f, 0.0f }, textureIndex, tilingFactor);
		DrawVertex(transform * s_Data.QuadVertexPositions[2], color, { 1.0f, 1.0f }, textureIndex, tilingFactor);
		DrawVertex(transform * s_Data.QuadVertexPositions[3], color, { 0.0f, 1.0f }, textureIndex, tilingFactor);

		s_Data.QuadIndexCount += 6;
		
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const float tilingFactor, const glm::vec4& tintColor)
	{
		MBL_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndexCount) {
			EndBatch();
			Flush();
			BeginBatch();
		}

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (s_Data.TextureSlotIndex >= s_Data.MaxTextures) {
				EndBatch();
				Flush();
				BeginBatch();
			}

			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawVertex(transform * s_Data.QuadVertexPositions[0], tintColor, { 0.0f, 0.0f }, textureIndex, tilingFactor);
		DrawVertex(transform * s_Data.QuadVertexPositions[1], tintColor, { 1.0f, 0.0f }, textureIndex, tilingFactor);
		DrawVertex(transform * s_Data.QuadVertexPositions[2], tintColor, { 1.0f, 1.0f }, textureIndex, tilingFactor);
		DrawVertex(transform * s_Data.QuadVertexPositions[3], tintColor, { 0.0f, 1.0f }, textureIndex, tilingFactor);

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawVertex(const glm::vec3& position, const glm::vec4& color, const glm::vec2& textureCoordinates, const float& textureIndex, const float& tilingFactor)
	{
		s_Data.QuadBufferPtr[0] = position.x;
		s_Data.QuadBufferPtr[1] = position.y;
		s_Data.QuadBufferPtr[2] = position.z;

		s_Data.QuadBufferPtr[3] = color.r;
		s_Data.QuadBufferPtr[4] = color.g;
		s_Data.QuadBufferPtr[5] = color.b;
		s_Data.QuadBufferPtr[6] = color.a;

		s_Data.QuadBufferPtr[7] = textureCoordinates.x;
		s_Data.QuadBufferPtr[8] = textureCoordinates.y;

		s_Data.QuadBufferPtr[9] = textureIndex;

		s_Data.QuadBufferPtr[10] = tilingFactor;
	
		s_Data.QuadBufferPtr += 11;
	}

	Renderer2D::Statistics Renderer2D::GetStats(){
		return s_Data.Stats;
	}

	void Renderer2D::ResetStats(){
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}
}


