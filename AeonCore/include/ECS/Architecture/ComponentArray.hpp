#pragma once
/*!***************************************************************************
	\file			ComponentArray.hpp
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu
	\brief			This header file consists of functions declarations 
					of classes for managing component in an ECS system.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <array>
#include <cassert>
#include <unordered_map>
#include "ECS.hpp"
#include "Core/Log.h"

namespace AeonCore
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray()						  = default;
		virtual void SetComponentsSize(unsigned int size) = 0;
		virtual void EntityDestroyed(EntityID _entity)    = 0;
		virtual void DestroyAllEntities()				  = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		ComponentArray(unsigned int size);

		T& AddComponent(EntityID _entity, const T& _component);

		void RemoveComponent(EntityID _entity);

		T &GetData(EntityID _entity);

		//bool HasData(EntityID _entity);

		void SetComponentsSize(unsigned size) override;
		void EntityDestroyed(EntityID _entity) override;
		void DestroyAllEntities() override;
	private:
		std::vector<T>							mComponentArray{};
		std::unordered_map<EntityID, size_t>	mEntityToIndexMap{};
		std::unordered_map<size_t, EntityID>	mIndexToEntityMap{};
		unsigned int 							mUseCount{};
	};

	template <typename T>
	ComponentArray<T>::ComponentArray(unsigned size) {
		SetComponentsSize(size);
	}

	template<typename T>
	T& ComponentArray<T>::AddComponent(EntityID _entity, const T& _component)
	{
		AeonCore_ASSERT(mEntityToIndexMap.find(_entity) == mEntityToIndexMap.end(), "Component added to same _entity more than once.");

		// Put new entry at end
		size_t newIndex = mUseCount;
		mEntityToIndexMap[_entity] = newIndex;
		mIndexToEntityMap[newIndex] = _entity;
		mComponentArray.emplace_back(_component);
		//mComponentArray[newIndex] = _component;
		++mUseCount;

		return mComponentArray[newIndex];
	}

	template<typename T>
	void ComponentArray<T>::RemoveComponent(EntityID _entity)
	{
		AeonCore_ASSERT(mEntityToIndexMap.find(_entity) != mEntityToIndexMap.end(), "Removing non-existent _component.");
		size_t indexOfRemovedEntity = mEntityToIndexMap[_entity];
		size_t indexOfLastElement = mUseCount - 1;

		mComponentArray[indexOfRemovedEntity].Destroy();
		mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement]; // move last element into the element being removed to "overwrite" the removed element

		// Remap the last component's entity data ownership
		EntityID entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
		mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		mEntityToIndexMap.erase(_entity);
		mIndexToEntityMap.erase(indexOfLastElement);

		--mUseCount;
		mComponentArray.pop_back(); // removes the last element from the vector
	}

	template<typename T>
	T & ComponentArray<T>::GetData(EntityID _entity)
	{
		AeonCore_ASSERT(mEntityToIndexMap.find(_entity) != mEntityToIndexMap.end(), "Retrieving non-existent _component.");

		return mComponentArray[mEntityToIndexMap[_entity]];
	}

	/*template<typename T>
	bool ComponentArray<T>::HasData(EntityID _entity)
	{
		if ( mEntityToIndexMap.find(_entity) == mEntityToIndexMap.end() )
		{
			return false;
		}
		return true;
	}*/

	template <typename T>
	void ComponentArray<T>::SetComponentsSize(unsigned size) {
		mComponentArray.reserve(size);
	}

	template<typename T>
	void ComponentArray<T>::EntityDestroyed(EntityID _entity)
	{
		if ( mEntityToIndexMap.find(_entity) != mEntityToIndexMap.end() )
		{
			RemoveComponent(_entity);
		}
	}

	template <typename T>
	void ComponentArray<T>::DestroyAllEntities() {
		mEntityToIndexMap.clear();
		mIndexToEntityMap.clear();
		for (size_t i = 0; i < mUseCount; i++) {
			mComponentArray[i].Destroy();
		}
		mComponentArray.clear();
		mUseCount = 0;
	}
}

