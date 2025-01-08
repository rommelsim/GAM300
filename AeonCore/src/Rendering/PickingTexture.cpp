#include "acpch.h"

/*!***************************************************************************
	\file			PickingTexture.cpp
	\author			Rommel
	\par			\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Rendering/PickingTexture.h"

namespace AeonCore
{
	
	bool PickingTexture::Init(unsigned int width, unsigned int height)
	{
		// FBO crreate
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		// Create texture 
		glGenTextures(1, &m_pickingTexture);
		glBindTexture(GL_TEXTURE_2D, m_pickingTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pickingTexture, 0);

		// Create texture object for depth buffer
		glGenTextures(1, &m_depthTexture);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

		// Disable reading to avold problems wiht older gpu
		glReadBuffer(GL_NONE);

		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		// Verfiy fbo status
		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (Status != GL_FRAMEBUFFER_COMPLETE)
		{
			AeonCore_ERROR("FrameBuffer Error: {}", Status);
			return false;
		}

		// Restore to original framebuffer
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return true;
	}

	void AeonCore::PickingTexture::EnableWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	}

	void AeonCore::PickingTexture::DisableWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	PickingTexture::PixelInfo PickingTexture::ReadPixel(unsigned int x, unsigned int y)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		PixelInfo pixel;
		glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &pixel);

		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

		return pixel;
	}
}