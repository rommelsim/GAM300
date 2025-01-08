#pragma once
/*!***************************************************************************
	\file			PropertiesPanel.h
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of function declaration for the inspector
					panel on the editor.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <ECS/Architecture/Coordinator.hpp>
#include <ECS/Components/Render.h> 
#include <ECS/Components/UI.h> 
#include <ECS/Components/Particle.h> 

namespace Editor {

	class PropertiesPanel {
		public:
			/*!***************************************************************************
			*   \brief			This function will take in an id and call all system to
								draw on the inspector
				\param  _fn		AeonCore::EntityID
				\return			void
			*******************************************************************************/
			static void Show(AeonCore::EntityID entity);
			/*!***************************************************************************
			*   \brief			This function will take in an id and call all the transform system to
								draw on the inspector
				\param  _fn		AeonCore::EntityID 
				\return			void
			*******************************************************************************/
			static void TransformComponent(AeonCore::EntityID& entityTransform);
			/*!***************************************************************************
			*   \brief			This function will take in an id and call all the collision system to
								draw on the inspector
				\param  _fn		AeonCore::EntityID
				\return			void
			*******************************************************************************/
			//static void CollisionComponent(AeonCore::EntityID& entityCollision);
			/*!***************************************************************************
			*   \brief			This function will take in an id and call all the script system to
								draw on the inspector
				\param  _fn		AeonCore::EntityID
				\return			void
			*******************************************************************************/
			static void ScriptComponent(AeonCore::EntityID& entityScript);
			/*!***************************************************************************
			*   \brief			This function will take in an id and call all the lighting system to
								draw on the inspector
				\param  _fn		AeonCore::EntityID
				\return			void
			*******************************************************************************/
			static void LightingComponent(AeonCore::EntityID& entityLighting);
			/*!***************************************************************************
			*   \brief			This function will take in an id and call all the render system to
								draw on the inspector
				\param  _fn		AeonCore::EntityID
				\return			void
			*******************************************************************************/
			static void RenderComponent(AeonCore::EntityID& entityRender);
			/*!***************************************************************************
			*   \brief			This function will take in an id and call all the audio system to
								draw on the inspector
				\param  _fn		AeonCore::EntityID
				\return			void
			*******************************************************************************/
			static void Audio(AeonCore::EntityID& entityAudio);

			///////////////////////////////////////////////////////////////////////////////////////////////////// ROY /////////////////////////////
			/*!***************************************************************************
			*   \brief			This function will take in an id and call all the audio listener to
								draw on the inspector
				\param  _fn		AeonCore::EntityID
				\return			void
			*******************************************************************************/
			static void AudioListener(AeonCore::EntityID& entityAudioListener);

			static void UIComponent(AeonCore::EntityID& entityComponent);
			/*!***************************************************************************
			*   \brief			This function will take in an id and call all the font system to
								draw on the inspector
				\param  _fn		AeonCore::EntityID
				\return			void
			*******************************************************************************/
			static void FontComponent(AeonCore::EntityID& entityFont);

			/*!***************************************************************************
			*   \brief			This function will take in an id and call all the particle system to
								draw on the inspector
				\param  _fn		AeonCore::EntityID
				\return			void
			*******************************************************************************/
			static void ParticlesComponent(AeonCore::EntityID& entityParticles);

			/*!***************************************************************************
			*   \brief			This function will take in an id and call all the font system to
								draw on the inspector
				\param  _fn		AeonCore::EntityID
				\return			void
			*******************************************************************************/
			static void RigidBodyComponent(AeonCore::EntityID& entityRigidBody);

			static void Collider(AeonCore::EntityID& entityCube);
			//static void SphereCollider(AeonCore::EntityID& entitySphere);
			//static void CapsuleCollider(AeonCore::EntityID& entityCapsule);
			static void CameraComponent(AeonCore::EntityID& entityCamera);
			/*!***************************************************************************
			*   \brief			This function will allow the user to add components to the AeonCore::EntityID
				\param  _fn		AeonCore::EntityID
				\return			void
			*******************************************************************************/
			static void AddComponenet(AeonCore::EntityID& entityAdd);


			static void CompressTexture(AeonCore::Render& renderComp,const char* payloadData, const std::string& textureType);
			static void CompressTexture(AeonCore::UI& uiComp, const char* payloadData, const std::string& textureType);
			static void CompressTexture(AeonCore::Particles& particlesComp, const char* payloadData, const std::string& textureType);

			static void CompressObject(AeonCore::Render& renderComp, const char* payloadData);
			static void CompressObject(AeonCore::Particles& particleComp, const char* payloadData);

			static inline float ToRadians(float degrees) {
				return degrees * (3.14159265358979323846f / 180.0f);
			}

			static inline float ToDegrees(float radians) {
				return radians * (180.0f / 3.14159265358979323846f);
			}

			static std::string s_fileExt;
		private:
			static bool m_showError;
	};

}