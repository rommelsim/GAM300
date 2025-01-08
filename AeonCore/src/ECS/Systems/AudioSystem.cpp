#include "acpch.h"
/*!***************************************************************************
    \file			AudioSystem.cpp
    \author			Wang Yi Long Roy
    \par			w.yilongroy\@digipen.edu
    \date			Oct 1, 2023
    \brief			This source file consists of function definitions for
                    calling audio component functions.


    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Systems/AudioSystem.h"
#include "ECS/Architecture/Coordinator.hpp"
#include "ECS/Components/Audio.h"
#include "Core/Log.h"
#include "Input/Input.h"
#include "Audio/FmodCore.h"
#include <ECS/Components/AudioListener.h>
#include <Camera/CameraManager.h>
#include <glm/gtx/quaternion.hpp>

//this is for the game engine


namespace AeonCore
{
    FMODCoreClass& audioInstance = FMODCoreClass::GetInstance(); // Create an instance of the FMODCoreClass
    //Input inputInstance;

    void AudioSystem::Init()
    {
        
        // Initialize your audio system here
        // For example, you can initialize FMOD or any other audio-related setup
        
        // Initialize FMOD Core and create the FMOD System
        audioInstance.CInit();
    }

    void AudioSystem::Update()
    {        

        ZoneScopedN("Audio Update");
        // Run the update loop to process audio events (call this regularly in your application loop)
        Application& app = Application::GetApplication();

        //if (app.GetLoadStatus() != LoadStatus::NotRunning) return;

        // since scene stopped, we set the boolean to false
        if (app.GetRuntimeState() == RuntimeState::Stop && hasOnAwakeRun)
        {
            hasOnAwakeRun = false;
        }
        
        audioInstance.CUpdate();

        // Update your audio system here
        // You can handle audio-related updates, such as playing sounds, here
        Coordinator& coord = Coordinator::GetInstance();


        // Convert the position to FMOD_VECTOR
        //FMOD_VECTOR fmodPosition = FMOD_VECTOR(position.x, position.y, position.z);s
        auto& activeCam = CameraManager::GetInstance().GetActiveCamera();
        glm::vec3 cameraPosition = activeCam.GetCameraPosition();

        /*
         * lookAtDir is forward vector, why we have this is because camera script
         * modifies target vector, so when target vector is modified, view matrix
         * is calculated directly. Thus front vector is not set.
         *
         * The up vector must be relative to the forward vector, because fmod requires
         * the up vector to be relative to the forward vector, so we must calculate the
         * up vector from the lookAtDir, using cross product.
         *
         * In order to get the up vector, we must calculated the right vector.
         */

        glm::vec3 lookAtDir = activeCam.GetCameraTarget() - activeCam.GetCameraPosition();
    	lookAtDir = glm::normalize(lookAtDir);

        if (glm::length2(lookAtDir) > 0.0f)
        {
            glm::vec3 lookAtRight = glm::normalize(glm::cross(lookAtDir, glm::vec3(0, 1, 0)));
            glm::vec3 lookAtUp = glm::normalize(glm::cross(lookAtRight, lookAtDir));

            FMOD_VECTOR fmodPosition = FMOD_VECTOR(cameraPosition.x, cameraPosition.y, cameraPosition.z);
            //FMOD_VECTOR fmodVelocity = FMOD_VECTOR
            // Set 3D attributes for the listener
            FMOD_VECTOR fmodUp = FMOD_VECTOR{ lookAtUp.x, lookAtUp.y, lookAtUp.z };
            FMOD_VECTOR fmodForward = FMOD_VECTOR{ lookAtDir.x, lookAtDir.y, lookAtDir.z };
            audioInstance.Set3DListenerAttribute(fmodPosition,
                FMOD_VECTOR{ 0, 0, 0 },
                fmodForward, fmodUp);

            /*AeonCore_INFO("audio listener position x{}, y{}, z{}", cameraPosition.x, cameraPosition.y, cameraPosition.z);
            AeonCore_INFO("audio listener forward x{}, y{}, z{}", fmodForward.x, fmodForward.y, fmodForward.z);
            AeonCore_INFO("audio listener up x{}, y{}, z{}", fmodUp.x, fmodUp.y, fmodUp.z);*/
        }

        // Iterate through the entities in mEntities and update audio components
        for (auto const& entity1 : mEntities)
        {
            /*
                this line of code is to guard that entity must contain audio sound
                else break.
            */
            if (coord.HasComponent<AudioComponent>(entity1) && coord.HasComponent<Transform>(entity1))
            {
                // Get the entity and its audio component
                AudioComponent& audioComp = coord.GetComponent<AudioComponent>(entity1);

                // Check if the audio component is 3D
                if (audioComp.GetIIIDMode() == true)
                {
                    // Get the transform component
                    Transform& transform = coord.GetComponent<Transform>(entity1);
                    // Get the position of the entity
                    glm::vec3 position = transform.GetPosition();
                    // Get the FMOD channel associated with the sound
                    FMOD::Channel* pChannel = audioInstance.GetChannel(audioComp);
                    // Check if the entity has an AudioListenerComponent
                    //if (coord.HasComponent<AudioListenerComponent>(entity1))
                    
                    // Check if the channel is valid
                    if (pChannel)
                    {
                        // Convert the position to FMOD_VECTOR
                        FMOD_VECTOR fmodPosition = FMOD_VECTOR(position.x, position.y, position.z);

                        // Set 3D attributes for the channel
                        audioInstance.Set3DAttributes(pChannel, fmodPosition, FMOD_VECTOR{ 0, 0, 0 });
                    }
                }
                //if (AeonCore::Input::IsKeyTriggered(AC_KEY_F)) OLD CODE THAT IS USE AS EXAMPLE
                //{
                //    audioComp.PlaySound();
                //    audioInstance.CFadeIn(audioComp.GetChannelType(), audioComp.GetChannelID(), 5.0f, audioComp.GetVolume()); // Adjust the duration as needed
                //}
                //if (AeonCore::Input::IsKeyTriggered(AC_KEY_K))
                //{
                //    audioInstance.CFadeOut(audioComp.GetChannelType(), audioComp.GetChannelID(), 5.0f, audioComp.GetVolume());
                //}
                /*if (AeonCore::Input::IsKeyTriggered(AC_KEY_T))
                {
                    audioInstance.SetChannelTypeVolume(AudioComponent::ChannelType::BGM, 0.5f);
                    AeonCore_INFO("current volume {}", audioComp.GetVolume());
                }*/
                if (audioComp.GetPlayOnAwake() && app.GetRuntimeState() == RuntimeState::Start && !hasOnAwakeRun)
                {
                    audioComp.PlaySound();
                    //audioInstance.CFadeIn(audioComp.GetChannelType(), audioComp.GetChannelID(), 5.0f, audioComp.GetVolume()); // Adjust the duration as needed
                }
                // OLD CODE THAT IS MEANT TO TEST FOR BGM/SFX/VOICE VOLUME CONTROLS
                /*if (audioComp.GetChannelType() == AudioComponent::ChannelType::BGM)
                {
                    if (Input::IsKeyTriggered(AC_KEY_1))
                    {
                        audioComp.TestSetChannelTypeVolume(AudioComponent::ChannelType::BGM, 1.0f);
                        AeonCore_INFO("1");
                    }
                    else if (Input::IsKeyTriggered(AC_KEY_2))
                    {
                        audioComp.TestSetChannelTypeVolume(AudioComponent::ChannelType::BGM, 0.5f);
                        AeonCore_INFO("2");
                    }
                    else if (Input::IsKeyTriggered(AC_KEY_3))
                    {
                        audioComp.TestSetChannelTypeVolume(AudioComponent::ChannelType::BGM, 0.0f);
                        AeonCore_INFO("3");
                    }
                }*/
            }
        }
        if (app.GetRuntimeState() == RuntimeState::Start && !hasOnAwakeRun)
        {
            hasOnAwakeRun = true;
        }
    }

    void AudioSystem::Destroy()
    {
        // Clean up any resources or shutdown your audio system here
        // For example, release FMOD resources or shutdown the audio system
        // 
        //// Shutdown the audio system
        audioInstance.CShutdown();
    }

    void AudioSystem::OnGamePause() {
        Coordinator& coord = Coordinator::GetInstance();
        // We only want to pause sounds in the current scene, hence mEntities loop
	    for (auto entityId : mEntities) {
		    AudioComponent& audioComp = coord.GetComponent<AudioComponent>(entityId);

            if (audioInstance.GetAudioState(audioComp) == FMODCoreClass::AudioState::PLAYING) {
	            audioComp.mIsGamePaused = true;
                audioComp.SetPause();
            }
	    }
    }

    void AudioSystem::OnGameUnpause() {
        Coordinator& coord = Coordinator::GetInstance();
        // We only want to unpause sounds in the current scene, hence mEntities loop
	    for (auto entityId : mEntities) {
		    AudioComponent& audioComp = coord.GetComponent<AudioComponent>(entityId);

            if (audioComp.mIsGamePaused) {
                audioComp.mIsGamePaused = false;
	            audioComp.SetResume();
            }
	    }
    }

    void AudioSystem::OnCachedSceneActive() {
        Coordinator& coord = Coordinator::GetInstance();
        // We only want to unpause sounds when a cached scene becomes active again, hence mEntities loop
	    for (auto entityId : mEntities) {
		    AudioComponent& audioComp = coord.GetComponent<AudioComponent>(entityId);

            // if an audio is not paused by "game pause", we resume playing the sounds
            if (!audioComp.mIsGamePaused) {
	            audioComp.SetResume();
            }
	    }
    }
    void AudioSystem::OnScenedSceneInactive() {
        Coordinator& coord = Coordinator::GetInstance();
        // We only want to pause sounds when a cached scene becomes inactive, hence mEntities loop
	    for (auto entityId : mEntities) {
		    AudioComponent& audioComp = coord.GetComponent<AudioComponent>(entityId);

            // if an audio wasn't paused by "game pause" (game pause as in press ESC to pause) AND the audio is playing
            if (!audioComp.mIsGamePaused && audioInstance.GetAudioState(audioComp) == FMODCoreClass::AudioState::PLAYING) {
	            audioComp.SetPause();
            }
	    }
    }
}
