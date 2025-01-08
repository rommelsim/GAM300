#pragma once
/*!***************************************************************************
	\file			Toolbar.h
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Nov 1, 2023
	\brief			This header file consists of function declaration of toolbar
					panel on the editor.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
namespace Editor {
	class ToolBars {
	public:

		ToolBars() = default;
		/*!***************************************************************************
		*   \brief			This function show the play pause stop button for the editor
			\param  _fn		nil
			\return			void
		*******************************************************************************/
		 void Show();
	private:
		bool sceneSerialized = false;
	};
}