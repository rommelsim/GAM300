#pragma once
/*!***************************************************************************
	\file			PerformanceGraph.h
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function defintiion to show
					the performance of various system on the editor.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
//Editor
#include "PerformanceGraph.h"
#include "../ImGuiUtility.h"
//External lib
#include <imgui.h>
//Engine
#include <Core/Application.h>
//#include <Core/Config.h>
#include <Core/Log.h>

namespace Editor {

	PerformanceGraph::PerformanceGraph()
	{

	}
	PerformanceGraph::~PerformanceGraph()
	{
	}
	void PerformanceGraph::Show()
	{
		AeonCore::Rendering render;
		float fps = 60.0f; (void)fps;
		ImGui::Begin("Performance Graph");
		ImGui::Text("OpenGL info: ");

		auto test = &AeonCore::TimerStep::GetInstance();
		auto fpsRate = 1.0f/ test->get_delta_time();
		//ImGuiUtility::DrawLabelControl("Vendor: ", render.GetGLVendor());
		//ImGuiUtility::DrawLabelControl("Version: ", render.GetGLVersion());
		//ImGuiUtility::DrawLabelControl("Shader: ", render.GetGLShader());
		//ImGuiUtility::DrawLabelControl("Renderer: ", render.GetGLRenderer());
		ImGui::Text("FPS: %.2f", fpsRate);
		//AeonCore_WARN("FPS: {}%.2f", fpsRate);
		//std::cout << "FPS: " << g_Config.m_CurrentFrameRate<<std::endl;
		static float fpsValue[90] = {};
		//ImGui::PlotLines("##FPS", arr, IM_ARRAYSIZE(arr));
		/*if (frames.size() > 100) {
			for (unsigned int i = 1; i < frames.size(); i++) {
				frames[i - 1] = frames[i];
			}
			frames[frames.size() - 1] = AeonCore::g_Config.m_CurrentFrameRate;
		}
		else {
			frames.push_back(AeonCore::g_Config.m_CurrentFrameRate);
		}*/
		
		if (!frames.empty()) {
			ImGui::PlotLines("##FPS", &frames[0],(int)frames.size());
		}
		else {
			ShutDown();
		}

		ImGui::End();
	}
	void PerformanceGraph::ShutDown()
	{
		frames.clear(); 
	}
}