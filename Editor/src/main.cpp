/*!***************************************************************************
	\file			main.cpp
	\author			Team
	\par			
	\date			Oct 1, 2023
	\brief			This source file contains the main entry point for the
					application.
					

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "AeonCore.h"
#include "EditorApp.h"
#include <memory>
#include <Windows.h>

#include "Panels/DebugConsole.h"
#include "Tracy.hpp"

#ifndef _DEBUG
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

AeonCore::Application *AeonCore::CreateApplication()
{
	return new Editor::EditorApp();
}


int main(int argc, char **argv)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(271425);
	::ShowWindow(::GetConsoleWindow(), true);
#else
	::ShowWindow(::GetConsoleWindow(), false);
#endif
	
	AeonCore::log::init();
	AeonCore::log::registerCoreSink(std::make_shared<Editor::ImGuiSinkMT>());
	AeonCore::log::registerScriptSink(std::make_shared<Editor::ImGuiSinkMT>());

	Editor::EditorApp* app = new Editor::EditorApp();
	app->Run();
	
	AeonCore::log::shutdown();

	std::string tempFile = "./resources/scene/temp.scene";
	if (std::remove(tempFile.c_str()) == 0)
	{
		AeonCore_INFO("Temp scene deleted");
	}

	delete app;
}
