#include "acpch.h"
/*!***************************************************************************
    \file			AudioListener.cpp
    \author			Wang Yi Long Roy
    \par			w.yilongroy\@digipen.edu
    \date			Oct 1, 2023
    \brief			This source file consists of function definitions for
                    the audio Listener component functions for the imgui.


    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Components/AudioListener.h"
#include "Audio/FmodCore.h"
#include "Serialization/RegisterManager.hpp"

namespace AeonCore
{
    AudioListenerComponent::AudioListenerComponent() :  mPosition(FMOD_VECTOR(0.0f, 0.0f, 0.0f)),
                                                        mVelocity(FMOD_VECTOR(0.0f, 0.0f, 0.0f)),
                                                        mForward(FMOD_VECTOR(0.0f, 0.0f, -1.0f)),
                                                        mUp(FMOD_VECTOR(0.0f, 1.0f, 0.0f))
    {
        // Optionally, you can set default values for the listener attributes.
        // These values will be updated by the game logic during runtime.
        //FMODCoreClass::GetInstance().Set3DListenerAttribute(mPosition, mVelocity, mForward, mUp);
    }

    void AudioListenerComponent::SetPosition(const FMOD_VECTOR& position)
    {
        mPosition = position;
        UpdateListenerAttributes();
    }

    void AudioListenerComponent::SetVelocity(const FMOD_VECTOR& velocity)
    {
        mVelocity = velocity;
        UpdateListenerAttributes();
    }

    void AudioListenerComponent::SetForward(const FMOD_VECTOR& forward)
    {
        mForward = forward;
        UpdateListenerAttributes();
    }

    void AudioListenerComponent::SetUp(const FMOD_VECTOR& up)
    {
        mUp = up;
        UpdateListenerAttributes();
    }

    void AudioListenerComponent::UpdateListenerAttributes()
    {
        // Update the FMOD listener attributes based on the current values
        FMODCoreClass::GetInstance().Set3DListenerAttribute(mPosition, mVelocity, mForward, mUp);
        AeonCore_INFO("postion {} {} {}", mPosition.x, mPosition.y, mPosition.z);
        AeonCore_INFO("velocity {} {} {}", mVelocity.x, mVelocity.y, mVelocity.z);
        AeonCore_INFO("forward {} {} {}", mForward.x, mForward.y, mForward.z);
        AeonCore_INFO("up {} {} {}", mUp.x, mUp.y, mUp.z);
    }

    RTTR_REGISTRATION
    {
        RegisterManager::Instance().AddRegister([]() {
        rttr::registration::class_<AudioListenerComponent>("AudioListener")
            .constructor<>();
            }, 5);
    }
}