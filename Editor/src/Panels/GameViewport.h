#pragma once
/*!***************************************************************************
	\file			GameViewport.h
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of function declaration to show
					a game viewport on the editor. (Upcoming)
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <Window/Window.h>
#include <Rendering/Framebuffer/OpenGLFramebuffer.h>

namespace Editor {
	class GameViewport : public AeonCore::Singleton<GameViewport> {

	public:


		void Init();
		static void ShowViewport();
		void ShutDown();
		GameViewport();
		~GameViewport();
	private:
		AeonCore::Window* m_GameViewport;
	};
}