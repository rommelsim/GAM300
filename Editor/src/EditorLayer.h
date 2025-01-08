#pragma once
/*!***************************************************************************
	\file			Editor.h
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function declaration to setup and display
					the editor
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <Core/Singleton.hpp>
#include <Editor/ImGuiLayer.h>
#include "Panels/AssetBrowser.h"
#include "Panels/PerformanceGraph.h"
#include "Panels/SceneHierarchy.h"
#include "Panels/Toolbar.h"
#include "Panels/Camera.h"
#include "Panels/Model.h"

namespace Editor {
	class EditorLayer : public AeonCore::ImGuiLayer, public AeonCore::Singleton<EditorLayer> {
	public:
		EditorLayer();
		~EditorLayer();

		void OnAttach() override;
		void OnDetach() override;

		void OnImGuiRender() override;
		void Begin() override;
		void End() override;


		 static int GetEntityID()		{ return m_EntityID;	  }
		static void SetEntityID(int _EntityID) {  m_EntityID =_EntityID; }
	private:
		friend class Singleton<EditorLayer>;
		AssetBrowser m_AssetBrowser;
		PerformanceGraph m_PerformanceGraph;
		SceneHierarchyPanel m_SceneHierarchy;
		ToolBars m_ToolBar;
		Camera m_Camera;
		ModelEditor m_Model;

		static int m_EntityID;
		float mTime = 0.0f;
	};
}