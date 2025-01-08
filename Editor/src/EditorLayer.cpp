/*!***************************************************************************
	\file			Editor.cpp
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function definition to setup and display
					the editor
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

// From Engine
#include <Core/Log.h>
#include <Window/WindowsWindow.h>
//#include "Editor/ImGuiBuild.h"
// Panels Header file 
#include "EditorApp.h"
#include "EditorLayer.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/EditorViewport.h"
#include "Panels/GameViewport.h"
#include "Panels/AssetBrowser.h"
#include "Panels/DebugConsole.h"
#include "Panels/PerformanceGraph.h"
#include "Panels/SceneHierarchy.h"
#include "Panels/MenuBar.h"
#include "Panels/Model.h"
#include "ImGuiUtility.h"


namespace Editor
{
	int EditorLayer::m_EntityID = 0;
	EditorLayer::EditorLayer() : ImGuiLayer("EditorLayer")
	{
	}
	EditorLayer::~EditorLayer()
	{
	}
	void EditorLayer::OnAttach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		ImGuiUtility::SetDarkThemeColor();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		AeonCore::Application& app = EditorApp::GetApplication();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

			//AeonCore_INFO("   Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
			//AeonCore_INFO("   Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
			//AeonCore_INFO("   Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
			//AeonCore_INFO("   Shader: {0}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));


	}

	void EditorLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorLayer::OnImGuiRender()
	{

		static bool dockspace = true;
		static bool optFullscreen = true;
		static bool optPadding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (optFullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;
		if (!optPadding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &dockspace, window_flags);
		if (!optPadding)
			ImGui::PopStyleVar();

		if (optFullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minDockSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 350.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minDockSizeX;


		//Call All Panels Funtion here
		{
			MenuBar::ShowMenuBar();
			EditorViewport::GetInstance().ShowViewport(m_SceneHierarchy.GetSelectedEntityID());
			m_AssetBrowser.Show();
			DebugConsole::Show();
			//m_PerformanceGraph.Show();
			m_SceneHierarchy.OnImGuiRender();
			m_ToolBar.Show();
			uint32_t ent = m_SceneHierarchy.GetSelectedEntityID();
			m_Camera.Show(ent);
			m_Model.Show();
			if (AeonCore::Coordinator::GetInstance().EntityExists(ent)) {
				PropertiesPanel::Show(ent);
			} else {
				m_SceneHierarchy.SetSelectedEntityID(0);
			}
			/*if (EditorViewport::GetInstance().PixelPicking(GetEntityID()) == true) {
				PropertiesPanel::Show(m_SceneHierarchy.GetSelectedEntityID());
				
			}
			else if((EditorViewport::GetInstance().PixelPicking(GetEntityID()) == false)&&GetEntityID()==0){
				PropertiesPanel::Show(m_SceneHierarchy.GetSelectedEntityID());
			}*/
			
			
		}
		ImGui::End();
	}

	void EditorLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void EditorLayer::End()
	{
		ImGuiIO &io = ImGui::GetIO();
		AeonCore::Application & app = AeonCore::Application::GetApplication();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
		{
			GLFWwindow *backupContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupContext);
		}
	}

}