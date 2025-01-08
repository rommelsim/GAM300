#include "acpch.h"
/*!***************************************************************************
	\file			FramebufferManager.cpp
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					manage framebuffers
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Rendering/Framebuffer/FramebufferManager.h"
#include "Rendering/Framebuffer/OpenGLFramebuffer.h"

namespace AeonCore
{
	std::unordered_map<std::string, Framebuffer*> FramebufferManager::Framebuffers;

	FramebufferManager::~FramebufferManager()
	{
		for (const auto& frame : Framebuffers)
		{
			if (frame.second)
			{
				OpenGLFramebuffer* OpenGLFrameBufferPtr = reinterpret_cast<OpenGLFramebuffer*>(frame.second);
				delete OpenGLFrameBufferPtr;
				//delete frame.second;
			}
		}

		Framebuffers.clear();
	}

	Framebuffer* FramebufferManager::CreateFramebuffer(std::string name, int width, int height)
	{
		auto find = Framebuffers.find(name);
		if (find == Framebuffers.end())
		{
			FramebufferSpecification spec;
			spec.width = width;
			spec.height = height;
			spec.Attachments = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
			
			Framebuffers.emplace(name, Framebuffer::Create(spec));
			return Framebuffers[name];
		}
		return find->second;
	}

	Framebuffer* FramebufferManager::GetFramebuffer(std::string name)
	{
		auto find = Framebuffers.find(name);
		if (find != Framebuffers.end())
		{
			return find->second;
		}
		AeonCore_ASSERT(find->second, "framebuffer not found!");
		return 0;
	}

	void FramebufferManager::RescaleFramebuffer(std::string name, int width, int height)
	{
		auto find = Framebuffers.find(name);
		if (find != Framebuffers.end())
		{
			find->second->Resize(width, height);
			
		}
		AeonCore_ASSERT(find->second, "Framebuffer not found!");
	}

	void FramebufferManager::BindFramebuffer(std::string name)
	{
		auto find = Framebuffers.find(name);
		if (find != Framebuffers.end())
		{
			find->second->Bind();
		}
	}

	void FramebufferManager::UnbindFramebuffer(std::string name)
	{
		auto find = Framebuffers.find(name);
		if (find != Framebuffers.end())
		{
			find->second->Unbind();
		}
	}

	unsigned int FramebufferManager::GetFrameTexture(std::string name)
	{
		auto find = Framebuffers.find(name);
		if (find != Framebuffers.end())
		{
			return find->second->GetColorAttachmentRendererID();
		}

		AeonCore_ASSERT(find->second->GetColorAttachmentRendererID(), "FrameBuffer ID is not good :(");
		return 0;
	}

	int FramebufferManager::ReadPixel(std::string name, uint32_t attachmentIndex, int x, int y)
	{
		auto find = Framebuffers.find(name);
		if (find != Framebuffers.end())
		{
			return find->second->ReadPixel(attachmentIndex, x, y);
		}

		AeonCore_ASSERT(find->second->GetColorAttachmentRendererID(), "FrameBuffer ID is not good :(");
		return -1;
	}

	uint32_t FramebufferManager::GetFrameBufferID(std::string name)
	{
		auto find = Framebuffers.find(name);
		if (find != Framebuffers.end())
		{
			return find->second->GetFrameBufferID();
		}
	}

	unsigned int FramebufferManager::GetCurrentFrameBufferAttachedToOpenGL()
	{
		GLint currentFramebuffer = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFramebuffer);
		return currentFramebuffer;
	}
	void FramebufferManager::ResetToDefaultFrameBufferOpenGL()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}