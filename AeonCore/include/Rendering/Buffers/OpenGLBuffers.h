#pragma once

/*****************************************************************//**
 * \file   OpenGLBuffers.h
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

#include <Rendering/Buffers/Buffer.h>
#include <Resource/Shader.h>

namespace AeonCore
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(std::vector<glm::mat4> _matrices);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		OpenGLVertexBuffer(void* vertices, uint32_t size);
		OpenGLVertexBuffer(uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind()		const override;
		virtual void UnBind()	const override;

		virtual const	BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void	SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		//virtual void	UpdateBufferStore(float* vertices, uint32_t size) override;
		virtual void	UpdateBufferStore(void* vertices, uint32_t size) override;

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		OpenGLIndexBuffer(uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind()		const;
		virtual void UnBind()	const;

		virtual uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		enum BLOCK_BINDING_POINT
		{
			MATRICES = 0,
			LIGHTING = 1
		};

		OpenGLUniformBuffer(uint32_t bytes, GLenum usageHint = GL_STREAM_DRAW);
		void Bind() const override;
		void UnBind() const override;

		void AttachShaderUniformBlock(Shader& shaderPgm, const char* uniformName, BLOCK_BINDING_POINT _bindingPoint, GLsizeiptr offset = 0);
		void SetData(uint32_t offset, uint32_t bytes, void* data) override;
	private:

		GLuint mBytes;
		GLuint mUBO;
		GLuint mBlockIndex;
	};
}	