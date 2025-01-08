#include "acpch.h"
/*!***************************************************************************
	\file            ScriptAPI.cpp
	\author          Lam En Qing
	\par             enqing.lam\@digipen.edu
	\brief           This file defines the functions declared in ScriptAPI.h
					 and registers the internal call functions in
					 RegisterFunctions
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Scripting/ScriptAPI.h"

#include <cmath>
#include <functional> // function
#include <memory>
#include <mono/metadata/reflection.h>

#include "Camera/CameraManager.h"
#include "Core/SceneManager.h"
#include "Core/TimeStep.h"
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/AudioSystem.h"
#include "Input/Input.h"
#include "Physics/ACPhysicsGeom.hpp"
#include "Scripting/Scripting.h"

namespace AeonCore {
	static std::unordered_map<MonoType*, std::function<bool(EntityID)>> sEntityHasComponentFuncs;

	/// <summary>
	/// Registers all Component types for the HasComponent function. Unfortunately, they must manually registered.
	/// </summary>
	/// <typeparam name="...Component"></typeparam>
	template<typename... Component>
	void RegisterComponent() {
		AeonCore_INFO("Registered component types for Mono are:");
		// this lambda unpacks the variadic template types
		([]()
			{
				std::string_view typeName(typeid(Component).name());
				size_t pos = typeName.find_last_of(':'); // ignore namespace, care only about the struct name

				std::string structName(typeName.substr(pos + 1));
				std::string csharpTypeName(Scripting::sCoreNamespace + "." + structName);

				MonoType* monoType = mono_reflection_type_from_name(csharpTypeName.data(), Scripting::GetCoreAssemblyImage());
				if (monoType == nullptr)
				{
					AeonCore_ERROR("Could not find Mono component type {}", csharpTypeName);
					return;
				}

				sEntityHasComponentFuncs[monoType] = [](EntityID entityId) {
					return Coordinator::GetInstance().HasComponent<Component>(entityId);
				};

				AeonCore_INFO("\t{}", structName);
			}(), ...);
	}

	void ScriptAPI::RegisterFunctions() {
		AEON_REGISTER_SCRIPTING_FUNCTION(NativeLog_Log);
		AEON_REGISTER_SCRIPTING_FUNCTION(NativeLog_LogWarn);
		AEON_REGISTER_SCRIPTING_FUNCTION(NativeLog_LogError);

		AEON_REGISTER_SCRIPTING_FUNCTION(SceneManager_LoadScene);
		AEON_REGISTER_SCRIPTING_FUNCTION(SceneManager_LoadSceneThread);
		AEON_REGISTER_SCRIPTING_FUNCTION(SceneManager_RemoveSceneFromCache);

		#pragma region Audio
		AEON_REGISTER_SCRIPTING_FUNCTION(Audio_SetChannelTypeVolume);
		AEON_REGISTER_SCRIPTING_FUNCTION(Audio_GetChannelTypeVolume);
		AEON_REGISTER_SCRIPTING_FUNCTION(Audio_SetMasterVolume);
		AEON_REGISTER_SCRIPTING_FUNCTION(Audio_GetMasterVolume);
		AEON_REGISTER_SCRIPTING_FUNCTION(Audio_OnGamePause);
		AEON_REGISTER_SCRIPTING_FUNCTION(Audio_OnGameResume);
		AEON_REGISTER_SCRIPTING_FUNCTION(Audio_StopAllSounds);
		#pragma endregion

		AEON_REGISTER_SCRIPTING_FUNCTION(GameState_IsPaused);
		AEON_REGISTER_SCRIPTING_FUNCTION(GameState_SetIsPaused);
		AEON_REGISTER_SCRIPTING_FUNCTION(GameState_QuitGame);

		AEON_REGISTER_SCRIPTING_FUNCTION(Graphics_GetEnvironmentLightColor);
		AEON_REGISTER_SCRIPTING_FUNCTION(Graphics_SetEnvironmentLightColor);
		AEON_REGISTER_SCRIPTING_FUNCTION(Graphics_GetGamma);
		AEON_REGISTER_SCRIPTING_FUNCTION(Graphics_SetGamma);
		AEON_REGISTER_SCRIPTING_FUNCTION(Graphics_GetHDR);
		AEON_REGISTER_SCRIPTING_FUNCTION(Graphics_SetHDR);
		AEON_REGISTER_SCRIPTING_FUNCTION(Graphics_RequestSwitchCamera);

		AEON_REGISTER_SCRIPTING_FUNCTION(Time_GetDeltaTime);
		AEON_REGISTER_SCRIPTING_FUNCTION(Time_GetElapsedTime);

		AEON_REGISTER_SCRIPTING_FUNCTION(Behaviour_GetScriptInstance);

		#pragma region Builder
		AEON_REGISTER_SCRIPTING_FUNCTION(Builder_AddTransformComponent);
		AEON_REGISTER_SCRIPTING_FUNCTION(Builder_AddRigidBodyComponent);
		AEON_REGISTER_SCRIPTING_FUNCTION(Builder_AddColliderComponent);
		AEON_REGISTER_SCRIPTING_FUNCTION(Builder_ColliderComponent_SetShapeCube);
		AEON_REGISTER_SCRIPTING_FUNCTION(Builder_ColliderComponent_SetShapeSphere);
		AEON_REGISTER_SCRIPTING_FUNCTION(Builder_ColliderComponent_SetShapeCapsule);
		AEON_REGISTER_SCRIPTING_FUNCTION(Builder_AddRenderComponent);
		AEON_REGISTER_SCRIPTING_FUNCTION(Builder_AddUIComponent);
		AEON_REGISTER_SCRIPTING_FUNCTION(Builder_AddBehaviourComponent);
		AEON_REGISTER_SCRIPTING_FUNCTION(Builder_AddAudioComponent);
		AEON_REGISTER_SCRIPTING_FUNCTION(Builder_AddParticlesComponent);
		#pragma endregion

		#pragma region Entity
		AEON_REGISTER_SCRIPTING_FUNCTION(Entity_CreateEntity);
		AEON_REGISTER_SCRIPTING_FUNCTION(Entity_DestroyEntity);
		AEON_REGISTER_SCRIPTING_FUNCTION(Entity_EntityExists);
		AEON_REGISTER_SCRIPTING_FUNCTION(Entity_GetSceneFromEntityID);
		AEON_REGISTER_SCRIPTING_FUNCTION(Entity_GetEntityByName);
		AEON_REGISTER_SCRIPTING_FUNCTION(Entity_GetName);
		AEON_REGISTER_SCRIPTING_FUNCTION(Entity_SetName);
		AEON_REGISTER_SCRIPTING_FUNCTION(Entity_HasComponent);
		AEON_REGISTER_SCRIPTING_FUNCTION(Entity_GetParent);
		AEON_REGISTER_SCRIPTING_FUNCTION(Entity_SetParent);
		AEON_REGISTER_SCRIPTING_FUNCTION(Entity_GetChildren);
		#pragma endregion

		#pragma region AudioComponent
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_PlaySound);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_PauseSound);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_ResumeSound);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_StopSound);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_IsPaused);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_GetVolume);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_SetVolume);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_GetPitch);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_SetPitch);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_GetPan);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_SetPan);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_GetLoop);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_SetLoop);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_GetChannelType);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_SetChannelType);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_GetSoundFile);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_SetSoundFile);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_FadeIn);
		AEON_REGISTER_SCRIPTING_FUNCTION(AudioComponent_FadeOut);
		#pragma endregion

		#pragma region CameraComponent
		AEON_REGISTER_SCRIPTING_FUNCTION(CameraComponent_GetPosition);
		AEON_REGISTER_SCRIPTING_FUNCTION(CameraComponent_SetPosition);
		AEON_REGISTER_SCRIPTING_FUNCTION(CameraComponent_GetTarget);
		AEON_REGISTER_SCRIPTING_FUNCTION(CameraComponent_SetTarget);
		AEON_REGISTER_SCRIPTING_FUNCTION(CameraComponent_GetYaw);
		AEON_REGISTER_SCRIPTING_FUNCTION(CameraComponent_SetYaw);
		AEON_REGISTER_SCRIPTING_FUNCTION(CameraComponent_GetPitch);
		AEON_REGISTER_SCRIPTING_FUNCTION(CameraComponent_SetPitch);
		AEON_REGISTER_SCRIPTING_FUNCTION(CameraComponent_GetZoom);
		AEON_REGISTER_SCRIPTING_FUNCTION(CameraComponent_SetZoom);
		#pragma endregion

		#pragma region TransformComponent
		AEON_REGISTER_SCRIPTING_FUNCTION(TransformComponent_GetPosition);
		AEON_REGISTER_SCRIPTING_FUNCTION(TransformComponent_SetPosition);
		AEON_REGISTER_SCRIPTING_FUNCTION(TransformComponent_GetScale);
		AEON_REGISTER_SCRIPTING_FUNCTION(TransformComponent_SetScale);
		AEON_REGISTER_SCRIPTING_FUNCTION(TransformComponent_GetRotation);
		AEON_REGISTER_SCRIPTING_FUNCTION(TransformComponent_SetRotation);
		#pragma endregion

		#pragma region RigidBodyComponent
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_AddForce);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_AddImpulse);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_GetMass);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_SetMass);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_GetLinearDrag);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_SetLinearDrag);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_GetAngularDrag);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_SetAngularDrag);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_GetUseGravity);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_SetUseGravity);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_GetIsAutoCenterOfMass);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_SetIsAutoCenterOfMass);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_GetCenterOfMass);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_SetCenterOfMass);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_GetIsKinematic);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_SetIsKinematic);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_GetVelocity);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_SetVelocity);
		AEON_REGISTER_SCRIPTING_FUNCTION(RigidBodyComponent_Raycast);
		#pragma endregion

		#pragma region ColliderComponent
		AEON_REGISTER_SCRIPTING_FUNCTION(ColliderComponent_GetColliderType);
		AEON_REGISTER_SCRIPTING_FUNCTION(ColliderComponent_IsTrigger);
		AEON_REGISTER_SCRIPTING_FUNCTION(ColliderComponent_SetIsTrigger);
		AEON_REGISTER_SCRIPTING_FUNCTION(ColliderComponent_GetOffset);
		AEON_REGISTER_SCRIPTING_FUNCTION(ColliderComponent_SetOffset);
		AEON_REGISTER_SCRIPTING_FUNCTION(ColliderComponent_CubeGeom_GetHalfExtentSize);
		AEON_REGISTER_SCRIPTING_FUNCTION(ColliderComponent_CubeGeom_SetHalfExtentSize);
		AEON_REGISTER_SCRIPTING_FUNCTION(ColliderComponent_SphereGeom_GetRadius);
		AEON_REGISTER_SCRIPTING_FUNCTION(ColliderComponent_SphereGeom_SetRadius);
		AEON_REGISTER_SCRIPTING_FUNCTION(ColliderComponent_CapsuleGeom_GetHalfHeight);
		AEON_REGISTER_SCRIPTING_FUNCTION(ColliderComponent_CapsuleGeom_GetRadius);
		#pragma endregion

		#pragma region RenderComponent
		AEON_REGISTER_SCRIPTING_FUNCTION(RenderComponent_GetModelPath);
		AEON_REGISTER_SCRIPTING_FUNCTION(RenderComponent_SetModelPath);
		AEON_REGISTER_SCRIPTING_FUNCTION(RenderComponent_GetDiffuseTexturePath);
		AEON_REGISTER_SCRIPTING_FUNCTION(RenderComponent_SetDiffuseTexturePath);
		AEON_REGISTER_SCRIPTING_FUNCTION(RenderComponent_GetSpecularTexturePath);
		AEON_REGISTER_SCRIPTING_FUNCTION(RenderComponent_SetSpecularTexturePath);
		AEON_REGISTER_SCRIPTING_FUNCTION(RenderComponent_GetShininess);
		AEON_REGISTER_SCRIPTING_FUNCTION(RenderComponent_SetShininess);
		#pragma endregion

		#pragma region LightComponent
		AEON_REGISTER_SCRIPTING_FUNCTION(LightComponent_GetLightType);
		AEON_REGISTER_SCRIPTING_FUNCTION(LightComponent_SetLightType);
		AEON_REGISTER_SCRIPTING_FUNCTION(LightComponent_GetColor);
		AEON_REGISTER_SCRIPTING_FUNCTION(LightComponent_SetColor);
		AEON_REGISTER_SCRIPTING_FUNCTION(LightComponent_GetDirection);
		AEON_REGISTER_SCRIPTING_FUNCTION(LightComponent_SetDirection);
		AEON_REGISTER_SCRIPTING_FUNCTION(LightComponent_GetIntensity);
		AEON_REGISTER_SCRIPTING_FUNCTION(LightComponent_SetIntensity);
		AEON_REGISTER_SCRIPTING_FUNCTION(LightComponent_GetInnerCutoff);
		AEON_REGISTER_SCRIPTING_FUNCTION(LightComponent_SetInnerCutoff);
		AEON_REGISTER_SCRIPTING_FUNCTION(LightComponent_GetOuterCutoff);
		AEON_REGISTER_SCRIPTING_FUNCTION(LightComponent_SetOuterCutoff);
		#pragma endregion

		#pragma region ParticlesComponent
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_GetDiffuseTexture);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_SetDiffuseTexture);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_GetSpecularTexture);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_SetSpecularTexture);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_GetMaximumParticles);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_SetMaximumParticles);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_GetSpawnParticleRate);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_SetSpawnParticleRate);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_GetParticleLife);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_SetParticleLife);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_GetParticleGravity);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_SetParticleGravity);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_GetParticleOffset);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_SetParticleOffset);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_GetParticleMinVelocity);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_SetParticleMinVelocity);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_GetParticleMaxVelocity);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_SetParticleMaxVelocity);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_GetModelPath);
		AEON_REGISTER_SCRIPTING_FUNCTION(ParticlesComponent_SetModelPath);
		#pragma endregion

		#pragma region UIComponent
		AEON_REGISTER_SCRIPTING_FUNCTION(UIComponent_GetFormat);
		AEON_REGISTER_SCRIPTING_FUNCTION(UIComponent_SetFormat);
		AEON_REGISTER_SCRIPTING_FUNCTION(UIComponent_GetLayer);
		AEON_REGISTER_SCRIPTING_FUNCTION(UIComponent_SetLayer);
		AEON_REGISTER_SCRIPTING_FUNCTION(UIComponent_GetOrder);
		AEON_REGISTER_SCRIPTING_FUNCTION(UIComponent_SetOrder);
		AEON_REGISTER_SCRIPTING_FUNCTION(UIComponent_GetOpacity);
		AEON_REGISTER_SCRIPTING_FUNCTION(UIComponent_SetOpacity);
		AEON_REGISTER_SCRIPTING_FUNCTION(UIComponent_GetTexture);
		AEON_REGISTER_SCRIPTING_FUNCTION(UIComponent_SetTexture);
		AEON_REGISTER_SCRIPTING_FUNCTION(UIComponent_GetText);
		AEON_REGISTER_SCRIPTING_FUNCTION(UIComponent_SetText);
		AEON_REGISTER_SCRIPTING_FUNCTION(UIComponent_GetTextColor);
		AEON_REGISTER_SCRIPTING_FUNCTION(UIComponent_SetTextColor);
		#pragma endregion

		#pragma region Input
		AEON_REGISTER_SCRIPTING_FUNCTION(Input_GetKeyPressed);
		AEON_REGISTER_SCRIPTING_FUNCTION(Input_GetKeyHeld);
		AEON_REGISTER_SCRIPTING_FUNCTION(Input_GetMouseButtonHeld);
		AEON_REGISTER_SCRIPTING_FUNCTION(Input_RegisterMapping);
		AEON_REGISTER_SCRIPTING_FUNCTION(Input_IsMappingKeyPressed);
		AEON_REGISTER_SCRIPTING_FUNCTION(Input_IsMappingKeyHeld);
		AEON_REGISTER_SCRIPTING_FUNCTION(Input_IsMappingJoystickButtonPressed);
		AEON_REGISTER_SCRIPTING_FUNCTION(Input_GetMappingJoystickAxesValue);
		AEON_REGISTER_SCRIPTING_FUNCTION(Input_IsJoystickConnected);
		AEON_REGISTER_SCRIPTING_FUNCTION(Input_GetConnectedJoysticks);
		AEON_REGISTER_SCRIPTING_FUNCTION(Input_IsJoystickButtonPressed);
		AEON_REGISTER_SCRIPTING_FUNCTION(Input_GetJoystickAxesValue);
		#pragma endregion

		#pragma region Math
		AEON_REGISTER_SCRIPTING_FUNCTION(Math_Sqrt);
		AEON_REGISTER_SCRIPTING_FUNCTION(Math_Sin);
		AEON_REGISTER_SCRIPTING_FUNCTION(Math_Cos);
		AEON_REGISTER_SCRIPTING_FUNCTION(Math_Tan);
		AEON_REGISTER_SCRIPTING_FUNCTION(Math_ASin);
		AEON_REGISTER_SCRIPTING_FUNCTION(Math_ACos);
		AEON_REGISTER_SCRIPTING_FUNCTION(Math_ATan);
		#pragma endregion
	}

	void ScriptAPI::RegisterComponentTypes() {
		sEntityHasComponentFuncs.clear();

		RegisterComponent<AudioComponent, Camera, Transform, RigidBody, Collider, Render, Lighting, Particles, UI>();
	}

	#pragma region NativeLog
	void ScriptAPI::NativeLog_Log(MonoString* logMsg) {
		AeonCore_SCRIPT_LOG(ScriptUtils::MonoToCString(logMsg));
	}

	void ScriptAPI::NativeLog_LogWarn(MonoString* logMsg) {
		AeonCore_SCRIPT_WARN(ScriptUtils::MonoToCString(logMsg));
	}

	void ScriptAPI::NativeLog_LogError(MonoString* logMsg) {
		AeonCore_SCRIPT_ERROR(ScriptUtils::MonoToCString(logMsg));
	}
	#pragma endregion

	#pragma region SceneManager
	void ScriptAPI::SceneManager_LoadScene(MonoString* inFilename, bool shouldCache) {
		SceneManager::LoadScene(ScriptUtils::MonoToCString(inFilename), true, shouldCache);
	}

	void ScriptAPI::SceneManager_LoadSceneThread(MonoString* inFilename, bool shouldCache) {
		SceneManager::LoadSceneThread(ScriptUtils::MonoToCString(inFilename), shouldCache);
	}

	void ScriptAPI::SceneManager_RemoveSceneFromCache(MonoString* inFilename) {
		SceneManager::RemoveSceneFromCache(ScriptUtils::MonoToCString(inFilename));
	}
	#pragma endregion

	#pragma region Audio
	void ScriptAPI::Audio_SetChannelTypeVolume(int channelType, float volume) {
		FMODCoreClass::GetInstance().SetChannelTypeVolume(static_cast<AudioComponent::ChannelType>(channelType), volume);
	}

	float ScriptAPI::Audio_GetChannelTypeVolume(int channelType) {
		auto& fmodInst = FMODCoreClass::GetInstance();
		AudioComponent::ChannelType type = static_cast<AudioComponent::ChannelType>(channelType);
		switch(type) {
			case AudioComponent::ChannelType::SFX:
				return fmodInst.GetSFXChannelVolume();
			case AudioComponent::ChannelType::BGM:
				return fmodInst.GetBGMChannelVolume();
			case AudioComponent::ChannelType::Voice:
				return fmodInst.GetVOIChannelVolume();
		}
		return 0.0f;
	}

	void ScriptAPI::Audio_SetMasterVolume(float volume) {
		FMODCoreClass::GetInstance().SetChannelMasterVolume(volume);
	}
	float ScriptAPI::Audio_GetMasterVolume() {
		return FMODCoreClass::GetInstance().GetMasterVolume();
	}

	void ScriptAPI::Audio_OnGamePause() {
		std::dynamic_pointer_cast<AudioSystem>(Coordinator::GetInstance().GetSystem<AudioSystem>())->OnGamePause();
	}
	void ScriptAPI::Audio_OnGameResume() {
		std::dynamic_pointer_cast<AudioSystem>(Coordinator::GetInstance().GetSystem<AudioSystem>())->OnGameUnpause();
	}

	void ScriptAPI::Audio_StopAllSounds() {
        FMODCoreClass::GetInstance().StopAllChannels();
	}
	#pragma endregion

	#pragma region GameState
	bool ScriptAPI::GameState_IsPaused() {
		return Application::GetApplication().IsPaused();
	}

	void ScriptAPI::GameState_SetIsPaused(bool value) {
		Application::GetApplication().SetIsPaused(value);
	}

	void ScriptAPI::GameState_QuitGame() {
		Application::GetApplication().Close();
	}
	#pragma endregion

	#pragma region Graphics
	void ScriptAPI::Graphics_GetEnvironmentLightColor(glm::vec3* outLightColor) {
		*outLightColor = LightManager::GetInstance().GetAmbientLight();
	}

	void ScriptAPI::Graphics_SetEnvironmentLightColor(glm::vec3* inLightColor) {
		//Rendering::GetInstance().SetUniversalLightColor(*inLightColor);
		LightManager::GetInstance().SetAmbientLight(*inLightColor);
	}

	float ScriptAPI::Graphics_GetGamma() {
		return RenderSystem::mGamma;
	}
	void ScriptAPI::Graphics_SetGamma(float inGamma) {
		if (inGamma < 1.0f) inGamma = 1.0f;
		else if (inGamma > 10.0f) inGamma = 10.0f;
		RenderSystem::mGamma = inGamma;
	}

	bool ScriptAPI::Graphics_GetHDR() {
		return RenderSystem::HDR;
	}
	void ScriptAPI::Graphics_SetHDR(bool hasHdr) {
		RenderSystem::HDR = hasHdr;
	}

	void ScriptAPI::Graphics_RequestSwitchCamera(EntityID ownerCamera) {
		auto& camMgr = CameraManager::GetInstance();
		camMgr.SwitchGameCamera(ownerCamera);
		camMgr.SetRequestSwitch(true);
	}
	#pragma endregion

	#pragma region Time
	float ScriptAPI::Time_GetDeltaTime() {
		return TimerStep::GetInstance().get_delta_time();
	}

	float ScriptAPI::Time_GetElapsedTime() {
		return TimerStep::GetInstance().elapsed_time();
	}
	#pragma endregion

	MonoObject* ScriptAPI::Behaviour_GetScriptInstance(EntityID entityId, MonoString* inScriptFullName) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Behaviour>(entityId)) {
			std::string scriptName = ScriptUtils::MonoToCString(inScriptFullName);

			if (Scripting::DoesBehaviourClassExists(scriptName)) {
				Behaviour& bvrComp = coord.GetComponent<Behaviour>(entityId);
				auto resultItr = bvrComp.mBehaviourNames.find(scriptName);

				if (resultItr != bvrComp.mBehaviourNames.end()) {
					//return Scripting::GetBehaviourInstance(entityId, resultItr->second)->GetManagedInstance();
					return bvrComp.GetBehaviourScript(resultItr->second)->GetManagedInstance();
				}
			}
		}
		return nullptr;
	}

	#pragma region Builders
	void ScriptAPI::Builder_AddTransformComponent(EntityID entityId, glm::vec3* inPosVec, glm::vec3* inScaleVec, glm::vec3* inRotVec) {
		Coordinator& coord = Coordinator::GetInstance();

		if (coord.EntityExists(entityId) && !coord.HasComponent<Transform>(entityId)) {
			Transform transform;
			transform.SetPosition(*inPosVec);
			transform.SetScale(*inScaleVec);
			transform.SetEulerAngle(*inRotVec);
			coord.AddComponent(entityId, transform);
		}
	}

	void ScriptAPI::Builder_AddRigidBodyComponent(EntityID entityId, int type, float mass, float linearDrag, float angularDrag, bool useGravity, bool isKinematic) {
		Coordinator& coord = Coordinator::GetInstance();

		if (coord.EntityExists(entityId) && !coord.HasComponent<RigidBody>(entityId)) {
			RigidBody rigidBody;
			rigidBody.SetRigidBodyType(static_cast<RigidBody::RigidBodyType>(type));
			rigidBody.SetMass(mass);
			rigidBody.SetLinearDrag(linearDrag);
			rigidBody.SetAngularDrag(angularDrag);
			rigidBody.SetUseGravity(useGravity);
			rigidBody.SetIsKinematic(isKinematic);
			coord.AddComponent(entityId, rigidBody);
		}
	}

	Collider* ScriptAPI::Builder_AddColliderComponent(EntityID entityId, glm::vec3* inOffset, bool isTrigger) {
		Coordinator& coord = Coordinator::GetInstance();

		if (coord.EntityExists(entityId) && !coord.HasComponent<Collider>(entityId)) {
			Collider collider;
			collider.SetOffset(*inOffset);
			collider.SetTrigger(isTrigger);
			return &coord.AddComponent(entityId, collider);
		}

		return nullptr;
	}

	void ScriptAPI::Builder_ColliderComponent_SetShapeCube(Collider* collider, glm::vec3* inHalfSize) {
		if (collider != nullptr) {
			// means you want to change the collider type if curr one is diff from Cube
			if (collider->GetColliderType() != Collider::ColliderType::CUBE) {
				delete collider->GetGeometry(); // must deallocate the old geometry

				collider->SetColliderType(Collider::ColliderType::CUBE);
				collider->SetGeometry(new ACPhysics::CubeGeometry(*inHalfSize));
			} else {
				ACPhysics::CubeGeometry* cubeGeom = dynamic_cast<ACPhysics::CubeGeometry*>(collider->GetGeometry());
				cubeGeom->SetHalfExtents(*inHalfSize);
				
				collider->SetGeometry(cubeGeom);
			}
		}
	}
	void ScriptAPI::Builder_ColliderComponent_SetShapeSphere(Collider* collider, float radius) {
		if (collider != nullptr) {
			// means you want to change the collider type if curr one is diff from Sphere
			if (collider->GetColliderType() != Collider::ColliderType::SPHERE) {
				delete collider->GetGeometry(); // must deallocate the old geometry

				collider->SetColliderType(Collider::ColliderType::SPHERE);
				collider->SetGeometry(new ACPhysics::SphereGeometry(radius));
			} else {
				ACPhysics::SphereGeometry* sphereGeom = dynamic_cast<ACPhysics::SphereGeometry*>(collider->GetGeometry());
				sphereGeom->SetRadius(radius);	
				collider->SetGeometry(sphereGeom);
			}
		}
	}
	void ScriptAPI::Builder_ColliderComponent_SetShapeCapsule(Collider* collider, float radius, float halfHeight) {
		if (collider != nullptr) {
			// means you want to change the collider type if curr one is diff from Sphere
			if (collider->GetColliderType() != Collider::ColliderType::CAPSULE) {
				delete collider->GetGeometry(); // must deallocate the old geometry

				collider->SetColliderType(Collider::ColliderType::CAPSULE);
				collider->SetGeometry(new ACPhysics::CapsuleGeometry(radius, halfHeight));
			} else {
				ACPhysics::CapsuleGeometry* capsuleGeom = dynamic_cast<ACPhysics::CapsuleGeometry*>(collider->GetGeometry());
				capsuleGeom->SetRadius(radius);
				capsuleGeom->SetHalfHeight(halfHeight);
				
				collider->SetGeometry(capsuleGeom);
			}
		}
	}

	void ScriptAPI::Builder_AddRenderComponent(EntityID entityId, MonoString* inModelPath, MonoString* inDiffusePath, MonoString* inSpecularPath, float shininess) {
		Coordinator& coord = Coordinator::GetInstance();

		if (coord.EntityExists(entityId) && !coord.HasComponent<Render>(entityId)) {
			Render render(ScriptUtils::MonoToCString(inModelPath), coord.GetEntity(entityId)->GetEntityName(), entityId);

			render.SetDiffuseTexture(ScriptUtils::MonoToCString(inDiffusePath));
			render.SetShininess(shininess);

			std::string specPath = ScriptUtils::MonoToCString(inSpecularPath);
			if (!specPath.empty())
				render.SetSpecularTexture(specPath);

			coord.AddComponent(entityId, render);
		}
	}

	void ScriptAPI::Builder_AddUIComponent(EntityID entityId, int formatType, int layerType, UI::Order order, MonoString* inTexturePath, float opacity, MonoString* inText, glm::vec3* inTextColor) {
		Coordinator& coord = Coordinator::GetInstance();

		if (coord.EntityExists(entityId) && !coord.HasComponent<UI>(entityId)) {
			UI uiComp(entityId);

			uiComp.SetFormat(static_cast<UI::Format>(formatType));
			uiComp.SetLayer(static_cast<UI::Layer>(layerType));
			uiComp.SetOrder(order);
			uiComp.SetOpacity(opacity);

			switch (uiComp.GetFormat()) {
				case UI::Format::Image:
					{
						uiComp.SetTexturePath(ScriptUtils::MonoToCString(inTexturePath));
					}
					break;
				case UI::Format::Word:
					{
						uiComp.SetText(ScriptUtils::MonoToCString(inText));
						uiComp.SetColor(*inTextColor);
					}
					break;
				default: ;
			}

			coord.AddComponent(entityId, uiComp);
		}
	}

	void ScriptAPI::Builder_AddBehaviourComponent(EntityID entityId, MonoArray* inScriptNameArr) {
		Coordinator& coord = Coordinator::GetInstance();

		if (coord.EntityExists(entityId) && !coord.HasComponent<Behaviour>(entityId)) {
			Behaviour& bvrComp = coord.AddComponent(entityId, Behaviour(entityId));
			
			unsigned int length = static_cast<unsigned int>(mono_array_length(inScriptNameArr));
			//AeonCore_WARN("Builder_Behaviour - Length is {}", length);
			// Reading string array. Source: https://mono.github.io/mail-archives/mono-list/2015-November/051909.html
			for (unsigned int i = 0; i < length; i++) {
				MonoString* mono_scriptName = mono_array_get(inScriptNameArr, MonoString*, i);
				std::string scriptName = ScriptUtils::MonoToCString(mono_scriptName);

				//AeonCore_WARN("Builder_Behaviour: {} | {}", i, scriptName);

				bvrComp.AddBehaviourScript(scriptName);
			}
		}
	}

	void ScriptAPI::Builder_AddAudioComponent(EntityID entityId, int channelType, MonoString* inSoundFilepath, float volume, float pitch, float pan) {
		Coordinator& coord = Coordinator::GetInstance();

		if (coord.EntityExists(entityId) && !coord.HasComponent<AudioComponent>(entityId)) {
			AudioComponent& audioComp = coord.AddComponent(entityId, AudioComponent());
			audioComp.setChannelType(static_cast<AudioComponent::ChannelType>(channelType));
			audioComp.setSoundFile(ScriptUtils::MonoToCString(inSoundFilepath));
			audioComp.SetVolume(volume);
			audioComp.SetPitch(pitch);
			audioComp.SetPan(pan);
			audioComp.SetPlayOnAwake(false);
		}
	}

	void ScriptAPI::Builder_AddParticlesComponent(EntityID entityId, MonoString* diffuseTexture, MonoString* specularTexture, unsigned maxParticles, unsigned spawnRate, float initialLife, glm::vec3* gravity, glm::vec3* offset, glm::vec3* minVelocity, glm::vec3* maxVelocity, MonoString* modelPath) {
		Coordinator& coord = Coordinator::GetInstance();

		if (coord.EntityExists(entityId) && !coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.AddComponent(entityId, Particles(entityId, maxParticles, spawnRate));
			particlesComp.SetDiffuseTexture(ScriptUtils::MonoToCString(diffuseTexture));
			particlesComp.SetSpecularTexture(ScriptUtils::MonoToCString(specularTexture));
			particlesComp.SetParticleLife(initialLife);
			particlesComp.SetParticleGravity(*gravity);
			particlesComp.SetParticleOffset(*offset);
			particlesComp.SetParticleMinVelocity(*minVelocity);
			particlesComp.SetParticleMaxVelocity(*maxVelocity);
			particlesComp.SetModel(ScriptUtils::MonoToCString(modelPath));
		}
	}
	#pragma endregion

	#pragma region Entity
	EntityID ScriptAPI::Entity_CreateEntity(MonoString* inName) {
		return Coordinator::GetInstance().CreateEntity(ScriptUtils::MonoToCString(inName)).GetEntityID();
	}

	void ScriptAPI::Entity_DestroyEntity(EntityID entityID) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityID)) {
			Coordinator::GetInstance().DestroyEntity(entityID);
		}
	}

	bool ScriptAPI::Entity_EntityExists(EntityID entityID) {
		return Coordinator::GetInstance().EntityExists(entityID);
	}

	SceneID ScriptAPI::Entity_GetSceneFromEntityID(EntityID entityID) {
		return ECSUtils::ExtractSceneID(entityID);
	}

	EntityID ScriptAPI::Entity_GetEntityByName(MonoString* name, bool* outIsSuccess) {
		Entity* entity = Coordinator::GetInstance().GetEntityByName(ScriptUtils::MonoToCString(name));
		if (entity == nullptr)
		{
			*outIsSuccess = false;
			return 0;
		}

		*outIsSuccess = true;
		return entity->GetEntityID();
	}

	MonoString* ScriptAPI::Entity_GetName(EntityID entityID) {
		Entity* entity = Coordinator::GetInstance().GetEntity(entityID);

		return ScriptUtils::CToMonoString(entity != nullptr ? entity->GetEntityName().c_str() : "");
		//return ScriptUtils::CToMonoString("Cherno_a_new_religion");
	}

	void ScriptAPI::Entity_SetName(EntityID entityID, MonoString* name) {
		Entity* entity = Coordinator::GetInstance().GetEntity(entityID);

		if (entity != nullptr) {
			entity->SetEntityName(ScriptUtils::MonoToCString(name));
		}
		//std::cout << "Entity_SetName: name is " << ScriptUtils::MonoToCString(name) << std::endl;
	}

	/*MonoString* ScriptAPI::Entity_GetTag(EntityID entityID) {
		/*EntityID* entity = Coordinator::GetEntity(entityID);

		return ScriptUtils::CToMonoString(entity != nullptr ? entity->GetKeyTag().c_str() : "");*

		return ScriptUtils::CToMonoString("SomeEntityTag");
	}*/

	/*void ScriptAPI::Entity_SetTag(EntityID entityID, MonoString* tagStr) {
		Entity* entity = Coordinator::GetEntity(entityID);

		if (entity != nullptr) {
			entity->SetKeyTag(ScriptUtils::MonoToCString(tagStr, true));
		}
		std::cout << "Entity_SetTag: tag is " << ScriptUtils::MonoToCString(tagStr, true) << std::endl;
	}*/

	bool ScriptAPI::Entity_HasComponent(EntityID entityID, MonoReflectionType* componentType) {
		if (Coordinator::GetInstance().GetEntity(entityID) == nullptr) {
			return false;
		}

		MonoType* managedType = mono_reflection_type_get_type(componentType);

		auto itr = sEntityHasComponentFuncs.find(managedType);
		if (itr == sEntityHasComponentFuncs.end())
		{
			AeonCore_ASSERT(false, "Entity_HasComponent: {} does not exist in s_EntityHasComponentFuncs", mono_type_get_name(managedType));
			return false;
		}

		//std::cout << "Entity_HasComponent: componentType is " << mono_type_get_name(managedType) << std::endl;
		return (itr->second)(entityID);
	}

	EntityID ScriptAPI::Entity_GetParent(EntityID entityID, bool* outIsSuccess) {
		Entity* entity = Coordinator::GetInstance().GetEntity(entityID);
		if (entity != nullptr) {
			*outIsSuccess = true;
			return entity->GetParent();
		}
		*outIsSuccess = false;
		return 107;
	}

	void ScriptAPI::Entity_SetParent(EntityID parentID, EntityID childID) {
		Coordinator& _coordInst = Coordinator::GetInstance();
		if (_coordInst.GetEntity(parentID) != nullptr && _coordInst.GetEntity(childID) != nullptr) {
			_coordInst.ToChild(parentID, childID);
		}
	}

	MonoArray* ScriptAPI::Entity_GetChildren(EntityID entityID) {
		auto children = Coordinator::GetInstance().GetChildObjects(entityID);
		return ScriptUtils::CreateMonoArrayUnsignedInt(static_cast<int>(children.size()), children.data());

		//std::vector<EntityID> testChildren{ 5, 10, 20 };
		//return ScriptUtils::CreateMonoArrayUnsignedInt(testChildren.size(), testChildren.data());
	}
	#pragma endregion

	#pragma region Audio Component
	void ScriptAPI::AudioComponent_PlaySound(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			coord.GetComponent<AudioComponent>(entityId).PlaySound();
		}
	}
	void ScriptAPI::AudioComponent_PauseSound(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			coord.GetComponent<AudioComponent>(entityId).SetPause();
		}
	}
	void ScriptAPI::AudioComponent_ResumeSound(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			coord.GetComponent<AudioComponent>(entityId).SetResume();
		}
	}
	void ScriptAPI::AudioComponent_StopSound(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			coord.GetComponent<AudioComponent>(entityId).StopSound();
		}
	}

	bool ScriptAPI::AudioComponent_IsPaused(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			AudioComponent& audioComp = coord.GetComponent<AudioComponent>(entityId);
			return FMODCoreClass::GetInstance().GetAudioState(audioComp) == FMODCoreClass::AudioState::PAUSED;
		}
		return false;
	}

	float ScriptAPI::AudioComponent_GetVolume(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			return coord.GetComponent<AudioComponent>(entityId).GetVolume();
		}
		return 0.0f;
	}
	void ScriptAPI::AudioComponent_SetVolume(EntityID entityId, float volume) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			coord.GetComponent<AudioComponent>(entityId).SetVolume(volume);
		}
	}

	bool ScriptAPI::AudioComponent_GetLoop(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			return coord.GetComponent<AudioComponent>(entityId).GetLoop();
		}
		return false;
	}
	void ScriptAPI::AudioComponent_SetLoop(EntityID entityId, bool loop) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			coord.GetComponent<AudioComponent>(entityId).SetLoop(loop);
		}
	}

	int ScriptAPI::AudioComponent_GetChannelType(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			return static_cast<int>(coord.GetComponent<AudioComponent>(entityId).GetChannelType());
		}
		return static_cast<int>(AudioComponent::ChannelType::SFX);
	}
	void ScriptAPI::AudioComponent_SetChannelType(EntityID entityId, int channelType) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			coord.GetComponent<AudioComponent>(entityId).setChannelType(static_cast<AudioComponent::ChannelType>(channelType));
		}
	}

	float ScriptAPI::AudioComponent_GetPitch(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			return coord.GetComponent<AudioComponent>(entityId).GetPitch();
		}
		return 0.0f;
	}
	void ScriptAPI::AudioComponent_SetPitch(EntityID entityId, float pitch) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			coord.GetComponent<AudioComponent>(entityId).SetPitch(pitch);
		}
	}

	float ScriptAPI::AudioComponent_GetPan(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			return coord.GetComponent<AudioComponent>(entityId).GetPan();
		}
		return 0.0f;
	}
	void ScriptAPI::AudioComponent_SetPan(EntityID entityId, float pan) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			coord.GetComponent<AudioComponent>(entityId).SetPan(pan);
		}
	}

	MonoString* ScriptAPI::AudioComponent_GetSoundFile(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			return ScriptUtils::CToMonoString(coord.GetComponent<AudioComponent>(entityId).GetSoundFile().c_str());
		}
		return ScriptUtils::CToMonoString("");
	}
	void ScriptAPI::AudioComponent_SetSoundFile(EntityID entityId, MonoString* inSoundFile) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<AudioComponent>(entityId)) {
			coord.GetComponent<AudioComponent>(entityId).setSoundFile(ScriptUtils::MonoToCString(inSoundFile));
		}
	}

	void ScriptAPI::AudioComponent_FadeIn(EntityID entityID, float fadeDuration)
	{
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityID) && coord.HasComponent<AudioComponent>(entityID))
		{
			coord.GetComponent<AudioComponent>(entityID).FadeIn(fadeDuration);
		}
	}

	void ScriptAPI::AudioComponent_FadeOut(EntityID entityID, float fadeDuration)
	{
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityID) && coord.HasComponent<AudioComponent>(entityID))
		{
			coord.GetComponent<AudioComponent>(entityID).FadeOut(fadeDuration);
		}
	}
	#pragma endregion	

	#pragma region Camera Component
	void ScriptAPI::CameraComponent_GetPosition(EntityID entityId, glm::vec3* outPos) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Camera>(entityId)) {
			*outPos = coord.GetComponent<Camera>(entityId).GetCameraPosition();
		}
		else {
			*outPos = glm::vec3(0.0f);
		}
	}

	void ScriptAPI::CameraComponent_SetPosition(EntityID entityId, glm::vec3* inPos) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Camera>(entityId)) {
			coord.GetComponent<Camera>(entityId).SetCameraPosition(*inPos);
		}
	}

	void ScriptAPI::CameraComponent_GetTarget(EntityID entityId, glm::vec3* outTarget) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Camera>(entityId)) {
			*outTarget = coord.GetComponent<Camera>(entityId).GetCameraTarget();
		}
		else {
			*outTarget = glm::vec3(0.0f);
		}
	}

	void ScriptAPI::CameraComponent_SetTarget(EntityID entityId, glm::vec3* inTarget) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Camera>(entityId)) {
			coord.GetComponent<Camera>(entityId).SetCameraTarget(*inTarget);
		}
	}

	float ScriptAPI::CameraComponent_GetYaw(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Camera>(entityId)) {
			return coord.GetComponent<Camera>(entityId).GetCameraYaw();
		}
		return 0.0f;
	}

	void ScriptAPI::CameraComponent_SetYaw(EntityID entityId, float yaw) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Camera>(entityId)) {
			coord.GetComponent<Camera>(entityId).SetCameraYaw(yaw);
		}
	}

	float ScriptAPI::CameraComponent_GetPitch(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Camera>(entityId)) {
			return coord.GetComponent<Camera>(entityId).GetCameraPitch();
		}
		return 0.0f;
	}

	void ScriptAPI::CameraComponent_SetPitch(EntityID entityId, float pitch) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Camera>(entityId)) {
			coord.GetComponent<Camera>(entityId).SetCameraPitch(pitch);
		}
	}

	float ScriptAPI::CameraComponent_GetZoom(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Camera>(entityId)) {
			return coord.GetComponent<Camera>(entityId).GetCameraZoom();
		}
		return 0.0f;
	}

	void ScriptAPI::CameraComponent_SetZoom(EntityID entityId, float zoom) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Camera>(entityId)) {
			coord.GetComponent<Camera>(entityId).SetCameraZoom(zoom);
		}
	}
	#pragma endregion
	
	#pragma region Transform Component
	void ScriptAPI::TransformComponent_SetPosition(EntityID entityId, glm::vec3* inPos) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Transform>(entityId)) {
			coord.GetComponent<Transform>(entityId).SetPosition(*inPos);
		}
	}

	void ScriptAPI::TransformComponent_GetPosition(EntityID entityId, glm::vec3* outPos) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Transform>(entityId)) {
			*outPos = coord.GetComponent<Transform>(entityId).GetPosition();
		}
		else {
			*outPos = glm::vec3(0.0f);
		}
	}

	void ScriptAPI::TransformComponent_SetScale(EntityID entityId, glm::vec3* inScale) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Transform>(entityId)) {
			coord.GetComponent<Transform>(entityId).SetScale(*inScale);
		}
	}

	void ScriptAPI::TransformComponent_GetScale(EntityID entityId, glm::vec3* outScale) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Transform>(entityId)) {
			*outScale = coord.GetComponent<Transform>(entityId).GetScale();
		}
		else {
			*outScale = glm::vec3(0.0f);
		}
	}

	void ScriptAPI::TransformComponent_GetRotation(EntityID entityId, glm::vec3* outRotation) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Transform>(entityId)) {
			*outRotation = coord.GetComponent<Transform>(entityId).GetEulerAngle();
		} else {
			*outRotation = glm::vec3(0.0f);
		}
	}
	void ScriptAPI::TransformComponent_SetRotation(EntityID entityId, glm::vec3* inRotation) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Transform>(entityId)) {
			coord.GetComponent<Transform>(entityId).SetEulerAngle(*inRotation);
		}
	}
	#pragma endregion

	#pragma region RigidBody Component
	void ScriptAPI::RigidBodyComponent_AddForce(std::uint32_t entityID, glm::vec3* inForce) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityID) && coord.HasComponent<RigidBody>(entityID)) {
			AeonCore_INFO("ScriptAPI, AddForce");
			coord.GetComponent<RigidBody>(entityID).AddForce(*inForce);
		}
	}

	void ScriptAPI::RigidBodyComponent_AddImpulse(EntityID entityID, glm::vec3* inForce) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityID) && coord.HasComponent<RigidBody>(entityID)) {
			AeonCore_INFO("ScriptAPI, AddImpulse");
			coord.GetComponent<RigidBody>(entityID).AddImpulse(*inForce);
		}
	}

	float ScriptAPI::RigidBodyComponent_GetMass(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId)) {
			return coord.GetComponent<RigidBody>(entityId).GetMass();
		}
		return 0.0f;
	}
	void ScriptAPI::RigidBodyComponent_SetMass(EntityID entityId, float mass) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId)) {
			coord.GetComponent<RigidBody>(entityId).SetMass(mass);
		}
	}

	float ScriptAPI::RigidBodyComponent_GetLinearDrag(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId)) {
			return coord.GetComponent<RigidBody>(entityId).GetLinearDrag();
		}
		return 0.0f;
	}
	void ScriptAPI::RigidBodyComponent_SetLinearDrag(EntityID entityId, float linearDrag) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId)) {
			coord.GetComponent<RigidBody>(entityId).SetLinearDrag(linearDrag);
		}
	}

	float ScriptAPI::RigidBodyComponent_GetAngularDrag(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId)) {
			return coord.GetComponent<RigidBody>(entityId).GetAngularDrag();
		}
		return 0.0f;
	}
	void ScriptAPI::RigidBodyComponent_SetAngularDrag(EntityID entityId, float angularDrag) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId)) {
			coord.GetComponent<RigidBody>(entityId).SetAngularDrag(angularDrag);
		}
	}

	bool ScriptAPI::RigidBodyComponent_GetUseGravity(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId)) {
			return coord.GetComponent<RigidBody>(entityId).GetUseGravity();
		}
		return false;
	}
	void ScriptAPI::RigidBodyComponent_SetUseGravity(EntityID entityId, bool useGravity) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId)) {
			coord.GetComponent<RigidBody>(entityId).SetUseGravity(useGravity);
		}
	}

	bool ScriptAPI::RigidBodyComponent_GetIsAutoCenterOfMass(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId)) {
			return coord.GetComponent<RigidBody>(entityId).GetIsAutoCenterOfMass();
		}
		return false;
	}
	void ScriptAPI::RigidBodyComponent_SetIsAutoCenterOfMass(EntityID entityId, bool isAutoCenterOfMass) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId)) {
			coord.GetComponent<RigidBody>(entityId).SetIsAutoCenterOfMass(isAutoCenterOfMass);
		}
	}

	void ScriptAPI::RigidBodyComponent_GetCenterOfMass(EntityID entityId, glm::vec3* outMassVec) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId)) {
			*outMassVec = coord.GetComponent<RigidBody>(entityId).GetCenterOfMass();
		}
		else {
			*outMassVec = glm::vec3(0.0f);
		}
	}
	void ScriptAPI::RigidBodyComponent_SetCenterOfMass(EntityID entityId, glm::vec3* inMassVec) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId)) {
			coord.GetComponent<RigidBody>(entityId).SetCenterOfMass(*inMassVec);
		}
	}

	bool ScriptAPI::RigidBodyComponent_GetIsKinematic(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId)) {
			return coord.GetComponent<RigidBody>(entityId).GetIsKinematic();
		}
		return false;
	}
	void ScriptAPI::RigidBodyComponent_SetIsKinematic(EntityID entityId, bool isKinematic) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId)) {
			coord.GetComponent<RigidBody>(entityId).SetIsKinematic(isKinematic);
		}
		//AeonCore_INFO("RigidBodyComponent_SetIsKinematic: {}", isKinematic);
	}

	void ScriptAPI::RigidBodyComponent_GetVelocity(EntityID entityId, glm::vec3* outVelocity) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId)) {
			RigidBody& rb = coord.GetComponent<RigidBody>(entityId);
			if (rb.GetRigidBodyType() == RigidBody::RigidBodyType::DYNAMIC)
				*outVelocity = rb.GetVelocity();
			else
				AeonCore_WARN("ScriptAPI: Cannot get velocity as rigidbody is static");
		}
		else {
			*outVelocity = glm::vec3(0.0f);
		}
	}
	void ScriptAPI::RigidBodyComponent_SetVelocity(EntityID entityId, glm::vec3* inVelocity) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId)) {
			RigidBody& rb = coord.GetComponent<RigidBody>(entityId);
			if (rb.GetRigidBodyType() == RigidBody::RigidBodyType::DYNAMIC)
				rb.SetVelocity(*inVelocity);
			else
				AeonCore_WARN("ScriptAPI: Cannot set velocity as rigidbody is static");
		}
	}

	bool ScriptAPI::RigidBodyComponent_Raycast(EntityID entityId, const glm::vec3 &origin, const glm::vec3 &direction, float maxDistance, RigidBody::RaycastResult* result)
	{
		Coordinator &coord = Coordinator::GetInstance();
		if ( coord.EntityExists(entityId) && coord.HasComponent<RigidBody>(entityId) )
		{
			return coord.GetComponent<RigidBody>(entityId).Raycast(origin, direction, maxDistance,*result);
		}
		return false;
	}
	#pragma endregion

	#pragma region Collider Component
	int ScriptAPI::ColliderComponent_GetColliderType(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Collider>(entityId)) {
			return static_cast<int>(coord.GetComponent<Collider>(entityId).GetColliderType());
		}
		return static_cast<int>(Collider::ColliderType::NONE);
	}

	bool ScriptAPI::ColliderComponent_IsTrigger(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Collider>(entityId)) {
			return coord.GetComponent<Collider>(entityId).IsTrigger();
		}
		return false;
	}

	void ScriptAPI::ColliderComponent_SetIsTrigger(EntityID entityId, bool inIsTrigger) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Collider>(entityId)) {
			coord.GetComponent<Collider>(entityId).SetTrigger(inIsTrigger);
		}
	}

	void ScriptAPI::ColliderComponent_GetOffset(EntityID entityId, glm::vec3* outOffset) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Collider>(entityId)) {
			*outOffset = coord.GetComponent<Collider>(entityId).GetOffset();
		}
		else {
			*outOffset = glm::vec3(0.0f);
		}
	}

	void ScriptAPI::ColliderComponent_SetOffset(EntityID entityId, glm::vec3* inOffset) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Collider>(entityId)) {
			coord.GetComponent<Collider>(entityId).SetOffset(*inOffset);
		}
	}

	void ScriptAPI::ColliderComponent_CubeGeom_GetHalfExtentSize(EntityID entityId, glm::vec3* outHalfExtentSize) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Collider>(entityId)) {
			auto& collider = coord.GetComponent<Collider>(entityId);
			auto* geom = collider.GetGeometry();

			if (collider.GetColliderType() == Collider::ColliderType::CUBE && geom != nullptr) {
				auto* cubeGeom = dynamic_cast<ACPhysics::CubeGeometry*>(geom);
				*outHalfExtentSize = cubeGeom->GetHalfExtents();
				return;
			}
		}
		*outHalfExtentSize = glm::vec3(0.0f);
	}

	void ScriptAPI::ColliderComponent_CubeGeom_SetHalfExtentSize(EntityID entityId, glm::vec3* inHalfExtentSize) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Collider>(entityId)) {
			auto& collider = coord.GetComponent<Collider>(entityId);
			auto* geom = collider.GetGeometry();

			if (collider.GetColliderType() == Collider::ColliderType::CUBE && geom != nullptr) {
				auto* cubeGeom = dynamic_cast<ACPhysics::CubeGeometry*>(geom);
				cubeGeom->SetHalfExtents(*inHalfExtentSize);
				collider.SetGeometry(geom);
			}
		}
	}

	float ScriptAPI::ColliderComponent_SphereGeom_GetRadius(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Collider>(entityId)) {
			auto& collider = coord.GetComponent<Collider>(entityId);
			auto* geom = collider.GetGeometry();

			if (collider.GetColliderType() == Collider::ColliderType::SPHERE && geom != nullptr) {
				auto* sphereGeom = dynamic_cast<ACPhysics::SphereGeometry*>(geom);
				return sphereGeom->GetRadius();
			}
		}
		return 0.0f;
	}

	void ScriptAPI::ColliderComponent_SphereGeom_SetRadius(EntityID entityId, float radius) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Collider>(entityId)) {
			auto& collider = coord.GetComponent<Collider>(entityId);
			auto* geom = collider.GetGeometry();

			if (collider.GetColliderType() == Collider::ColliderType::SPHERE && geom != nullptr) {
				auto* sphereGeom = dynamic_cast<ACPhysics::SphereGeometry*>(geom);
				sphereGeom->SetRadius(radius);
				collider.SetGeometry(geom);
			}
		}
	}

	float ScriptAPI::ColliderComponent_CapsuleGeom_GetHalfHeight(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Collider>(entityId)) {
			auto& collider = coord.GetComponent<Collider>(entityId);
			auto* geom = collider.GetGeometry();

			if (collider.GetColliderType() == Collider::ColliderType::CAPSULE && geom != nullptr) {
				auto* capGeom = dynamic_cast<ACPhysics::CapsuleGeometry*>(geom);
				return capGeom->GetHalfHeight();
			}
		}
		return 0.0f;
	}

	float ScriptAPI::ColliderComponent_CapsuleGeom_GetRadius(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Collider>(entityId)) {
			auto& collider = coord.GetComponent<Collider>(entityId);
			auto* geom = collider.GetGeometry();

			if (collider.GetColliderType() == Collider::ColliderType::CAPSULE && geom != nullptr) {
				auto* capgeom = dynamic_cast<ACPhysics::CapsuleGeometry*>(geom);
				return capgeom->GetRadius();
			}
		}
		return 0.0f;
	}
	#pragma endregion

	#pragma region Render Component
	MonoString* ScriptAPI::RenderComponent_GetModelPath(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Render>(entityId)) {
			return ScriptUtils::CToMonoString(coord.GetComponent<Render>(entityId).GetModelPath().c_str());
		}
		return ScriptUtils::CToMonoString("");
	}

	void ScriptAPI::RenderComponent_SetModelPath(EntityID entityId, MonoString* modelPath) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Render>(entityId)) {
			coord.GetComponent<Render>(entityId).SetModel(ScriptUtils::MonoToCString(modelPath));
		}
	}

	MonoString* ScriptAPI::RenderComponent_GetDiffuseTexturePath(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Render>(entityId)) {
			return ScriptUtils::CToMonoString(coord.GetComponent<Render>(entityId).GetDiffuseTexture().c_str());
		}
		return ScriptUtils::CToMonoString("");
	}

	void ScriptAPI::RenderComponent_SetDiffuseTexturePath(EntityID entityId, MonoString* diffusePath) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Render>(entityId)) {
			coord.GetComponent<Render>(entityId).SetDiffuseTexture(ScriptUtils::MonoToCString(diffusePath));
		}
	}

	MonoString* ScriptAPI::RenderComponent_GetSpecularTexturePath(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Render>(entityId)) {
			return ScriptUtils::CToMonoString(coord.GetComponent<Render>(entityId).GetSpecularTexture().c_str());
		}
		return ScriptUtils::CToMonoString("");
	}

	void ScriptAPI::RenderComponent_SetSpecularTexturePath(EntityID entityId, MonoString* specularPath) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Render>(entityId)) {
			coord.GetComponent<Render>(entityId).SetSpecularTexture(ScriptUtils::MonoToCString(specularPath));
		}
	}

	float ScriptAPI::RenderComponent_GetShininess(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Render>(entityId)) {
			coord.GetComponent<Render>(entityId).GetShininess();
		}
		return 0.0f;
	}

	void ScriptAPI::RenderComponent_SetShininess(EntityID entityId, float shininess) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Render>(entityId)) {
			coord.GetComponent<Render>(entityId).SetShininess(shininess);
		}
	}
	#pragma endregion

	#pragma region Light Component
	int ScriptAPI::LightComponent_GetLightType(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Lighting>(entityId)) {
			return static_cast<int>(coord.GetComponent<Lighting>(entityId).GetLightType());
		}
		return static_cast<int>(Lighting::LightType::SPOTLIGHT);
	}

	void ScriptAPI::LightComponent_SetLightType(EntityID entityId, int inLightType) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Lighting>(entityId)) {
			coord.GetComponent<Lighting>(entityId).SetLightType(static_cast<Lighting::LightType>(inLightType));
		}
	}

	void ScriptAPI::LightComponent_GetColor(EntityID entityId, glm::vec3* outColor) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Lighting>(entityId)) {
			Lighting& lightComp = coord.GetComponent<Lighting>(entityId);
			*outColor = lightComp.GetColor();
		}
		else {
			*outColor = glm::vec3(0.0f);
		}
	}

	void ScriptAPI::LightComponent_SetColor(EntityID entityId, glm::vec3* inColor) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Lighting>(entityId)) {
			coord.GetComponent<Lighting>(entityId).SetColor(*inColor);
		}
	}

	void ScriptAPI::LightComponent_GetDirection(EntityID entityId, glm::vec3* outDir) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Lighting>(entityId)) {
			Lighting& lightComp = coord.GetComponent<Lighting>(entityId);
			*outDir = lightComp.GetLightDirection();
		}
		else {
			*outDir = glm::vec3(0.0f);
		}
	}

	void ScriptAPI::LightComponent_SetDirection(EntityID entityId, glm::vec3* inDir) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Lighting>(entityId)) {
			coord.GetComponent<Lighting>(entityId).SetLightDirection(*inDir);
		}
	}

	float ScriptAPI::LightComponent_GetIntensity(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Lighting>(entityId)) {
			return coord.GetComponent<Lighting>(entityId).GetLightIntensity();
		}
		return 0.0f;
	}

	void ScriptAPI::LightComponent_SetIntensity(EntityID entityId, float intensity) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Lighting>(entityId)) {
			coord.GetComponent<Lighting>(entityId).SetLightIntensity(static_cast<short>(intensity));
		}
	}

	float ScriptAPI::LightComponent_GetInnerCutoff(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Lighting>(entityId)) {
			return coord.GetComponent<Lighting>(entityId).GetLightCutoff();
		}
		return 0.0f;
	}

	void ScriptAPI::LightComponent_SetInnerCutoff(EntityID entityId, float innerCutoff) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Lighting>(entityId)) {
			coord.GetComponent<Lighting>(entityId).SetLightCutoff(static_cast<short>(innerCutoff));
		}
	}

	float ScriptAPI::LightComponent_GetOuterCutoff(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Lighting>(entityId)) {
			return coord.GetComponent<Lighting>(entityId).GetLightOuterCutoff();
		}
		return 0.0f;
	}

	void ScriptAPI::LightComponent_SetOuterCutoff(EntityID entityId, float outerCutoff) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Lighting>(entityId)) {
			coord.GetComponent<Lighting>(entityId).SetLightOuterCutoff(static_cast<short>(outerCutoff));
		}
	}
	#pragma endregion

	#pragma region Particles Component
	MonoString* ScriptAPI::ParticlesComponent_GetDiffuseTexture(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			return ScriptUtils::CToMonoString(particlesComp.GetDiffuseTexture().c_str());
		}
		return ScriptUtils::CToMonoString("");
	}
	void ScriptAPI::ParticlesComponent_SetDiffuseTexture(EntityID entityId, MonoString* diffuseTexture) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			particlesComp.SetDiffuseTexture(ScriptUtils::MonoToCString(diffuseTexture));
		}
	}

	MonoString* ScriptAPI::ParticlesComponent_GetSpecularTexture(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			return ScriptUtils::CToMonoString(particlesComp.GetSpecularTexture().c_str());
		}
		return ScriptUtils::CToMonoString("");
	}
	void ScriptAPI::ParticlesComponent_SetSpecularTexture(EntityID entityId, MonoString* specularTexture) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			particlesComp.SetSpecularTexture(ScriptUtils::MonoToCString(specularTexture));
		}
	}

	unsigned int ScriptAPI::ParticlesComponent_GetMaximumParticles(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			return particlesComp.GetMaximumParticles();
		}
		return 0;
	}
	void ScriptAPI::ParticlesComponent_SetMaximumParticles(EntityID entityId, unsigned int maxParticles) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			particlesComp.SetMaximumParticles(maxParticles);
		}
	}

	unsigned int ScriptAPI::ParticlesComponent_GetSpawnParticleRate(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			return particlesComp.GetSpawnParticleRate();
		}
		return 0;
	}
	void ScriptAPI::ParticlesComponent_SetSpawnParticleRate(EntityID entityId, unsigned spawnRate) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			particlesComp.SetSpawnParticleRate(spawnRate);
		}
	}

	float ScriptAPI::ParticlesComponent_GetParticleLife(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			return particlesComp.GetParticleLife();
		}
		return 0.0f;
	}
	void ScriptAPI::ParticlesComponent_SetParticleLife(EntityID entityId, float particleLife) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			particlesComp.SetParticleLife(particleLife);
		}
	}

	void ScriptAPI::ParticlesComponent_GetParticleGravity(EntityID entityId, glm::vec3* outGravity) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			*outGravity = particlesComp.GetParticleGravity();
		}
		else {
			*outGravity = glm::vec3(0.0f);
		}
	}
	void ScriptAPI::ParticlesComponent_SetParticleGravity(EntityID entityId, glm::vec3* inGravity) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			particlesComp.SetParticleGravity(*inGravity);
		}
	}

	void ScriptAPI::ParticlesComponent_GetParticleOffset(EntityID entityId, glm::vec3* outOffset) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			*outOffset = particlesComp.GetParticleOffset();
		}
		else {
			*outOffset = glm::vec3(0.0f);
		}
	}
	void ScriptAPI::ParticlesComponent_SetParticleOffset(EntityID entityId, glm::vec3* inOffset) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			particlesComp.SetParticleOffset(*inOffset);
		}
	}

	void ScriptAPI::ParticlesComponent_GetParticleMinVelocity(EntityID entityId, glm::vec3* outMinVelocity) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			*outMinVelocity = particlesComp.GetMinVelocity();
		}
		else {
			*outMinVelocity = glm::vec3(0.0f);
		}
	}
	void ScriptAPI::ParticlesComponent_SetParticleMinVelocity(EntityID entityId, glm::vec3* inMinVelocity) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			particlesComp.SetParticleMinVelocity(*inMinVelocity);
		}
	}

	void ScriptAPI::ParticlesComponent_GetParticleMaxVelocity(EntityID entityId, glm::vec3* outMaxVelocity) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			*outMaxVelocity = particlesComp.GetMaxVelocity();
		}
		else {
			*outMaxVelocity = glm::vec3(0.0f);
		}
	}
	void ScriptAPI::ParticlesComponent_SetParticleMaxVelocity(EntityID entityId, glm::vec3* inMaxVelocity) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			particlesComp.SetParticleMaxVelocity(*inMaxVelocity);
		}
	}

	MonoString* ScriptAPI::ParticlesComponent_GetModelPath(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			return ScriptUtils::CToMonoString(particlesComp.GetModelPath().c_str());
		}
		return ScriptUtils::CToMonoString("");
	}
	void ScriptAPI::ParticlesComponent_SetModelPath(EntityID entityId, MonoString* modelPath) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<Particles>(entityId)) {
			Particles& particlesComp = coord.GetComponent<Particles>(entityId);
			particlesComp.SetModel(ScriptUtils::MonoToCString(modelPath));
		}
	}
	#pragma endregion

	#pragma region UI Component
	int ScriptAPI::UIComponent_GetFormat(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<UI>(entityId)) {
			UI& uiComp = coord.GetComponent<UI>(entityId);
			return static_cast<int>(uiComp.GetFormat());
		}
		return static_cast<int>(UI::Format::Image);
	}

	void ScriptAPI::UIComponent_SetFormat(EntityID entityId, int inFormat) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<UI>(entityId)) {
			UI& uiComp = coord.GetComponent<UI>(entityId);
			uiComp.SetFormat(static_cast<UI::Format>(inFormat));
		}
	}

	int ScriptAPI::UIComponent_GetLayer(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<UI>(entityId)) {
			UI& uiComp = coord.GetComponent<UI>(entityId);
			return static_cast<int>(uiComp.GetLayer());
		}
		return static_cast<int>(UI::Layer::Default);
	}

	void ScriptAPI::UIComponent_SetLayer(EntityID entityId, int inLayer) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<UI>(entityId)) {
			UI& uiComp = coord.GetComponent<UI>(entityId);
			uiComp.SetLayer(static_cast<UI::Layer>(inLayer));
		}
	}

	UI::Order ScriptAPI::UIComponent_GetOrder(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<UI>(entityId)) {
			return coord.GetComponent<UI>(entityId).GetOrder();
		}
		return 0;
	}

	void ScriptAPI::UIComponent_SetOrder(EntityID entityId, UI::Order inOrder) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<UI>(entityId)) {
			coord.GetComponent<UI>(entityId).SetOrder(inOrder);
		}
	}

	float ScriptAPI::UIComponent_GetOpacity(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<UI>(entityId)) {
			return coord.GetComponent<UI>(entityId).GetOpacity();
		}
		return 0.0f;
	}

	void ScriptAPI::UIComponent_SetOpacity(EntityID entityId, float inOpacity) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<UI>(entityId)) {
			coord.GetComponent<UI>(entityId).SetOpacity(inOpacity);
		}
	}

	MonoString* ScriptAPI::UIComponent_GetTexture(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<UI>(entityId)) {
			UI& uiComp = coord.GetComponent<UI>(entityId);
			return ScriptUtils::CToMonoString(uiComp.GetTexturePath().c_str());
		}
		return ScriptUtils::CToMonoString("");
	}

	void ScriptAPI::UIComponent_SetTexture(EntityID entityId, MonoString* inTexturePath) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<UI>(entityId)) {
			UI& uiComp = coord.GetComponent<UI>(entityId);
			uiComp.SetTexturePath(ScriptUtils::MonoToCString(inTexturePath));
		}
	}

	MonoString* ScriptAPI::UIComponent_GetText(EntityID entityId) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<UI>(entityId)) {
			UI& uiComp = coord.GetComponent<UI>(entityId);
			return ScriptUtils::CToMonoString(uiComp.GetText().c_str());
		}
		return ScriptUtils::CToMonoString("");
	}

	void ScriptAPI::UIComponent_SetText(EntityID entityId, MonoString* inText) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<UI>(entityId)) {
			UI& uiComp = coord.GetComponent<UI>(entityId);
			uiComp.SetText(ScriptUtils::MonoToCString(inText));
		}
	}

	void ScriptAPI::UIComponent_GetTextColor(EntityID entityId, glm::vec3* outColor) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<UI>(entityId)) {
			UI& uiComp = coord.GetComponent<UI>(entityId);
			*outColor = uiComp.GetColor();
		}
		else {
			*outColor = glm::vec3(0.0f);
		}
	}

	void ScriptAPI::UIComponent_SetTextColor(EntityID entityId, glm::vec3* inColor) {
		Coordinator& coord = Coordinator::GetInstance();
		if (coord.EntityExists(entityId) && coord.HasComponent<UI>(entityId)) {
			UI& uiComp = coord.GetComponent<UI>(entityId);
			uiComp.SetColor(*inColor);
		}
	}
	#pragma endregion

	#pragma region Input
	bool ScriptAPI::Input_GetKeyPressed(int key) {
		return Input::IsKeyTriggered(key);
	}

	bool ScriptAPI::Input_GetKeyHeld(int key) {
		return Input::IsKeyHeld(key);
	}

	bool ScriptAPI::Input_GetMouseButtonHeld(int button) {
		return Input::IsMouseButtonHeld(button);
	}

	void ScriptAPI::Input_RegisterMapping(MonoString* name, int _kbmInputType, int _keycode, int _joystickInput) {
		Input::GetInstance().RegisterMapping(ScriptUtils::MonoToCString(name), KeyMouseInput{static_cast<KBMInputType>(_kbmInputType), _keycode}, static_cast<JoyStickInput>(_joystickInput));
	}

	bool ScriptAPI::Input_IsMappingKeyPressed(MonoString* name) {
		return Input::GetInstance().IsMappingKeyPressed(ScriptUtils::MonoToCString(name));
	}

	bool ScriptAPI::Input_IsMappingKeyHeld(MonoString* name) {
		return Input::GetInstance().IsMappingKeyHeld(ScriptUtils::MonoToCString(name));
	}

	bool ScriptAPI::Input_IsMappingJoystickButtonPressed(MonoString* name, int jid) {
		return Input::GetInstance().IsMappingJoyStickButtonPressed(ScriptUtils::MonoToCString(name), jid);
	}

	float ScriptAPI::Input_GetMappingJoystickAxesValue(MonoString* name, int jid) {
		return Input::GetInstance().GetMappingJoystickAxesValue(ScriptUtils::MonoToCString(name), jid);
	}

	bool ScriptAPI::Input_IsJoystickConnected(int jid) {
		return Input::GetInstance().IsJoyStickConnected(jid);
	}

	MonoArray* ScriptAPI::Input_GetConnectedJoysticks() {
		auto& joyInfoMap = Input::GetInstance().mJoyStickInfoMap;
		std::vector<int> data;
		data.reserve(joyInfoMap.size());
		for (auto& joyPair : joyInfoMap) {
			data.push_back(joyPair.first);
		}
		return ScriptUtils::CreateMonoArrayInt(static_cast<int>(joyInfoMap.size()), data.data());
	}

	bool ScriptAPI::Input_IsJoystickButtonPressed(int jid, int joystickInput) {
		return Input::GetInstance().IsJoystickButtonPressed(jid, static_cast<JoyStickInput>(joystickInput));
	}

	float ScriptAPI::Input_GetJoystickAxesValue(int jid, int joystickInput) {
		return Input::GetInstance().GetJoystickAxesValue(jid, static_cast<JoyStickInput>(joystickInput));
	}
	#pragma endregion

	#pragma region Math
	float ScriptAPI::Math_Sqrt(float value) {
		return sqrtf(value);
	}

	float ScriptAPI::Math_Sin(float value) {
		return sinf(value);
	}

	float ScriptAPI::Math_Cos(float value) {
		return cosf(value);
	}

	float ScriptAPI::Math_Tan(float value) {
		return tanf(value);
	}

	float ScriptAPI::Math_ASin(float value) {
		return asinf(value);
	}

	float ScriptAPI::Math_ACos(float value) {
		return acosf(value);
	}

	float ScriptAPI::Math_ATan(float value) {
		return atanf(value);
	}

	float ScriptAPI::Math_ATan2(float y, float x)
	{
		return std::atan2(y, x); 
	}
	#pragma endregion

}
