/*!***************************************************************************
    \file			SceneHierarchy.cpp
    \author			Chew Shi Ru , Lum En Qing
    \par			shiru.chew\@digipen.edu
    \date			Oct 1, 2023
    \brief			This source file consists of function definition to show the
                    entity hierarchy.
    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
//Editor
#include "SceneHierarchy.h"
#include "../ImGuiUtility.h"
//Engine
#include <imgui.h>
#include <iostream>
#include <ECS/Components/Transform.h>
#include <ECS/Architecture/ECS.hpp>
#include <ECS/Systems/InputSystem.h>
#include <Core/Log.h>
//External Lib
#include <algorithm> // for std::transform
#include <cctype>    // for std::tolower

namespace Editor {
    AeonCore::EntityID  SceneHierarchyPanel::m_EntityId = AeonCore::INVALID_ENTITY_ID;
    SceneHierarchyPanel::SceneHierarchyPanel(AeonCore::Entity entity)
        :m_HeldContext(entity.GetEntityID())
    {
        // Rest of your constructor code
        m_EntityId = entity.GetEntityID();
    }
    void SceneHierarchyPanel::OnImGuiRender()
    {
        AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();

        ImGui::Begin("Scene Hierarchy");
        if (ImGui::BeginPopupContextWindow("##EntityList", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
            //auto flag = AeonCore::Input::IsKeyHeld(AC_KEY_RIGHT_SHIFT) || AeonCore::Input::IsKeyHeld(AC_KEY_LEFT_SHIFT) && AeonCore::Input::IsKeyTriggered(AC_KEY_N);
            if (ImGui::MenuItem("New") ) {
                AeonCore::EntityID e0 = gCoordinator.CreateEntity().GetEntityID(); //by giving the entity a name you can get the entity by name too
                gCoordinator.AddComponent(e0, AeonCore::Transform());

            }

            ImGui::EndPopup();
        }

        std::string searchEntity;
        if (ImGuiUtility::DrawEnterInput("Search", "##search", &searchEntity)) {
            SetSearchEntity(searchEntity);
            m_Search=true;

        }
        //std::unordered_map<std::string, AeonCore::Entity*> entityMap;
        m_Search ? SearchEntity(GetSearchEntity()) : DisplayAllEntity();
        
        ImGui::Dummy(ImVec2(0, 50));


        ImGui::End();

        if (m_ShouldCreateChild) {
            AeonCore::EntityID child = gCoordinator.CreateChild(m_ParentIDOfChild, "Child 1").GetEntityID();
            gCoordinator.AddComponent(child, AeonCore::Transform());
            m_ShouldCreateChild = false;
        }

        if (m_ShouldDeleteChild) {
            gCoordinator.DestroyEntity(gCoordinator.GetEntity(m_ParentIDOfChild)->GetEntityID());
            m_ShouldDeleteChild = false;
        
        }

     

    }

    void SceneHierarchyPanel::EntityOnRightClick(AeonCore::Entity* entityClicked)
    {
        AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();

        //bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity")) {
               
                if (entityClicked->IsParent()) {
                    //gCoordinator.DestroyEntity(gCoordinator.GetChildObject(entityClicked)->GetEntityID());
                    m_ShouldDeleteChild = true;
                    m_ParentIDOfChild = entityClicked->GetEntityID();
					SetSelectedEntityID(AeonCore::INVALID_ENTITY_ID); // invalidate regardless of parent or child
                }
                else
                {
                    gCoordinator.DestroyEntity(entityClicked->GetEntityID());

                    if (entityClicked->GetEntityID() == GetSelectedEntityID())
						SetSelectedEntityID(AeonCore::INVALID_ENTITY_ID);
                }

            }
            if (ImGui::MenuItem("Create Child")) {

                //Entity ent = gCoordinator.CreateChild(gCoordinator.GetEntity(node)->GetEntityID(), "Child 1");
                m_ShouldCreateChild = true;
                m_ParentIDOfChild = entityClicked->GetEntityID();
            }
            if (ImGui::MenuItem("Duplicate Entity")) {
                gCoordinator.DuplicateEntity(*entityClicked, entityClicked->GetParent());
            }
            ImGui::EndPopup();
        }


    }

    void SceneHierarchyPanel::EntityOnLeftClick(AeonCore::Entity* entityClicked)
    {

        if (ImGui::IsItemDeactivated()) {
            m_EntityId = entityClicked->GetEntityID();
        }
        if (ImGui::IsItemActive())
        {
            m_HeldContext = entityClicked->GetEntityID();
        }

    }
    void SceneHierarchyPanel::DragDropEntity(AeonCore::EntityID entityTarget)
    {
       // AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();
        ImGuiDragDropFlags srcFlag = ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers;
        if (ImGui::BeginDragDropSource(srcFlag))
        {
            //if (gCoordinator.GetEntity(entityTarget)->IsChild()) {
            //    ImGui::SetDragDropPayload("MovingOutNode", &m_HeldContext, sizeof(AeonCore::EntityID));
            //}
            //else {
            //    ImGui::SetDragDropPayload("MovingNode", &m_HeldContext, sizeof(AeonCore::EntityID));
            //}
            ImGui::SetDragDropPayload("MovingNode", &m_HeldContext, sizeof(AeonCore::EntityID));
            ImGui::EndDragDropSource();
        }
    }
    void SceneHierarchyPanel::DragDropTarget(AeonCore::EntityID entityTarget)
    {
        if (ImGui::BeginDragDropTarget())
        {
            AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MovingNode"))
            {
                gCoordinator.ToChild(entityTarget, *reinterpret_cast<AeonCore::EntityID*>(payload->Data));
            }
            ImGui::EndDragDropTarget();
        }
    }

    bool SceneHierarchyPanel::SearchQuery(const std::string& entityName, std::string& searchQuery)
    {
        auto findQuery = entityName.find(searchQuery) != std::string::npos;
        return findQuery;
    }

    void SceneHierarchyPanel::DisplayAllEntity()
    {
        AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();
        auto& test = gCoordinator.GetEntities();
        for (auto i : test) {
            if (gCoordinator.EntityExists(i.GetEntityID()) && !i.IsChild()) {
                DrawEntityNode(i.GetEntityID());
            }
        }

    }

    void SceneHierarchyPanel::SearchEntity(std::string entityName)
    {
        AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();
        auto& test = gCoordinator.GetEntities();
        std::string searchEntityLower = GetSearchEntity(); 
        std::transform(searchEntityLower.begin(), searchEntityLower.end(), searchEntityLower.begin(), tolower);
        for (auto i : test) {
            if (gCoordinator.EntityExists(i.GetEntityID())) {
                std::string entityNameLower = i.GetEntityName();
                std::transform(entityNameLower.begin(), entityNameLower.end(), entityNameLower.begin(),tolower);
                DrawEntityNode(i.GetEntityID());
                if ((searchEntityLower == entityNameLower && m_Search)||(entityNameLower.find(searchEntityLower)!=std::string::npos && m_Search)) {
                    SetSelectedEntityID(i.GetEntityID());
                    if (ImGui::IsItemActive())
                    {
                        m_HeldContext = i.GetEntityID();
                    }
                    m_Search = false;
                }

            }


        }
    }

    void SceneHierarchyPanel::DrawEntityNode(AeonCore::EntityID node)
    {
        AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();

        // restructure 2.0
        AeonCore::Entity* entityNode = gCoordinator.GetEntity(node);
        std::vector<AeonCore::EntityID> childEntities = gCoordinator.GetChildObjects(node);


        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (node == m_EntityId)
            flags |= ImGuiTreeNodeFlags_Selected;

        // If no children, make it a leaf
        if (!entityNode->IsParent()) {
            flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            ImGui::TreeNodeEx((void*)entityNode, flags, entityNode->GetEntityName().c_str());
            EntityOnLeftClick(entityNode);
            EntityOnRightClick(entityNode);
            DragDropEntity(node);
            DragDropTarget(node);
        }
        else {
            bool is_open = ImGui::TreeNodeEx((void*)entityNode, flags, entityNode->GetEntityName().c_str());
            EntityOnLeftClick(entityNode);
            EntityOnRightClick(entityNode);
            DragDropEntity(node);
            DragDropTarget(node);

            if (is_open) {
                for (AeonCore::EntityID childEnt : childEntities) {
                    DrawEntityNode(childEnt);
                }

                ImGui::TreePop();
            }
        }
    
       
    }


}