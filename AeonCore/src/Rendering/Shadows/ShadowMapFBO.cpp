#pragma once
#include "acpch.h"
#include "Rendering/Shadows/ShadowMapFBO.h"

namespace AeonCore
{
	ShadowMapFBO::ShadowMapFBO()
	{
		m_fbo = 0;
		m_shadowMap = 0;
	}

	ShadowMapFBO::~ShadowMapFBO()
	{
		if (m_fbo != 0) {
			glDeleteFramebuffers(1, &m_fbo);
		}

		if (m_shadowMap != 0) {
			glDeleteTextures(1, &m_shadowMap);
		}
	}

	bool ShadowMapFBO::Init(unsigned int Width, unsigned int Height, bool ForPCF, MODE mode)
	{
		bool ret = false;
		ret = InitDSA(Width, Height, ForPCF, mode);
		return ret;
	}

	void ShadowMapFBO::BindForWriting()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glViewport(0, 0, m_width, m_height);  // set the width/height of the shadow map!
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void ShadowMapFBO::BindForReading(GLenum TextureUnit)
	{
		BindForReadingDSA(TextureUnit);
	}

	bool ShadowMapFBO::InitDSA(unsigned int Width, unsigned int Height, bool ForPCF, MODE mode)
	{
		m_width = Width;
		m_height = Height;

		if (mode == MODE::DIRECTIONAL || mode == MODE::SPOT)
		{

			// Create the FBO
			glCreateFramebuffers(1, &m_fbo);

			// Create the depth buffer
			glCreateTextures(GL_TEXTURE_2D, 1, &m_shadowMap);

			int Levels = 1;
			glTextureStorage2D(m_shadowMap, Levels, GL_DEPTH_COMPONENT32, Width, Height);

			GLint FilterType = ForPCF ? GL_LINEAR : GL_NEAREST;

			glTextureParameteri(m_shadowMap, GL_TEXTURE_MIN_FILTER, FilterType);
			glTextureParameteri(m_shadowMap, GL_TEXTURE_MAG_FILTER, FilterType);

			glTextureParameteri(m_shadowMap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTextureParameteri(m_shadowMap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTextureParameterfv(m_shadowMap, GL_TEXTURE_BORDER_COLOR, borderColor);

			glNamedFramebufferTexture(m_fbo, GL_DEPTH_ATTACHMENT, m_shadowMap, 0);

			// Disable read/writes to the color buffer
			glNamedFramebufferReadBuffer(m_fbo, GL_NONE);
			glNamedFramebufferDrawBuffer(m_fbo, GL_NONE);

			GLenum Status = glCheckNamedFramebufferStatus(m_fbo, GL_FRAMEBUFFER);

			if (Status != GL_FRAMEBUFFER_COMPLETE) {
				printf("FB error, status: 0x%x\n", Status);
				return false;
			}
		}

		// Point light framebuffer init
		else
		{
			glGenFramebuffers(1, &m_fbo);

			// create depth cubemap texture
			glGenTextures(1, &m_shadowMap);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);

			for (unsigned int i = 0; i < 6; ++i)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			// attach depth texture as FBO's depth buffer
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowMap, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		return true;
	}

	void ShadowMapFBO::BindForReadingDSA(GLenum TextureUnit)
	{
		glBindTextureUnit(TextureUnit - GL_TEXTURE0, m_shadowMap);
	}
	
}
