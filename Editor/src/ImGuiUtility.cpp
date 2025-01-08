/*!***************************************************************************
    \file			ImGuiUtility.cpp
    \author			Chew Shi Ru
    \par			shiru.chew@digipen.edu
    \date			Oct 1, 2023
    \brief			This source file consists of function defintion to
                    draw ImGui function aids in the process when doing the editor.
    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
//Editor
#include "ImGuiUtility.h"
//External lib
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
//Engine
#include <Core/Log.h>

namespace Editor {
    bool ImGuiUtility::DrawVec3Control(const std::string& label, glm::vec3& values, float reset, float columnWidth)
    {
        bool rommel_has_brain{ false };
        //To provide a unique ID for label so that they are not tied up
        ImGui::PushID(label.c_str());
        ImGui::Columns(2 , label.c_str(),false);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.5f,0.5f });
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImVec2 buttonSize = { lineHeight + 3.0f,lineHeight };
        //X
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.6f,0.0f,0.0f,1.f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.4f,0.0f,0.0f,1.f });
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.6f,0.0f,0.0f,1.f });
        if (ImGui::Button("X", buttonSize))
        {
            rommel_has_brain = true; // not really
            values.x = reset;
        }
        ImGui::SameLine();
        rommel_has_brain |= ImGui::DragFloat("##X", &values.x, 0.01f);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleColor(3);
        //Y
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.6f,0.0f,1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f,0.4f,0.0f,1.0f });
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.6f,0.0f,1.0f });
        if (ImGui::Button("Y", buttonSize))
        {
            rommel_has_brain = true; // not really
            values.y = reset;
        }
        ImGui::SameLine();
        rommel_has_brain |= ImGui::DragFloat("##Y", &values.y, 0.01f);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleColor(3);
        //Z
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.0f,0.6f,1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f,0.0f,0.4f,1.0f });
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.0f,0.6f,1.0f });
        if (ImGui::Button("Z", buttonSize))
        {
            rommel_has_brain = true; // not really
            values.z = reset;
        }
        ImGui::SameLine();
        rommel_has_brain |= ImGui::DragFloat("##Z", &values.z, 0.01f);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleVar();
        ImGui::Columns(1);
        ImGui::PopID();
        ImGui::PopStyleColor(3);

        return rommel_has_brain;
    }

    bool ImGuiUtility::DrawVec3ControlTmpForXuan(const std::string &label, glm::vec3 &values, float min, float max, float reset, float columnWidth)
    {
        bool rommel_has_brain{ false };
        //To provide a unique ID for label so that they are not tied up
        ImGui::PushID(label.c_str());
        ImGui::Columns(2, label.c_str(), false);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.5f,0.5f });
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImVec2 buttonSize = { lineHeight + 3.0f,lineHeight };
        //X
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.6f,0.0f,0.0f,1.f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.4f,0.0f,0.0f,1.f });
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.6f,0.0f,0.0f,1.f });
        if ( ImGui::Button("X", buttonSize) )
        {
            rommel_has_brain = true; // not really
            values.x = reset;
        }
        ImGui::SameLine();
        rommel_has_brain |= ImGui::DragFloat("##X", &values.x, 0.01f, min, max);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleColor(3);
        //Y
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.6f,0.0f,1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f,0.4f,0.0f,1.0f });
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.6f,0.0f,1.0f });
        if ( ImGui::Button("Y", buttonSize) )
        {
            rommel_has_brain = true; // not really
            values.y = reset;
        }
        ImGui::SameLine();
        rommel_has_brain |= ImGui::DragFloat("##Y", &values.y, 0.01f, min, max);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleColor(3);
        //Z
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.0f,0.6f,1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f,0.0f,0.4f,1.0f });
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.0f,0.6f,1.0f });
        if ( ImGui::Button("Z", buttonSize) )
        {
            rommel_has_brain = true; // not really
            values.z = reset;
        }
        ImGui::SameLine();
        rommel_has_brain |= ImGui::DragFloat("##Z", &values.z, 0.01f, min, max);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleVar();
        ImGui::Columns(1);
        ImGui::PopID();
        ImGui::PopStyleColor(3);

        return rommel_has_brain;
    }

	//bool ImGuiUtility::DrawVec4Control(const std::string& label, glm::quat& values, float reset, float columnWidth)
	//{
 //       bool rommel_has_brain{ false };

 //       //To provide a unique ID for label so that they are not tied up
 //       ImGui::PushID(label.c_str());
 //       ImGui::Columns(2, label.c_str(), false);
 //       ImGui::SetColumnWidth(0, columnWidth);
 //       ImGui::Text(label.c_str());
 //       ImGui::NextColumn();

 //       ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
 //       ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.5f,0.5f });
 //       float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
 //       ImVec2 buttonSize = { lineHeight + 3.0f,lineHeight };
 //       //X
 //       ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.6f,0.0f,0.0f,1.f });
 //       ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.4f,0.0f,0.0f,1.f });
 //       ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.6f,0.0f,0.0f,1.f });
 //       if (ImGui::Button("X", buttonSize))
 //       {
 //           rommel_has_brain = true; // not really
 //           values.x = reset;
 //       }
 //       ImGui::SameLine();
 //       rommel_has_brain |= ImGui::DragFloat("##X", &values.x, 0.01f);
 //       ImGui::PopItemWidth();
 //       ImGui::SameLine();
 //       ImGui::PopStyleColor(3);
 //       //Y
 //       ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.6f,0.0f,1.0f });
 //       ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f,0.4f,0.0f,1.0f });
 //       ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.6f,0.0f,1.0f });
 //       if (ImGui::Button("Y", buttonSize))
 //       {
 //           rommel_has_brain = true; // not really
 //           values.y = reset;
 //       }
 //       ImGui::SameLine();
 //       rommel_has_brain |= ImGui::DragFloat("##Y", &values.y, 0.01f);
 //       ImGui::PopItemWidth();
 //       ImGui::SameLine();
 //       
 //       ImGui::PopStyleColor(3);
 //       //Z
 //       ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.0f,0.6f,1.0f });
 //       ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f,0.0f,0.4f,1.0f });
 //       ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.0f,0.6f,1.0f });
 //       if (ImGui::Button("Z", buttonSize))
 //       {
 //           rommel_has_brain = true; // not really
 //           values.z = reset;
 //       }
 //       ImGui::SameLine();
 //       rommel_has_brain |= ImGui::DragFloat("##Z", &values.z, 0.01f);
 //       ImGui::PopItemWidth();
 //       ImGui::SameLine();
 //       
 //       
 //       ImGui::PopStyleColor(3);
 //       //W
 //       ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.0f,0.6f,1.0f });
 //       ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f,0.0f,0.4f,1.0f });
 //       ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.0f,0.6f,1.0f });
 //       if (ImGui::Button("W", buttonSize))
 //       {
 //           rommel_has_brain = true; // not really
 //           values.w = reset;
 //       }
 //       ImGui::SameLine();
 //       rommel_has_brain |= ImGui::DragFloat("##W", &values.w, 0.01f);
 //       ImGui::PopItemWidth();
 //       ImGui::SameLine();
 //       
 //       
 //       ImGui::PopStyleVar();
 //       ImGui::Columns(1);
 //       ImGui::PopID();
 //       ImGui::PopStyleColor(3);

 //       return rommel_has_brain;
	//}
	void ImGuiUtility::DrawTextLabelControl(const std::string& tag, const char* label, char* buffer, size_t bufferSize, float columnWidth)
	{
        ImGui::Columns(2,label,false);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(tag.c_str());
        ImGui::NextColumn();
        //no changes should be make 
        ImGui::InputText(label, buffer, bufferSize);
        ImGui::Columns(1,label);

	}
    void ImGuiUtility::DrawLabelControl(const std::string& tag, const std::string& label, float columnWidth)
    {
        ImGui::Columns(2, label.c_str(), false);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(tag.c_str());
        ImGui::NextColumn();
        //no changes should be make 
        ImGui::Text(label.c_str());
        ImGui::Columns(1);

    }
    bool ImGuiUtility::DrawEnterInput(const std::string& tag, const char* label, std::string* text, float columnWidth)
    {
        bool input;
        ImGui::Columns(2, label, false);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(tag.c_str());
        ImGui::NextColumn();
        //input = ImGui::InputText(label, &text, ImGuiInputTextFlags_EnterReturnsTrue,ImGuiInputTextFlags_CallbackResize,InputTextCallback);
        input =ImGui::InputText(label, text,ImGuiInputTextFlags_EnterReturnsTrue);
        
        //ImGui::inputtext
        ImGui::Columns(1);
        return input;
    }
	void ImGuiUtility::DrawDragFloat(const std::string& tag, const char* label, float* v, float columnWidth)
	{
        ImGui::Dummy(ImVec2{ 0.05f,0.05f });
        ImGui::Columns(2, label, false);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(tag.c_str());
        ImGui::NextColumn();
        ImGui::DragFloat(label, v, 0.01f,0.0f,1.0f);
        ImGui::Columns(1);
	}
    void ImGuiUtility::DrawDragFloat(const std::string& tag, const char* label, float* v, float min, float max, float columnWidth)
    {
        ImGui::Dummy(ImVec2{ 0.05f,0.05f });
        ImGui::Columns(2, label, false);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(tag.c_str());
        ImGui::NextColumn();
        ImGui::DragFloat(label, v, 0.01f, min, max);
        ImGui::Columns(1);
    }
	void ImGuiUtility::DrawDropDragTarget(const std::string& tag, const char* label, float columnWidth)
	{
        ImGui::Columns(2, label, false);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(tag.c_str());
        ImGui::NextColumn();
        //ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.604f,0.325f,0.204f ,1.0f });
        //left align button text; if center aligned is wanted change value to ImVec2{0.5f,0.5f}
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2{ 0.0f,0.0f });
        ImGui::PushID(tag.c_str());
        ImGui::Button(label, ImVec2{ 160.0f,0.0f });
        ImGui::PopID();
        ImGui::Columns(1);
        //ImGui::PopStyleColor();
        ImGui::PopStyleVar();
	}
    void ImGuiUtility::DrawColor(const std::string& tag, const char* label, float value[3], float columnWidth)
    {
        ImGui::Columns(2, label, false);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(tag.c_str());
        ImGui::NextColumn();
        ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
        ImGui::ColorEdit3(label, value, ImGuiColorEditFlags_Float);
        ImGui::Columns(1);
        ImGui::PopItemWidth();
	}
    void ImGuiUtility::DrawColor(const std::string& label, AeonCore::Render& renderCompo,  const glm::vec3& component, float columnWidth) {
        float color[3] = { component.r, component.g, component.b };
        ImGui::Columns(2, label.c_str(), false);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();
        ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
        auto colorLabel = "##Color" + label + "Picker";
        ImGui::ColorEdit3(colorLabel.c_str(), color, ImGuiColorEditFlags_Float);
        ImGui::Columns(1);
        ImGui::PopItemWidth();
        //checking might change to enum 
       // if (label == " Ambient") {
       //     renderCompo.SetLightAmbient(glm::vec3(color[0], color[1], color[2]));

            //std::cout << "Ambient Color" << color[0] << " " << color[1] << " " << color[2]<<std::endl;
        //}
        //if (label == " Diffuse") {
        //    renderCompo.SetLightDiffuse(glm::vec3(color[0], color[1], color[2]));
        //    //std::cout << "Diffuse Color" << color[0] << " " << color[1] << " " << color[2] << std::endl;
        //}
        //if (label == " Specular") {
        //    renderCompo.SetLightSpecular(glm::vec3(color[0], color[1], color[2]));
        //    //std::cout << "Specular Color" << color[0] << " " << color[1] << " " << color[2] << std::endl;
        //}

    }


	void ImGuiUtility::SetDarkThemeColor()
	{
        auto& color = ImGui::GetStyle().Colors;
        auto& style = ImGui::GetStyle();
        style.FrameRounding = 6.0f;
        //Headers
        color[ImGuiCol_Header] = ImVec4{ 0.243f,0.243f,0.243f ,1.0f };
        color[ImGuiCol_HeaderHovered] = ImVec4{ 0.022f,0.22f,0.22f ,1.0f };

        //Frame
        color[ImGuiCol_FrameBg] = ImVec4{ 0.1f,0.1f,0.1f,1.0f };
        color[ImGuiCol_FrameBgHovered] = ImVec4{ 0.35f,0.35f,0.35f,1.0f };
        color[ImGuiCol_FrameBgActive] = ImVec4{ 0.43f,0.43f,0.43f,1.0f };
     
        // Checbox
        color[ImGuiCol_CheckMark] = ImVec4{ 1.0f,1.0f,1.0f,1.0f };


        // Buttons
        color[ImGuiCol_Button] = ImVec4{ 0.1f,0.1f,0.1f,1.0f };
        color[ImGuiCol_ButtonHovered] = ImVec4{ 0.43f,0.43f,0.43f,1.0f };
        color[ImGuiCol_ButtonActive] = ImVec4{ 0.47f,0.47f,0.47f,1.0f };

        // Tabs
        color[ImGuiCol_Tab] = ImVec4{ 0.22f,0.22f,0.22f,1.0f };
        color[ImGuiCol_TabHovered] = ImVec4{ 0.22f,0.22f,0.22f,1.0f };
        color[ImGuiCol_TabActive] = ImVec4{ 0.36f,0.36f,0.36f,1.0f };
        color[ImGuiCol_TabUnfocused] = ImVec4{ 0.16f,0.16f,0.16f,1.0f };
        color[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.35f,0.35f,0.35f,1.0f };

        // Title
        color[ImGuiCol_TitleBg] = ImVec4{ 0.16f,0.16f,0.16f,1.0f };
        color[ImGuiCol_TitleBgActive] = ImVec4{ 0.16f,0.16f,0.16f,1.0f };

        // Others
        color[ImGuiCol_WindowBg] = ImVec4{ 0.18f,0.18f,0.18f,1.0f };
        color[ImGuiCol_PopupBg] = ImVec4{ 0.18f,0.18f,0.18f,1.0f };
        color[ImGuiCol_DockingPreview] = ImVec4{ 0.11f,0.11f,0.11f,1.0f };
	}
    bool ImGuiUtility::DrawCheckBox(const std::string& tag, const char* label, bool* v, float columnWidth)
    {
        //bool check;
        ImGui::Columns(2, "checkbox", false);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(tag.c_str());
        ImGui::NextColumn();
        if (ImGui::Checkbox(label, v)) {
            if (v) {
                return true;
            }
            else {
                return false;
            }
        }
        ImGui::Columns(1, "checkbox", false);
       
       return true;
    }

    bool ImGuiUtility::DrawInputInt(const std::string& tag, const char* label, int* value, float columWidth)
    {
        bool input;
        ImGui::Columns(2, label, false);
        ImGui::SetColumnWidth(0, columWidth);
        ImGui::Text(tag.c_str());
        ImGui::NextColumn();
        input = ImGui::InputInt(label, value);
        ImGui::Columns(1);
        return input;
    }

    bool ImGuiUtility::DrawInputInt(const std::string& tag, const char* label, unsigned int* value, float columWidth)
    {
        bool input;
        int intValue = static_cast<int>(*value);
        if (intValue < 0)
            intValue = 0;
        ImGui::Columns(2, label, false);
        ImGui::SetColumnWidth(0, columWidth);
        ImGui::Text(tag.c_str());
        ImGui::NextColumn();
        input = ImGui::InputInt(label, &intValue);  
        *value = static_cast<unsigned int>(intValue);
        ImGui::Columns(1);
        return input;
    }


    bool ImGuiUtility::DrawInputFloat(const std::string& tag, const char* label, float* value, float step, float step_fast, const char* format, float coloumWidth)
    {
        bool input;
        ImGui::Columns(2, label, false);
        ImGui::SetColumnWidth(0, coloumWidth);
        ImGui::Text(tag.c_str());
        ImGui::NextColumn();
        input = ImGui::InputFloat(label, value, step, step_fast, format, ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::Columns(1);
        return input;
    }

    bool ImGuiUtility::DrawInputDouble(const std::string& tag, const char* label, double* value, float step, float step_fast, const char* format, float coloumWidth)
    {
        bool input;
        ImGui::Columns(2, label, false);
        ImGui::SetColumnWidth(0, coloumWidth);
        ImGui::Text(tag.c_str());
        ImGui::NextColumn();
        input = ImGui::InputDouble(label, value, step, step_fast, format, ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::Columns(1);
        return input;
    }
    void ImGuiUtility::DrawErrorMessage(const std::string& tag, std::string fileExtension)
    {
        if (ImGui::BeginPopupModal(tag.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("An error has occurred:");
            ImGui::Text("Error message: Wrong file extension %s", fileExtension.c_str());
            ImGui::Text("File should be compiled check Asset Browser again");
            if (ImGui::Button("OK", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

    }
}