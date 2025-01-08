/*!***************************************************************************
	\file			DebugConsole.cpp
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function definition to show
					debug console on the editor.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "DebugConsole.h"
#include <imgui.h>
#include <tuple>
namespace Editor {
	std::vector<std::tuple<std::string,int,std::string>>m_logMessage;
		bool debug = true;
		bool warn = true;
		bool error = true;
		bool script = true; 
		bool engine = true;
        void RenderLogMessage(const std::tuple<std::string, int, std::string>& logMessage)
        {
            //const std::string& name = std::get<0>(logMessage);
            int messageId = std::get<1>(logMessage);
            const std::string& message = std::get<2>(logMessage);

            // Determine text color based on message ID
            ImVec4 color;
            if (messageId == 2) {
                color = ImVec4(0.0f, 0.8f, 0.0f, 1.0f); // Green for debug
            }
            else if (messageId == 3) {
                color = ImVec4(0.8f, 0.8f, 0.0f, 1.0f); // Yellow for warning
            }
            else if (messageId == 4) {
                color = ImVec4(0.8f, 0.0f, 0.0f, 1.0f); // Red for error
            }

            // Render text with appropriate color
            ImGui::TextColored(color, message.c_str());
        }

        void DebugConsole::Show()
        {
            ImGui::Begin("Debug Console", NULL, ImGuiWindowFlags_MenuBar);
            ImVec2 windowSize = ImGui::GetWindowSize();
            ImGui::BeginMenuBar();
            ImGui::Checkbox("Debug", &debug);
            ImGui::SameLine();
            ImGui::Checkbox("Warn", &warn);
            ImGui::SameLine();
            ImGui::Checkbox("Error", &error);
            ImGui::SameLine();
            ImGui::Checkbox("Script", &script);
            ImGui::SameLine();
            ImGui::Checkbox("Engine", &engine);
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
            if (ImGui::Button("Clear All")) {
                m_logMessage.clear();
            }
            ImGui::PopStyleColor();

            ImGui::EndMenuBar();

            for (const auto& logMessage : m_logMessage) {
                bool shouldRender = false;
                const std::string& name = std::get<0>(logMessage);
                int messageId = std::get<1>(logMessage);

                if ((name == "Script" && script) || (name == "Engine" && engine)) {
                    if ((messageId == 2 && debug) || (messageId == 3 && warn) || (messageId == 4 && error)) {
                        shouldRender = true;
                    }
                }

                if (shouldRender) {
                    RenderLogMessage(logMessage);
                }
                //std::cout << name << std::endl;
            }

            if (ImGui::BeginPopupContextWindow("##ClearLog", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
                if (ImGui::MenuItem("Clear")) {
                    m_logMessage.clear();
                }
                ImGui::EndPopup();
            }

            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                ImGui::SetScrollHereY(1.0f);
            }

            ImGui::End();
        }

	void DebugConsole::LogMessage(std::string name, std::string message, int messageId)
	{
		m_logMessage.push_back(std::make_tuple(name,messageId,message));

	}
	void DebugConsole::ShutDown()
	{
	}
}
