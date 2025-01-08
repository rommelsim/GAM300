#include "acpch.h"
/*!***************************************************************************
    \file			Audio.cpp
    \author			Wang Yi Long Roy
    \par			w.yilongroy\@digipen.edu
    \date			Oct 1, 2023
    \brief			This source file consists of function definitions for
                    the audio component functions for the imgui.


    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Components/Audio.h"
#include "Audio/FmodCore.h"
#include "ECS/Architecture/Coordinator.hpp"
#include "Resource/ResourceManager.h"
#include "Serialization/RegisterManager.hpp"

// this is for imgui
static int Index = 0;


namespace AeonCore
{
    // but empty constructor does not load sound, so when you click on AddComponent inside the PropertiesPanel, this empty constructor gets called.
    AudioComponent::AudioComponent() : mVolume(1.0f)/*, mPosition({ 0.0f, 0.0f, 0.0f })*/,
                                       mAudio(""), mChannelID(-1), mPlayCheckBox(false), mChannelType()
	{
	    //id = Coordinator::GetInstance().GetComponentType<AudioComponent>();
    }

    /*
        will take in data input from the user - audio files and type of audio
        will initialize private functions and data
    */
    AudioComponent::AudioComponent(std::string soundName)
        : mVolume(1.0f)/*, mPosition({ 0.0f, 0.0f, 0.0f })*/, mAudio(soundName),
        mChannelID(-1), mPlayCheckBox(false), mChannelType()
	{
        
        if (!soundName.empty())
        {
            ResourceManager::GetInstance().LoadSound(soundName);
            if (mPlayOnAwake)
            {
                PlaySound();
            }
        }
    }

    void AudioComponent::Destroy()
    {
        StopSound();
    }

    // Then when you drag-drop, setSoundFile is called, but before, there was no loadSound
    void AudioComponent::setSoundFile(std::string _audioFile)
	{
	    mAudio = _audioFile;
        //FMODCoreClass::GetInstance().cLoadSound(_audioFile);
        ResourceManager::GetInstance().LoadSound(_audioFile);
    }

    void AudioComponent::setChannelType(ChannelType _channelType) {
        if (_channelType != mChannelType) {
	        // ask fmodcoreclass to update first, in order to know what the "old" type is
        	FMODCoreClass::GetInstance().UpdateChannelType(*this, _channelType);
        }
        // after updating, we set the channel type
	    mChannelType = _channelType;
    }

    void AudioComponent::PlaySound()
    {
        FMODCoreClass::GetInstance().CPlaySound(*this);
    }

    void AudioComponent::StopAllSounds()
    {
        FMODCoreClass::GetInstance().StopAllChannels();
    }

    void AudioComponent::FadeIn(float duration)
    {
        FMODCoreClass::GetInstance().CFadeIn(mChannelType, mChannelID, duration, mVolume);
    }

    void AudioComponent::FadeOut(float duration)
    {
        FMODCoreClass::GetInstance().CFadeOut(mChannelType, mChannelID, duration, mVolume);
    }

    void AudioComponent::TestSetChannelTypeVolume(AudioComponent::ChannelType channelType, float volumePercentage)
    {
        FMODCoreClass::GetInstance().SetChannelTypeVolume(channelType, volumePercentage);
    }

    void AudioComponent::SetPause()
    {
        FMODCoreClass::GetInstance().PauseChannel(*this);
    }

    void AudioComponent::SetResume()
    {
        FMODCoreClass::GetInstance().ResumeChannel(*this);
        mIsGamePaused = false;
    }

    void AudioComponent::StopSound()
    {
        FMODCoreClass::GetInstance().StopChannel(*this);
    }


    RTTR_REGISTRATION
    {
        RegisterManager::Instance().AddRegister([]() {
        rttr::registration::class_<AudioComponent>("Audio")
            .constructor<std::string const&>()
            .property_readonly("id", &AudioComponent::GetComponentID)
            .property("name", &AudioComponent::GetSoundFile, &AudioComponent::setSoundFile)
            .property("volume", &AudioComponent::GetVolume, &AudioComponent::SetVolume)
            .property("loop", &AudioComponent::GetLoop, &AudioComponent::SetLoop)
            .property("pitch", &AudioComponent::GetPitch, &AudioComponent::SetPitch)
            .property("pan", &AudioComponent::GetPan, &AudioComponent::SetPan)
            .property("mute", &AudioComponent::GetMute, &AudioComponent::SetMute)
            .property("onAwake", &AudioComponent::GetPlayOnAwake, &AudioComponent::SetPlayOnAwake)
            .property("type", &AudioComponent::GetChannelType, &AudioComponent::setChannelType) 
            .property("3D mode", &AudioComponent::GetIIIDMode, &AudioComponent::SetIIIDMode);
            }, 6);
    }
}
