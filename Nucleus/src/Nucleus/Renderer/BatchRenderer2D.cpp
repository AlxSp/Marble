#include "ncpch.h"

#include "BatchRenderer2D.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Nucleus {

	static const size_t MaxQuadCount = 10000;
	static const size_t MaxVertexCount = MaxQuadCount * 4;
	static const size_t MaxBufferSize = MaxVertexCount * 10;
	static const size_t MaxIndexCount = MaxQuadCount * 6;
	static const size_t MaxTextures = 32;

	struct BatchRenderer2DStorage {
		Ref<VertexArray> QuadVertexArray;

		float* QuadBuffer = nullptr;
		float* QuadBufferPtr = nullptr;

		uint32_t WhiteTextureSlot = 0;

		uint32_t IndexCount = 0;

		std::array<Ref<Nucleus::Texture2D>, MaxTextures> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		Ref<Shader> BatchShader;
	};

	static BatchRenderer2DStorage s_BatchData;

	void BatchRenderer2D::Init()
	{
		NC_PROFILE_FUNCTION();

		s_BatchData.QuadVertexArray = VertexArray::Create();

		s_BatchData.QuadBuffer = new float[MaxBufferSize];
		
		Ref<VertexBuffer> m_QuadVertexBuffer;
		m_QuadVertexBuffer.reset(VertexBuffer::Create(s_BatchData.QuadBuffer, sizeof(float) * MaxBufferSize, BufferType::Dynamic));

		m_QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float4, "a_Color"},
			{ ShaderDataType::Float2, "a_TexCoord"},
		    { ShaderDataType::Float,  "TexIndex"}
		});

		s_BatchData.QuadVertexArray->AddVertexBuffer(m_QuadVertexBuffer);

		uint32_t indices[MaxIndexCount];
		uint32_t offset = 0;
		
		for (int i = 0; i < MaxIndexCount; i += 6) {
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;
			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}
		
		Ref<IndexBuffer> m_SquareIndexBuffer;
		m_SquareIndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		s_BatchData.QuadVertexArray->SetIndexBuffer(m_SquareIndexBuffer);

		s_BatchData.TextureSlots[0] = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_BatchData.TextureSlots[0]->SetData(&whiteTextureData, sizeof(uint32_t));
		
		for (size_t i = 1; i < MaxTextures; i++) {
			s_BatchData.TextureSlots[i] = 0;
		}

		s_BatchData.BatchShader = Shader::Create("assets/shaders/BatchShader.glsl");
		s_BatchData.BatchShader->Bind();

		int samplers[MaxTextures];
		for (int i = 0; i < MaxTextures; i++)
			samplers[i] = i;

		s_BatchData.BatchShader->SetSamplers("u_Textures", samplers, MaxTextures);

		
	}

	void BatchRenderer2D::Shutdown()
	{
		NC_PROFILE_FUNCTION();

		delete[] s_BatchData.QuadBuffer;
	}

	void BatchRenderer2D::BeginScene(const OrthographicCamera& camera)
	{
		NC_PROFILE_FUNCTION();

		s_BatchData.QuadBufferPtr = s_BatchData.QuadBuffer;

		s_BatchData.BatchShader->Bind();
		s_BatchData.BatchShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_BatchData.BatchShader->SetMat4("u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)));

		s_BatchData.TextureSlots[0]->Bind(s_BatchData.WhiteTextureSlot);
	}

	void BatchRenderer2D::EndScene()
	{
		NC_PROFILE_FUNCTION();
		
		EndBatch();
		Flush();
	}

	void BatchRenderer2D::BeginBatch()
	{
		NC_PROFILE_FUNCTION();

		s_BatchData.QuadBufferPtr = s_BatchData.QuadBuffer;
	}

	void BatchRenderer2D::EndBatch()
	{
		NC_PROFILE_FUNCTION();

		uint32_t size = (uint32_t*)s_BatchData.QuadBufferPtr - (uint32_t*)s_BatchData.QuadBuffer;
		if (size > 0)
			s_BatchData.QuadVertexArray->GetVertexBuffers()[0]->StreamTo(0, size * sizeof(float), s_BatchData.QuadBuffer);
	}

	void BatchRenderer2D::Flush()
	{
		NC_PROFILE_FUNCTION();

		s_BatchData.QuadVertexArray->Bind();

		RenderCommand::DrawIndexed(DrawMode::Triangles, s_BatchData.IndexCount);

		s_BatchData.IndexCount = 0;
		s_BatchData.TextureSlotIndex = 1;
	}

	void BatchRenderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		NC_PROFILE_FUNCTION();

		if (s_BatchData.IndexCount >= MaxIndexCount - 6) {
			EndBatch();
			Flush();
			BeginBatch();
		}

		float textureIndex = 0.0f;

		DrawVertex({ position.x, position.y, position.z }, color, { 0.0f, 0.0f }, textureIndex);
		DrawVertex({ position.x + size.x, position.y, position.z }, color, { 1.0f, 0.0f }, textureIndex);
		DrawVertex({ position.x + size.x, position.y + size.y, position.z }, color, { 1.0f, 1.0f }, textureIndex);
		DrawVertex({ position.x, position.y + size.y, position.z }, color, { 1.0f, 0.0f }, textureIndex);

		s_BatchData.IndexCount += 6;

	}

	void BatchRenderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		NC_PROFILE_FUNCTION();

		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void BatchRenderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor)
	{
		NC_PROFILE_FUNCTION();

		if (s_BatchData.IndexCount >= MaxIndexCount - 6 || s_BatchData.TextureSlotIndex > 31) {
			EndBatch();
			Flush();
			BeginBatch();
		}

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_BatchData.TextureSlotIndex; i++) {
			if (s_BatchData.TextureSlots[i] == texture) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			texture->Bind(s_BatchData.TextureSlotIndex);
			s_BatchData.TextureSlots[s_BatchData.TextureSlotIndex] = texture;

			textureIndex = (float)s_BatchData.TextureSlotIndex;
			s_BatchData.TextureSlotIndex++;
		}

		DrawVertex({ position.x, position.y, position.z }, tintColor, { 0.0f, 0.0f }, textureIndex);
		DrawVertex({ position.x + size.x, position.y, position.z }, tintColor, { 1.0f, 0.0f }, textureIndex);
		DrawVertex({ position.x + size.x, position.y + size.y, position.z }, tintColor, { 1.0f, 1.0f }, textureIndex);
		DrawVertex({ position.x, position.y + size.y, position.z }, tintColor, { 1.0f, 0.0f }, textureIndex);

		s_BatchData.IndexCount += 6;
	}

	void BatchRenderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor)
	{
		NC_PROFILE_FUNCTION();

		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tintColor);
	}

	void BatchRenderer2D::DrawVertex(const glm::vec3& position, const glm::vec4& color, const glm::vec2& textureCoordinates, const float& textureIndex)
	{
		s_BatchData.QuadBufferPtr[0] = position.x;
		s_BatchData.QuadBufferPtr[1] = position.y;
		s_BatchData.QuadBufferPtr[2] = position.z;

		s_BatchData.QuadBufferPtr[3] = color.x;
		s_BatchData.QuadBufferPtr[4] = color.y;
		s_BatchData.QuadBufferPtr[5] = color.z;
		s_BatchData.QuadBufferPtr[6] = color.w;

		s_BatchData.QuadBufferPtr[7] = textureCoordinates[0];
		s_BatchData.QuadBufferPtr[8] = textureCoordinates[1];

		s_BatchData.QuadBufferPtr[9] = textureIndex;

		s_BatchData.QuadBufferPtr += 10;
	}
}


