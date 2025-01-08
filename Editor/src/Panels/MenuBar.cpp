/*!***************************************************************************
    \file			MenuBar.cpp
    \author			Chua Yip Xuan, Chew Shi Ru
    \par			yipxuan.chua\@digipen.edu, shiru.chew\@digipen.edu
    \date			Oct 1, 2023
    \brief			This source file consists of function defintion for menu bar on
                    the editor
    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
//Editor
#include "MenuBar.h"
#include "../EditorApp.h"
#include "../ImGuiUtility.h"
#include "PerformanceGraph.h"
//External Library
#include <ShObjIdl.h>
#include <locale>
#include <codecvt>
#include <imgui.h>
//Engine
#include <ECS/Architecture/ECS.hpp>
#include "ECS/Systems/InputSystem.h"
#include "ECS/Systems/PhysicsSystem.h"
#include <Core/Log.h>
#include <Core/Application.h>
#include <iostream>

#include "SceneHierarchy.h"

namespace Editor
{
    PerformanceGraph mPerform;
    void MenuBar::ShowMenuBar()
    {
        static bool showDiagnostic = false;
        static bool showShortcuts = false;
        static char label[256] = { "New Scene" };
        if (ImGui::BeginMenuBar())
        {
            AeonCore::Application& engineApp = AeonCore::Application::GetApplication();
            ImGui::BeginDisabled(engineApp.GetRuntimeState() == AeonCore::RuntimeState::Start);
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New"))
                {
                    NewFile();
                    /*auto system = gCoordinator.GetSystem<AeonCore::PhysicsSystem>();
                    auto physicsSystem = dynamic_pointer_cast<AeonCore::PhysicsSystem>(system);
                    physicsSystem->DeleteAllActors();
                    physicsSystem->CreateXPlane(0);*/
                }
                if (ImGui::MenuItem("Open"))
                {
                    OpenFile();
                }
                if (ImGui::MenuItem("Save"))
                {
                    SaveFile();
                }
                if (ImGui::MenuItem("Exit"))
                {
                    EditorApp::GetApplication().Close();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Others")) {
                if (ImGui::MenuItem("Performance")) {
                    showDiagnostic = true;
                }
                if (ImGui::MenuItem("Short-cuts")) {
                    showShortcuts = true;
                }
                ImGui::EndMenu();
            }
        }
        ImGui::EndDisabled();
        ImGui::EndMenuBar();
        if (showDiagnostic) {
            mPerform.Show();
        }
        if (showShortcuts) {
            AeonCore_WARN("It went in");
            ImGui::SetNextWindowSize(ImVec2(600, 300));
            ImGui::Begin("ShortCuts",&showShortcuts,ImGuiWindowFlags_AlwaysAutoResize);
            if (ImGui::BeginTable("##shortcut", 2, ImGuiTableFlags_Borders)) {

                const char* shortcuts[] = { "ShortCuts","W A S D","CTRL + S", "CTRL + O", "CTRL + N", "CTRL + E", "CTRL + 8", "CTRL + 9", "CTRL + 0", "SHIFT + N"};
                const char* functions[] = { "Function","Camera Movement","Save File", "Open File", "New File", "Exit", "Play", "Pause", "Stop","New Entity"};

                for (int i = 0; i < sizeof(shortcuts) / sizeof(shortcuts[0]); i++) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text(shortcuts[i]);
                    ImGui::TableNextColumn();
                    ImGui::Text(functions[i]);
                }

                ImGui::EndTable();
            }
            ImGui::End();
        }
          
        HotKeyOperation();
    }



    void MenuBar::SaveFile()
    {

        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (SUCCEEDED(hr))
        {
            IFileSaveDialog* pfd = NULL;
            hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
            if (SUCCEEDED(hr))
            {
                COMDLG_FILTERSPEC comDlgFS[1] = { {L"Scene", L"*.scene"} };
                pfd->SetFileTypes(1, comDlgFS);
                pfd->SetDefaultExtension(L"scene");
                hr = pfd->Show(NULL);
                if (SUCCEEDED(hr))
                {
                    IShellItem* pItem;
                    hr = pfd->GetResult(&pItem);
                    if (SUCCEEDED(hr))
                    {
                        LPWSTR filePath;
                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
                        if (SUCCEEDED(hr))
                        {
                            // Got the file path
                            std::wstring ws(filePath);
                            std::string narrowPath(ws.begin(), ws.end());
                            CoTaskMemFree(filePath);

                            // Perform the save operation here
                            AeonCore::SceneManager::SaveScene(narrowPath.c_str());
                        }
                        pItem->Release();
                    }
                }
                pfd->Release();
            }
            CoUninitialize();
        }
    }
    void MenuBar::OpenFile()
    {
        bool wasFileChosen = false;
        std::string narrowPath{};
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (SUCCEEDED(hr))
        {
            IFileOpenDialog* pfd = NULL;
            hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
            if (SUCCEEDED(hr))
            {
                wchar_t filePath[MAX_PATH];

                // Get the full path of the executable file
                DWORD result = GetModuleFileName(NULL, filePath, MAX_PATH);
                static_cast<void>(result);
                for (int i = 0; i < 4; ++i)
                {
                    wchar_t* lastSlash = wcsrchr(filePath, L'\\');
                    if (lastSlash)
                    {
                        *lastSlash = L'\0'; // Set the last slash to null terminator
                    }

                }

                // Print or use the updated file path (three directories back)
                wprintf(L"The path three directories back is: %s\n", filePath);
                //wchar_t initialFolderPath[MAX_PATH];
                wcscat_s(filePath, MAX_PATH, L"\\Editor\\resources\\scene");  // Assuming the "resources" folder is in the same directory as the executable

                IShellItem* defaultFolder;
                SHCreateItemFromParsingName(filePath, nullptr, IID_IShellItem, reinterpret_cast<void**>(&defaultFolder));
                pfd->SetFolder(defaultFolder);
                wprintf(L"Folder path: %s\n", filePath);
                // Setting filter
                COMDLG_FILTERSPEC comDlgFS[1] = { {L"Scene", L"*.scene"} };
                pfd->SetFileTypes(1, comDlgFS);

                hr = pfd->Show(nullptr);
                if (SUCCEEDED(hr))
                {
                    IShellItem* pItem;
                    hr = pfd->GetResult(&pItem);
                    if (SUCCEEDED(hr))
                    {
                        LPWSTR filepath;
                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &filepath);
                        if (SUCCEEDED(hr))
                        {
                            // Got the file path
                            char buf[260] = { 0 };
                            WideCharToMultiByte(CP_UTF8, 0, filepath, static_cast<int>(wcslen(filepath)), buf, 260, nullptr, nullptr);

                            int size_needed = WideCharToMultiByte(CP_UTF8, 0, filepath, -1, NULL, 0, NULL, NULL);
                            std::string strTo(size_needed, 0);
                            WideCharToMultiByte(CP_UTF8, 0, filepath, -1, &strTo[0], size_needed, NULL, NULL);
                            narrowPath = strTo;

                            wasFileChosen = true;
                            //std::cout << narrowPath << std::endl;
                            // Use the 'buf' for further processing
                            //narrowPath = WCharToString(filepath);
                            // Remember to free the allocated memory for filepath
                            CoTaskMemFree(filepath);
                        }
                        pItem->Release();
                    }
                }
                pfd->Release();
            }
        }
        CoUninitialize();

        if (wasFileChosen) {
	        AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();
        	gCoordinator.EmptyActiveScene();
        	AeonCore::SceneManager::LoadScene(narrowPath);
			SceneHierarchyPanel::SetSelectedEntityID(AeonCore::INVALID_ENTITY_ID);
        }
        /*AeonCore::Serialization::GetInstance().DeserializeScene(narrowPath);

        AeonCore::Coordinator &gCoordinator = AeonCore::Coordinator::GetInstance();
        auto physicsSystem = std::dynamic_pointer_cast<AeonCore::PhysicsSystem>(gCoordinator.GetSystem<AeonCore::PhysicsSystem>());
        physicsSystem->AfterSceneLoaded();*/
    }
    void MenuBar::NewFile()
    {
        AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();
        gCoordinator.EmptyActiveScene();
        //Create default entity
        auto defaultEntity = gCoordinator.CreateEntity("Default Entity");
        gCoordinator.AddComponent(defaultEntity.GetEntityID(), AeonCore::Transform());
        glm::vec3 newpos = glm::vec3(0.0f, 100.0f, 0.0f);
        gCoordinator.GetComponent<AeonCore::Transform>(defaultEntity).SetPosition(newpos);
        gCoordinator.AddComponent(defaultEntity.GetEntityID(), AeonCore::Lighting());
        glm::vec3 newdir = glm::vec3(1.5f, -1.0f, 0.0f);
        auto lightType = AeonCore::Lighting::LightType::DIRECTIONAL;
        auto light = gCoordinator.GetComponent<AeonCore::Lighting>(defaultEntity);
        light.SetLightDirection(newdir);
        light.SetLightType(lightType);

		//SceneHierarchyPanel::SetSelectedEntityID(AeonCore::INVALID_ENTITY_ID);
        SceneHierarchyPanel::SetSelectedEntityID(defaultEntity.GetEntityID());
    }
    void MenuBar::HotKeyOperation()
    {
        AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();
        
        //Ctrl + S(Save) , + N(New), + O(Open)
        if (AeonCore::Input::IsKeyHeld(AC_KEY_LEFT_CONTROL)|| AeonCore::Input::IsKeyHeld(AC_KEY_RIGHT_CONTROL)) {
            if (AeonCore::Input::IsKeyTriggered(AC_KEY_S)) {
                SaveFile();
            }
            if (AeonCore::Input::IsKeyTriggered(AC_KEY_N)) {
                NewFile();
            }
            if (AeonCore::Input::IsKeyTriggered(AC_KEY_O)) {
                OpenFile();
            }
            if (AeonCore::Input::IsKeyTriggered(AC_KEY_E)) {
                EditorApp::GetApplication().Close();
            }
            if (AeonCore::Input::IsKeyTriggered(AC_KEY_8)) {
                AeonCore::Application::GetApplication().OnRuntimeStart();
            }
            if (AeonCore::Input::IsKeyTriggered(AC_KEY_9)) {
                AeonCore::Application::GetApplication().OnRuntimePause();

            }
            if (AeonCore::Input::IsKeyTriggered(AC_KEY_0)) {
                AeonCore::Application::GetApplication().OnRuntimeStop();
            }
          
        }
        if (AeonCore::Input::IsKeyHeld(AC_KEY_LEFT_SHIFT) && AeonCore::Input::IsKeyTriggered(AC_KEY_N)) {
            AeonCore::EntityID e0 = gCoordinator.CreateEntity().GetEntityID(); //by giving the entity a name you can get the entity by name too
            gCoordinator.AddComponent(e0, AeonCore::Transform());
        }

    }

}

   

