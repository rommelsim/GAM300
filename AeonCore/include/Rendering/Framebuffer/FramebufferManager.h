#pragma once
/*!***************************************************************************
	\file			FramebufferManager.h
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					manage framebuffers
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/AeonAPI.h"
#include "Rendering/Framebuffer/FrameBuffer.h"

namespace AeonCore
{
	class AEON_API FramebufferManager : public Singleton<FramebufferManager>
	{
	public:
		~FramebufferManager();
		
		Framebuffer* CreateFramebuffer(std::string name, int width, int height);
		Framebuffer* GetFramebuffer(std::string name);
		void		 RescaleFramebuffer(std::string name, int width, int height);
		void		 BindFramebuffer(std::string name);
		void		 UnbindFramebuffer(std::string name);
		unsigned int GetFrameTexture(std::string name);
		int			 ReadPixel(std::string name, uint32_t attachmentIndex, int x, int y);
		uint32_t	 GetFrameBufferID(std::string name);
		unsigned int GetCurrentFrameBufferAttachedToOpenGL();
		void	     ResetToDefaultFrameBufferOpenGL();
	private:
		static std::unordered_map<std::string, Framebuffer*> Framebuffers;
	};
}
