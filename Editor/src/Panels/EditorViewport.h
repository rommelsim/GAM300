#pragma once
/*!***************************************************************************
	\file			EditorViewport.h
	\author			Rommel, Lee Zhong En, Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of function declaration for the editor
					viewport
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <Rendering/Framebuffer/OpenGLFramebuffer.h>
#include <ECS/Architecture/Coordinator.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Window/Window.h>
#include "PropertiesPanel.h"
#include "../EditorLayer.h"
namespace Editor {
	class EditorViewport : public AeonCore::Singleton<EditorViewport>{
	public:
		//Camera* editorvp_camera;
		/*!***************************************************************************
		*   \brief			Initialize viewport camera
			\param			nil
			\return			void
		*******************************************************************************/
		void Init();
		/*!***************************************************************************
		*   \brief			Show viewport
			\param			EntityID entitiy
			\return			void
		*******************************************************************************/
		void ShowViewport(AeonCore::EntityID entitiy);
		void ShutDown();
		static bool PixelPicking(AeonCore::EntityID entityID);

		AeonCore::EntityID GetPixelID()			  { return m_pixelID; }
		void SetPixelID(AeonCore::EntityID _pixelID) { m_pixelID = _pixelID;}
		EditorViewport();
		~EditorViewport();

	private:
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		// Viewport -> Clip Space -> View -> World
		glm::vec3 ConvertViewportToNDC(int x, int y, int width, int height);
		glm::vec4 ConvertNDCToClip(glm::vec3 ray_nds);
		glm::vec4 ConvertClipToEye(glm::mat4 projection, glm::vec4 ray_clip);
		glm::vec3 ConvertEyeToWorld(glm::mat4 view, glm::vec3 ray_eye);
		
		AeonCore::Window* m_window;		// handle to application window.
		AeonCore::EntityID m_pixelID;
		PropertiesPanel m_Props;
		//EditorLayer m_Editor;
	};
}