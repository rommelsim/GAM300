#pragma once
/*!***************************************************************************
	\file			Model.h
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Nov 1, 2023
	\brief			This header file consists of function definiton for the model loading on the editor
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <filesystem>
namespace Editor {


	class ModelEditor {

	public:

		ModelEditor();
		void Show();
	private:
		std::filesystem::path m_Directory;
	};
}