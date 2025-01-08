#include "acpch.h"
/*!***************************************************************************
	\file			Serialization.cpp
	\author			Chua Yip Xuan and Oh Swee Leng Valerie
	\par			yipxuan.chua\@digipen.edu & o.sweelengvalerie\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function definitions to
					Serialize and Deserialize the Scene(s) file. The
					implementation uses rttr and rapidjson as helper libraries
					to serialize/deserialize the scenes.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <istreamwrapper.h>

#include "Core/Log.h"
#include "ECS/Components/Behaviour.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Render.h"
#include "ECS/Components/Font.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/Audio.h"
#include "ECS/Components/UI.h"
#include "ECS/Components/AudioListener.h"
#include "ECS/Components/RigidBody.h"
#include "ECS/Components/Collider.h"
#include "ECS/Components/Particle.h"
#include "Resource/Material.h"
#include "Rendering/Rendering.h"

#include "ECS/Architecture/Coordinator.hpp"
#include "Serialization/Serialization.hpp"

#include "Core/Application.h"
#include "ECS/Systems/PhysicsSystem.h"
#include "Serialization/RegisterManager.hpp"

#include "Scripting/Scripting.h"

namespace AeonCore
{
	std::filesystem::path FindBaseDirectory(const std::filesystem::path &_startDir)
	{
		std::filesystem::path currentDir = _startDir;

		while ( currentDir.has_parent_path() )
		{
			if ( std::filesystem::exists(currentDir / "GAM300_Team6.sln") )
			{
				return currentDir;
			}
			currentDir = currentDir.parent_path();
		}

		// If we didn't find the marker file, return an empty path
		return {};
	}

	bool Serialization::LoadConfig()
	{
		rapidjson::Document config;

		 // Get the directory of the currently executing program
		std::filesystem::path currentExecutableDir = std::filesystem::current_path();

		// Find the base directory
		/*std::filesystem::path baseDirectory = FindBaseDirectory(currentExecutableDir);
		if ( baseDirectory.empty() )
		{
			AeonCore_ERROR("Failed to find base directory.");
			return false;
		}*/

		// Append the relative path to the desired file
		std::filesystem::path configPath = currentExecutableDir / "resources" / "scene" / "config.scene";

		std::ifstream configFile(configPath);
		if ( !configFile.is_open() )
		{
			AeonCore_ERROR("Failed to open config file.");
			return false;
		}

		rapidjson::IStreamWrapper configStream(configFile);
		config.ParseStream(configStream);

		mEntityKey = config["EntityKey"].GetString();
		mComponentKey = config["ComponentKey"].GetString();

		if ( !mEntityKey.empty() || !mComponentKey.empty() )
		{
			return true;
		}
		AeonCore_ERROR("Scene config file has missing legends!");
		return false;
	}

	const IComponent* Serialization::GetComponentByType(Entity &_entity, const rttr::type &_type)
	{
		Coordinator &gCoordinator = Coordinator::GetInstance();

		if ( _type == rttr::type::get<Transform>() )
		{
			return &gCoordinator.GetComponent<Transform>(_entity);
		}
		// Add similar blocks for other component types...
		if (_type == rttr::type::get<Behaviour>())
		{
			return &gCoordinator.GetComponent<Behaviour>(_entity);
		}
		if (_type == rttr::type::get<Render>())
		{
			return &gCoordinator.GetComponent<Render>(_entity);
		}
		if (_type == rttr::type::get<Lighting>())
		{
			return &gCoordinator.GetComponent<Lighting>(_entity);
		}
		if (_type == rttr::type::get<Font>())
		{
			return &gCoordinator.GetComponent<Font>(_entity);
		}
		if (_type == rttr::type::get<Camera>())
		{
			return &gCoordinator.GetComponent<Camera>(_entity);
		}
		if (_type == rttr::type::get<AudioComponent>())
		{
			return &gCoordinator.GetComponent<AudioComponent>(_entity);
		}
		if (_type == rttr::type::get<UI>())
		{
			return &gCoordinator.GetComponent<UI>(_entity);
		}
		if (_type == rttr::type::get<AudioListenerComponent>())
		{
			return &gCoordinator.GetComponent<AudioListenerComponent>(_entity);
		}
		if (_type == rttr::type::get<RigidBody>())
		{
			return &gCoordinator.GetComponent<RigidBody>(_entity);
		}
		if (_type == rttr::type::get<Collider>())
		{
			return &gCoordinator.GetComponent<Collider>(_entity);
		}
		if (_type == rttr::type::get<Particles>())
		{
			return &gCoordinator.GetComponent<Particles>(_entity);
		}

		AeonCore_ERROR("Serialization: Component type not recognized.");
		return nullptr; // IComponent();
	}

	bool Serialization::HasComponentOfType(Entity &_entity, const rttr::type &_type)
	{
		Coordinator &gCoordinator = Coordinator::GetInstance();

		if ( _type == rttr::type::get<Transform>() )
		{
			return gCoordinator.HasComponent<Transform>(_entity);
		}
		// Add similar blocks for other component types...
		if (_type == rttr::type::get<Behaviour>())
		{
			return gCoordinator.HasComponent<Behaviour>(_entity);
		}
		if (_type == rttr::type::get<Render>())
		{
			return gCoordinator.HasComponent<Render>(_entity);
		}
		if (_type == rttr::type::get<Lighting>())
		{
			return gCoordinator.HasComponent<Lighting>(_entity);
		}
		if (_type == rttr::type::get<Font>())
		{
			return gCoordinator.HasComponent<Font>(_entity);
		}
		if (_type == rttr::type::get<Camera>())
		{
			return gCoordinator.HasComponent<Camera>(_entity);
		}
		if (_type == rttr::type::get<AudioComponent>())
		{
			return gCoordinator.HasComponent<AudioComponent>(_entity);
		}
		if (_type == rttr::type::get<UI>())
		{
			return gCoordinator.HasComponent<UI>(_entity);
		}
		if (_type == rttr::type::get<AudioListenerComponent>())
		{
			return gCoordinator.HasComponent<AudioListenerComponent>(_entity);
		}
		if (_type == rttr::type::get<RigidBody>())
		{
			return gCoordinator.HasComponent<RigidBody>(_entity);
		}
		if (_type == rttr::type::get<Collider>())
		{
			return gCoordinator.HasComponent<Collider>(_entity);
		}
		if (_type == rttr::type::get<Particles>())
		{
			return gCoordinator.HasComponent<Particles>(_entity);
		}
		return false; // Return false if the component type is not recognized.
	}

	std::shared_ptr<Scene> Serialization::DeserializeScene(const std::string& _fn)
	{
		mDeserializedEntityMapping.clear(); // data is no longer needed, so clear

		Coordinator &gCoordinator = Coordinator::GetInstance();

		//Scene::GetInstance().Destroy();

		std::ifstream file{ _fn };

		if ( !file.is_open() )
		{
			AeonCore_ERROR("Failed to open file: " + _fn);
			return nullptr;
		}

		rapidjson::IStreamWrapper isw(file);
		rapidjson::Document document;
		document.ParseStream(isw);

		if ( document.HasParseError() )
		{
			AeonCore_ERROR("JSON parsing failed with error code: " + std::to_string(document.GetParseError()));
			return nullptr;
		}

 		if (!document.IsObject()) {
			AeonCore_ASSERT(false, "File " + _fn + " did not start as a JSON object")
 			return nullptr;
 		}

		LoadConfig();

		std::shared_ptr<Scene> newScene = SceneManager::GetInstance().GetActiveScene();
		SceneType sceneType = SceneType::WillSpawn;
		unsigned int maxEntities = MAX_ENTITIES;

		if (document.HasMember("scene_info")) {
			const auto& sceneInfoObj = document["scene_info"];
			if (sceneInfoObj.HasMember("type")) {
				int sceneTypeInt = sceneInfoObj["type"].GetInt();

				if (sceneTypeInt < static_cast<int>(SceneType::MAX)) {
					sceneType = static_cast<SceneType>(sceneTypeInt);
					if (sceneType == SceneType::WillNotSpawn && !Application::GetApplication().IsEditorOn()) {
						if (document.HasMember(mEntityKey.c_str())) {
							auto& entitiesArr = document[mEntityKey.c_str()];
							if (entitiesArr.IsArray())
								maxEntities = entitiesArr.Size();
						}
					}
				}
			}
		}

		if (newScene == nullptr || !newScene->Empty())
			newScene = SceneManager::CreateNewScene(sceneType, maxEntities);
		else
			newScene->SetSceneType(sceneType);

		SceneManager::GetInstance().SetLoadThreadScene(newScene);

		if (document.HasMember(mEntityKey.c_str()))
		{
			const rapidjson::Value &entities = document[mEntityKey.c_str()];

			for ( rapidjson::SizeType i = 0; i < entities.Size(); i++ )
			{
				const rapidjson::Value &entityData = entities[i];

				// Temporary entity for the sake of deserializing and holding all the information
				Entity tempEnt = Entity();
				// Set entity properties using RTTR
				rttr::instance obj(tempEnt);
				// Deserialize the entity data to the temporary entity
				DeserializeEntityProperties(entityData, obj);
			
				// Request engine to create entity, to get "proper" id	
				Entity& newEntity = newScene->CreateEntity(tempEnt.GetEntityName()); //seems like here is the problem
				// Copy deserialized data to engine-created entity
				newEntity.Copy(tempEnt);
				newEntity.SetParentID(tempEnt.GetParent());
				// Deserialized entity ID (it's what the ids in the scene file knows) and a pointer to the engine entity. This is to properly create the parent-child relationship at the end of the deserialization process
				mDeserializedEntityMapping.insert({ tempEnt.GetEntityID(), &newEntity });

				 // Components
				if ( entityData.HasMember(mComponentKey.c_str()) )
				{
					const rapidjson::Value &components = entityData[mComponentKey.c_str()];

					for ( rapidjson::SizeType j = 0; j < components.Size(); j++ ) // goes through component array
					{
						const rapidjson::Value &componentData = components[j];
						// deserialize each component that entity has
						if (componentData.HasMember("Transform"))
						{
							const rapidjson::Value& compProps = componentData["Transform"];
							Transform transform;
						
							rttr::instance propObj(transform);
							//AeonCore_INFO("Component: {}", propObj.get_type().get_name().to_string());

							DeserializeComponentProperties(propObj, compProps);
							gCoordinator.AddComponent(newEntity, transform, newScene);
						}

						if (componentData.HasMember("Render")) // problem with getting ambient onwards (possibly due to Material component)														   
						{
							const rapidjson::Value& compProps = componentData["Render"];
							//Material material;
							Render render(newEntity.GetEntityName(), newEntity.GetEntityID());
							/*auto test = newEntity.GetUUID().GetUUID();
							auto test2 = gCoordinator.GetEntity(newEntity.GetEntityID())->GetUUID().GetUUID();*/

							//Render render(gCoordinator.GetEntity(newEntity.GetEntityID())->GetEntityName(), (int)(newEntity.GetUUID().GetUUID()));
							//rttr::instance materialObj(material);
							rttr::instance renderObj(render);
							//AeonCore_INFO("Component: {}", renderObj.get_type().get_name().to_string());

							//DeserializeComponentProperties(materialObj, compProps);
							DeserializeComponentProperties(renderObj, compProps);
							gCoordinator.AddComponent(newEntity, render, newScene);

							AeonCore_INFO("UUID: {}", newEntity.GetUUID().GetUUID());
						}

						if (componentData.HasMember("Lighting"))
						{
							const rapidjson::Value& compProps = componentData["Lighting"];
							Lighting lighting(newEntity.GetEntityID());

							rttr::instance propObj(lighting);
							//AeonCore_INFO("Component: {}", propObj.get_type().get_name().to_string());

							DeserializeComponentProperties(propObj, compProps);
							gCoordinator.AddComponent(newEntity, lighting, newScene);
						}
					
						if (componentData.HasMember("Camera"))
						{
							const rapidjson::Value& compProps = componentData["Camera"];
							Camera camera;

							rttr::instance propObj(camera);
							//AeonCore_INFO("Component: {}", propObj.get_type().get_name().to_string());

							DeserializeComponentProperties(propObj, compProps);
							gCoordinator.AddComponent(newEntity, camera, newScene);
						}

						if (componentData.HasMember("Audio")) 
						{
							const rapidjson::Value& compProps = componentData["Audio"];
							AudioComponent audio("");

							rttr::instance propObj(audio);
							//AeonCore_INFO("Component: {}", propObj.get_type().get_name().to_string());

							DeserializeComponentProperties(propObj, compProps);
							gCoordinator.AddComponent(newEntity, audio, newScene);
						}

						if (componentData.HasMember("Font")) // problem with rttr getter setter difference
						{
							const rapidjson::Value& compProps = componentData["Font"];
							Font font;

							rttr::instance propObj(font);
							//AeonCore_INFO("Component: {}", propObj.get_type().get_name().to_string());

							DeserializeComponentProperties(propObj, compProps);
							gCoordinator.AddComponent(newEntity, font, newScene);
						}

						if (componentData.HasMember("Behaviour"))
						{
							const rapidjson::Value& compProps = componentData["Behaviour"];
							Behaviour behaviour(newEntity.GetEntityID());

							rttr::instance propObj(behaviour);
							//AeonCore_INFO("Component: {}", propObj.get_type().get_name().to_string());

							DeserializeComponentProperties(propObj, compProps);
							gCoordinator.AddComponent(newEntity, behaviour, newScene);
						}
						if (componentData.HasMember("UI"))
						{
							const rapidjson::Value& compProps = componentData["UI"];
							UI ui(newEntity.GetEntityID());

							rttr::instance propObj(ui);
							//AeonCore_INFO("Component: {}", propObj.get_type().get_name().to_string());

							DeserializeComponentProperties(propObj, compProps);
							gCoordinator.AddComponent(newEntity, ui, newScene);
						}
						if (componentData.HasMember("AudioListener"))
						{
							const rapidjson::Value& compProps = componentData["AudioListener"];
							AudioListenerComponent listener;

							rttr::instance propObj(listener);
							//AeonCore_INFO("Component: {}", propObj.get_type().get_name().to_string());

							DeserializeComponentProperties(propObj, compProps);
							gCoordinator.AddComponent(newEntity, listener, newScene);
						}
						if (componentData.HasMember("Collider"))
						{
							const rapidjson::Value& compProps = componentData["Collider"];
							Collider collider;

							rttr::instance propObj(collider);
							//AeonCore_INFO("Component: {}", propObj.get_type().get_name().to_string());

							DeserializeComponentProperties(propObj, compProps);
							gCoordinator.AddComponent(newEntity, collider, newScene);
						}
						if (componentData.HasMember("RigidBody"))
						{
							const rapidjson::Value& compProps = componentData["RigidBody"];
							RigidBody rigidbody;

							rttr::instance propObj(rigidbody);
							//AeonCore_INFO("Component: {}", propObj.get_type().get_name().to_string());

							DeserializeComponentProperties(propObj, compProps);
							gCoordinator.AddComponent(newEntity, rigidbody, newScene);

						}
						if (componentData.HasMember("Particles"))
						{
							const rapidjson::Value& compProps = componentData["Particles"];
							Particles particle(newEntity.GetEntityID());

							rttr::instance propObj(particle);
							//AeonCore_INFO("Component: {}", propObj.get_type().get_name().to_string());

							DeserializeComponentProperties(propObj, compProps);
							gCoordinator.AddComponent(newEntity, particle, newScene);

						}
					}
				}
			}
		}

		if (document.HasMember("Settings"))
		{
			const rapidjson::Value& settings = document["Settings"];

			for (rapidjson::SizeType j = 0; j < settings.Size(); j++)
			{
				const rapidjson::Value& settingsData = settings[j];
				for (auto it = settingsData.MemberBegin(); it != settingsData.MemberEnd(); ++it)
				{
					const rapidjson::Value& value = it->value;
					auto& renderInstance = AeonCore::LightManager::GetInstance();
					rttr::instance renderObj(renderInstance);
					auto renderProp = renderObj.get_derived_type();

					for (const auto& prop : renderProp.get_properties())
					{
						DeserializeRttrProperty(renderObj, prop, value);
					}
				}
			}
			newScene->SetAmbientLight(LightManager::GetInstance().GetAmbientLight());
		}

		// Create proper parent-child relationship. Map the local parent id to the proper id
		for (auto&[_, entity] : mDeserializedEntityMapping) {
			if (entity->IsChild()) {
				// Find the parent using the local parent id
				auto parentItr = mDeserializedEntityMapping.find(entity->GetParent());
				if (parentItr == mDeserializedEntityMapping.end()) {
					AeonCore_ERROR("Failed to find the parent id {}", entity->GetParent());
					continue;
				}

				// Set the ID to be invalid, since we know the new parent ID, this prevents the "remove old parent" logic from running in ToChild() which will cause issue!
				entity->SetParentID(INVALID_ENTITY_ID);

				// Add the child entity to the parent entity
				newScene->ToChild(*(parentItr->second), *entity);
			}
		}
		mDeserializedEntityMapping.clear(); // data is no longer needed, so clear

		AeonCore_INFO("opened file ", _fn);
		
		return newScene;
	}

	void Serialization::SerializeScene(std::string _fn)
	{
		Coordinator &gCoordinator = Coordinator::GetInstance();
		LoadConfig();
		rapidjson::Document document;
		document.SetObject();

		rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

		// Serialize Scene Info
		{
			rapidjson::Value sceneInfoObj(rapidjson::kObjectType);
			rttr::instance sceneInstRttr(SceneManager::GetInstance().GetActiveScene().get());
			rttr::type sceneTypeRttr = sceneInstRttr.get_type();

			for (const auto& prop : sceneTypeRttr.get_properties()) {
				std::string propKey = prop.get_name().to_string();
				auto propValue = prop.get_value(sceneInstRttr);

				rapidjson::Value jsonValue;

				if (propValue.is_type<SceneType>()) {
					// SceneType has registered with RTTR for the enum values in Scene.cpp, so refer to link on how the value's name is extracted from the value: https://www.rttr.org/doc/master/register_enums_page.html
					//std::string sceneType = propValue.get_type().get_enumeration().value_to_name(propValue).to_string();

					jsonValue = rapidjson::Value(static_cast<int>(propValue.get_value<SceneType>()));
				}
				else
					jsonValue = SerializeRttrType(allocator, propValue);

				sceneInfoObj.AddMember(rapidjson::Value(propKey.c_str(), static_cast<rapidjson::SizeType>(propKey.size()), allocator).Move(), jsonValue, allocator);
			}

			document.AddMember("scene_info", sceneInfoObj, allocator);
		}

		// Serialize entities
		rapidjson::Value entities(rapidjson::kArrayType);


		for ( Entity &entity : gCoordinator.GetEntities() )
		{
			rapidjson::Value entityData(rapidjson::kObjectType);

			// Serialize entity properties using RTTR
			SerializeEntityProperties(entity, entityData, allocator);


			// Serialize components
			rapidjson::Value components(rapidjson::kArrayType);

			rttr::type baseType = rttr::type::get<IComponent>();
			rttr::array_range<rttr::type> derivedTypes = baseType.get_derived_classes();

			for ( const rttr::type &componentType : derivedTypes )
			{
				// Check if the entity has the component of the current type
				if ( HasComponentOfType(entity, componentType) )
				{
					// Retrieve the component from the entity
					const IComponent* component = GetComponentByType(entity, componentType);

					rapidjson::Value componentData(rapidjson::kObjectType);

					std::string typeName = componentType.get_name().to_string();

					// Serialize the component's properties
					rapidjson::Value serializedProperties = SerializeComponentProperties(*(component), allocator);

					componentData.AddMember(rapidjson::Value(typeName.c_str(), allocator).Move(), serializedProperties, allocator);

					components.PushBack(componentData, allocator);
				}
			}

			entityData.AddMember(rapidjson::Value(mComponentKey.c_str(),
												static_cast<rapidjson::SizeType>(mComponentKey.size()),
												allocator).Move(),
							   components,
							   allocator);

			entities.PushBack(entityData, allocator);
		}
		
		document.AddMember(rapidjson::Value(mEntityKey.c_str(), 
											static_cast<rapidjson::SizeType>(mEntityKey.size()),
											allocator).Move(), 
						   entities, 
						   allocator);

		auto& RenderingInstance = AeonCore::LightManager::GetInstance();
		rttr::instance renderObj(RenderingInstance);
		auto renderProp = renderObj.get_derived_type();

		rapidjson::Value settingsArray(rapidjson::kArrayType);
			
		for (const auto& prop : renderProp.get_properties())
		{
			std::string key = prop.get_name().to_string();
			rttr::variant value = prop.get_value(renderObj);

			rapidjson::Value instanceObj(rapidjson::kObjectType);
			rapidjson::Value serializedInstance = SerializeRttrType(allocator, value);

			instanceObj.AddMember(rapidjson::Value(key.c_str(), allocator).Move(),
				serializedInstance, allocator);
			settingsArray.PushBack(instanceObj, allocator);
		}
		document.AddMember("Settings", settingsArray, allocator);


		// Write to file
		std::ofstream file(_fn);
		if ( file.is_open() )
		{
			rapidjson::StringBuffer buffer;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
			//writer.SetIndent(' ', 4); // Set indentation to use 4 spaces

			document.Accept(writer);
			file << buffer.GetString();
			file.close();
			AeonCore_INFO("Scene serialized successfully.");
		}
		else
		{
			AeonCore_ERROR("Failed to open file for writing: " + _fn);
		}

	}

	void Serialization::SerializeEntityProperties(const Entity &_entity, rapidjson::Value &_entityData, rapidjson::Document::AllocatorType &_allocator)
	{
		rttr::instance obj(_entity);  
		rttr::type t = obj.get_type();  

		for ( const auto &prop : t.get_properties() )
		{
			rapidjson::Value key(prop.get_name().data(), _allocator);

			rttr::variant value = prop.get_value(obj);
			if ( value.is_type<std::string>() )
			{
				std::string strValue = value.get_value<std::string>();

				_entityData.AddMember(key, 
									 rapidjson::Value(
										 strValue.c_str(),
										 static_cast<rapidjson::SizeType>(strValue.size()),
										 _allocator).Move(),
									 _allocator);
			}
			else if ( value.is_type<bool>() )
			{
				_entityData.AddMember(key, value.get_value<bool>(), _allocator);
			}
			else if ( value.is_type<int>() )
			{
				_entityData.AddMember(key, value.get_value<int>(), _allocator);
			}
			else if ( value.is_type<UUID>() )
			{
				std::string uuidStr = value.get_value<UUID>().ToString();
				_entityData.AddMember(key, 
									 rapidjson::Value(
										 uuidStr.c_str(), 
										 static_cast<rapidjson::SizeType>(uuidStr.size()),
										 _allocator).Move(),
									 _allocator);
			}
			else if ( value.is_type<EntityID>() )
			{
				const EntityID entityId = value.get_value<AeonCore::EntityID>();
				_entityData.AddMember(key, entityId, _allocator);
			}
		}
	}


	rapidjson::Value Serialization::SerializeComponentProperties(const IComponent &_component, rapidjson::Document::AllocatorType &_allocator)
	{
		//rapidjson::Value componentData(rapidjson::kObjectType);
		rapidjson::Value propertiesArray(rapidjson::kArrayType);

		rttr::instance obj(_component);
		rttr::type objType = obj.get_derived_type();

		for ( const auto &prop : objType.get_properties() )
		{
			rapidjson::Value propertyObject(rapidjson::kObjectType);
			std::string key = prop.get_name().to_string();
			rttr::variant value = prop.get_value(obj);

			if ( key == "id" && value.is_type<int>() ) // Assuming the ID is of type int
			{
				//int idValue = value.get_value<int>();
				//componentData.AddMember(rapidjson::StringRef(key.c_str()), idValue, allocator);

				//propertyObject.AddMember(rapidjson::Value(key.c_str(),
				//										 static_cast<rapidjson::SizeType>(key.size()), 
				//										 _allocator).Move(), idValue, _allocator);
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			if (value.is_type<int>())
			{
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			else if ( value.is_type<glm::vec3>() )
			{
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);

			}
			else if ( value.is_type<glm::quat>() )
			{
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			else if ( value.is_type<glm::mat4>() )
			{
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			else if (value.is_type<GLuint>())
			{
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			else if (value.is_type<std::string>())
			{
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			else if (value.is_type<bool>())
			{ 
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}	
			else if (value.is_type<float>())
			{
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			else if (value.is_type<short>())
			{
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			else if (value.is_type<Lighting::LightType>())
			{
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			else if (value.is_type<AudioComponent::ChannelType>())
			{
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			else if (value.is_type<CameraRole>())
			{
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			else if (value.is_type<UI::Layer>())
			{
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			else if (value.is_type<UI::Format>())
			{
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			else if (value.is_type<RigidBody::RigidBodyType>())
			{
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			else if (value.is_type<Collider::ColliderType>())
			{
				propertyObject.AddMember(rapidjson::Value(key.c_str(),
					static_cast<rapidjson::SizeType>(key.size()),
					_allocator).Move(), SerializeRttrType(_allocator, value), _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			else if (objType == rttr::type::get<Behaviour>() && value.is_type<Behaviour::BehaviourNamesMap>()) {
				Behaviour::BehaviourNamesMap& bvrNameMapValue = value.get_value<Behaviour::BehaviourNamesMap>();
				rapidjson::Value jsonScripts(rapidjson::kObjectType);
				if (!bvrNameMapValue.empty()) {
					EntityID ownerID = objType.get_property_value("ownerID", obj).get_value<EntityID>();
					for (auto& bvrPair : bvrNameMapValue) {
						const std::string& bvrName(bvrPair.first);
						rapidjson::Value jsonBvrObject(rapidjson::kObjectType);

						ScriptInstanceRef scriptInst = Scripting::GetBehaviourInstance(ownerID, bvrPair.second);
						if (!scriptInst->GetScriptClass()->IsClassValid()) continue;

						// go through all overriden fields
						for (auto&[fieldName, fieldInst] : scriptInst->GetFieldMap()) {
							// check if the field exists inside the script class
							if (fieldInst.mField->mClassField) {
								bool setValue = true;
								rapidjson::Value jsonFieldObject(rapidjson::kObjectType);

								// Get field type and set it into the JSON
								std::string fieldType(ScriptUtils::ScriptDataTypeToString(fieldInst.mField->mType));
								jsonFieldObject.AddMember("type", rapidjson::Value(fieldType.c_str(), _allocator), _allocator);

								rapidjson::Value jsonFieldValue;
								switch (fieldInst.mField->mType) {
									case ScriptDataType::Bool:
										{
											jsonFieldValue.SetBool(fieldInst.GetValue<bool>());
										}
										break;
									case ScriptDataType::Float:
										{
											jsonFieldValue.SetFloat(fieldInst.GetValue<float>());
										}
										break;
									case ScriptDataType::Double:
										{
											jsonFieldValue.SetDouble(fieldInst.GetValue<double>());
										}
										break;
									case ScriptDataType::Int:
										{
											jsonFieldValue.SetInt(fieldInst.GetValue<int>());
										}
										break;
									case ScriptDataType::String:
										{
											std::string strFieldValue(fieldInst.GetValueDataBuffer());
											jsonFieldValue.SetString(fieldInst.GetValueDataBuffer(), static_cast<rapidjson::SizeType>(strFieldValue.size()));
										}
										break;
									case ScriptDataType::Vector3:
										{
											glm::vec3 vec3FieldValue(fieldInst.GetValue<glm::vec3>());
											//AeonCore_INFO("Serializing Vector3 field:{},{},{}", vec3FieldValue.x, vec3FieldValue.y, vec3FieldValue.z);

											jsonFieldValue.SetObject();
											jsonFieldValue.AddMember("x", vec3FieldValue.x, _allocator);
											jsonFieldValue.AddMember("y", vec3FieldValue.y, _allocator);
											jsonFieldValue.AddMember("z", vec3FieldValue.z, _allocator);
										}
										break;
									case ScriptDataType::None:
									default:
									{
										setValue = false;
									}
								}

								if (setValue) {
									jsonFieldObject.AddMember("value", jsonFieldValue, _allocator);

									// save the jsonFieldObject into jsonBvrObject
									jsonBvrObject.AddMember(rapidjson::Value(fieldName.c_str(), static_cast<rapidjson::SizeType>(fieldName.size()), _allocator).Move(),
										jsonFieldObject, _allocator);
								}
							}
						}

						jsonScripts.AddMember(rapidjson::Value(bvrName.c_str(), static_cast<rapidjson::SizeType>(bvrName.size()), _allocator).Move(), jsonBvrObject, _allocator);
					}
				}
				propertyObject.AddMember(rapidjson::Value(key.c_str(), static_cast<rapidjson::SizeType>(key.size()), _allocator).Move(), jsonScripts, _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			else if (value.is_type<ACPhysics::ACPhysicsMaterial>())
			{
				rapidjson::Value phyObj(rapidjson::kObjectType);
				ACPhysics::ACPhysicsMaterial phyMat = value.get_value<ACPhysics::ACPhysicsMaterial > ();

				rttr::instance rttrMat(phyMat);
				rttr::type rttrMatType = rttrMat.get_derived_type();

				for (const auto& matProp : rttrMatType.get_properties())
				{
					std::string matPropKey = matProp.get_name().to_string();

					phyObj.AddMember(rapidjson::Value(matPropKey.c_str(),
						static_cast<rapidjson::SizeType>(matPropKey.size()),
						_allocator).Move(), SerializeRttrType(_allocator, matProp.get_value(rttrMat)), _allocator);
					//AeonCore_INFO("{} : {}", matProp.get_name().to_string(), matProp.get_type().get_name());
				}
				propertyObject.AddMember(rapidjson::Value(key.c_str(), static_cast<rapidjson::SizeType>(key.size()), _allocator).Move(), phyObj, _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			else if (value.is_type<ACPhysics::Geometry*>())
			{
				rapidjson::Value geomObject(rapidjson::kObjectType);

				std::string compName = objType.get_name().data();
				auto colliderTypeProp = objType.get_property("collider type");
				Collider::ColliderType colliderType = colliderTypeProp.get_value(obj).get_value<Collider::ColliderType>();

				ACPhysics::Geometry* baseGeom = value.get_value<ACPhysics::Geometry*>();
				if (colliderType == Collider::ColliderType::CUBE)
				{
					ACPhysics::CubeGeometry* geom = dynamic_cast<ACPhysics::CubeGeometry*>(baseGeom);

					rttr::instance geomObj(geom);
					rttr::type geomType = geomObj.get_derived_type();

					for (const auto& geomProp : geomType.get_properties())
					{
						std::string geomKey = geomProp.get_name().to_string();

						geomObject.AddMember(rapidjson::Value(geomKey.c_str(),
							static_cast<rapidjson::SizeType>(geomKey.size()),
							_allocator).Move(), SerializeRttrType(_allocator, geomProp.get_value(geomObj)), _allocator);
						//AeonCore_INFO("{} : {}", geomProp.get_name().to_string(), geomProp.get_type().get_name());
					}
				}
				if (colliderType == Collider::ColliderType::SPHERE)
				{
					ACPhysics::SphereGeometry* geom = dynamic_cast<ACPhysics::SphereGeometry*>(baseGeom);

					rttr::instance geomObj(geom);
					rttr::type geomType = geomObj.get_derived_type();

					for (const auto& geomProp : geomType.get_properties())
					{
						std::string geomKey = geomProp.get_name().to_string();

						geomObject.AddMember(rapidjson::Value(geomKey.c_str(),
							static_cast<rapidjson::SizeType>(geomKey.size()),
							_allocator).Move(), SerializeRttrType(_allocator, geomProp.get_value(geomObj)), _allocator);
						//AeonCore_INFO("{} : {}", geomProp.get_name().to_string(), geomProp.get_type().get_name());
					}
				}
				if (colliderType == Collider::ColliderType::CAPSULE)
				{
					ACPhysics::CapsuleGeometry* geom = dynamic_cast<ACPhysics::CapsuleGeometry*>(baseGeom);

					rttr::instance geomObj(geom);
					rttr::type geomType = geomObj.get_derived_type();

					for (const auto& geomProp : geomType.get_properties())
					{
						std::string geomKey = geomProp.get_name().to_string();

						geomObject.AddMember(rapidjson::Value(geomKey.c_str(),
							static_cast<rapidjson::SizeType>(geomKey.size()),
							_allocator).Move(), SerializeRttrType(_allocator, geomProp.get_value(geomObj)), _allocator);
						//AeonCore_INFO("{} : {}", geomProp.get_name().to_string(), geomProp.get_type().get_name());
					}
				}
				propertyObject.AddMember(rapidjson::Value(key.c_str(), static_cast<rapidjson::SizeType>(key.size()), _allocator).Move(), geomObject, _allocator);
				propertiesArray.PushBack(propertyObject, _allocator);
			}
			// Handle other types similarly 
		}

		return propertiesArray;
	}

	rapidjson::Value Serialization::SerializeRttrType(rapidjson::Document::AllocatorType& _allocator, rttr::variant _value)
	{
		rapidjson::Value jsonValue;

		if (_value.is_type<int>())
		{
			jsonValue.SetInt(_value.get_value<int>());
		}
		else if (_value.is_type<UI::Order>())
		{
			jsonValue.SetUint(_value.get_value<unsigned int>());
		}
		else if (_value.is_type<glm::vec3>())
		{
			glm::vec3 vecValue = _value.get_value<glm::vec3>();
			rapidjson::Value vecObject(rapidjson::kObjectType);
			vecObject.AddMember("x", vecValue.x, _allocator);
			vecObject.AddMember("y", vecValue.y, _allocator);
			vecObject.AddMember("z", vecValue.z, _allocator);

			return vecObject;

		}
		else if (_value.is_type<glm::quat>())
		{
			glm::quat quatValue = _value.get_value<glm::quat>();
			rapidjson::Value quatObject(rapidjson::kObjectType);
			quatObject.AddMember("x", quatValue.x, _allocator);
			quatObject.AddMember("y", quatValue.y, _allocator);
			quatObject.AddMember("z", quatValue.z, _allocator);
			quatObject.AddMember("w", quatValue.w, _allocator);
			
			return quatObject;
		}
		else if (_value.is_type<glm::mat4>())
		{
			glm::mat4 matValue = _value.get_value<glm::mat4>();
			rapidjson::Value matObject(rapidjson::kArrayType);
			for (int i = 0; i < 4; ++i)
			{
				glm::vec4 col = matValue[i];
				rapidjson::Value colObject(rapidjson::kObjectType);
				colObject.AddMember("x", col.x, _allocator);
				colObject.AddMember("y", col.y, _allocator);
				colObject.AddMember("z", col.z, _allocator);
				colObject.AddMember("w", col.w, _allocator);
				matObject.PushBack(colObject, _allocator);
			}
			
			return matObject;
		}
		else if (_value.is_type<GLuint>())
		{
			jsonValue.SetUint(_value.get_value<GLuint>());
		}
		else if (_value.is_type<std::string>())
		{
			std::string str = _value.get_value<std::string>();
			return rapidjson::Value(str.c_str(), _allocator);
		}

		else if (_value.is_type<bool>())
		{
			jsonValue.SetBool(_value.get_value<bool>());
		}
		else if (_value.is_type<float>())
		{
			jsonValue.SetFloat(_value.get_value<float>());
		}
		else if (_value.is_type<short>())
		{
			short val = _value.get_value<short>();
			jsonValue.SetInt(static_cast<int>(val));
		}
		else if (_value.is_type<Lighting::LightType>())
		{
			std::string lighttype;
			if (_value.get_value<Lighting::LightType>() == Lighting::LightType::SPOTLIGHT)
			{
				lighttype = "SPOTLIGHT";
			}
			else if (_value.get_value<Lighting::LightType>() == Lighting::LightType::POINT)
			{
				lighttype = "POINT";
			}
			else if (_value.get_value<Lighting::LightType>() == Lighting::LightType::DIRECTIONAL)
			{
				lighttype = "DIRECTIONAL";
			}
			return rapidjson::Value(lighttype.c_str(), _allocator);
		}
		else if (_value.is_type<AudioComponent::ChannelType>())
		{
			std::string channeltype;
			switch (_value.get_value<AudioComponent::ChannelType>())
			{
			case AudioComponent::ChannelType::Voice:
				channeltype = "Voice";
				break;
			case AudioComponent::ChannelType::BGM:
				channeltype = "BGM";
				break;
			default:
				channeltype = "SFX";
				break;
			}
			return rapidjson::Value(channeltype.c_str(), _allocator);
		}
		else if (_value.is_type<UI::Layer>())
		{
			std::string layertype;
			switch (_value.get_value<UI::Layer>())
			{
			case UI::Layer::Background:
				layertype = "Background";
				break;
			case UI::Layer::Foreground:
				layertype = "Foreground";
				break;
			default:
				layertype = "Default";
				break;
			}
			return rapidjson::Value(layertype.c_str(), _allocator);
			}
		else if (_value.is_type<UI::Format>())
		{
			std::string formattype;
			switch (_value.get_value<UI::Format>())
			{
			case UI::Format::Word:
				formattype = "Word";
				break;
			default:
				formattype = "Image";
				break;
			}
			return rapidjson::Value(formattype.c_str(), _allocator);
			}
		else if (_value.is_type<RigidBody::RigidBodyType>())
		{
			//AeonCore_INFO("Serialize RigidBodyType");
			std::string rigidbodytype;
			switch (_value.get_value<RigidBody::RigidBodyType>())
			{
			case RigidBody::RigidBodyType::DYNAMIC:
				rigidbodytype = "DYNAMIC";
				break;
			default:
				rigidbodytype = "STATIC";
				break;
			}
			return rapidjson::Value(rigidbodytype.c_str(), _allocator);
		}
		else if (_value.is_type<Collider::ColliderType>())
		{
			//AeonCore_INFO("Serialize ColliderType");
			std::string colliderType;
			switch (_value.get_value<Collider::ColliderType>())
			{
			case Collider::ColliderType::CUBE:
				colliderType = "CUBE";
				break;
			case Collider::ColliderType::SPHERE:
				colliderType = "SPHERE";
				break;
			case Collider::ColliderType::CAPSULE:
				colliderType = "CAPSULE";
				break;
			default:
				colliderType = "NONE";
			}
			return rapidjson::Value(colliderType.c_str(), _allocator);
			}
		else if (_value.is_type<CameraRole>())
		{
			//AeonCore_INFO("Serialize CameraRole");
			std::string cameraRole;
			switch (_value.get_value<CameraRole>())
			{
			case CameraRole::CAMERA:
				cameraRole = "CAMERA";
				break;
			
			default:
				cameraRole = "NONE";
			}
			return rapidjson::Value(cameraRole.c_str(), _allocator);
		}

		return jsonValue;
	}

	void Serialization::DeserializeEntityProperties(const rapidjson::Value& _entityData, rttr::instance& _obj)
	{
		rttr::type objType = _obj.get_type();

		for (const auto& prop : _obj.get_derived_type().get_properties())
		{
			std::string key = prop.get_name().to_string();
			if (_entityData.HasMember(key.c_str()))
			{
				rttr::variant value = prop.get_value(_obj);
				if (value.is_type<std::string>())
				{
					prop.set_value(_obj, std::string(_entityData[key.c_str()].GetString())); // or GetInt(), GetBool(), etc.
				}
				else if (value.is_type<int>())
				{
					prop.set_value(_obj, _entityData[key.c_str()].GetInt());
				}
				else if (value.is_type<bool>())
				{
					prop.set_value(_obj, _entityData[key.c_str()].GetBool());
				
				}
				else if (value.is_type<UUID>())
				{
					//prop.set_value(obj, entityData[key.c_str()].GetUint64());
					UUID uuid;
					uuid.FromString(_entityData[key.c_str()].GetString());
					prop.set_value(_obj, uuid);
					//AeonCore_INFO("Key: {}, Type: {}, Value: {}", key, prop.get_type().get_name().to_string(), uuid.ToString());
				}
				else if (value.is_type<EntityID>())												
				{
					prop.set_value(_obj, _entityData[key.c_str()].GetUint());
					/*const EntityID entityID = value.get_value<AeonCore::EntityID>();
					prop.set_value(obj, entityID);*/
				}
				if (_entityData["mIsChild"].GetBool()) {

					//int parent = _entityData["mParent"].GetInt();
					string name = _entityData["mName"].GetString();
				}
	
				//LOOK HERE FOR PARENT_CHILD/////
			}
			//if (_entityData.HasMember("mIsChild")) {
			//	
			//}
		}
	}

	void Serialization::DeserializeComponentProperties(rttr::instance& _propObj, const rapidjson::Value& _compProps)
	{
		rapidjson::SizeType i = 0;
		rttr::type propObjType = _propObj.get_derived_type();
		for (const auto& prop : propObjType.get_properties()) // properties of component (position, scale, etc)
		{
			std::string key = prop.get_name().to_string();
			if (key == "id")
			{
				continue;
			}
			//AeonCore_INFO("deserialize comp props, key: {}", key);
			// this will go through the position / rotation / scale in rapidjson

			if (i >= _compProps.Size() && prop.get_type().is_valid())
			{
				if (prop.get_type().is_pointer())
				{
					prop.set_value(_propObj, nullptr);
				}
				else
				{
					prop.set_value(_propObj, rttr::variant{});;
				}
				//AeonCore_INFO("Set Default Value: {}", key);
			}
			else
			{
				const rapidjson::Value& propVal = _compProps[i++];

				if (propVal.HasMember(key.c_str()))
				{
					const rapidjson::Value& value = propVal[key.c_str()];
					if (prop.get_type() == rttr::type::get<glm::vec3>())
					{
						DeserializeRttrProperty(_propObj, prop, value);
					}

					else if (prop.get_type() == rttr::type::get<glm::quat>())
					{
						DeserializeRttrProperty(_propObj, prop, value);
					}

					else if (prop.get_type() == rttr::type::get<glm::mat4>())
					{
						DeserializeRttrProperty(_propObj, prop, value);
					}

					else if (prop.get_type() == rttr::type::get<std::string>())
					{
						DeserializeRttrProperty(_propObj, prop, value);
					}
					else if (prop.get_type() == rttr::type::get<float>())
					{
						DeserializeRttrProperty(_propObj, prop, value);
					}
					else if (prop.get_type() == rttr::type::get<bool>())
					{
						DeserializeRttrProperty(_propObj, prop, value);
					}
					else if (prop.get_type() == rttr::type::get<int>())
					{
						DeserializeRttrProperty(_propObj, prop, value);
					}
					else if (prop.get_type() == rttr::type::get<short>())
					{
						DeserializeRttrProperty(_propObj, prop, value);
					}
					else if (prop.get_type() == rttr::type::get<Lighting::LightType>())
					{
						std::string type = value.GetString();
						std::unordered_map<std::string, Lighting::LightType> typeMap = {
							{"SPOTLIGHT", Lighting::LightType::SPOTLIGHT},
							{"POINT", Lighting::LightType::POINT},
							{"DIRECTIONAL", Lighting::LightType::DIRECTIONAL}
						};
						auto it = typeMap.find(type);
						if (it != typeMap.end())
						{
							Lighting::LightType enumValue = it->second;
							prop.set_value(_propObj, enumValue);
						}
					}

					else if (prop.get_type() == rttr::type::get<AudioComponent::ChannelType>())
					{
						std::string type = value.GetString();

						// Create a mapping from string to enum values
						std::unordered_map<std::string, AudioComponent::ChannelType> typeMap = {
							{ "SFX", AudioComponent::ChannelType::SFX },
							{ "BGM", AudioComponent::ChannelType::BGM },
							{ "Voice", AudioComponent::ChannelType::Voice }
						};

						auto it = typeMap.find(type);
						if (it != typeMap.end())
						{
							// Match found, set the enum value
							AudioComponent::ChannelType enumValue = it->second;
							prop.set_value(_propObj, enumValue);
						}
					}

					else if (prop.get_type() == rttr::type::get<CameraRole>())
					{
						std::string type = value.GetString();

						// Create a mapping from string to enum values
						std::unordered_map<std::string, CameraRole> typeMap = {
							{ "NONE", CameraRole::NONE },
							{ "CAMERA", CameraRole::CAMERA}
						};

						auto it = typeMap.find(type);
						if (it != typeMap.end())
						{
							// Match found, set the enum value
							CameraRole enumValue = it->second;
							prop.set_value(_propObj, enumValue);
							//AeonCore_WARN("Channel: {}", static_cast<int>(enumValue));
						}
					}


					else if (prop.get_type() == rttr::type::get<UI::Layer>())
					{
						std::string type = value.GetString();

						// Create a mapping from string to enum values
						std::unordered_map<std::string, UI::Layer> typeMap = {
							{ "Default", UI::Layer::Default },
							{ "Foreground", UI::Layer::Foreground },
							{ "Background", UI::Layer::Background }
						};
						auto it = typeMap.find(type);
						if (it != typeMap.end())
						{
							// Match found, set the enum value
							UI::Layer enumValue = it->second;
							prop.set_value(_propObj, enumValue);
							//AeonCore_WARN("Layer: {}", static_cast<int>(enumValue));
						}
					}

					else if (prop.get_type() == rttr::type::get<UI::Format>())
					{
						std::string type = value.GetString();
						std::unordered_map<std::string, UI::Format> formatMap = {
							{ "Image", UI::Format::Image },
							{ "Word", UI::Format::Word },
						};
						auto it2 = formatMap.find(type);
						if (it2 != formatMap.end())
						{
							UI::Format formatvalue = it2->second;
							prop.set_value(_propObj, formatvalue);
						}
					}
			

					else if (prop.get_type() == rttr::type::get<RigidBody::RigidBodyType>())
					{
						std::string type = value.GetString();
						std::unordered_map<std::string, RigidBody::RigidBodyType> typeMap = {
							{"STATIC", RigidBody::RigidBodyType::STATIC},
							{"DYNAMIC", RigidBody::RigidBodyType::DYNAMIC}
						};
						auto it = typeMap.find(type);
						if (it != typeMap.end())
						{
							RigidBody::RigidBodyType enumValue = it->second;
							prop.set_value(_propObj, enumValue);
							AeonCore_WARN("RigidBody: {}", static_cast<int>(enumValue));
						}
					}

					else if (prop.get_type() == rttr::type::get<Collider::ColliderType>())
					{
						std::string type = value.GetString();

						// Create a mapping from string to enum values
						std::unordered_map<std::string, Collider::ColliderType> typeMap = {
							{ "NONE", Collider::ColliderType::NONE },
							{ "CUBE", Collider::ColliderType::CUBE },
							{ "SPHERE", Collider::ColliderType::SPHERE },
							{ "CAPSULE", Collider::ColliderType::CAPSULE }
						};

						auto it = typeMap.find(type);
						if (it != typeMap.end())
						{
							// Match found, set the enum value
							Collider::ColliderType enumValue = it->second;
							prop.set_value(_propObj, enumValue);
						}
					}

					else if (prop.get_type() == rttr::type::get<GLuint>())
					{
						DeserializeRttrProperty(_propObj, prop, value);
					}

					else if (prop.get_type() == rttr::type::get<ACPhysics::ACPhysicsMaterial>())
					{
						ACPhysics::ACPhysicsMaterial mat;
						rttr::instance rttrMat(mat);
						for (const auto& matProp : rttrMat.get_type().get_properties())
						{
							DeserializeRttrProperty(rttrMat, matProp, value[matProp.get_name().to_string().c_str()]);
						}
						prop.set_value(_propObj, mat);
					}

					else if (prop.get_type() == rttr::type::get<ACPhysics::Geometry*>())
					{
						auto colliderTypeProp = propObjType.get_property("collider type").get_value(_propObj);
						auto colliderType = colliderTypeProp.get_value<Collider::ColliderType>();

						if (colliderType == Collider::ColliderType::CUBE)
						{
							ACPhysics::CubeGeometry* geom = new ACPhysics::CubeGeometry();
							rttr::instance rttrGeom(geom);
							for (const auto& geomProp : rttrGeom.get_type().get_properties())
							{
								DeserializeRttrProperty(rttrGeom, geomProp, value[geomProp.get_name().to_string().c_str()]);
							}
							ACPhysics::Geometry* baseGeom = prop.get_value(_propObj).get_value<ACPhysics::Geometry*>();
							if (baseGeom)
							{
								delete baseGeom;
							}
							baseGeom = geom;
							prop.set_value(_propObj, baseGeom);
						}
						else if (colliderType == Collider::ColliderType::SPHERE)
						{
							ACPhysics::SphereGeometry* geom = new ACPhysics::SphereGeometry();
							rttr::instance rttrGeom(geom);
							for (const auto& geomProp : rttrGeom.get_type().get_properties())
							{
								DeserializeRttrProperty(rttrGeom, geomProp, value[geomProp.get_name().to_string().c_str()]);
							}
							ACPhysics::Geometry* baseGeom = prop.get_value(_propObj).get_value<ACPhysics::Geometry*>();
							if (baseGeom)
							{
								delete baseGeom;
							}
							baseGeom = geom;
							prop.set_value(_propObj, baseGeom);
						}
						else if (colliderType == Collider::ColliderType::CAPSULE)
						{
							ACPhysics::CapsuleGeometry* geom = new ACPhysics::CapsuleGeometry();
							rttr::instance rttrGeom(geom);
							for (const auto& geomProp : rttrGeom.get_type().get_properties())
							{
								DeserializeRttrProperty(rttrGeom, geomProp, value[geomProp.get_name().to_string().c_str()]);
							}
							ACPhysics::Geometry* baseGeom = prop.get_value(_propObj).get_value<ACPhysics::Geometry*>();
							if (baseGeom)
							{
								delete baseGeom;
							}
							baseGeom = geom;
							prop.set_value(_propObj, baseGeom);
						}
					}

					else if (propObjType == rttr::type::get<Behaviour>() && prop.get_type() == rttr::type::get<Behaviour::BehaviourNamesMap>()) {
						// deserializing "scripts", a JSON object. Inside the object has keys (script names) and values (JSON object)

						EntityID ownerID = propObjType.get_property_value("ownerID", _propObj).get_value<EntityID>();
						bool isRuntimeStopped = Application::GetApplication().GetRuntimeState() == RuntimeState::Stop; // used when setting field value
						//AeonCore_INFO("Deserializing behaviour scripts, their owner is {}", ownerID);

						// deserialize every script inside object "scripts"
						for (rapidjson::Value::ConstMemberIterator jsonScriptItr = value.MemberBegin(); jsonScriptItr != value.MemberEnd(); ++jsonScriptItr) {
							std::string scriptName = jsonScriptItr->name.GetString();

							// Does not exist, no need to deserialize
							if (!Scripting::DoesBehaviourClassExists(scriptName)) continue;

							// Add the script to the behaviour component
							auto bvrId = propObjType.invoke("AddBehaviourScript", _propObj, {scriptName});
							//AeonCore_INFO("\tReading script name:{}, {}", scriptName, bvrId.get_value<int>());
							ScriptInstanceRef scriptInst = Scripting::GetBehaviourInstance(ownerID, bvrId.get_value<int>());

							// Deserialize the script's fields
							const rapidjson::Value& jsonScriptObj = jsonScriptItr->value;
							for (rapidjson::Value::ConstMemberIterator jsonFieldItr = jsonScriptObj.MemberBegin(); jsonFieldItr != jsonScriptObj.MemberEnd(); ++jsonFieldItr) {
								std::string fieldName = jsonFieldItr->name.GetString();

								// if the script class does not have this field name, we don't need to deserialize it
								if (!scriptInst->GetScriptClass()->GetFields().contains(fieldName)) continue;
								const rapidjson::Value& jsonFieldObj = jsonFieldItr->value;
								const rapidjson::Value& jsonFieldType = jsonFieldObj["type"];
								const rapidjson::Value& jsonFieldValue = jsonFieldObj["value"];

								//AeonCore_INFO("\t\tReading field name, type:{}, {}", fieldName, jsonFieldType.GetString());

								ScriptDataType fieldType = ScriptUtils::ScriptDataTypeFromString(jsonFieldType.GetString());

								switch (fieldType) {
									case ScriptDataType::Bool:
										{
											scriptInst->SetFieldValue(fieldName, jsonFieldValue.GetBool(), isRuntimeStopped);
											//AeonCore_INFO("\t\t\tReading field value:{}", jsonFieldValue.GetBool());
											break;
										}
									case ScriptDataType::Float:
										{
											scriptInst->SetFieldValue(fieldName, jsonFieldValue.GetFloat(), isRuntimeStopped);
											//AeonCore_INFO("\t\t\tReading field value:{}", jsonFieldValue.GetFloat());
											break;
										}
									case ScriptDataType::Double:
										{
											scriptInst->SetFieldValue(fieldName, jsonFieldValue.GetDouble(), isRuntimeStopped);
											//AeonCore_INFO("\t\t\tReading field value:{}", jsonFieldValue.GetDouble());
											break;
										}
									case ScriptDataType::Int:
										{
											scriptInst->SetFieldValue(fieldName, jsonFieldValue.GetInt(), isRuntimeStopped);
											//AeonCore_INFO("\t\t\tReading field value:{}", jsonFieldValue.GetInt());
											break;
										}
									case ScriptDataType::String:
										{
											scriptInst->SetFieldValueStr(fieldName, jsonFieldValue.GetString(), isRuntimeStopped);
											//AeonCore_INFO("\t\t\tReading field value:{}", jsonFieldValue.GetString());
											break;
										}
									case ScriptDataType::Vector3:
										{
											glm::vec3 vec = glm::vec3(jsonFieldValue["x"].GetFloat(), jsonFieldValue["y"].GetFloat(), jsonFieldValue["z"].GetFloat());
											scriptInst->SetFieldValue(fieldName, vec, isRuntimeStopped);
											//AeonCore_INFO("\t\t\tReading field value:{}, {}, {}", vec.x, vec.y, vec.z);
											break;
										}
								}
							}
						}
					}
				}
			}
		}
	}

	void Serialization::DeserializeRttrProperty(rttr::instance& _propObj, const rttr::property& _prop, const rapidjson::Value& _value)
	{
		if (_prop.get_type() == rttr::type::get<glm::vec3>())
		{
			// get default values then update
			glm::vec3 vecVal = {};
			vecVal = { _value["x"].GetFloat(),
					   _value["y"].GetFloat(),
					   _value["z"].GetFloat() };
			_prop.set_value(_propObj, vecVal); // set property value to component
			//AeonCore_INFO("Vec: {} {} {}", vecVal.x, vecVal.y, vecVal.z);
		}
		else if (_prop.get_type() == rttr::type::get<glm::quat>())
		{
			// get default values then update
			glm::quat quatVal = _prop.get_value(_propObj).get_value<glm::quat>();
			quatVal.x = _value["x"].GetFloat();
			quatVal.y = _value["y"].GetFloat();
			quatVal.z = _value["z"].GetFloat();
			quatVal.w = _value["w"].GetFloat();
			_prop.set_value(_propObj, quatVal);
			//AeonCore_INFO("Quat: {} {} {} {}", quatVal.x, quatVal.y, quatVal.z, quatVal.w);
		}
		else if (_prop.get_type() == rttr::type::get<glm::mat4>())
		{
			glm::mat4 matVal = _prop.get_value(_propObj).get_value<glm::mat4>();
			glm::vec4 col = {};
			for (rapidjson::SizeType j = 0; j < _value.Size(); ++j)
			{
				const rapidjson::Value& valArray = _value[j];
				col.x = valArray["x"].GetFloat();
				col.y = valArray["y"].GetFloat();
				col.z = valArray["z"].GetFloat();
				col.w = valArray["w"].GetFloat();

				matVal[j] = col;
			}
			_prop.set_value(_propObj, matVal);
		}

		else if (_prop.get_type() == rttr::type::get<std::string>())
		{
			std::string str = _value.GetString();
			if (str.empty())
			{
				return;
			}
			if (str.find('/') != std::string::npos || str.find('\\') != std::string::npos)
			{
				//parent path
				std::filesystem::path working_dir(std::filesystem::current_path());
				auto parent_path = working_dir.string();
				std::size_t pos = str.find("resources");//break the string of parent path and /resources/textures/filename
				//std::size_t pos = str.find("/");//break the string of parent path and /resources/textures/filename
				std::string fileName;
				if (pos != std::string::npos) { // this check if '/' is the end of string if not it will save the rest of the path to fileName
					//path: parentpath/resources/textures/filename
					fileName = str.substr(pos-1);
					std::replace(fileName.begin(), fileName.end(), '\\', '/');

				}
				//add the respective device parent path to the filename 
				_prop.set_value(_propObj, parent_path+fileName);
			}
			else
			{
				_prop.set_value(_propObj, std::string(_value.GetString()));
			}
			//AeonCore_INFO("String: {}", value.GetString());
		}

		else if (_prop.get_type() == rttr::type::get<float>())
		{
			_prop.set_value(_propObj, _value.GetFloat());
			//AeonCore_INFO("Float: {}", value.GetFloat());
		}

		else if (_prop.get_type() == rttr::type::get<bool>())
		{
			_prop.set_value(_propObj, _value.GetBool());
			//AeonCore_INFO("Bool: {}", value.GetBool());
		}
		else if (_prop.get_type() == rttr::type::get<int>())
		{
			_prop.set_value(_propObj, _value.GetInt());
			//AeonCore_INFO("Int: {}", value.GetInt());
		}
		else if (_prop.get_type() == rttr::type::get<short>())
		{
			int value = _value.GetInt();
			_prop.set_value(_propObj, static_cast<short>(value));
		}
		else if (_prop.get_type() == rttr::type::get<GLuint>())
		{
			_prop.set_value(_propObj, _value.GetUint());
			//AeonCore_INFO("U Int: {}", propVal[key.c_str()].GetUint());
		}
		else if (_prop.get_type() == rttr::type::get<UI::Order>())
		{
			_prop.set_value(_propObj, _value.GetUint());
		}
	}

}
