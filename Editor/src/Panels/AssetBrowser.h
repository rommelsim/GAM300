#pragma once
/*!***************************************************************************
	\file			AssetBrowser.h
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of function declaration to show the
					content browser on the editor.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <filesystem>
namespace Editor {

	class AssetBrowser {


	public:
		/*!***************************************************************************
		*   \brief			Constructor
			\param  _fn		nil
			\return			void
		*******************************************************************************/
		AssetBrowser();
		/*!***************************************************************************
		*   \brief			Show Content Browser
			\return			void
		*******************************************************************************/
		 void Show();
		 /*!***************************************************************************
		 *   \brief			Temporary fixed for the material and objects drag drop
			 \param  _fn	Filepath from object directory
			 \return		void
		 *******************************************************************************/
		 void TempFunction(std::filesystem::path x);

		 bool SearchandDisplay(const std::filesystem::path& directory, const std::string& search);
		 //void ShutDown();

		 void DrawButton(const std::filesystem::path& path);
	
	private:
		 std::filesystem::path m_CurrentDirectory;
		 bool showFindFile = false;
		 std::string file = "";
		 std::string previousSearch = "";
		 bool fileSearch = false;
		 std::vector<std::filesystem::path> filename;
	};

 }