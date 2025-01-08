#pragma once
/*!***************************************************************************
	\file			MenuBar.h
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of function declaration for menu bar on
					the editor
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <string>
namespace Editor {

	class MenuBar {

	public : 
		/*!***************************************************************************
		*   \brief			This function will draw the menu bar on the editor
			\param  _fn		nil
			\return			void
		*******************************************************************************/
		static void ShowMenuBar();
		static void SaveFile();
		static void OpenFile();
		static void NewFile();
		static void HotKeyOperation();

		std::string GetInputFile() { return m_inputFile; }
		void SetInputFile(std::string _inputFile) { m_inputFile = _inputFile; }
	private : 
		std::string m_inputFile;
	};
}
