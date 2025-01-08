#pragma once
/*!***************************************************************************
	\file			OpennGLFramebuffer.h
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					create and manage opengl framebuffer
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/AeonAPI.h"
#include "Rendering/Framebuffer/FrameBuffer.h"
#include <glad/glad.h>
namespace AeonCore
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();
		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(int x, int y) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override		{ return m_ColorAttachments[index]; }
		virtual const FramebufferSpecification& GetSpecification()		  const override		{ return m_specification; }


		virtual void SetSpecification(const FramebufferSpecification& spec) override { m_specification = spec; }

		void EnableShadowMapping(bool enable); // Method to enable/disable shadow mapping	(not used)
		void BindForShadowMap(); // Method to bind the framebuffer for shadow mapping		(not used)

		virtual const uint32_t& GetFrameBufferID() const { return m_RendererID; }
	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;
		
		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;

		bool m_ShadowEnabled = false; // Flag to indicate whether shadow mapping is enabled (not used)
		uint32_t m_ShadowMap = 0; // ID of the depth texture for shadow mapping			    (not used)
	};
}