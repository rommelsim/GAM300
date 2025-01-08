/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe
 * file:	InternalCalls.cs
 * authors:
		Lam En Qing				(enqing.lam@digipen.edu)
 *
 * brief:	This file declares static internal functions
            that are meant to name-match the C++ functions
            defined in ScriptAPI.cpp. They allow C# to call
            stuffs from C++
 *
 *
 * All content © 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
-----------------------------------------------------------*/
using AeonScriptCore.Physics;
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace AeonScriptCore
{
    using EntityID = System.UInt32;
    using SceneID = System.Byte;

    public struct RaycastResult
    {
        public EntityID id;
        public Vector3 HitPosition;
        public Vector3 HitNormal;
        public float Distance;
    }

    internal class InternalCalls
    {
        #region NativeLog
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog_Log(string logMsg);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog_LogWarn(string logMsg);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog_LogError(string logMsg);
        #endregion

        #region SceneManager
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SceneManager_LoadScene(string inFilename, [MarshalAs(UnmanagedType.Bool)] bool shouldCache);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SceneManager_LoadSceneThread(string inFilename, [MarshalAs(UnmanagedType.Bool)] bool shouldCache);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SceneManager_RemoveSceneFromCache(string inFilename);
        #endregion

        #region Audio
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Audio_SetChannelTypeVolume(int channelType, float volume);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Audio_GetChannelTypeVolume(int channelType);
                
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Audio_SetMasterVolume(float volume);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Audio_GetMasterVolume();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Audio_OnGamePause();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Audio_OnGameResume();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Audio_StopAllSounds();
        #endregion

        #region GameState
        [MethodImpl(MethodImplOptions.InternalCall)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal extern static bool GameState_IsPaused();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GameState_SetIsPaused([MarshalAs(UnmanagedType.Bool)] bool isPaused);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GameState_QuitGame();
        #endregion

        #region Graphics
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Graphics_GetEnvironmentLightColor(out Vector3 lightColor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Graphics_SetEnvironmentLightColor(ref Vector3 lightColor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Graphics_GetGamma();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Graphics_SetGamma(float inGamma);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal extern static bool Graphics_GetHDR();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Graphics_SetHDR([MarshalAs(UnmanagedType.Bool)] bool inHdr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Graphics_RequestSwitchCamera(EntityID ownerID);
        #endregion

        #region Time
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Time_GetDeltaTime();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Time_GetElapsedTime();
        #endregion

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static object Behaviour_GetScriptInstance(EntityID entityID, string scriptFullName);

        #region Builders
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Builder_AddTransformComponent(EntityID entityID, ref Vector3 position, ref Vector3 scale, ref Vector3 rotation);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Builder_AddRigidBodyComponent(EntityID entityID, int type, float mass, float linearDrag, float angularDrag, [MarshalAs(UnmanagedType.Bool)] bool useGravity, [MarshalAs(UnmanagedType.Bool)] bool isKinematic);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static IntPtr Builder_AddColliderComponent(EntityID entityID, ref Vector3 offset, [MarshalAs(UnmanagedType.Bool)] bool isTrigger);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Builder_ColliderComponent_SetShapeCube(IntPtr colliderPtr, ref Vector3 halfSize);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Builder_ColliderComponent_SetShapeSphere(IntPtr colliderPtr, float radius);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Builder_ColliderComponent_SetShapeCapsule(IntPtr colliderPtr, float radius, float halfHeight);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Builder_AddRenderComponent(EntityID entityID, string modelPath, string diffusePath, string specularPath, float shininess);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Builder_AddUIComponent(EntityID entityID, int format, int layer, uint order, string texture, float opacity, string text, ref Vector3 textColor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Builder_AddBehaviourComponent(EntityID entityID, string[] scriptNameArr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Builder_AddAudioComponent(EntityID entityId, int channelType, string soundFilepath, float volume, float pitch, float pan);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Builder_AddParticlesComponent(uint entityId, string diffuseTexture, string specularTexture, uint maxParticles, uint spawnRate, float initialLife, ref Vector3 gravity, ref Vector3 offset, ref Vector3 minVelocity, ref Vector3 maxVelocity, string modelPath);
        #endregion

        #region Entity
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static EntityID Entity_CreateEntity(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Entity_DestroyEntity(EntityID entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal extern static bool Entity_EntityExists(EntityID entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static SceneID Entity_GetSceneFromEntityID(EntityID entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static EntityID Entity_GetEntityByName(string name, [MarshalAs(UnmanagedType.Bool)] out bool DoesEntityExist);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string Entity_GetName(EntityID entityId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Entity_SetName(EntityID entityId, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal extern static bool Entity_HasComponent(EntityID entityId, Type componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static EntityID Entity_GetParent(EntityID entityId, [MarshalAs(UnmanagedType.Bool)] out bool DoesParentExist);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Entity_SetParent(EntityID parentId, EntityID childId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal extern static EntityID[] Entity_GetChildren(EntityID entityId);
        #endregion

        #region AudioComponent
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioComponent_PlaySound(EntityID entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioComponent_PauseSound(EntityID entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioComponent_ResumeSound(EntityID entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioComponent_StopSound(EntityID entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal extern static bool AudioComponent_IsPaused(EntityID entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float AudioComponent_GetVolume(EntityID entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioComponent_SetVolume(EntityID entityID, float volume);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float AudioComponent_GetPitch(EntityID entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioComponent_SetPitch(EntityID entityID, float pitch);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float AudioComponent_GetPan(EntityID entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioComponent_SetPan(EntityID entityID, float pan);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static int AudioComponent_GetChannelType(EntityID entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioComponent_SetChannelType(EntityID entityID, int channelType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string AudioComponent_GetSoundFile(EntityID entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioComponent_SetSoundFile(EntityID entityID, string soundFilepath);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioComponent_FadeIn(EntityID entityID, float fadeDuration);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void AudioComponent_FadeOut(EntityID entityID, float fadeDuration);
        #endregion

        #region CameraComponent
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_GetPosition(EntityID entityId, out Vector3 outPos);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetPosition(EntityID entityId, ref Vector3 inPos);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_GetTarget(EntityID entityId, out Vector3 outTarget);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetTarget(EntityID entityId, ref Vector3 inTarget);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_GetYaw(EntityID entityId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_SetYaw(EntityID entityId, float yaw);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_GetPitch(EntityID entityId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_SetPitch(EntityID entityId, float pitch);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_GetZoom(EntityID entityId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_SetZoom(EntityID entityId, float zoom);
        #endregion

        #region TransformComponent
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetPosition(EntityID entityId, ref Vector3 inPos);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetPosition(EntityID entityId, out Vector3 outPos);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetScale(EntityID entityId, ref Vector3 inScale);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetScale(EntityID entityId, out Vector3 outScale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetRotation(EntityID entityId, ref Vector3 inRotation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetRotation(EntityID entityId, out Vector3 outRotation);
        #endregion

        #region RigidBodyComponent
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool RigidBodyComponent_Raycast(EntityID entityID, ref Vector3 origin, ref Vector3 direction, float maxDistance, out RaycastResult result);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_AddForce(EntityID entityID, ref Vector3 force);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_AddImpulse(EntityID entityID, ref Vector3 force);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float RigidBodyComponent_GetMass(EntityID entityId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetMass(EntityID entityId, float mass);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float RigidBodyComponent_GetLinearDrag(EntityID entityId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetLinearDrag(EntityID entityId, float linearDrag);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float RigidBodyComponent_GetAngularDrag(EntityID entityId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetAngularDrag(EntityID entityId, float angularDrag);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal extern static bool RigidBodyComponent_GetUseGravity(EntityID entityId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetUseGravity(EntityID entityId, [MarshalAs(UnmanagedType.Bool)] bool useGravity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal extern static bool RigidBodyComponent_GetIsAutoCenterOfMass(EntityID entityId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetIsAutoCenterOfMass(EntityID entityId, [MarshalAs(UnmanagedType.Bool)] bool isAutoCenterOfMass);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_GetCenterOfMass(EntityID entityId, out Vector3 centerOfMass);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetCenterOfMass(EntityID entityId, ref Vector3 centerOfMass);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal extern static bool RigidBodyComponent_GetIsKinematic(EntityID entityId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetIsKinematic(EntityID entityId, [MarshalAs(UnmanagedType.Bool)] bool isKinematic);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_GetVelocity(EntityID entityId, out Vector3 velocity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetVelocity(EntityID entityId, ref Vector3 velocity);
        #endregion

        #region ColliderComponent
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static int ColliderComponent_GetColliderType(EntityID entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal extern static bool ColliderComponent_IsTrigger(EntityID entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ColliderComponent_SetIsTrigger(EntityID entityId, [MarshalAs(UnmanagedType.Bool)] bool isTrigger);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ColliderComponent_GetOffset(EntityID entityID, out Vector3 outOffset);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ColliderComponent_SetOffset(EntityID entityId, ref Vector3 inOffset);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ColliderComponent_CubeGeom_GetHalfExtentSize(EntityID entityId, out Vector3 outHalfExtentSize);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ColliderComponent_CubeGeom_SetHalfExtentSize(EntityID entityId, ref Vector3 inHalfExtentSize);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float ColliderComponent_SphereGeom_GetRadius(EntityID entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ColliderComponent_SphereGeom_SetRadius(EntityID entityID, float radius);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float ColliderComponent_CapsuleGeom_GetHalfHeight(EntityID entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float ColliderComponent_CapsuleGeom_GetRadius(EntityID entityID);
        #endregion

        #region RenderComponent
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string RenderComponent_GetModelPath(EntityID entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RenderComponent_SetModelPath(EntityID entityID, string modelPath);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string RenderComponent_GetDiffuseTexturePath(EntityID entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RenderComponent_SetDiffuseTexturePath(EntityID entityID, string diffusePath);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string RenderComponent_GetSpecularTexturePath(EntityID entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RenderComponent_SetSpecularTexturePath(EntityID entityID, string specularPath);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float RenderComponent_GetShininess(EntityID entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RenderComponent_SetShininess(EntityID entityID, float shininess);
        #endregion

        #region LightComponent
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static int LightComponent_GetLightType(EntityID entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void LightComponent_SetLightType(EntityID entityID, int lightType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void LightComponent_SetColor(EntityID entityId, ref Vector3 inColor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void LightComponent_GetColor(EntityID entityId, out Vector3 outColor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void LightComponent_SetDirection(EntityID entityId, ref Vector3 inDirection);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void LightComponent_GetDirection(EntityID entityId, out Vector3 outDirection);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float LightComponent_GetIntensity(EntityID entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void LightComponent_SetIntensity(EntityID entityID, float intensity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float LightComponent_GetInnerCutoff(EntityID entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void LightComponent_SetInnerCutoff(EntityID entityID, float innerCutoff);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float LightComponent_GetOuterCutoff(EntityID entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void LightComponent_SetOuterCutoff(EntityID entityID, float outerCutoff);
        #endregion

        #region ParticlesComponent
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string ParticlesComponent_GetDiffuseTexture(EntityID entityId);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ParticlesComponent_SetDiffuseTexture(EntityID entityId, string diffuseTexture);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string ParticlesComponent_GetSpecularTexture(EntityID entityId);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ParticlesComponent_SetSpecularTexture(EntityID entityId, string specularTexture);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static uint ParticlesComponent_GetMaximumParticles(EntityID entityId);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ParticlesComponent_SetMaximumParticles(EntityID entityId, uint maxParticles);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static uint ParticlesComponent_GetSpawnParticleRate(EntityID entityId);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ParticlesComponent_SetSpawnParticleRate(EntityID entityId, uint spawnRate);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float ParticlesComponent_GetParticleLife(EntityID entityId);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ParticlesComponent_SetParticleLife(EntityID entityId, float particleLife);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ParticlesComponent_GetParticleGravity(EntityID entityId, out Vector3 outGravity);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ParticlesComponent_SetParticleGravity(EntityID entityId, ref Vector3 inGravity);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ParticlesComponent_GetParticleOffset(EntityID entityId, out Vector3 outOffset);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ParticlesComponent_SetParticleOffset(EntityID entityId, ref Vector3 inOffset);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ParticlesComponent_GetParticleMinVelocity(EntityID entityId, out Vector3 outMinVelocity);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ParticlesComponent_SetParticleMinVelocity(EntityID entityId, ref Vector3 inMinVelocity);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ParticlesComponent_GetParticleMaxVelocity(EntityID entityId, out Vector3 outMaxVelocity);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ParticlesComponent_SetParticleMaxVelocity(EntityID entityId, ref Vector3 inMaxVelocity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string ParticlesComponent_GetModelPath(EntityID entityId);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ParticlesComponent_SetModelPath(EntityID entityId, string modelPath);
        #endregion

        #region UIComponent
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static int UIComponent_GetFormat(EntityID entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_SetFormat(EntityID entityID, int format);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static int UIComponent_GetLayer(EntityID entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_SetLayer(EntityID entityID, int layer);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static uint UIComponent_GetOrder(EntityID entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_SetOrder(EntityID entityID, uint order);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float UIComponent_GetOpacity(EntityID entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_SetOpacity(EntityID entityID, float opacity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string UIComponent_GetTexture(EntityID entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_SetTexture(EntityID entityID, string texture);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string UIComponent_GetText(EntityID entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_SetText(EntityID entityID, string text);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_SetTextColor(EntityID entityId, ref Vector3 inColor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_GetTextColor(EntityID entityId, out Vector3 outColor);
        #endregion

        #region Input
        [MethodImpl(MethodImplOptions.InternalCall)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal extern static bool Input_GetKeyPressed(int key);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal extern static bool Input_GetKeyHeld(int key);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal extern static bool Input_GetMouseButtonHeld(int button);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_RegisterMapping(string name, int kbmInputType, int keycode, int joystickInput);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal extern static bool Input_IsMappingKeyPressed(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal extern static bool Input_IsMappingKeyHeld(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMappingJoystickButtonPressed(string name, int jid);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Input_GetMappingJoystickAxesValue(string name, int jid);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsJoystickConnected(int jid);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static int[] Input_GetConnectedJoysticks();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsJoystickButtonPressed(int jid, int joystickInput);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Input_GetJoystickAxesValue(int jid, int joystickInput);
        #endregion

        #region Math
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Math_Sqrt(float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Math_Sin(float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Math_Cos(float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Math_Tan(float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Math_ASin(float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Math_ACos(float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Math_ATan(float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Math_ATan2(float x, float y);
        #endregion
    }
}
