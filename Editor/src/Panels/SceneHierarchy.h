#pragma once
/*!***************************************************************************
	\file			SceneHierarchy.h
	\author			Chew Shi Ru , Lam En Qing
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of function declaration to show the
					entity hierarchy.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <ECS/Architecture/Coordinator.hpp>
namespace Editor {
	class SceneHierarchyPanel {
	public:
		/*!***************************************************************************
		*   \brief			Constructor
		*******************************************************************************/
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(AeonCore::Entity entity);
		/*!***************************************************************************
		*   \brief			To show hierarchy on the editor
			\param			nil
			\return			void
		*******************************************************************************/
		void OnImGuiRender();
		/*!***************************************************************************
		*   \brief			To show pop up options on right clicked (eg. create child or delete entity)
			\param			EntityID entityClicked
			\return			void
		*******************************************************************************/
		void EntityOnRightClick(AeonCore::Entity* entityClicked);
		/*!***************************************************************************
		*   \brief			To identify the selected entity
			\param			EntityID entityClicked
			\return			void
		*******************************************************************************/
		void EntityOnLeftClick(AeonCore::Entity* entityClicked);
		/*!***************************************************************************
		*   \brief			Recursive function to show the entity hierarchy
			\param			EntityID node
			\return			void
		*******************************************************************************/
		void DrawEntityNode(AeonCore::EntityID node);
		/*!***************************************************************************
		*   \brief			Function allow the drag and drop to occur from imgui side
			\param			EntityID entityTarget
			\return			void
		*******************************************************************************/
		void DragDropEntity(AeonCore::EntityID entityTarget);
		/*!***************************************************************************
		*   \brief			Function allow to set child by dragging child entity to 
							parent
			\param			EntityID entityTarget
			\return			void
		*******************************************************************************/
		void DragDropTarget(AeonCore::EntityID entityTarget);
;		//Coordinator& gCoordinate;
		bool SearchQuery(const std::string& entityName, std::string& searchQuery);


		void DisplayAllEntity();
		void SearchEntity(std::string entityName);

		static AeonCore::EntityID GetSelectedEntityID()												{ return m_EntityId;			 }
		static void SetSelectedEntityID(AeonCore::EntityID _entityID)			{ m_EntityId = _entityID;		 }

		inline std::string  GetSearchEntity()							{ return m_SearchEntity;		 }
		void SetSearchEntity(std::string _entitySearch)					{ m_SearchEntity = _entitySearch;}
	private:
		AeonCore::Entity   m_SelectionContext;
		static AeonCore::EntityID m_EntityId;
		AeonCore::EntityID m_HeldContext;
		AeonCore::EntityID m_ParentIDOfChild{ AeonCore::INVALID_ENTITY_ID }; // solely used for creating a child
		std::string m_SearchEntity;
		bool m_ShouldCreateChild{ false };
		bool m_ShouldDeleteChild{ false };
		bool m_Search{ false };
	};
}