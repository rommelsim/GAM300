/*!***************************************************************************
	\file			AssetBrowser.cpp
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function definitions show 
					content browser on the editor.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
//Editor
#include "AssetBrowser.h"
//External Library
#include <imgui.h>
//Engine
#include <Resource/ResourceManager.h>
#include <Core/Log.h>
#include <Resource/Texture.h>
#include "../ImGuiUtility.h"

//#include <imgui_impl_win32.h>
namespace Editor {

	extern const std::filesystem::path s_AssetBrowserPath	= "./resources";
	extern const std::filesystem::path s_AssetShaderPath	= "./resources/shaders";
	extern const std::filesystem::path s_AssetObjectPath	= "./resources/objects";
	extern const std::filesystem::path s_AssetScriptsPath	= "./resources/scripts";
	extern const std::filesystem::path s_AssetAudioPath		= "./resources/audios";
	extern const std::filesystem::path s_AssetTexturePath	= "./resources/textures";
	extern const std::filesystem::path s_AssetFontPath		= "./resources/fonts";
	extern const std::filesystem::path s_AssetScenePath		= "./resources/scene";

	/*
		----- Things to do -----
		1. Drag Drop of the new button 
		2. Display Image if is a dds
		3. show search related to it as well 
		4. Back button tied to the top as well 
	
	*/
	//.filename is temporary will change to .stem in upcoming milestone to include thumbnail
	AssetBrowser::AssetBrowser()
	{
		m_CurrentDirectory = s_AssetBrowserPath;
	}
	std::string searching;
	void AssetBrowser::Show()
	{

		//stem return file name without extension 
		ImGui::Begin("AssetBrowser", NULL, ImGuiWindowFlags_MenuBar);
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImGui::BeginMenuBar();
		if (ImGuiUtility::DrawEnterInput("Search", "##search", &searching)) {
			AeonCore_WARN("Search: {0}", searching.c_str());
			file = searching;
			fileSearch = true;
			filename.clear();
		}
		
		ImGui::EndMenuBar();
		
		if (fileSearch) {
			previousSearch = file;
			showFindFile = SearchandDisplay(m_CurrentDirectory, previousSearch);
			if (!showFindFile||searching=="") {
				fileSearch = false;
			}
		}
		for (size_t i = 0; i < filename.size(); ++i) {
			//std::cout << filename[i].string() << std::endl;
			DrawButton(filename[i]);
		}
		ImGui::Separator();
		// Display the button for the found file (if any)
		if (!showFindFile) {
			// Display a message if the file is not found
			ImGui::Text("File not found: %s", file.c_str());
		}
		const int columns = 4; // You can adjust this value as needed
		ImGui::Columns(columns, 0, false);
		//Back Button
		{
			if (m_CurrentDirectory != std::filesystem::path(s_AssetBrowserPath)) {

				if (ImGui::BeginPopupContextWindow("##BackButton", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {

					if (ImGui::MenuItem("Back")) {
						m_CurrentDirectory = m_CurrentDirectory.parent_path();
					}

					ImGui::EndPopup();
				}
				//ImGui::PopStyleColor(3);
			}
		}

		//Shader folder Green button->frag file : Blue button ->vert file
		if (m_CurrentDirectory == std::filesystem::path(s_AssetShaderPath)) {
			for (auto& it : std::filesystem::directory_iterator(s_AssetShaderPath)) {
				const auto& path = it.path();

				if (path.extension() == ".frag") {
					std::string assetName = path.stem().string();
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.8f,0.0f,1.f });
					ImGui::Button(path.stem().string().c_str());
					ImGui::PopStyleColor();
				}
				ImGui::SameLine();
				if (path.extension() == ".vert") {
					std::string assetName = path.stem().string();
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.0f,0.8f,1.f });
					ImGui::Button(path.stem().string().c_str());
					ImGui::PopStyleColor();
				}
		
			}
		}
		//Object Directory
		else if (m_CurrentDirectory == std::filesystem::path(s_AssetObjectPath)) {
			for (auto& it : std::filesystem::directory_iterator(s_AssetObjectPath)) {
				const auto& path = it.path();
				std::string assetFile = path.filename().string();
				ImGui::Button(assetFile.c_str());

				if (ImGui::BeginDragDropSource()) {

					ImGui::SetDragDropPayload("CONTENT_OBJECT_ITEM", assetFile.c_str(), assetFile.size() + 1);
					ImGui::Text(path.filename().string().c_str());
					ImGui::EndDragDropSource();
				}
				ImGui::NextColumn();
			}

		}
		

		//Scripts Directory
		else if (m_CurrentDirectory == std::filesystem::path(s_AssetScriptsPath)) {
			for (auto& it : std::filesystem::directory_iterator(s_AssetScriptsPath)) {
				const auto& path = it.path();
				ImGui::Button(path.filename().string().c_str());
			}
		}
		//Audio Directory
		else if (m_CurrentDirectory == std::filesystem::path(s_AssetAudioPath)) {
			for (auto& it : std::filesystem::directory_iterator(s_AssetAudioPath)) {
				const auto& path = it.path();
				std::string assetFile = path.filename().string();
				ImGui::Button(assetFile.c_str());
				if (ImGui::BeginDragDropSource()) {

					ImGui::SetDragDropPayload("CONTENT_ITEM", assetFile.c_str(), assetFile.size() + 1);
					ImGui::Text(path.filename().string().c_str());
					ImGui::EndDragDropSource();
				}
				ImGui::NextColumn();
			}
		}
		else if (m_CurrentDirectory == std::filesystem::path(s_AssetScenePath)) {
			for (auto& it : std::filesystem::directory_iterator(s_AssetScenePath)) {
				const auto& path = it.path();
				std::string assetFile = path.filename().string();
				ImGui::Button(assetFile.c_str());
				if (ImGui::BeginDragDropSource()) {

					ImGui::SetDragDropPayload("CONTENT_SCENE_ITEM", assetFile.c_str(), assetFile.size() + 1);
					ImGui::Text(path.filename().string().c_str());
					ImGui::EndDragDropSource();
				}
				ImGui::NextColumn();
			}
		}
		//Audio Directory
		else if (m_CurrentDirectory == std::filesystem::path(s_AssetFontPath)) {
			for (auto& it : std::filesystem::directory_iterator(s_AssetFontPath)) {
				const auto& path = it.path();
				std::string assetFile = path.filename().string();
				ImGui::Button(assetFile.c_str());
				if (ImGui::BeginDragDropSource()) {

					ImGui::SetDragDropPayload("CONTENT_FONT_ITEM", assetFile.c_str(), assetFile.size() + 1);
					ImGui::Text(path.filename().string().c_str());
					ImGui::EndDragDropSource();
				}
				ImGui::NextColumn();
			}
		}
		//texture Directory

		else if (m_CurrentDirectory == std::filesystem::path(s_AssetTexturePath)) {

			for (auto& it : std::filesystem::directory_iterator(s_AssetTexturePath)) {
	
				auto& resourceManager = AeonCore::ResourceManager::GetInstance();
				std::filesystem::path working_dir(std::filesystem::current_path());
				auto parent_path = working_dir.string();
				const auto& path = it.path();
				std::string assetFile = path.filename().string();
				std::string assetFileExt = path.filename().extension().string();
				std::string loadFile = parent_path + "/resources/textures/" + assetFile;
				resourceManager.LoadTexture(loadFile, loadFile, AeonCore::Texture::Type::None);
				auto loadFileTexture = *resourceManager.GetTexture(loadFile);
				ImGui::Image(reinterpret_cast<void*>((uint64_t)(loadFileTexture.GetTextureID())), ImVec2(120.0f, 120.0f));
				ImGui::Button(assetFile.c_str(), ImVec2(120.0f, 20.0f));

				if (ImGui::BeginDragDropSource()) {

					ImGui::SetDragDropPayload("CONTENT_ITEM", assetFile.c_str(), assetFile.size() + 1);
					if (assetFileExt == ".dds") {
						ImGui::Image(reinterpret_cast<void*>((uint64_t)(loadFileTexture.GetTextureID())), ImVec2(120.0f, 120.0f));
					}
					else {
						ImGui::Text(assetFile.c_str());
					}
					ImGui::EndDragDropSource();
				}
				
				ImGui::NextColumn();
			}

			
		}
		

		//Recursive for main file explorer
		else
		{
			for (auto& iterator : std::filesystem::directory_iterator(m_CurrentDirectory)) {
				const auto& path = iterator.path();
				 auto filenamepathCurrent = path.filename();

				ImGui::PushID(path.filename().string().c_str());
				

				float ht = ImGui::GetFrameHeight();
				ImGui::Button(path.filename().string().c_str(), ImVec2{ 100.0f,ht });
		
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					if (iterator.is_directory()) {
						m_CurrentDirectory /= path.filename();
					}
				
				}
			
				ImGui::NextColumn();

				ImGui::PopID();
			}
		}



		ImGui::Columns(1);

		ImGui::End();
	}
	void AssetBrowser::TempFunction(std::filesystem::path x)
	{
		for (auto& it2 : std::filesystem::directory_iterator(x)) {
			const auto& path2 = it2.path();
			std::string assetFile = path2.filename().string();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f,0.0f,0.8f,1.f });
			ImGui::Button(assetFile.c_str());
			ImGui::PopStyleColor();
			if (ImGui::BeginDragDropSource()) {

				ImGui::SetDragDropPayload("CONTENT_ITEM", assetFile.c_str(), assetFile.size() + 1);
				ImGui::Text(path2.filename().string().c_str());
				ImGui::EndDragDropSource();
			}
			ImGui::NextColumn();
		}
	}
	//Search Function
	bool AssetBrowser::SearchandDisplay(const std::filesystem::path& directory, const std::string& search)
	{
		// Convert search string to lowercase
		std::string searchLowercase = search;
		std::transform(searchLowercase.begin(), searchLowercase.end(), searchLowercase.begin(), [](unsigned char c) { return std::tolower(c); });

		bool found = false; // Track if any match is found
		std::filesystem::path fileMatch;
		for (const auto& entry : std::filesystem::directory_iterator(directory)) {
			const auto& path = entry.path();

			if (std::filesystem::is_directory(path)) {
				// Recursively search within subdirectories
				if (SearchandDisplay(path, search)) {
					found = true; // Set found to true if a match is found in subdirectories
				}
			}
			else {
				// Convert filename and stem to lowercase
				std::string filenameLowercase = path.filename().string();
				std::transform(filenameLowercase.begin(), filenameLowercase.end(), filenameLowercase.begin(), [](unsigned char c) { return std::tolower(c); });

				std::string stemnameLowercase = path.stem().string();
				std::transform(stemnameLowercase.begin(), stemnameLowercase.end(), stemnameLowercase.begin(), [](unsigned char c) { return std::tolower(c); });

				// Check if the filename or stem matches the search query (case-insensitive)
				if (searchLowercase == filenameLowercase || searchLowercase == stemnameLowercase) {
					// If found, you can perform additional actions here
					fileMatch = path;
					//std::cout << fileMatch << std::endl;
					found = true; // Set found to true if a match is found
					searchLowercase = "";
				}
			}
		}
		std::string searchDone = "";
		searching = searchDone;
		if (found) {
			filename.push_back(fileMatch);
		}
		return found; // Return whether any match is found
	}

	void AssetBrowser::DrawButton(const std::filesystem::path& path)
	{
		if (!filename.empty()) {
			std::string selectfile = path.filename().string();
			if (path.filename().extension() == ".dds" || path.filename().extension() == ".png" || path.filename().extension() == ".wav") {
				ImGui::Button(path.filename().string().c_str());
				if (ImGui::BeginDragDropSource()) {

					ImGui::SetDragDropPayload("CONTENT_ITEM", selectfile.c_str(), selectfile.size() + 1);
					ImGui::Text(selectfile.c_str());
					ImGui::EndDragDropSource();
				}


			}
			//object
			if (path.filename().extension() == ".bin" || path.filename().extension() == ".obj") {
				ImGui::Button(path.filename().string().c_str());
				if (ImGui::BeginDragDropSource()) {
					ImGui::SetDragDropPayload("CONTENT_OBJECT_ITEM", selectfile.c_str(), selectfile.size() + 1);
					ImGui::Text(selectfile.c_str());
					ImGui::EndDragDropSource();
				}

			}
		}
	
	}
	
}