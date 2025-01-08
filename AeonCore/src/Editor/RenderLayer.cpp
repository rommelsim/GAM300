#include "acpch.h"
/*!***************************************************************************
	\file			RenderLayer.cpp
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function definitions for the
					RenderLayer class, which is responsible for rendering
					graphics and managing rendering-related tasks in the
					application.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/


#include "Editor/RenderLayer.h"

#include "ECS/Components/Render.h"

/// 1 - Enable
///	0 - Disable
#define TEST_DRAW_FUNCTIONS 0

namespace AeonCore
{
	RenderLayer::RenderLayer()
		:Layer("Render Layer")
	{
	}

	RenderLayer::~RenderLayer()
	{
	}

	void RenderLayer::OnAttach()
	{

	}

	void RenderLayer::OnDetach()
	{
		//delete m_renderLayer;
	}

	void RenderLayer::OnUpdate()
	{
		// Update internal camera stuff

		// Do not put draw calls here!!!
	}

	void RenderLayer::onRender()
	{
		Rendering::FrameStart();
		//FrameBufferManager::GetInstance().BindFrameBuffer("f1");

#if TEST_DRAW_FUNCTIONS
		//TODO: Remove this once we integrate into ECS
		{
			///	COLOURS
			glm::vec3 red = { 1,0,0 };
			glm::vec3 blue = { 0,0,1 };
			glm::vec3 green = { 0,1,0 };
			glm::vec3 white = { 1,1,1 };

			glm::quat quatRotation = { glm::radians(45.f), 1.0f, 0.0f, 0.0f };
			glm::vec3 rotation = glm::eulerAngles(quatRotation);
			glm::vec3 position, scale;
			float radius;

			Camera* cam = EditorViewport::GetInstance().editorvp_camera;

			///	DRAW LINES
			{
				glm::vec3 origin = { 0,0,0 };
				glm::vec3 x_axis = { 1,0,0 };
				glm::vec3 y_axis = { 0,1,0 };
				glm::vec3 z_axis = { 0,0,1 };

				Rendering::GetInstance().DrawLine(origin, x_axis, red, cam);
				Rendering::GetInstance().DrawLine(origin, y_axis, green, cam);
				Rendering::GetInstance().DrawLine(origin, z_axis, blue, cam);
			}

			/// DRAW TEXTURE
			{
				position = { 1, 1, 1 };
				scale = { 1,1,1 };
				Rendering::GetInstance().DrawTexture("container", scale, rotation, position, cam);
			}

			/// DRAW CUBE FILLED
			{
				position = { 2, 2, 2 };
				scale = { 1,1,1 };
				Rendering::GetInstance().DrawCubeFilled(scale, rotation, position, red, cam);
			}

			/// DRAW CUBE WIREFRAME
			{
				position = { 2, 2, 2 };
				scale = { 1.5,1.5,1.5 };
				Rendering::GetInstance().DrawCubeFrame(scale, rotation, position, green, cam);
			}

			/// DRAW TRIANGLE
			{
				position = { -1, -1, -1 };
				scale = { 1,1,1 };
				Rendering::GetInstance().DrawTriangle(scale, rotation, position, green, cam);
			}

			/// DRAW CIRCLE
			{
				radius = 1.0f;
				position = { -2, -2, -2 };
				Rendering::GetInstance().DrawCircle(radius, rotation, position, blue, cam);
			}

			///	DRAW CIRCLE FRAME
			{
				radius = 1.5f;
				position = { -2, -2, -2 };
				Rendering::GetInstance().DrawCircleFrame(radius, rotation, position, green, cam);
			}

			/// DRAW SPHERE
			{
				radius = 1.0f;
				position = { 0, 5, 0 };
				Rendering::GetInstance().DrawSphere(radius, position, white, cam);
			}

			/// DRAW SPHERE FRAME
			{
				radius = 1.5f;
				position = { 0, 5, 0 };
				Rendering::GetInstance().DrawSphereFrame(radius, position, green, cam);
			}
		}
#endif

		//FrameBufferManager::GetInstance().UnBindFrameBuffer("f1");
	}
}
