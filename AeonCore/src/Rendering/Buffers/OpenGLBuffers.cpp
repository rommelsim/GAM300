#include "acpch.h"
/*****************************************************************//**
 * \file   OpenGLBuffers.cpp
 * \brief  Contains function that creates Buffers for OpenGL using VertexBuffer and IndexBuffer Interface
 *
 *
 * \author Rommel
 * \date   October 2023
 *
 * Copyright (C) 2023 Digipen Insitute Of Technology
 * Reproduction or disclosure of this file or its contents without
 * the prior consent of Digipen Insitute Of Technology is prohibited.
 *********************************************************************/

#include <Rendering/Buffers/OpenGLBuffers.h>

namespace AeonCore
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(std::vector<glm::mat4> _matrices)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, _matrices.size() * sizeof(glm::mat4), _matrices.data(), GL_DYNAMIC_DRAW);
	}
	// Not used...
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		if (vertices)
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
		else
			glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(void* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		if (vertices)
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
		else
			glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}
	
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void AeonCore::OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	
	void OpenGLVertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// void OpenGLVertexBuffer::UpdateBufferStore(float* vertices, uint32_t size)
	// {
	// 	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	// 	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
	// }

	void OpenGLVertexBuffer::UpdateBufferStore(void* vertices, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
	}

	//// INDEX BUFFER OBJECTS

	
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_DYNAMIC_DRAW);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), 0, GL_DYNAMIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t bytes, GLenum usageHint) : mBytes(bytes)
	{
		glGenBuffers(1, &mUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, mUBO);
		glBufferData(GL_UNIFORM_BUFFER, mBytes, 0, usageHint);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	void OpenGLUniformBuffer::Bind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, mUBO);
	}
	void OpenGLUniformBuffer::UnBind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	void OpenGLUniformBuffer::AttachShaderUniformBlock(Shader& shaderPgm, const char* uniformName, BLOCK_BINDING_POINT _bindingPoint, GLsizeiptr offset)
	{
		mBlockIndex = glGetUniformBlockIndex(shaderPgm.ID, uniformName);

		if (mBlockIndex == GL_INVALID_INDEX)
		{
			AeonCore_ERROR("Block Index not found");
			AeonCore_WARN("Shader ID: {}", shaderPgm.ID);
			AeonCore_WARN("Uniform Name: {}", uniformName);
			return;
		}
		glUniformBlockBinding(shaderPgm.ID, mBlockIndex, _bindingPoint);
		glBindBufferRange(GL_UNIFORM_BUFFER, _bindingPoint, mUBO, offset, mBytes);
	}
	void OpenGLUniformBuffer::SetData(uint32_t offset, uint32_t bytes, void* data)
	{
		if (offset > mBytes)
		{
			AeonCore_ERROR("Cannot set offset as it is larger than current buffer size");
			AeonCore_WARN("Offset: {}", offset);
			AeonCore_WARN("Buffer size allocated: {}", mBytes);
			return;
		}

		if (offset + bytes > mBytes)
		{
			AeonCore_ERROR("Cannot set data because it would overflow current buffer size");
			AeonCore_WARN("Offset: {}", offset);
			AeonCore_WARN("Data size: {}", bytes);
			return;
		}

		glBindBuffer(GL_UNIFORM_BUFFER, mUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, bytes, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}