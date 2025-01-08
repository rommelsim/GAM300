#ifndef OPENGLVERTEXARRAY_H
#define OPENGLVERTEXARRAY_H
/*****************************************************************//**
 * \file   OpenGLVertexArray.h
 * \brief  Contains functoin to create VAO using VertexArray Interface
 * 
 * 
 * \author Rommel
 * \date   October 2023
 * 
 * Copyright (C) 2023 Digipen Insitute Of Technology
 * Reproduction or disclosure of this file or its contents without 
 * the prior consent of Digipen Insitute Of Technology is prohibited.
 *********************************************************************/


#include <Rendering/Arrays/VertexArray.h>

namespace AeonCore
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind()	  const override;
		virtual void UnBind() const override;

		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) override;
		std::shared_ptr<IndexBuffer> GetIndexBuffer() override;

	private:
		uint32_t m_RendererID;

		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
}

#endif // !OPENGLVERTEXARRAY_H
