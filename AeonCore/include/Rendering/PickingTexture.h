#pragma once
/*****************************************************************//**
 * \file   PickingTexture.h
 * \brief  Contains ......
 *
 *
 *
 * \author Rommel, Waikit
 * \date   October 2023
 *
 * Copyright (C) 2023 Digipen Insitute Of Technology
 * Reproduction or disclosure of this file or its contents without
 * the prior consent of Digipen Insitute Of Technology is prohibited.
 *********************************************************************/

#include "AeonCore.h"
#include <glad/glad.h>

namespace AeonCore
{
	class AEON_API PickingTexture
	{
	public:
		struct PixelInfo
		{
			float ObjectID;
			float DrawID;
			float PrimID;

			PixelInfo()
			{
				ObjectID = 0.0f;
				DrawID = 0.0f;
				PrimID = 0.0f;
			}
		};
		PixelInfo ReadPixel(unsigned int x, unsigned int y);
		bool Init(unsigned int width, unsigned int height);
		void EnableWriting();
		void DisableWriting();

	private:
		GLuint m_fbo;
		GLuint m_pickingTexture;
		GLuint m_depthTexture;
	};
}