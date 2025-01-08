
#ifndef FRAME_BUFFER
#define FRAME_BUFFER

/*****************************************************************//**
 * \file   FrameBuffer.h
 * \brief  Handles rendering display to external framebuffers.
 *
 *
 * \author Rommel
 * \date   October 2023
 *
 * Copyright (C) 2023 Digipen Insitute Of Technology
 * Reproduction or disclosure of this file or its contents without
 * the prior consent of Digipen Insitute Of Technology is prohibited.
 *********************************************************************/

#include <Core/Singleton.hpp>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include "Core/Log.h"

#include "Core/AeonAPI.h"

namespace AeonCore
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8, RGBA16F,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defualts
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			:TextureFormat(format)
		{

		}
		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list< FramebufferTextureSpecification> attachments)
			:Attachments(attachments)
		{

		}

		std::vector<FramebufferTextureSpecification> Attachments;
	};
	
	struct FramebufferSpecification
	{
		int width = 0, height = 0;
		FramebufferAttachmentSpecification Attachments;
		uint32_t samples = 1;
		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(int x, int y) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;
		static Framebuffer* Create(const FramebufferSpecification& spec);

		//virtual const FrameBufferSpecification& GetSpecification() = 0;
		virtual void SetSpecification(const FramebufferSpecification& spec) = 0;
		virtual const uint32_t& GetFrameBufferID() const = 0;
	};

	// // Manager to handle FrameBuffer operations...
	// class AEON_API FrameBufferManager : public Singleton<FrameBufferManager>
	// {
	// private:
	// 	static inline std::unordered_map<std::string, OpenGLFramebuffer*> Framebuffers;
	// public:
	// 	void					Destroy();
	// 	void					CreateFrameBuffer	(std::string, float width, float height);
	// 	unsigned int			GetFrameTexture		(std::string name);
	// 	void					BindFrameBuffer		(std::string name);
	// 	void					UnBindFrameBuffer	(std::string name);
	// 	void					RescaleFrameBuffer(std::string name, float width, float height);
	// 
	// 	~FrameBufferManager();
	// };
}

#endif // !FRAME_BUFFER

