#pragma once
/*!***************************************************************************
	\file			Serialization.h
	\author			Chua Yip Xuan and Oh Swee Leng Valerie
	\par			yipxuan.chua\@digipen.edu & o.sweelengvalerie\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of function declarations to
					Serialize and Deserialize the Scene(s) file. The
					implementation uses rttr and rapidjson as helper libraries
					to serialize/deserialize the scenes.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <document.h>
#include <stringbuffer.h>
#include <prettywriter.h>
#include <writer.h>
#include <rttr/type>

#include "Core/AeonAPI.h"
#include "Core/Singleton.hpp"
#include "ECS/Architecture/Entity.hpp"
#include "ECS/Components/IComponent.h"

namespace AeonCore
{
	class Serialization : public Singleton<Serialization>
	{
	public:
		bool LoadConfig();

		const IComponent* GetComponentByType(Entity &entity, const rttr::type &_type);

		bool HasComponentOfType(Entity &entity, const rttr::type &_type);

		std::shared_ptr<Scene> DeserializeScene(const std::string& _fn);

		void SerializeScene(std::string _fn);

		void SerializeEntityProperties(const Entity &_entity, rapidjson::Value &_entityData, rapidjson::Document::AllocatorType &_allocator);

		rapidjson::Value SerializeComponentProperties(const IComponent &_component, rapidjson::Document::AllocatorType &_allocator);

		rapidjson::Value SerializeRttrType(rapidjson::Document::AllocatorType& _allocator, rttr::variant _value);

		void DeserializeEntityProperties(const rapidjson::Value& entityData, rttr::instance& _obj);

		void DeserializeComponentProperties(rttr::instance& _propObj, const rapidjson::Value& _compProps);

		void DeserializeRttrProperty(rttr::instance& _rttrInstance, const rttr::property& _rttrProp, const rapidjson::Value& _jsonProp);

	private:
		friend class Singleton<Serialization>;

		std::unordered_map<EntityID, Entity*> mDeserializedEntityMapping{}; // deserialized entity ID mapped to the engine-created entity

		std::string mEntityKey		{};
		std::string mComponentKey	{};
	};

}