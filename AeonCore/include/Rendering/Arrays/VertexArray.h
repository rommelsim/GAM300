#pragma once


/*****************************************************************//**
 * \file   VertexArray.h
 * \brief  Contains Interface to create VAO 
 *
 *
 * \author Rommel
 * \date   October 2023
 *
 * Copyright (C) 2023 Digipen Insitute Of Technology
 * Reproduction or disclosure of this file or its contents without
 * the prior consent of Digipen Insitute Of Technology is prohibited.
 *********************************************************************/

#include <memory>
#include <vector>
#include "Core/Log.h"
#include <Rendering/Buffers/Buffer.h>

namespace AeonCore
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind()	  const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
		virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() = 0;

		static VertexArray* Create();
	};
}