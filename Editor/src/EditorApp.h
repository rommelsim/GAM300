#pragma once
/*!***************************************************************************
	\file			EditorApp.h
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function declaration to setup
					and display the editor
					
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <AeonCore.h>

#include <FileWatch.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Editor {
	class EditorApp : public AeonCore::Application
	{
	public:
		bool mDidSubmitToMainThread{ false };

		EditorApp();
		~EditorApp() = default;
		static EditorApp* GetEditorApp();

		void InitializeFileWatch();
		void UnsetFileWatch();
	private:
		std::unique_ptr<filewatch::FileWatch<std::string>> mScriptFileWatch{};
	};

}