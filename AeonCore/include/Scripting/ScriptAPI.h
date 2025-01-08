#pragma once
/*!***************************************************************************
	\file            ScriptAPI.h
	\author          Lam En Qing
	\par             enqing.lam\@digipen.edu
	\brief           This file contains the function declaration to register
					 functions and component types. Internal call functions
					 are declared here.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "ECS/Architecture/Coordinator.hpp"
#include "ECS/Components/UI.h"
#include "ECS/Components/RigidBody.h"

#include <mono/metadata/reflection.h>

#include "ECS/Components/Collider.h"

namespace AeonCore {
	namespace ScriptAPI {
		/// <summary>
		/// Register C++ functions that has a C# counterpart in the InternalCalls.cs, function names must exactly match on both sides
		/// </summary>
		void RegisterFunctions();
		void RegisterComponentTypes();

		void NativeLog_Log(MonoString* logMsg);
		void NativeLog_LogWarn(MonoString* logMsg);
		void NativeLog_LogError(MonoString* logMsg);

		void SceneManager_LoadScene(MonoString* inFilename, bool shouldCache);
		void SceneManager_LoadSceneThread(MonoString* inFilename, bool shouldCache);
		void SceneManager_RemoveSceneFromCache(MonoString* inFilename);

		void  Audio_SetChannelTypeVolume(int channelType, float volume);
		float Audio_GetChannelTypeVolume(int channelType);
		void  Audio_SetMasterVolume(float volume);
		float Audio_GetMasterVolume();
		void  Audio_OnGamePause();
		void  Audio_OnGameResume();
		void  Audio_StopAllSounds();

		bool GameState_IsPaused();
		void GameState_SetIsPaused(bool value);
		void GameState_QuitGame();

		void  Graphics_GetEnvironmentLightColor(glm::vec3* outLightColor);
		void  Graphics_SetEnvironmentLightColor(glm::vec3* inLightColor);
		float Graphics_GetGamma();
		void  Graphics_SetGamma(float inGamma);
		bool  Graphics_GetHDR();
		void  Graphics_SetHDR(bool hasHdr);
		void Graphics_RequestSwitchCamera(EntityID ownerCamera);

		float Time_GetDeltaTime();
		float Time_GetElapsedTime();

		MonoObject* Behaviour_GetScriptInstance(EntityID entityId, MonoString* inScriptFullName);

		void Builder_AddTransformComponent(EntityID entityId, glm::vec3* inPosVec, glm::vec3* inScaleVec, glm::vec3* inRotVec);
		void Builder_AddRigidBodyComponent(EntityID entityId, int type, float mass, float linearDrag, float angularDrag, bool useGravity, bool isKinematic);
		Collider* Builder_AddColliderComponent(EntityID entityId, glm::vec3* inOffset, bool isTrigger);
		void Builder_ColliderComponent_SetShapeCube(Collider* collider, glm::vec3* inHalfSize);
		void Builder_ColliderComponent_SetShapeSphere(Collider* collider, float radius);
		void Builder_ColliderComponent_SetShapeCapsule(Collider* collider, float radius, float halfHeight);
		void Builder_AddRenderComponent(EntityID entityId, MonoString* inModelPath, MonoString* inDiffusePath, MonoString* inSpecularPath, float shininess);
		void Builder_AddUIComponent(EntityID entityId, int formatType, int layerType, UI::Order order, MonoString* inTexturePath, float opacity, MonoString* inText, glm::vec3* inTextColor);
		void Builder_AddBehaviourComponent(EntityID entityId, MonoArray* inScriptNameArr);
		void Builder_AddAudioComponent(EntityID entityId, int channelType, MonoString* inSoundFilepath, float volume, float pitch, float pan);
		void Builder_AddParticlesComponent(EntityID entityId, MonoString* diffuseTexture, MonoString* specularTexture, unsigned int maxParticles, unsigned int spawnRate, float initialLife, glm::vec3* gravity, glm::vec3* offset, glm::vec3* minVelocity, glm::vec3* maxVelocity, MonoString* modelPath);

		EntityID Entity_CreateEntity(MonoString* inName);
		void Entity_DestroyEntity(EntityID entityID);
		bool Entity_EntityExists(EntityID entityID);
		SceneID Entity_GetSceneFromEntityID(EntityID entityID);
		EntityID Entity_GetEntityByName(MonoString* name, bool* outIsSuccess);
		MonoString* Entity_GetName(EntityID entityID);
		void Entity_SetName(EntityID entityID, MonoString* name);
		bool Entity_HasComponent(EntityID entityID, MonoReflectionType* componentType);
		EntityID Entity_GetParent(EntityID entityID, bool* outIsSuccess);
		void Entity_SetParent(EntityID parentID, EntityID childID);
		MonoArray* Entity_GetChildren(EntityID entityID);

		void  AudioComponent_PlaySound(EntityID entityID);
		void  AudioComponent_PauseSound(EntityID entityID);
		void  AudioComponent_ResumeSound(EntityID entityID);
		void  AudioComponent_StopSound(EntityID entityID);
		bool  AudioComponent_IsPaused(EntityID entityID);
		float AudioComponent_GetVolume(EntityID entityID);
		void  AudioComponent_SetVolume(EntityID entityID, float volume);
		float AudioComponent_GetPitch(EntityID entityID);
		void  AudioComponent_SetPitch(EntityID entityID, float pitch);
		float AudioComponent_GetPan(EntityID entityID);
		void  AudioComponent_SetPan(EntityID entityID, float pan);
		bool  AudioComponent_GetLoop(EntityID entityID);
		void  AudioComponent_SetLoop(EntityID entityID, bool loop);
		int   AudioComponent_GetChannelType(EntityID entityID);
		void  AudioComponent_SetChannelType(EntityID entityID, int channelType);
		MonoString* AudioComponent_GetSoundFile(EntityID entityID);
		void  AudioComponent_SetSoundFile(EntityID entityID, MonoString* inSoundFile);
		void  AudioComponent_FadeIn(EntityID entityID, float fadeDuration);
		void  AudioComponent_FadeOut(EntityID entityID, float fadeDuration);

		void  CameraComponent_GetPosition(EntityID entityId, glm::vec3* outPos);
		void  CameraComponent_SetPosition(EntityID entityId, glm::vec3* inPos);
		void  CameraComponent_GetTarget(EntityID entityId, glm::vec3* outTarget);
		void  CameraComponent_SetTarget(EntityID entityId, glm::vec3* inTarget);
		float CameraComponent_GetYaw(EntityID entityId);
		void  CameraComponent_SetYaw(EntityID entityId, float yaw);
		float CameraComponent_GetPitch(EntityID entityId);
		void  CameraComponent_SetPitch(EntityID entityId, float pitch);
		float CameraComponent_GetZoom(EntityID entityId);
		void  CameraComponent_SetZoom(EntityID entityId, float zoom);

		void TransformComponent_GetPosition(EntityID entityId, glm::vec3* outPos);
		void TransformComponent_SetPosition(EntityID entityId, glm::vec3* inPos);
		void TransformComponent_SetScale(EntityID entityId, glm::vec3* inScale);
		void TransformComponent_GetScale(EntityID entityId, glm::vec3* outScale);
		void TransformComponent_GetRotation(EntityID entityId, glm::vec3* outRotation);
		void TransformComponent_SetRotation(EntityID entityId, glm::vec3* inRotation);

		void  RigidBodyComponent_AddForce(std::uint32_t entityID, glm::vec3* inForce);
		void  RigidBodyComponent_AddImpulse(std::uint32_t entityID, glm::vec3* inForce);
		float RigidBodyComponent_GetMass(EntityID entityId);
		void  RigidBodyComponent_SetMass(EntityID entityId, float mass);
		float RigidBodyComponent_GetLinearDrag(EntityID entityId);
		void  RigidBodyComponent_SetLinearDrag(EntityID entityId, float linearDrag);
		float RigidBodyComponent_GetAngularDrag(EntityID entityId);
		void  RigidBodyComponent_SetAngularDrag(EntityID entityId, float angularDrag);
		bool  RigidBodyComponent_GetUseGravity(EntityID entityId);
		void  RigidBodyComponent_SetUseGravity(EntityID entityId, bool useGravity);
		bool  RigidBodyComponent_GetIsAutoCenterOfMass(EntityID entityId);
		void  RigidBodyComponent_SetIsAutoCenterOfMass(EntityID entityId, bool isAutoCenterOfMass);
		void  RigidBodyComponent_GetCenterOfMass(EntityID entityId, glm::vec3* outMassVec);
		void  RigidBodyComponent_SetCenterOfMass(EntityID entityId, glm::vec3* inMassVec);
		bool  RigidBodyComponent_GetIsKinematic(EntityID entityId);
		void  RigidBodyComponent_SetIsKinematic(EntityID entityId, bool isKinematic);
		void  RigidBodyComponent_GetVelocity(EntityID entityId, glm::vec3* outVelocity);
		void  RigidBodyComponent_SetVelocity(EntityID entityId, glm::vec3* inVelocity);
		bool  RigidBodyComponent_Raycast(EntityID entityId, const glm::vec3 &origin, const glm::vec3 &direction, float maxDistance, RigidBody::RaycastResult* result);

		int   ColliderComponent_GetColliderType(EntityID entityId);
		bool  ColliderComponent_IsTrigger(EntityID entityId);
		void  ColliderComponent_SetIsTrigger(EntityID entityId, bool inIsTrigger);
		void  ColliderComponent_GetOffset(EntityID entityId, glm::vec3* outOffset);
		void  ColliderComponent_SetOffset(EntityID entityId, glm::vec3* inOffset);
		void  ColliderComponent_CubeGeom_GetHalfExtentSize(EntityID entityId, glm::vec3* outHalfExtentSize);
		void  ColliderComponent_CubeGeom_SetHalfExtentSize(EntityID entityId, glm::vec3* inHalfExtentSize);
		float ColliderComponent_SphereGeom_GetRadius(EntityID entityId);
		void  ColliderComponent_SphereGeom_SetRadius(EntityID entityId, float radius);
		float ColliderComponent_CapsuleGeom_GetHalfHeight(EntityID entityId);
		float ColliderComponent_CapsuleGeom_GetRadius(EntityID entityId);

		MonoString* RenderComponent_GetModelPath(EntityID entityId);
		void RenderComponent_SetModelPath(EntityID entityId, MonoString* modelPath);
		MonoString* RenderComponent_GetDiffuseTexturePath(EntityID entityId);
		void RenderComponent_SetDiffuseTexturePath(EntityID entityId, MonoString* diffusePath);
		MonoString* RenderComponent_GetSpecularTexturePath(EntityID entityId);
		void  RenderComponent_SetSpecularTexturePath(EntityID entityId, MonoString* specularPath);
		float RenderComponent_GetShininess(EntityID entityId);
		void  RenderComponent_SetShininess(EntityID entityId, float shininess);

		int   LightComponent_GetLightType(EntityID entityId);
		void  LightComponent_SetLightType(EntityID entityId, int inLightType);
		void  LightComponent_GetColor(EntityID entityId, glm::vec3* outColor);
		void  LightComponent_SetColor(EntityID entityId, glm::vec3* inColor);
		void  LightComponent_GetDirection(EntityID entityId, glm::vec3* outDir);
		void  LightComponent_SetDirection(EntityID entityId, glm::vec3* inDir);
		float LightComponent_GetIntensity(EntityID entityId);
		void  LightComponent_SetIntensity(EntityID entityId, float intensity);
		float LightComponent_GetInnerCutoff(EntityID entityId);
		void  LightComponent_SetInnerCutoff(EntityID entityId, float innerCutoff);
		float LightComponent_GetOuterCutoff(EntityID entityId);
		void  LightComponent_SetOuterCutoff(EntityID entityId, float outerCutoff);

		MonoString* ParticlesComponent_GetDiffuseTexture(EntityID entityId);
		void ParticlesComponent_SetDiffuseTexture(EntityID entityId, MonoString* diffuseTexture);
		MonoString* ParticlesComponent_GetSpecularTexture(EntityID entityId);
		void  ParticlesComponent_SetSpecularTexture(EntityID entityId, MonoString* specularTexture);
		unsigned int ParticlesComponent_GetMaximumParticles(EntityID entityId);
		void  ParticlesComponent_SetMaximumParticles(EntityID entityId, unsigned int maxParticles);
		unsigned int ParticlesComponent_GetSpawnParticleRate(EntityID entityId);
		void  ParticlesComponent_SetSpawnParticleRate(EntityID entityId, unsigned int spawnRate);
		float ParticlesComponent_GetParticleLife(EntityID entityId);
		void  ParticlesComponent_SetParticleLife(EntityID entityId, float particleLife);
		void  ParticlesComponent_GetParticleGravity(EntityID entityId, glm::vec3* outGravity);
		void  ParticlesComponent_SetParticleGravity(EntityID entityId, glm::vec3* inGravity);
		void  ParticlesComponent_GetParticleOffset(EntityID entityId, glm::vec3* outOffset);
		void  ParticlesComponent_SetParticleOffset(EntityID entityId, glm::vec3* inOffset);
		void  ParticlesComponent_GetParticleMinVelocity(EntityID entityId, glm::vec3* outMinVelocity);
		void  ParticlesComponent_SetParticleMinVelocity(EntityID entityId, glm::vec3* inMinVelocity);
		void  ParticlesComponent_GetParticleMaxVelocity(EntityID entityId, glm::vec3* outMaxVelocity);
		void  ParticlesComponent_SetParticleMaxVelocity(EntityID entityId, glm::vec3* inMaxVelocity);
		MonoString* ParticlesComponent_GetModelPath(EntityID entityId);
		void  ParticlesComponent_SetModelPath(EntityID entityId, MonoString* modelPath);

		int  UIComponent_GetFormat(EntityID entityId);
		void UIComponent_SetFormat(EntityID entityId, int inFormat);
		int  UIComponent_GetLayer(EntityID entityId);
		void UIComponent_SetLayer(EntityID entityId, int inLayer);
		UI::Order UIComponent_GetOrder(EntityID entityId);
		void UIComponent_SetOrder(EntityID entityId, UI::Order inOrder);
		float UIComponent_GetOpacity(EntityID entityId);
		void  UIComponent_SetOpacity(EntityID entityId, float inOpacity);
		MonoString* UIComponent_GetTexture(EntityID entityId);
		void UIComponent_SetTexture(EntityID entityId, MonoString* inTexturePath);
		MonoString* UIComponent_GetText(EntityID entityId);
		void UIComponent_SetText(EntityID entityId, MonoString* inText);
		void UIComponent_GetTextColor(EntityID entityId, glm::vec3* outColor);
		void UIComponent_SetTextColor(EntityID entityId, glm::vec3* inColor);

		bool  Input_GetKeyPressed(int key);
		bool  Input_GetKeyHeld(int key);
		bool  Input_GetMouseButtonHeld(int button);
		void  Input_RegisterMapping(MonoString* name, int _kbmInputType, int _keycode, int _joystickInput);
		bool  Input_IsMappingKeyPressed(MonoString* name);
		bool  Input_IsMappingKeyHeld(MonoString* name);
		bool  Input_IsMappingJoystickButtonPressed(MonoString* name, int jid);
		float Input_GetMappingJoystickAxesValue(MonoString* name, int jid);
		bool  Input_IsJoystickConnected(int jid);
		MonoArray* Input_GetConnectedJoysticks();
		bool  Input_IsJoystickButtonPressed(int jid, int joystickInput);
		float Input_GetJoystickAxesValue(int jid, int joystickInput);

		float Math_Sqrt(float value);
		float Math_Sin(float value);
		float Math_Cos(float value);
		float Math_Tan(float value);
		float Math_ASin(float value);
		float Math_ACos(float value);
		float Math_ATan(float value);
		float Math_ATan2(float y, float x);
	}
}
