#include "mblpch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Marble {

	static GLenum BufferTypeToOpenGLBufferType(BufferType type) {
		switch (type) {
			case BufferType::Static:	return GL_STATIC_DRAW;
			case BufferType::Dynamic:	return GL_DYNAMIC_DRAW;
			case BufferType::Stream:	return GL_STREAM_DRAW;
		}
		MBL_CORE_ASSERT(false, "Unknown BufferType!");
		return 0;
	}

	///////////////////////////////////////////////////////////////////////
	//// Vertex Buffer ////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, BufferType type){
		MBL_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, BufferTypeToOpenGLBufferType(type));
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, BufferType type)
	{
		MBL_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, BufferTypeToOpenGLBufferType(type));
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		MBL_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		MBL_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		MBL_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		MBL_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	//// Vertex Buffer ////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////
	//// Index Buffer /////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t * indices, uint32_t count, BufferType type) : m_Count(count)
	{
		MBL_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, BufferTypeToOpenGLBufferType(type));
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		MBL_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		MBL_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		MBL_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	//// Index Buffer /////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////
}