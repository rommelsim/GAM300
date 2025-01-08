#include "acpch.h"
/*****************************************************************//**
 * \file   Buffer.cpp
 * \brief	Contains functions to create VBO and IBO for opengl
 * Can automatically deduce stride and offset.
 *
 *
 * \author Rommel
 * \date   October 2023
 *
 * Copyright (C) 2023 Digipen Insitute Of Technology
 * Reproduction or disclosure of this file or its contents without
 * the prior consent of Digipen Insitute Of Technology is prohibited.
 *********************************************************************/

#include <Rendering/Buffers/Buffer.h>
#include <Rendering/Buffers/OpenGLBuffers.h>


namespace AeonCore
{
	// VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	// {
	// 	return new OpenGLVertexBuffer(vertices, size);
	// }

	VertexBuffer* VertexBuffer::Create(void* vertices, uint32_t size)
	{
		return new OpenGLVertexBuffer(vertices, size);
	}

	VertexBuffer* VertexBuffer::Create(uint32_t size)
	{
		return new OpenGLVertexBuffer(size);
	}

	VertexBuffer* VertexBuffer::Create(std::vector<glm::mat4> _matrices)
	{
		return new OpenGLVertexBuffer(_matrices);
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		return new OpenGLIndexBuffer(indices, count);
	}

	IndexBuffer* IndexBuffer::Create(uint32_t count)
	{
		return new OpenGLIndexBuffer(count);
	}

	UniformBuffer* UniformBuffer::Create(const uint32_t& byteSize, GLenum usageHint)
	{
		return new OpenGLUniformBuffer(byteSize, usageHint);
	}

}
