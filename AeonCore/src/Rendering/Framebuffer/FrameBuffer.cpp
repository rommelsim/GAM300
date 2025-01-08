#include "acpch.h"
/*****************************************************************//**
 * \file   FrameBuffer.cpp
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

#include "Rendering/Framebuffer/OpenGLFramebuffer.h"

namespace AeonCore
{
	Framebuffer* Framebuffer::Create(const FramebufferSpecification& spec)
	{
		return std::make_unique<OpenGLFramebuffer>(spec).release();
	}
	
	
	// void FrameBufferManager::Destroy()
	// {
	// 	if (Framebuffers.size() > 0)
	// 	{
	// 		for (auto it = Framebuffers.begin(); it != Framebuffers.end(); it++)
	// 		{
	// 			delete it->second;
	// 		}
	// 		Framebuffers.clear();
	// 	}
	// }
	// 
	// void FrameBufferManager::CreateFrameBuffer(std::string name, float width, float height)
	// {
	// 	FramebufferSpecification spec;
	// 	spec.width = width;
	// 	spec.height = height;
	// 
	// 	// Framebuffer::Create(spec);
	// 
	// 	/*auto search = Framebuffers.find(name);
	// 	if (search == Framebuffers.end())
	// 	{
	// 		Framebuffers.emplace(name, fb);
	// 		AeonCore_INFO("Frame buffer created: {0}", name);
	// 		return;
	// 	}
	// 	AeonCore_ERROR("Frame buffer failed to create: {0}", name);*/
	// }
	// 
	// unsigned int FrameBufferManager::GetFrameTexture(std::string name)
	// {
	// 	auto search = Framebuffers.find(name);
	// 	if (search != Framebuffers.end())
	// 	{
	// 		return search->second->GetColorAttachmentRendererID();
	// 	}
	// 	else
	// 	{
	// 		AeonCore_WARN("Frame buffer not found!: {0}", name);
	// 	}
	// 	return {};
	// }
	// 
	// void FrameBufferManager::BindFrameBuffer(std::string name)
	// {
	// 	auto search = Framebuffers.find(name);
	// 	if (search != Framebuffers.end())
	// 	{
	// 		search->second->Bind();
	// 	}
	// }
	// 
	// void FrameBufferManager::UnBindFrameBuffer(std::string name)
	// {
	// 	auto search = Framebuffers.find(name);
	// 	if (search != Framebuffers.end())
	// 	{
	// 		search->second->Unbind();
	// 	}
	// }
	// 
	// void FrameBufferManager::RescaleFrameBuffer(std::string name, float width, float height)
	// {
	// 	auto search = Framebuffers.find(name);
	// 	if (search != Framebuffers.end())
	// 	{
	// 		// search->second->RescaleFrameBuffer(width, height);
	// 	}
	// }
	// 
	// FrameBufferManager::~FrameBufferManager()
	// {
	// 	Destroy();
	// }
	// 
	// 
	// Framebuffer* Framebuffer::Create(const FramebufferSpecification& spec)
	// {
	// 	return std::make_shared<OpenGLFramebuffer>(spec).get();
	// }

	

}