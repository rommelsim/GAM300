/*!***************************************************************************
	\file			main.cpp
	\author			
	\par			
	\brief			Entry point for a game application using the AeonCore framework.
					This file initializes and runs a game application based on the
					AeonCore framework. It loads a scene, sets up the runtime state,
					and handles the application's main loop.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <AeonCore.h>

class GameApp : public AeonCore::Application
{
public:
	GameApp() {
		//mEditorOn = false;
		if (!IsEditorOn())
		{
			AeonCore_INFO("Game Mode...");

			GLFWwindow* window		= static_cast<GLFWwindow*>(GetWindow().GetNativeWindow());
			GLFWmonitor* monitor	= glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			//	Set the window to full screen
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

			//	hide mouse cursor
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		// since by default, the runtime state is Stop, we should Start it up
		OnRuntimeStart();
		
		std::filesystem::path sceneFilepath = "./resources/scene/V1_LOGO.scene";

		AeonCore::SceneManager::LoadScene(sceneFilepath.string(), true, false);
	}
};

int main() {
#ifdef _DEBUG
	::ShowWindow(::GetConsoleWindow(), true);
#else
	::ShowWindow(::GetConsoleWindow(), false);
#endif
	AeonCore::log::init();

	GameApp* app = new GameApp();
	app->Run();
	
	delete app;
}