#include "acpch.h"
/*!***************************************************************************
	\file            Scripting.cpp
	\author          Lam En Qing
	\par             enqing.lam\@digipen.edu
	\brief           This file defines the functions declared in Scripting.h
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Scripting/Scripting.h"

#include <mono/metadata/attrdefs.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/threads.h>

#include "Core/Application.h"
#include "Scripting/ScriptAPI.h"

#include "Tracy.hpp"

namespace AeonCore {
	#pragma region ForwardDeclareFunctions
	char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize);
	MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath);
	const char* GetMsgFromMonoException(MonoObject* exceptionObj);
	ScriptDataType MonoTypeToScriptType(MonoType* monoType);

	template<typename T>
	MonoArray* CreateMonoArray(MonoClass* monoTypeClz, int count, T* dataArr);

	/// <summary>
	/// Get the field value of C# instance
	/// </summary>
	/// <param name="scriptClassInst">The C# class instance</param>
	/// <param name="monoField">MonoField*, mono's descriptor of the field</param>
	/// <param name="dataType">The data type of this field</param>
	/// <param name="dataBuffer">A reference of char*, this data buffer will be initialized inside this function, so pass in a char* variable that is null will do</param>
	void GetFieldDataFromMono(MonoObject* scriptClassInst, MonoClassField* monoField, ScriptDataType dataType, char*& dataBuffer);
	#pragma endregion

	struct ScriptingData {
		MonoDomain* mRootDomain{ nullptr };
		MonoDomain* mAppDomain{ nullptr };

		MonoAssembly* mCoreAssembly{ nullptr };
		MonoImage* mCoreImage{ nullptr };

		MonoAssembly* mAppAssembly{ nullptr };
		MonoImage* mAppImage{ nullptr };

		MonoThread* mMonoThread{ nullptr };

		std::unordered_map<std::string, MonoMethod*> mEventCallerMap;

		MonoClass* mBehaviourBaseClz{ nullptr };
		MonoMethod* mBehaviourBaseCtor{ nullptr }; // this constructor takes in one parameter, an EntityID

		std::unordered_map<std::string, ScriptClassRef> mBehaviourClasses;
		std::unordered_map<SceneID, std::unordered_map<EntityID, std::vector<ScriptInstanceRef>>> mEntityScriptInstances; // An entity paired with a vector of ScriptInstanceRef. Index of ScriptInstanceRef is the integer in Behaviour component's behaviourNames map

		std::unordered_map<SceneID, std::vector<ScriptInstance*>> mNewlyCreatedInstances;
		//std::vector<ScriptInstance*> mNewlyCreatedInstances; // ScriptInstances that have been instantiated but not run their OnCreate yet

#ifdef _DEBUG
		bool mEnableDebugging = true;
#else
		bool mEnableDebugging = false;
#endif
	};
	std::string gAppScriptNamespace("AeonScripts");
	std::string gScriptsFolderPath("./resources/scripts/");

	std::string Scripting::sCoreNamespace("AeonScriptCore");
	std::string gCoreAssemblyFilepath(gScriptsFolderPath + Scripting::sCoreNamespace + ".dll");
	std::string Scripting::sAppAssemblyFilepath(gScriptsFolderPath + gAppScriptNamespace + ".dll");
	bool Scripting::sTellEditorScriptsReloaded{ false };

	static ScriptingData* sData{ nullptr };

	#pragma region Scripting Implementation
	void Scripting::Init() {
		if (sData != nullptr)
			return;

		sData = new ScriptingData();
		sData->mNewlyCreatedInstances.reserve(50);
		InitMono();
		ScriptAPI::RegisterFunctions(); // Note: Could call RegisterFunctions anywhere inside the Init, as long as after InitMono

		bool status = LoadCoreAssembly();
		AeonCore_ASSERT(status, "Could not load {} assembly", sCoreNamespace);
		status = LoadAppAssembly();
		AeonCore_ASSERT(status, "Could not load {} assembly", gAppScriptNamespace);

		LoadAssemblyClasses();
		ScriptAPI::RegisterComponentTypes();

		/*ScriptClassRef testBvrClass = GetBehaviourClass(sCoreNamespace + ".TestBehaviour");
		if (testBvrClass.get() != nullptr) {

			int boom = 10;
			bool rah = true;

			void* args[] = {
				&boom,
				&rah
			};
			testBvrClass->InvokeMethod(nullptr, testBvrClass->GetMethod("InvokeMeTest", 2), args);
		}*/
	}

	void Scripting::Exit() {
		if (sData == nullptr)
			return;

		AeonCore_ERROR("Scripting is exiting");
		
		// since the application is shutting down, might as well clear sData
		sData->mEntityScriptInstances.clear();
		sData->mBehaviourClasses.clear();
		ShutdownMono();

		delete sData;
		sData = nullptr;
	}

	void Scripting::OnStart() {
		if (sData == nullptr) return;

		// Instantiate everything
		for (auto& [_, entityMap] : sData->mEntityScriptInstances) {
			for (auto&[entID, scriptInstList] : entityMap) {
				for (auto& scriptInst : scriptInstList) {
					scriptInst->Instantiate();
				}
			}
		}
	}
	void Scripting::OnStop() {
		if (sData == nullptr) return;

		for (auto& [_, entityMap] : sData->mEntityScriptInstances) {
			for (auto&[entID, scriptInstList] : entityMap) {
				for (auto& scriptInst : scriptInstList) {
					scriptInst->FreeMonoData();
				}
			}
		}
		sData->mNewlyCreatedInstances.clear(); // must clear the OnCreate queue since scene is stopping

		InvokeEventFunction("OnScriptingStop");
	}

	void Scripting::OnCachedSceneActive(SceneID sceneId) {
		if (sData == nullptr) return;

		auto sceneMapFind = sData->mEntityScriptInstances.find(sceneId);

		if (sceneMapFind != sData->mEntityScriptInstances.end()) {
			for (auto&[_, scriptInstList] : sceneMapFind->second) {
				for (auto& scriptInst : scriptInstList) {
					scriptInst->InvokeOnCachedSceneActive();
				}
			}
		}
	}

	void Scripting::OnCachedSceneInactive(SceneID sceneId) {
		if (sData == nullptr) return;

		auto sceneMapFind = sData->mEntityScriptInstances.find(sceneId);

		if (sceneMapFind != sData->mEntityScriptInstances.end()) {
			for (auto&[_, scriptInstList] : sceneMapFind->second) {
				for (auto& scriptInst : scriptInstList) {
					scriptInst->InvokeOnCachedSceneInactive();
				}
			}
		}
	}

	void Scripting::AttachMonoToThread() {
		if (sData == nullptr) return;

		if (sData->mMonoThread) return;

		sData->mMonoThread = mono_thread_attach(sData->mAppDomain);
	}
	void Scripting::DetachMonoFromThread() {
		if (sData == nullptr) return;

		mono_thread_detach(sData->mMonoThread);

		sData->mMonoThread = nullptr;
	}

	void Scripting::InvokeEventFunction(const std::string& eventName, void** params) {
		if (sData == nullptr) return;
		auto findResult = sData->mEventCallerMap.find(eventName);

		if (findResult != sData->mEventCallerMap.end() && findResult->second != nullptr) {
			ScriptUtils::MonoInvokeMethod(findResult->second, nullptr, params);
		}
	}

	void Scripting::RunScriptInstancesOnCreate() {
		if (sData == nullptr) return;

		Application& app = Application::GetApplication();
		if (app.GetRuntimeState() != RuntimeState::Start /*|| app.GetLoadStatus() != LoadStatus::NotRunning*/) {
			return;
		}

		ZoneScopedN("Script Running");
		SceneID activeScene = SceneManager::GetInstance().GetActiveScene()->GetSceneID();
		auto findResult = sData->mNewlyCreatedInstances.find(activeScene);
		if (findResult != sData->mNewlyCreatedInstances.end()) {
			for (const auto& inst : findResult->second) {
				inst->InvokeOnCreate();
			}
			sData->mNewlyCreatedInstances.erase(findResult);
		}
	}

	const std::unordered_map<std::string, ScriptClassRef>& Scripting::GetBehaviourClasses() {
		if (sData == nullptr) return std::unordered_map<std::string, ScriptClassRef>{};
		return sData->mBehaviourClasses;
	}

	bool Scripting::DoesBehaviourClassExists(const std::string& className) {
		return GetBehaviourClass(className) != nullptr;
	}

	ScriptClassRef Scripting::GetBehaviourClass(const std::string& name) {
		if (sData == nullptr) return nullptr;

		AeonCore_ASSERT(sData != nullptr, "The ScriptingData does not exist!");
		auto it = sData->mBehaviourClasses.find(name);
		if (it == sData->mBehaviourClasses.end())
			return nullptr;

		return it->second;
	}

	int Scripting::CreateBehaviourInstance(const EntityID& entityId, const std::string& fullClassName) {
		ScriptClassRef bvrClzRef = GetBehaviourClass(fullClassName);
		if (bvrClzRef) {
			AeonCore_ASSERT(sData != nullptr, "The ScriptingData does not exist!");

			auto& scriptVec = sData->mEntityScriptInstances[ECSUtils::ExtractSceneID(entityId)][entityId];
			scriptVec.emplace_back(new ScriptInstance(entityId, bvrClzRef));

			return static_cast<int>(scriptVec.size()) - 1;
		}

		return -1;
	}

	std::vector<ScriptInstanceRef>* Scripting::GetBehaviourInstances(EntityID ownerID) {
		if (sData == nullptr) return nullptr;

		auto sceneFind = sData->mEntityScriptInstances.find(ECSUtils::ExtractSceneID(ownerID));

		if (sceneFind != sData->mEntityScriptInstances.end()) {
			auto entityFind = sceneFind->second.find(ownerID);

			if (entityFind != sceneFind->second.end()) {
				return &entityFind->second;
			}
		}

		return nullptr;
	}

	ScriptInstanceRef Scripting::GetBehaviourInstance(EntityID ownerID, int behaviourIdx) {
		if (sData == nullptr) return nullptr;

		auto sceneFind = sData->mEntityScriptInstances.find(ECSUtils::ExtractSceneID(ownerID));
		if (sceneFind == sData->mEntityScriptInstances.end()) {
			AeonCore_ASSERT(false, "The Scene ID " + std::to_string(ECSUtils::ExtractSceneID(ownerID)) + " of the given Entity ID does not exist in entityScripts records");
			return nullptr;
		}

		auto entityInstItr = sceneFind->second.find(ownerID);
		if (entityInstItr == sceneFind->second.end()) {
			AeonCore_ASSERT(false, "The given Entity ID " + std::to_string(ownerID) + "does not own a Behaviour scripts");
			return nullptr;
		}

		std::vector<ScriptInstanceRef>& entityInstances = entityInstItr->second;
		AeonCore_ASSERT(behaviourIdx >= 0 && behaviourIdx < entityInstances.size(), "Behaviour index is not a valid number");

		return entityInstances[behaviourIdx];
	}

	void Scripting::DestroyBehaviourInstance(Behaviour* behaviourComp, int behaviourIdx, bool shouldErase) {
		//std::cout << "DestroyBehaviourInstance, does s_Data exist?" << (s_Data != nullptr) << std::endl;
		/*if (s_Data != nullptr)
			std::cout << "DestroyBehaviourInstance, map has data?" << (s_Data->m_EntityScriptInstances.size()) << std::endl;*/

		if (sData == nullptr)
			return;

		auto sceneFind = sData->mEntityScriptInstances.find(ECSUtils::ExtractSceneID(behaviourComp->GetOwnerID()));
		if (sceneFind == sData->mEntityScriptInstances.end()) {
			AeonCore_ASSERT(false, "The Scene ID does not own exist in entityScripts records");
			return;
		}

		auto entityInstItr = sceneFind->second.find(behaviourComp->GetOwnerID());
		if (entityInstItr == sceneFind->second.end()) {
			AeonCore_ASSERT(false, "The given Entity ID does not own any Behaviour scripts");
			return;
		}
		//AeonCore_ASSERT(entityInstItr != s_Data->m_EntityScriptInstances.end(), "The given Entity ID does not own any Behaviour scripts");

		std::vector<ScriptInstanceRef>& entityInstances = entityInstItr->second;
		if (behaviourIdx < 0 || behaviourIdx >= entityInstances.size()) {
			AeonCore_ASSERT(false, "Behaviour index is not a valid number");
			return;
		}
		//AeonCore_ASSERT(behaviourIdx >= 0 && behaviourIdx < entityInstances.size(), "Behaviour index is not a valid number");

		entityInstances[behaviourIdx]->FreeMonoData();

		int oldBvrIdx = static_cast<int>(entityInstances.size()) - 1; // the index of the last Behaviour instance
		entityInstances[behaviourIdx] = entityInstances.back(); // Replace the "deleted" instance with the last Behaviour instance. A more efficient way of deleting, just swap the "deleted" for the last element, then shrink the size
		entityInstances.resize(oldBvrIdx); // Coincidentally, the new size is 1 lesser than the old size, which is the same number as oldBvrIdx

		std::string removedScriptName;
		// Deletion is done, now need to update behaviourComp
		for (auto& idBvrNamePair : behaviourComp->mBehaviourNames) {
			if (idBvrNamePair.second == behaviourIdx) { // if same as the target behaviour to delete, we set this index to -1
				idBvrNamePair.second = -1;
				removedScriptName = idBvrNamePair.first;
			}
			if (idBvrNamePair.second == oldBvrIdx) { // the index of the Behaviour instance which got moved.
				idBvrNamePair.second = behaviourIdx; // overwrite the old index with the new location's index.
			}
		}

		if (!removedScriptName.empty() && shouldErase) {
			behaviourComp->mBehaviourNames.erase(removedScriptName);
		}
	}

	void Scripting::DestroyEntityBehaviours(Behaviour* behaviourComp) {
		//std::cout << "DestroyEntityBehaviours (" << behaviourComp->m_BehaviourNames.size() << "):";
		/*for (auto& pair : behaviourComp->mBehaviourNames) {
			//std::cout << '\t' << pair.first << ',' << pair.second;
			DestroyBehaviourInstance(behaviourComp, pair.second, false);
		}*/
		
		if (sData == nullptr) return;

		auto sceneFind = sData->mEntityScriptInstances.find(ECSUtils::ExtractSceneID(behaviourComp->GetOwnerID()));

		if (sceneFind == sData->mEntityScriptInstances.end()) {
			AeonCore_INFO("DestroyEntityBehaviours: Failed to find scene ID");
			return;
		}

		auto entityInstItr = sceneFind->second.find(behaviourComp->GetOwnerID());
		if (entityInstItr == sceneFind->second.end()) {
			AeonCore_INFO("Behaviour component {} does not own any scripts to destroy", behaviourComp->GetOwnerID());
			return;
		}

		sceneFind->second.erase(entityInstItr);

		behaviourComp->mBehaviourNames.clear();
		//std::cout << std::endl;
	}

	MonoImage* Scripting::GetCoreAssemblyImage() {
		if (sData == nullptr) return nullptr;

		return sData->mCoreImage;
	}

	void Scripting::InitMono() {
		mono_set_dirs(".", ".");
		mono_set_assemblies_path("./lib");

		if (sData->mEnableDebugging) {
			// Thanks to this on how to make line numbers be outputted instead of garbage: https://github.com/dotnet/runtime/issues/85223
			mono_debug_init(MONO_DEBUG_FORMAT_MONO);
		}

		sData->mRootDomain = mono_jit_init("GAM300_AeonStudios");
		AeonCore_ASSERT(sData->mRootDomain, "Mono failed to initialize");

		if (sData->mEnableDebugging)
			mono_debug_domain_create(sData->mRootDomain);

		mono_thread_set_main(mono_thread_current());
	}

	void Scripting::ShutdownMono() {
		mono_domain_set(mono_get_root_domain(), false);

		if (sData->mAppDomain)
			mono_domain_unload(sData->mAppDomain);
		sData->mAppDomain = nullptr;

		if (sData->mRootDomain)
			mono_jit_cleanup(sData->mRootDomain);
		sData->mRootDomain = nullptr;
	}

	bool Scripting::LoadCoreAssembly() {
		// Create app domain
		sData->mAppDomain = mono_domain_create_appdomain(const_cast<char*>("AeonScriptRuntime"), nullptr);
		mono_domain_set(sData->mAppDomain, true);

		sData->mCoreAssembly = LoadMonoAssembly(gCoreAssemblyFilepath);
		if (sData->mCoreAssembly == nullptr)
			return false;

		sData->mCoreImage = mono_assembly_get_image(sData->mCoreAssembly);
		return true;
	}

	bool Scripting::LoadAppAssembly() {
		sData->mAppAssembly = LoadMonoAssembly(sAppAssemblyFilepath);
		if (sData->mAppAssembly == nullptr)
			return false;

		sData->mAppImage = mono_assembly_get_image(sData->mAppAssembly);
		return true;
	}

	void Scripting::ReloadAssembly(bool wasPrevStateNotStop) {
		for (auto&[clzName, clzRef] : sData->mBehaviourClasses) {
			clzRef->InvalidateMonoData();
		}

		if (wasPrevStateNotStop) { // if the editor was not in stop state, meaning that the runtime was running or paused, we must destroy the ScriptInstances, because the scene was re-created!
			sData->mEntityScriptInstances.clear();
		}
		else { // if the editor was already in the stop state when hot-reload occurred, we must invalidate the fields
			for (auto& [_, entityMap] : sData->mEntityScriptInstances) {
				for (auto& entScriptList : entityMap) {
					for (ScriptInstanceRef& inst : entScriptList.second) {
						inst->FreeMonoData();

						for (auto&[fieldName, fieldRef] : inst->GetFieldMap()) {
							fieldRef.mField->mClassField = nullptr;
						}
					}
				}
			}
		}

		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(sData->mAppDomain);

		bool status = LoadCoreAssembly();
		AeonCore_ASSERT(status, "Could not load {} assembly", sCoreNamespace);
		status = LoadAppAssembly();
		AeonCore_ASSERT(status, "Could not load {} assembly", gAppScriptNamespace);

		LoadAssemblyClasses();
		ScriptAPI::RegisterComponentTypes();

		// Re-validate the ScriptClass and ScriptField references
		for (auto& [_, entityMap] : sData->mEntityScriptInstances) {
			for (auto& entScriptList : entityMap) {
				for (ScriptInstanceRef& inst : entScriptList.second) {
					ScriptClassRef scriptClass{ inst->GetScriptClass() };
					std::string className = scriptClass->GetNamespace() + "." + scriptClass->GetName();

					// Renew the ScriptClass reference, because it was invalidated
					scriptClass = GetBehaviourClass(className);
					inst->SetScriptClass(scriptClass);

					if (scriptClass) {
						// Renew the ScriptField pointer references, because they were invalidated
						for (auto&[fieldName, fieldInst] : inst->GetFieldMap()) {
							auto fieldFind = scriptClass->GetFields().find(fieldName);
							if (fieldFind != scriptClass->GetFields().end()) {
								fieldInst.mField = fieldFind->second;
							}
						}
					}
				}
			}
		}

		sTellEditorScriptsReloaded = true;
		AeonCore_WARN("Reloaded scripts");
	}
	#pragma endregion

	#pragma region ScriptFieldInstance
	ScriptFieldInstance::ScriptFieldInstance(ScriptFieldRef _scriptField, char* _value, size_t _dataSize) : mField{ _scriptField }, mValue{ new char[_dataSize] }, mDataSize{ _dataSize } {
		//std::cout << "Fields constructor" << std::endl;
		memcpy(mValue, _value, mDataSize);
	}

	ScriptFieldInstance::ScriptFieldInstance(const ScriptFieldInstance& copy) : mField{ copy.mField }, mValue{ new char[copy.mDataSize] }, mDataSize{ copy.mDataSize } {
		//std::cout << "Copy constructor" << std::endl;
		memcpy(mValue, copy.mValue, mDataSize);
	}

	ScriptFieldInstance::~ScriptFieldInstance() {
		if (mValue) {
			delete[] mValue;
		}
	}

	void ScriptFieldInstance::SetValueDataBuffer(char* data, size_t newSize, bool reallocOnlyIfLarger) {
		AeonCore_INFO("FieldInst::SetValueDataBuffer, incoming data size is {}", newSize);

		// Realloacate mem if the sizes differ
		if ((reallocOnlyIfLarger && newSize > mDataSize) || (!reallocOnlyIfLarger && newSize != mDataSize)) {
			AeonCore_INFO("FieldInst::SetValueDataBuffer realloc");
			if (mValue)
				delete[] mValue;

			mDataSize = newSize;
			mValue = new char[mDataSize];
		}

		memcpy(mValue, data, newSize);
	}

	char* ScriptFieldInstance::GetValueDataBuffer() {
		return mValue;
	}

	char* const ScriptFieldInstance::GetValueDataBuffer() const {
		return mValue;
	}
	#pragma endregion

	#pragma region ScriptClass
	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className) : mClassNamespace(classNamespace), mClassName(className) {
		mMonoClass = mono_class_from_name(sData->mAppImage, classNamespace.c_str(), className.c_str());

		mOnCreateMethod = GetMethod("OnCreate", 0);
		mUpdateMethod = GetMethod("Update", 0);
		mOnCachedSceneActiveMethod = GetMethod("OnCachedSceneActive", 0);
		mOnCachedSceneInactiveMethod = GetMethod("OnCachedSceneInactive", 0);

		mOnCollisionEnterMethod = GetMethod("OnCollisionEnter", 1);
		mOnCollisionUpdateMethod = GetMethod("OnCollisionUpdate", 1);
		mOnCollisionExitMethod = GetMethod("OnCollisionExit", 1);

		mOnTriggerEnterMethod = GetMethod("OnTriggerEnter", 1);
		mOnTriggerUpdateMethod = GetMethod("OnTriggerUpdate", 1);
		mOnTriggerExitMethod = GetMethod("OnTriggerExit", 1);
	}

	ScriptClass::~ScriptClass() {
		InvalidateMonoData();
	}

	void ScriptClass::InvalidateMonoData() {
		mMonoClass = nullptr;
		mOnCreateMethod = nullptr;
		mUpdateMethod = nullptr;
		mOnCachedSceneActiveMethod = nullptr;
		mOnCachedSceneInactiveMethod = nullptr;
		mOnCollisionEnterMethod = nullptr;
		mOnCollisionUpdateMethod = nullptr;
		mOnCollisionExitMethod = nullptr;
		mOnTriggerEnterMethod = nullptr;
		mOnTriggerUpdateMethod = nullptr;
		mOnTriggerExitMethod = nullptr;
	}

	bool ScriptClass::IsClassValid() const {
		return mMonoClass != nullptr;
	}

	MonoObject* ScriptClass::Instantiate() {
		if (sData == nullptr) return nullptr;
		MonoObject* instance = mono_object_new(sData->mAppDomain, mMonoClass); // allocate memory for the instance
		mono_runtime_object_init(instance); // invoke default parameter-less constructor
		return instance;
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& methodName, int parameterCount) {
		return mono_class_get_method_from_name(mMonoClass, methodName.c_str(), parameterCount);
	}

	const ScriptFieldMap& ScriptClass::GetFields() const {
		return mFields;
	}

	ScriptFieldMap& ScriptClass::GetFields() {
		return mFields;
	}

	const std::string& ScriptClass::GetNamespace() const {
		return mClassNamespace;
	}

	const std::string& ScriptClass::GetName() const {
		return mClassName;
	}

	std::string ScriptClass::GetFullName() const {
		return mClassNamespace + "." + mClassName;
	}
	#pragma endregion

	#pragma region ScriptInstance
	ScriptInstance::ScriptInstance(EntityID ownerID, ScriptClassRef scriptClass) : mOwnerID(ownerID), mScriptClass(scriptClass) {
		// If not paused, means application would be running, can create C# instances here
		if (Application::GetApplication().GetRuntimeState() != RuntimeState::Stop)
			Instantiate();
	}

	ScriptInstance::~ScriptInstance() {
		FreeMonoData();
	}

	const ScriptClassRef ScriptInstance::GetScriptClass() const {
		return mScriptClass;
	}

	const ScriptFieldInstanceMap& ScriptInstance::GetFieldMap() const {
		return mFieldInsts;
	}

	ScriptFieldInstanceMap& ScriptInstance::GetFieldMap() {
		return mFieldInsts;
	}

	MonoObject* ScriptInstance::GetManagedInstance() {
		return mInstance;
	}

	void ScriptInstance::Instantiate() {
		if (mInstance || !mScriptClass->IsClassValid()) return; // No need instantiate if alr instantiated before

		mInstance = mScriptClass->Instantiate();
		mInstHandle = mono_gchandle_new(mInstance, true);

		if (sData->mBehaviourBaseCtor) {
			//std::cout << m_ScriptClass->GetName() << " has a constructor" << std::endl;
			void* param = &mOwnerID;
			ScriptUtils::MonoInvokeMethod(sData->mBehaviourBaseCtor, mInstance, &param);

			for (auto&[fieldName, fieldInst] : mFieldInsts) {
				if (fieldInst.mField->mClassField != nullptr) {
					//AeonCore_INFO("In {} instantiate, setting field {}", mScriptClass->GetFullName(), fieldName);
					if (fieldInst.mField->mType == ScriptDataType::String) {
						SetFieldValueInternal(fieldInst.mField->mClassField, ScriptUtils::CToMonoString(fieldInst.GetValueDataBuffer()));
					}
					else {
						SetFieldValueInternal(fieldInst.mField->mClassField, fieldInst.GetValueDataBuffer());
					}
				}
			}
		}

		sData->mNewlyCreatedInstances[ECSUtils::ExtractSceneID(mOwnerID)].emplace_back(this);
	}

	#pragma region Invoke Junk
	void ScriptInstance::InvokeOnCreate() {
		if (mScriptClass->mOnCreateMethod) {
			//AeonCore_INFO("Invoking onCreate for {}", mScriptClass->GetName());
			ScriptUtils::MonoInvokeMethod(mScriptClass->mOnCreateMethod, mInstance);
		}
	}

	void ScriptInstance::InvokeOnCachedSceneActive() {
		if (mScriptClass->mOnCachedSceneActiveMethod) {
			ScriptUtils::MonoInvokeMethod(mScriptClass->mOnCachedSceneActiveMethod, mInstance);
		}
	}

	void ScriptInstance::InvokeOnCachedSceneInactive() {
		if (mScriptClass->mOnCachedSceneInactiveMethod) {
			ScriptUtils::MonoInvokeMethod(mScriptClass->mOnCachedSceneInactiveMethod, mInstance);
		}
	}

	void ScriptInstance::InvokeUpdate() {
		if (mInstance && mScriptClass->mUpdateMethod) {
			//AeonCore_INFO("Invoking update");
			ScriptUtils::MonoInvokeMethod(mScriptClass->mUpdateMethod, mInstance);
		}
	}

	void ScriptInstance::InvokeOnCollisionEnter(EntityID otherCollidingEntity) {
		if (mInstance && mScriptClass->mOnCollisionEnterMethod) {
			void* param = &otherCollidingEntity;
			ScriptUtils::MonoInvokeMethod(mScriptClass->mOnCollisionEnterMethod, mInstance, &param);
		}
	}

	void ScriptInstance::InvokeOnCollisionUpdate(EntityID otherCollidingEntity) {
		if (mInstance && mScriptClass->mOnCollisionUpdateMethod) {
			void* param = &otherCollidingEntity;
			ScriptUtils::MonoInvokeMethod(mScriptClass->mOnCollisionUpdateMethod, mInstance, &param);
		}
	}

	void ScriptInstance::InvokeOnCollisionExit(EntityID otherCollidingEntity) {
		if (mInstance && mScriptClass->mOnCollisionExitMethod) {
			void* param = &otherCollidingEntity;
			ScriptUtils::MonoInvokeMethod(mScriptClass->mOnCollisionExitMethod, mInstance, &param);
		}
	}

	void ScriptInstance::InvokeOnTriggerEnter(EntityID otherCollidingEntity) {
		if (mInstance && mScriptClass->mOnTriggerEnterMethod) {
			void* param = &otherCollidingEntity;
			ScriptUtils::MonoInvokeMethod(mScriptClass->mOnTriggerEnterMethod, mInstance, &param);
		}
	}

	void ScriptInstance::InvokeOnTriggerUpdate(EntityID otherCollidingEntity) {
		if (mInstance && mScriptClass->mOnTriggerUpdateMethod) {
			void* param = &otherCollidingEntity;
			ScriptUtils::MonoInvokeMethod(mScriptClass->mOnTriggerUpdateMethod, mInstance, &param);
		}
	}

	void ScriptInstance::InvokeOnTriggerExit(EntityID otherCollidingEntity) {
		if (mInstance && mScriptClass->mOnTriggerExitMethod) {
			void* param = &otherCollidingEntity;
			ScriptUtils::MonoInvokeMethod(mScriptClass->mOnTriggerExitMethod, mInstance, &param);
		}
	}
	#pragma endregion

	void ScriptInstance::ResetFields() {
		mFieldInsts.clear();
	}

	void ScriptInstance::SetFieldValueStr(const std::string& name, const std::string& value, bool useFieldInstance) {
		auto& fields = mScriptClass->GetFields();
		auto it = fields.find(name);

		if (it == fields.end()) {
			AeonCore_ERROR("Cannot find string field {} in {} to set", name, mScriptClass->GetName());
			return;
		}
		ScriptFieldRef& field = it->second;
		if (field->mClassField == nullptr) return;
		//AeonCore_INFO("SetFieldValueStr: incoming data is {}", value);

		if (useFieldInstance) {
			auto fieldItr = mFieldInsts.find(name);
			char* settingValue = const_cast<char*>(value.c_str());

			if (fieldItr != mFieldInsts.end()) { // field record exists
				fieldItr->second.SetValueDataBuffer(settingValue, value.length() + 1, true);
			}
			else { // if field record does not exist, add it into the map
				// write the overridden value
				mFieldInsts.insert({ name, ScriptFieldInstance(field, settingValue, value.length() + 1) });
			}
		}
		else {
			SetFieldValueInternal(field->mClassField, ScriptUtils::CToMonoString(value.c_str()));
		}
	}

	std::string ScriptInstance::GetFieldValueStr(const std::string& name, bool useFieldInstance, bool* isDefaultValue) {
		if (useFieldInstance) {
			auto it = mFieldInsts.find(name);

			if (it != mFieldInsts.end()) {
				if (isDefaultValue)
					*isDefaultValue = false;
				return it->second.GetValueDataBuffer();
			}
		}

		const auto& fields = mScriptClass->GetFields();
		auto it = fields.find(name);

		if (it == fields.end() || it->second->mClassField == nullptr)
			return "";

		if (isDefaultValue)
			*isDefaultValue = true;

		if (mInstance) {
			MonoString* monoStr;
			GetFieldValueInternal(it->second->mClassField, &monoStr);

			//std::cout << "GetFieldValueStr:" << ScriptUtils::MonoToCString(monoStr, true) << std::endl;

			return ScriptUtils::MonoToCString(monoStr);
		}
		return it->second->GetValue(); // return default field value
	}

	void ScriptInstance::SetFieldValueInternal(MonoClassField* monoField, void* buffer) {
		if (mInstance) {
			//std::cout << "SetFieldValueInternal" << std::endl;
			mono_field_set_value(mInstance, monoField, buffer);
		}
	}

	void ScriptInstance::GetFieldValueInternal(MonoClassField* monoField , void* buffer) {
		mono_field_get_value(mInstance, monoField, buffer);
	}

	void ScriptInstance::FreeMonoData() {
		if (mInstance)
			mono_gchandle_free(mInstHandle);

		mInstance = nullptr;
	}

	void ScriptInstance::SetScriptClass(ScriptClassRef scriptClass) {
		if (scriptClass != nullptr)
			mScriptClass = scriptClass;
	}
	#pragma endregion

	#pragma region Utils
	std::string ScriptUtils::MonoToCString(MonoString* monoString) {
		if (monoString == nullptr || mono_string_length(monoString) == 0)
			return "";

		char* cStr = mono_string_to_utf8(monoString);
		std::string stdStr{ cStr };

		mono_free(cStr);

		return stdStr;
	}

	MonoArray* ScriptUtils::CreateMonoArrayFloat(int count, float* dataArr) {
		return CreateMonoArray<float>(mono_get_single_class(), count, dataArr);
	}

	MonoArray* ScriptUtils::CreateMonoArrayDouble(int count, double* dataArr) {
		return CreateMonoArray<double>(mono_get_double_class(), count, dataArr);
	}

	MonoArray* ScriptUtils::CreateMonoArrayInt(int count, int* dataArr) {
		return CreateMonoArray<int>(mono_get_int32_class(), count, dataArr);
	}

	MonoArray* ScriptUtils::CreateMonoArrayUnsignedInt(int count, uint32_t* dataArr) {
		return CreateMonoArray<uint32_t>(mono_get_uint32_class(), count, dataArr);
	}

	MonoObject* ScriptUtils::MonoInvokeMethod(MonoMethod* method, MonoObject* classInstance, void** params) {
		MonoObject* exception{ nullptr };
		MonoObject* methodReturnValue{ nullptr };

		//AeonCore_INFO("Going to invoke method {} from class {}", mono_method_get_name(method), mClassName);

		try {
			methodReturnValue = mono_runtime_invoke(method, classInstance, params, &exception);
		}
		catch (...) {
			AeonCore_ERROR("Error invoking {}:{}", mono_method_get_name(method), GetMsgFromMonoException(exception));
		}

		if (exception) {
			AeonCore_ERROR("Error invoking {}:{}", mono_method_get_name(method), GetMsgFromMonoException(exception));
		}

		return methodReturnValue;
	}

	MonoString* ScriptUtils::CToMonoString(const char* cStr) {
		return mono_string_new(sData->mAppDomain, cStr);
	}

	template<typename T>
	MonoArray* CreateMonoArray(MonoClass* monoTypeClz, int count, T* dataArr) {
		MonoArray* someArr = mono_array_new(sData->mAppDomain, monoTypeClz, count);

		for (int i = 0; i < count; i++) {
			mono_array_set(someArr, T, i, dataArr[i]);
		}

		return someArr;
	}
	
	char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize) {
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		AeonCore_ASSERT(stream, "Failed to open " + filepath.string());

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = static_cast<uint32_t>(end - stream.tellg());

		if (size == 0)
			return nullptr;

		char* buffer = new char[size];
		stream.read((char*)buffer, size);
		stream.close();

		*outSize = size;
		return buffer;
	}

	MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath) {
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);

		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

		if (status != MONO_IMAGE_OK)
		{
			const char* errorMessage = mono_image_strerror(status);
			AeonCore_ERROR(errorMessage);
			return nullptr;
		}

		AeonCore_INFO("LoadMonoAssembly:{}", std::filesystem::absolute(assemblyPath).string());

		if (sData->mEnableDebugging) {
			std::filesystem::path pdbPath = assemblyPath;
			pdbPath.replace_extension(".pdb");

			if (std::filesystem::exists(pdbPath))
			{
				// Thanks to this source on making pdb loadable: https://github.com/godotengine/godot/issues/14822
				uint32_t pdbFileSize = 0;
				char* pdbFileData = ReadBytes(pdbPath, &pdbFileSize);
				mono_debug_open_image_from_memory(image, (const mono_byte*)pdbFileData, pdbFileSize);
				AeonCore_INFO("Loaded PDB {}", pdbPath);
				delete[] pdbFileData;
			}
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
		mono_image_close(image);

		delete[] fileData;

		return assembly;
	}

	inline const char* GetMsgFromMonoException(MonoObject* exceptionObj) {
		return mono_string_to_utf8(mono_object_to_string(exceptionObj, nullptr));
	}

	void Scripting::LoadAssemblyClasses() {
		sData->mBehaviourClasses.clear();
		sData->mEventCallerMap.clear();

		// Load from the Script Core dll
		sData->mBehaviourBaseClz = mono_class_from_name(sData->mCoreImage, sCoreNamespace.c_str(), "AeonBehaviour");
		MonoClass* showEditorClassAttr = mono_class_from_name(sData->mCoreImage, sCoreNamespace.c_str(), "ShowInEditor");
		MonoClass* hideEditorClassAttr = mono_class_from_name(sData->mCoreImage, sCoreNamespace.c_str(), "HideFromEditor");

		AeonCore_ASSERT(sData->mBehaviourBaseClz, "AeonBehaviour class does not exist in {}", sCoreNamespace);
		AeonCore_ASSERT(showEditorClassAttr, "ShowInEditor class does not exist in {}", sCoreNamespace);
		AeonCore_ASSERT(hideEditorClassAttr, "HideFromEditor class does not exist in {}", sCoreNamespace);

		sData->mBehaviourBaseCtor = mono_class_get_method_from_name(sData->mBehaviourBaseClz, ".ctor", 1);
		AeonCore_ASSERT(sData->mBehaviourBaseCtor, "AeonBehaviour class does not have a constructor with one parameter");

		MonoClass* clzEngineCallable = mono_class_from_name(sData->mCoreImage, sCoreNamespace.c_str(), "EngineCallable");
		if (clzEngineCallable) {
			AeonCore_INFO("EngineCallable was found!");
			// TODO: Ensure all methods inside EngineCallable is static!

			void* itr = nullptr;
			MonoMethod* method = mono_class_get_methods(clzEngineCallable, &itr);
			AeonCore_WARN("EngineCallable methods:");
			AeonCore_WARN("-----------------------------");
			while (method) {
				std::string mthd_name = mono_method_get_name(method);
				if (mthd_name != ".ctor") {
					AeonCore_WARN("\t{}", mthd_name);
					sData->mEventCallerMap.emplace(mthd_name, method);
				}
				method = mono_class_get_methods(clzEngineCallable, &itr);
			}
		}

		// Load from the Scripts dll
		AeonCore_ERROR("Please note that unknown C# data types will not be serialized");
		AeonCore_INFO("Loaded Behaviour Classes");
		AeonCore_INFO("-----------------------------");

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(sData->mAppImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++) {
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(sData->mAppImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(sData->mAppImage, cols[MONO_TYPEDEF_NAME]);

			MonoClass* monoClass = mono_class_from_name(sData->mAppImage, nameSpace, name);

			// if the read class is same as AeonBehaviour, skip
			if (!monoClass || !mono_class_is_subclass_of(monoClass, sData->mBehaviourBaseClz, false))
				continue;
			
			std::string fullName(nameSpace);
			fullName += ".";
			fullName += name;

			AeonCore_INFO(fullName);

			// ---- Retrieving all methods in a class ----
			// TODO: Remove methods retrieval in LoadAssemblyClasses
			void* itr = nullptr;
			/*if (monoClass == sData->mBehaviourBaseClz) {
				//AeonCore_INFO("\tHas {0} methods, methods available are:", mono_class_num_methods(monoClass));
				while (MonoMethod* classMethod = mono_class_get_methods(monoClass, &itr)) {
					std::string mthdName(mono_method_get_name(classMethod));
					int paramCount = mono_signature_get_param_count(mono_method_signature(classMethod));

					//AeonCore_INFO("\t\t{0} | parameters count is {1}", mthdName, paramCount);

					if (mthdName == ".ctor" && paramCount == 1) {
						sData->mBehaviourBaseConstructor = classMethod;
					}
				}
				continue;
			}*/

			// ---- Retrieving all fields in a class ----
			ScriptClassRef scriptableClass(new ScriptClass(nameSpace, name));
			sData->mBehaviourClasses[fullName] = scriptableClass;

			int fieldCount = mono_class_num_fields(monoClass);
			AeonCore_INFO("\tHas {0} fields, the serializable fields are:", fieldCount);

			itr = nullptr;
			MonoObject* scriptClassInst = scriptableClass->Instantiate();
			while (MonoClassField* classField = mono_class_get_fields(monoClass, &itr)) {

				const char* fieldName = mono_field_get_name(classField);
				uint32_t accessFlag = mono_field_get_flags(classField);

				if (accessFlag & MONO_FIELD_ATTR_STATIC)
					continue;

				MonoCustomAttrInfo* attrInfo = mono_custom_attrs_from_field(monoClass, classField);

				// if the field is private and has no attributes, means we should not "record" it
				if ((accessFlag & MONO_FIELD_ATTR_PRIVATE) && attrInfo == nullptr) {
					continue;
				}

				if (attrInfo != nullptr) {
					bool hasShowInEditor = mono_custom_attrs_has_attr(attrInfo, showEditorClassAttr);
					bool hasHideFromEditor = mono_custom_attrs_has_attr(attrInfo, hideEditorClassAttr);

					if ((accessFlag & MONO_FIELD_ATTR_PUBLIC) && hasHideFromEditor) {
						//std::cout << "Public and HideFromEditor for field '" << fieldName << "'" << std::endl;
						mono_custom_attrs_free(attrInfo);
						continue;
					}

					// Is private AND and has no ShowInEditor, skip
					if ((accessFlag & MONO_FIELD_ATTR_PRIVATE) && !hasShowInEditor) {
						//std::cout << "private and ShowInEditor for field '" << fieldName << "'" << std::endl;
						continue;
					}

					mono_custom_attrs_free(attrInfo);
				}

				MonoType* monoType = mono_field_get_type(classField);
				ScriptDataType fieldDataType = MonoTypeToScriptType(monoType);
				if (fieldDataType != ScriptDataType::None) {
					char* dataBuffer;
					GetFieldDataFromMono(scriptClassInst, classField, fieldDataType, dataBuffer);
					scriptableClass->mFields.try_emplace(fieldName, new ScriptField(fieldDataType, fieldName, classField, dataBuffer));
				}

				AeonCore_INFO("\t\t{0} | C# data type is {1} | C++ data type is {2}", fieldName, mono_type_get_name(monoType), (MonoTypeToScriptType(monoType) != ScriptDataType::None ? "known" : "not known"));
			}
		}
		AeonCore_INFO("-----------------------------");
	}

	inline ScriptDataType MonoTypeToScriptType(MonoType* monoType) {
		std::string typeName = mono_type_get_name(monoType);

		if (!typeName.compare("System.Boolean"))
			return ScriptDataType::Bool;
		else if (!typeName.compare("System.Single"))
			return ScriptDataType::Float;
		else if (!typeName.compare("System.Double"))
			return ScriptDataType::Double;
		else if (!typeName.compare("System.Int32"))
			return ScriptDataType::Int;
		else if (!typeName.compare("System.String"))
			return ScriptDataType::String;
		else if (!typeName.compare("AeonScriptCore.Vector3"))
			return ScriptDataType::Vector3;
		/*else if (!typeName.compare("System.Char"))
			return ScriptDataType::Char;
		else if (!typeName.compare("System.Byte"))
			return ScriptDataType::Byte;
		else if (!typeName.compare("System.Int16"))
			return ScriptDataType::Short;*/
		/*else if (!typeName.compare("System.Int64"))
			return ScriptDataType::Long;
		else if (!typeName.compare("System.UInt16"))
			return ScriptDataType::UShort;
		else if (!typeName.compare("System.UInt32"))
			return ScriptDataType::UInt;
		else if (!typeName.compare("System.UInt64"))
			return ScriptDataType::ULong;*/

		return ScriptDataType::None;
	}

	void GetFieldDataFromMono(MonoObject* scriptClassInst, MonoClassField* monoField, ScriptDataType dataType, char*& dataBuffer) {
		size_t field_data_size = 0;

		// strings must be done with special care
		if (dataType == ScriptDataType::String) {
			MonoString* monoStr;
			mono_field_get_value(scriptClassInst, monoField, &monoStr);
			std::string cStr(ScriptUtils::MonoToCString(monoStr));

			field_data_size = cStr.length() + 1; // + 1 includes null-terminating character
			dataBuffer = new char[field_data_size];
			memcpy(dataBuffer, cStr.c_str(), field_data_size);

			return;
		}

		// copy/extract data from mono into this "discardable" buffer
		char monoDataBuffer[64];
		mono_field_get_value(scriptClassInst, monoField, monoDataBuffer);

		// Create our 'managed' pointer
		if (dataType == ScriptDataType::Bool)
		{
			dataBuffer = new char[field_data_size = sizeof(bool)];
		}
		else if (dataType == ScriptDataType::Float)
		{
			dataBuffer = new char[field_data_size = sizeof(float)];
		}
		else if (dataType == ScriptDataType::Double)
		{
			dataBuffer = new char[field_data_size = sizeof(double)];
		}
		else if (dataType == ScriptDataType::Int)
		{
			dataBuffer = new char[field_data_size = sizeof(int)];
		}
		else if (dataType == ScriptDataType::Vector3)
		{
			dataBuffer = new char[field_data_size = sizeof(glm::vec3)];
		}

		// Copy the underlying data from the mono buffer into our own buffer
		if (dataBuffer) {
			memcpy(dataBuffer, monoDataBuffer, field_data_size);

			/*if (dataType == ScriptDataType::Vector3) {
				glm::vec3* test = reinterpret_cast<glm::vec3*>(dataBuffer);
				AeonCore_INFO("GetFieldDataFromMono, vec3: {}, {}, {}", test->x, test->y, test->z);
			}*/
		}
	}
	#pragma endregion

}