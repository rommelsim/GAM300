#pragma once
/*!***************************************************************************
	\file			Toolbar.cpp
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Nov 1, 2023
	\brief			This source file consists of function declaration for settings on
					the editor
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <ECS/Architecture/Coordinator.hpp>
namespace Editor {

	class Camera {
	public: 
		Camera() = default;
		/*!***************************************************************************
		*   \brief			This function will take in an id and display editor settings on the editor
			\param  _fn		AeonCore::EntityID
			\return			void
		*******************************************************************************/
		void Show(AeonCore::EntityID& cameraID);

	private:

		AeonCore::EntityID m_Camera;
	};
}
