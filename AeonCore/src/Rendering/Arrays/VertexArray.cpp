#include "acpch.h"
/*****************************************************************//**
 * \file   VertexArray.cpp
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

#include <Rendering/Arrays/VertexArray.h>
#include <Rendering/Arrays/OpenGLVertexArray.h>
#include <memory>
namespace AeonCore
{

	VertexArray* VertexArray::Create()
	{
		// std::unique_ptr<OpenGLVertexArray> vao = std::make_unique<OpenGLVertexArray>();
		 //return vao.release();
		return new OpenGLVertexArray();
	}

}

