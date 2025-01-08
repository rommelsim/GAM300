/*!***************************************************************************
	\file			Model.cpp
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Nov 1, 2023
	\brief			This source file consists of function defintion for model loading on
					the editor
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Model.h"
#include <imgui.h>
#include <iostream>
namespace Editor {

	extern const std::filesystem::path s_ModelPath = "./resources/objects";
	ModelEditor::ModelEditor()
	{
		m_Directory = s_ModelPath;
	}
	void ModelEditor::Show()
	{
		ImGui::Begin("Preloaded Model");
		ImGui::Columns(2, 0, false);


		for (auto& iterator : std::filesystem::directory_iterator(m_Directory)) {
			const auto& path = iterator.path();
			std::string objectFile = path.filename().string();
			ImGui::PushID(objectFile.c_str());


			float ht = ImGui::GetFrameHeight();
			if (path.filename().extension() == ".bin") {
				ImGui::Button(path.filename().string().c_str(), ImVec2{ 100.0f,ht });
				if (ImGui::BeginDragDropSource()) {

					ImGui::SetDragDropPayload("MOVING_ITEM", objectFile.c_str(), objectFile.size() + 1);
					ImGui::Text(objectFile.c_str());
					ImGui::EndDragDropSource();
				}
				ImGui::NextColumn();
			}

			ImGui::PopID();
		}


		ImGui::Columns(1);

		ImGui::End();
	}
}
