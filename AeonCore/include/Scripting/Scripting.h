#pragma once
/*!***************************************************************************
	\file            Scripting.h
	\author          Lam En Qing
	\par             enqing.lam\@digipen.edu
	\brief           This file declares various script classes and structs
					 to represent the C# behaviour script data
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/AeonAPI.h"

#include <map>
#include <memory>
#include <unordered_map>

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>

#include "Core/Log.h"
#include "ECS/Architecture/ECS.hpp"
#include "ECS/Components/Behaviour.h"
#include "ScriptUtils.h"

// This function must be called only once per function registered, and you only register after initialising Scripting
#define AEON_REGISTER_SCRIPTING_FUNCTION(Name) mono_add_internal_call((Scripting::sCoreNamespace + ".InternalCalls::" #Name).c_str(), Name)

namespace AeonCore {
	class ScriptClass;
	class ScriptInstance;

	// Represent a ScriptField's serialized information, except for value/data. The value come from ScriptFieldInstance
	struct AEON_API ScriptField {
		ScriptDataType mType{ ScriptDataType::None };
		std::string mName{};

		MonoClassField* mClassField{ nullptr }; // representation of the field in a C# class
		char* mDefaultValue{ nullptr };

		~ScriptField() {
			//AeonCore_INFO("ScriptField destructor, {}", mName);
			delete[] mDefaultValue;
		}

		template<typename T>
		const T& GetValue() const {
			return *(static_cast<T*>(static_cast<void*>(mDefaultValue)));
		}

		char* GetValue() const {
			return mDefaultValue;
		}
	};
	using ScriptFieldRef = std::shared_ptr<ScriptField>;
	using ScriptFieldMap = std::map<std::string, ScriptFieldRef>;

	// Data storage isolated from class's "global" fields map. The value stored for this field "overrides" the global fields map data which is the field's default value.
	// The field instance's value should be passed onto serialization to "override" the default value when ScriptInstances are created
	class AEON_API ScriptFieldInstance
	{
	public:
		ScriptFieldRef mField{nullptr}; // store a reference to the field
	private:
		char* mValue{ nullptr }; // value that overrides the default value

		size_t mDataSize{ 0 };
	public:
		/// <summary>
		/// Constructor of ScriptFieldInstance, takes in a reference to ScriptField as well as a const pointer to char, the default value
		/// </summary>
		/// <param name="_scriptField">A reference to the ScriptField</param>
		/// <param name="defaultValue">Type is char*, represents the data buffer of the default field value</param>
		/// <param name="_dataSize">Type is size_t, the size of the default field value data buffer</param>
		ScriptFieldInstance(ScriptFieldRef _scriptField, char* defaultValue, size_t _dataSize);

		/// <summary>
		/// Copy constructor of ScriptFieldInstance, performs a deep copy of the field values.
		/// </summary>
		/// <param name="copy">The ScriptFieldInstance you wish to make a copy of</param>
		ScriptFieldInstance(const ScriptFieldInstance& copy); // copy constructor

		ScriptFieldInstance() = delete;
		~ScriptFieldInstance();

		/// <summary>
		/// Sets the underlying field value's data buffer, a memory copy is performed rather than re-assigning the buffer pointer.
		/// A reallocation will be made if the new data's size differs from the old data's size. So be VERY particular with the size
		/// </summary>
		/// <param name="data">The new data buffer, contents of the incoming data buffer will get copied</param>
		/// <param name="dataSize">The size of the new data</param>
		/// <param name="reallocOnlyIfLarger">By default is false. If set to true, the function will reallocate memory if the new data size is larger</param>
		void SetValueDataBuffer(char* data, size_t dataSize, bool reallocOnlyIfLarger=false);

		/// <summary>
		/// Returns the underlying the field value's data buffer. Intended use is only for strings
		/// </summary>
		/// <returns>Char*</returns>
		char* GetValueDataBuffer();

		char* const GetValueDataBuffer() const;

		/// <summary>
		/// Set the value of this field, realloacation will be made if the incoming data size differs from the old data size
		/// </summary>
		/// <typeparam name="T">Can be any type, but should be supported by the enum ScriptDataType</typeparam>
		/// <param name="data">The data of type T</param>
		template<typename T>
		void SetValue(T data) {
			//std::cout << "FieldInst::SetValue, incoming data is " << data << std::endl;
			size_t newsize(sizeof(T));

			// realloacation should never happen, only new alloc
			if (newsize != mDataSize || mValue == nullptr) {
				std::cout << "FieldInst::SetValue realloc" << std::endl;
				if (mValue)
					delete[] mValue;

				mDataSize = newsize;
				mValue = new char[mDataSize];
			}

			memcpy(mValue, &data, mDataSize);
		}

		/// <summary>
		/// Get the value of this field
		/// </summary>
		/// <typeparam name="T">Can be any type, but should be supported by the enum ScriptDataType</typeparam>
		/// <returns>Reference to type T</returns>
		template<typename T>
		T& GetValue() {
			return *(static_cast<T*>(static_cast<void*>(mValue)));
		};

		/// <summary>
		/// Get the const value of this field
		/// </summary>
		/// <typeparam name="T">Can be any type, but should be supported by the enum ScriptDataType</typeparam>
		/// <returns>Reference to type T</returns>
		template<typename T>
		const T& GetValue() const {
			return *(static_cast<T*>(static_cast<void*>(mValue)));
		};
	};

	using ScriptFieldInstanceMap = std::unordered_map<std::string, ScriptFieldInstance>;

	// ScriptClass essentially describes a behaviour class so... maybe can store OnCreate, and other Behaviour functions in here
	class AEON_API ScriptClass {
	public:
		/*
		Default, parameter-less constructor that initialises the class's variables to empty values
		*/
		//ScriptClass();

		/// <summary>
		/// Construct the class given you know it's namespace and name
		/// </summary>
		/// <param name="classNamespace">The namespace to the class.</param>
		/// <param name="className">The name of the class</param>
		ScriptClass(const std::string& classNamespace, const std::string& className);

		~ScriptClass();
		void InvalidateMonoData();

		/// <summary>
		/// Is the ScriptClass's underlying mono class reference valid?
		/// </summary>
		/// <returns>True if valid, false if not</returns>
		bool IsClassValid() const;

		/// <summary>
		/// Creates and returns an instance of the MonoClass handled by the ScriptClass. Parameter-less constructor is invoked for this instantiation.
		/// </summary>
		/// <returns>A MonoObject*, representing an instance or object of the MonoClass</returns>
		MonoObject* Instantiate();

		/// <summary>
		/// Get a C# method's representation as MonoMethod pointer
		/// </summary>
		/// <param name="methodName">Name of the method in the class</param>
		/// <param name="parameterCount">The number of parameters this method has</param>
		/// <returns>A MonoMethod pointer, think of it as like a function pointer</returns>
		MonoMethod* GetMethod(const std::string& methodName, int parameterCount);

		/// <summary>
		/// Get serializable fields of this class
		/// </summary>
		/// <returns>A map of string to ScriptField, where string is the name of the field</returns>
		const ScriptFieldMap& GetFields() const;

		ScriptFieldMap& GetFields();

		/// <summary>
		/// Gets the namespace of the class, that is the path preceeding the class name
		/// </summary>
		/// <returns>A std::string</returns>
		const std::string& GetNamespace() const;

		/// <summary>
		/// Gets the class name, only the class name, not the full class name
		/// </summary>
		/// <returns>A std::string</returns>
		const std::string& GetName() const;
		
		/// <summary>
		/// Gets the full class name, that is namespace and class name combined
		/// </summary>
		/// <returns>A std::string</returns>
		std::string GetFullName() const;

	private:
		std::string mClassNamespace;
		std::string mClassName;

		ScriptFieldMap mFields;

		MonoClass* mMonoClass{ nullptr };

		MonoMethod* mOnCreateMethod{ nullptr };
		MonoMethod* mUpdateMethod{ nullptr };

		MonoMethod* mOnCachedSceneActiveMethod{ nullptr };
		MonoMethod* mOnCachedSceneInactiveMethod{ nullptr };

		MonoMethod* mOnCollisionEnterMethod{ nullptr };
		MonoMethod* mOnCollisionUpdateMethod{ nullptr };
		MonoMethod* mOnCollisionExitMethod{ nullptr };

		MonoMethod* mOnTriggerEnterMethod{ nullptr };
		MonoMethod* mOnTriggerUpdateMethod{ nullptr };
		MonoMethod* mOnTriggerExitMethod{ nullptr };

		friend class Scripting;
		friend class ScriptInstance;
	};

	using ScriptClassRef = std::shared_ptr<ScriptClass>;

	// ScriptInstance holds the C# script instance, ScriptClassRef (that describes what C# class is the script), ScriptFieldMap (the overridden field values)
	class AEON_API ScriptInstance {
	public:
		/// <summary>
		/// Instantiates the provided ScriptClass
		/// </summary>
		/// <param name="ownerID">The entity that owns this ScriptInstance</param>
		/// <param name="scriptClass">The ScriptClass you wish to instantiate</param>
		ScriptInstance(EntityID ownerID, ScriptClassRef scriptClass);
		ScriptInstance(const ScriptInstance&) = delete;
		~ScriptInstance();

		const ScriptClassRef GetScriptClass() const;

		const ScriptFieldInstanceMap& GetFieldMap() const;
		ScriptFieldInstanceMap& GetFieldMap();

		/// <summary>
		/// Get the C# instance of this particular ScriptInstance. The instance can be null
		/// </summary>
		/// <returns>A pointer of MonoObject</returns>
		MonoObject* GetManagedInstance();

		/// <summary>
		/// Create the C# instance of the class. Used by ScriptInstance constructor and ReloadAssembly function.
		/// </summary>
		void Instantiate();

		void InvokeOnCreate();

		void InvokeOnCachedSceneActive();

		void InvokeOnCachedSceneInactive();

		/// <summary>
		/// Invokes the Update function of the C# class, if it exists
		/// </summary>
		void InvokeUpdate();
		
		/// <summary>
		/// Invokes the OnCollisionEnter function of the C# class, if it exists
		/// </summary>
		/// <param name="otherCollidingEntity">The EntityID of the other colliding entity</param>
		void InvokeOnCollisionEnter(EntityID otherCollidingEntity);
		
		/// <summary>
		/// Invokes the OnCollisionUpdate function of the C# class, if it exists
		/// </summary>
		/// <param name="otherCollidingEntity">The EntityID of the other colliding entity</param>
		void InvokeOnCollisionUpdate(EntityID otherCollidingEntity);
		
		/// <summary>
		/// Invokes the OnCollisionExit function of the C# class, if it exists
		/// </summary>
		/// <param name="otherCollidingEntity">The EntityID of the other colliding entity</param>
		void InvokeOnCollisionExit(EntityID otherCollidingEntity);
		
		/// <summary>
		/// Invokes the OnTriggerEnter function of the C# class, if it exists
		/// </summary>
		/// <param name="otherCollidingEntity">The EntityID of the other colliding entity</param>
		void InvokeOnTriggerEnter(EntityID otherCollidingEntity);
		
		/// <summary>
		/// Invokes the OnTriggerUpdate function of the C# class, if it exists
		/// </summary>
		/// <param name="otherCollidingEntity">The EntityID of the other colliding entity</param>
		void InvokeOnTriggerUpdate(EntityID otherCollidingEntity);
		
		/// <summary>
		/// Invokes the OnTriggerExit function of the C# class, if it exists
		/// </summary>
		/// <param name="otherCollidingEntity">The EntityID of the other colliding entity</param>
		void InvokeOnTriggerExit(EntityID otherCollidingEntity);

		/// <summary>
		/// Resets the field values to default by clearing the fields data stored
		/// </summary>
		void ResetFields();

		/// <summary>
		/// Set a field in this instance
		/// </summary>
		/// <typeparam name="T">Templated type should be supported in ScriptDataType</typeparam>
		/// <param name="name">The type of the field</param>
		/// <param name="value">The field's new value</param>
		/// <param name="useFieldInstance">Whether to get our own field data storage or directly from mono.</param>
		template<typename T>
		void SetFieldValue(const std::string& name, T value, bool useFieldInstance=false) {
			auto& fields = mScriptClass->GetFields();
			auto it = fields.find(name);

			if (it == fields.end()) {
				AeonCore_ERROR("Cannot find string field {} in {} to set", name, mScriptClass->GetName());
				return;
			}
			ScriptFieldRef& field = it->second;
			if (!field->mClassField) return;

			if (useFieldInstance) {
				auto fieldItr = mFieldInsts.find(name);

				if (fieldItr != mFieldInsts.end()) {
					//std::cout << "Non mono SetFieldValue:" << ScriptUtils::ScriptDataTypeToString(fieldItr->second.mField->mType) << " | " << value << std::endl;
					fieldItr->second.SetValue<T>(value);
				}
				else {
					void* valueBuffer = &value;
					mFieldInsts.insert({ name, ScriptFieldInstance(field, static_cast<char*>(valueBuffer), sizeof(T)) });
					//mFieldInsts.try_emplace(name, field, valuePtr, sizeof(T));
				}
			}
			else {
				if (it != fields.end()) {
					SetFieldValueInternal(field->mClassField, &value);
				}
			}
		}

		/// <summary>
		/// Get the data of the field value. For strings, use GetFieldValueStr function instead, NOT this template function!!!
		/// </summary>
		/// <typeparam name="T">The type of the field</typeparam>
		/// <param name="name">The variable name of the field, corresponds to the field name in the C# class</param>
		/// <param name="useFieldInstance">Whether to get our own field data storage or directly from mono. If our own field data storage does not exist, we retrieve from mono.</param>
		/// <param name="isDefaultValue">Optional parameter, the function sets the boolean value if the default value was retrieved or not</param>
		/// <returns>Type T is returned</returns>
		template<typename T>
		T GetFieldValue(const std::string& name, bool useFieldInstance=false, bool* isDefaultValue=nullptr) {
			if (useFieldInstance) {
				auto it = mFieldInsts.find(name);

				if (it != mFieldInsts.end()) {
					if (isDefaultValue)
						*isDefaultValue = false;
					return it->second.GetValue<T>();
				}
			}

			const auto& fields = mScriptClass->GetFields();
			auto it = fields.find(name);

			T someValue{};
			if (it == fields.end() || it->second->mClassField == nullptr)
				return someValue;

			if (isDefaultValue)
				*isDefaultValue = true;

			if (mInstance) {
				//std::cout << "GetFieldValue (Mono):" << name << " | " << ScriptUtils::ScriptDataTypeToString(it->second.m_Type) << std::endl;
				GetFieldValueInternal(it->second->mClassField, &someValue);
				//mono_field_get_value(mInstance, field.mClassField, &someValue);

				return someValue;
			}
			return it->second->GetValue<T>(); // return default field value
		}

		/// <summary>
		/// Set a string field with the provided value. Use this only if you know your desired field is of string type!
		/// </summary>
		/// <param name="name">The variable name of the field, corresponds to the field name in the C# class</param>
		/// <param name="value">Type is std::string, the string value to set for the field</param>
		/// <param name="useFieldInstance">Whether to get our own field data storage or directly from mono.</param>
		void SetFieldValueStr(const std::string& name, const std::string& value, bool useFieldInstance=false);

		/// <summary>
		/// Get a string value of a field. Use this only if you know your desired field is of string type!
		/// </summary>
		/// <param name="name">The variable name of the field, corresponds to the field name in the C# class</param>
		/// <param name="useFieldInstance">Whether to get our own field data storage or directly from mono.</param>
		/// <param name="isDefaultValue">Optional parameter, the function sets the boolean value if the default value was retrieved or not</param>
		/// <returns>A std::string</returns>
		std::string GetFieldValueStr(const std::string& name, bool useFieldInstance = false, bool* isDefaultValue=nullptr);
	private:
		EntityID mOwnerID;
		ScriptFieldInstanceMap mFieldInsts{};

		ScriptClassRef mScriptClass;

		MonoObject* mInstance{ nullptr };
		std::uint32_t mInstHandle{ 0 };

		/// <summary>
		/// Internal use only, it sets the field value in the mono instance
		/// </summary>
		/// <param name="monoField"></param>
		/// <param name="buffer"></param>
		void SetFieldValueInternal(MonoClassField* monoField, void* buffer);

		/// <summary>
		/// Internal use only, it gets the field value, given that the provided buffer has been initialised. It places the contents of the value into the buffer.
		/// </summary>
		/// <param name="monoField">A MonoClassField*, it describes the C# field</param>
		/// <param name="buffer">The buffer data that will contain the retrieved field value. It must be initialized before passing in. See usage in ScriptInstance constructor</param>
		void GetFieldValueInternal(MonoClassField* monoField, void* buffer);

		/// <summary>
		/// Destroys the C# script instance and its handle
		/// </summary>
		void FreeMonoData();

		void SetScriptClass(ScriptClassRef scriptClass);

		friend class Scripting;
	};

	using ScriptInstanceRef = std::shared_ptr<ScriptInstance>;

	class AEON_API Scripting {
	public:
		static std::string sCoreNamespace;
		static std::string sAppAssemblyFilepath;
		static bool sTellEditorScriptsReloaded;

		/// <summary>
		/// Initializes the whole Scripting
		/// </summary>
		static void Init();

		/// <summary>
		/// Destroys and cleans up the Scripting
		/// </summary>
		static void Exit();

		// When scripting runtime starts running, instantiate all C# scripting instances
		static void OnStart();

		// When scripting runtime stops running, destroy all C# scripting instances
		static void OnStop();

		// When Scene is brought to foreground, do smth
		static void OnCachedSceneActive(SceneID sceneId);

		// When Scene is no longer active, do smth
		static void OnCachedSceneInactive(SceneID sceneId);

		// If any scripting code is going to run on a thread, must call this to let Mono run on that thread
		static void AttachMonoToThread();

		static void DetachMonoFromThread();

		/// <summary>
		/// Invokes a static function defined in EngineCallable.cs
		/// </summary>
		/// <param name="eventName">Name of the function defined in EngineCallable.cs</param>
		/// <param name="params">Pass in an array of void*</param>
		static void InvokeEventFunction(const std::string& eventName, void** params = nullptr);

		/// <summary>
		/// Runs all active scene's script instances' OnCreate that were requested to run when the script instances were instantiated
		/// </summary>
		static void RunScriptInstancesOnCreate();

		/// <summary>
		/// Get a map of Behaviour classes loaded in
		/// </summary>
		/// <returns>An unordered map of std::string to ScriptClassRef, where string represents the full class name and ScriptClassRef describes the C# class</returns>
		static const std::unordered_map<std::string, ScriptClassRef>& GetBehaviourClasses();

		/// <summary>
		/// Returns a boolean, does a behaviour class exists given a name
		/// </summary>
		/// <param name="className">Type of std::string const ref, the name of the behaviour class to look for</param>
		/// <returns>A boolean</returns>
		static bool DoesBehaviourClassExists(const std::string& className);

		/// <summary>
		/// Returns a shared_ptr<ScriptClass> if a behaviour class exist
		/// </summary>
		/// <param name="name">Type of std::string const ref, the name of the behaviour class</param>
		/// <returns>shared_ptr<ScriptClass> is returned, nullptr if a class cannot be retrieved</returns>
		static ScriptClassRef GetBehaviourClass(const std::string& name);

		/// <summary>
		/// Creates a Behaviour instance, given an Entity ID and the behaviour class' full name
		/// </summary>
		/// <param name="entityID">The ID of the entity who will own this Behaviour instance</param>
		/// <param name="fullClassName">The full class name of the Behaviour class. That is, the namespace and the Behaviour class name.</param>
		/// <returns>An integer. If creation is successful, the integer representing the index of the Behaviour instance in a vector of instances is returned. Otherwise if the creation failed, -1 is returned.</returns>
		static int CreateBehaviourInstance(const EntityID& entityID, const std::string& fullClassName);

		/// <summary>
		/// Get a pointer to a vector of ScriptInstanceRef. If the given EntityID does not have a script, nullptr is returned
		/// </summary>
		/// <param name="ownerID">The ID of an entity who may or may not have a script</param>
		/// <returns>A pointer of a vector of ScriptInstanceRef</returns>
		static std::vector<ScriptInstanceRef>* GetBehaviourInstances(EntityID ownerID);

		/// <summary>
		/// Get an instance of the Behaviour script class
		/// </summary>
		/// <param name="ownerID">The ID of the entity who owns the Behaviour instance</param>
		/// <param name="behaviourIdx">The index of the target behaviour, which is obtained from Behaviour::m_BehaviourNames</param>
		/// <returns></returns>
		static ScriptInstanceRef GetBehaviourInstance(EntityID ownerID, int behaviourIdx);

		/// <summary>
		/// Destroy a Behaviour instance
		/// </summary>
		/// <param name="behaviourComp">A pointer of Behaviour component, belonging to the provided entity ID.</param>
		/// <param name="behaviourIdx">The index of the target behaviour, which is obtained from Behaviour::m_BehaviourNames</param>
		/// <param name="shouldErase">Bool parameter. Decides if the function should erase the record inside Behaviour::mBehaviourNames</param>
		static void DestroyBehaviourInstance(Behaviour* behaviourComp, int behaviourIdx, bool shouldErase=true);

		/// <summary>
		/// Destroy all Behaviour scripts owned by a Behaviour component
		/// </summary>
		/// <param name="behaviourComp">A pointer to a Behaviour component</param>
		static void DestroyEntityBehaviours(Behaviour* behaviourComp);

		/// <summary>
		/// Gets the image of the core assembly, that is an image of the "AeonScriptCore.dll"
		/// </summary>
		/// <returns>A MonoImage*</returns>
		static MonoImage* GetCoreAssemblyImage();

		/// <summary>
		/// Unloads the loaded scripts, read them in from the files and load the scripts in.
		/// </summary>
		static void ReloadAssembly(bool wasPrevStateNotStop);
	private:
		static void InitMono();
		static void ShutdownMono();

		static bool LoadCoreAssembly();
		static bool LoadAppAssembly();

		static void LoadAssemblyClasses();
	};
}