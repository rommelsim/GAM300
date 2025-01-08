#pragma once
/*!***************************************************************************
    \file			Audio.h
    \author			Wang Yi Long Roy
    \par			w.yilongroy\@digipen.edu
    \date			Oct 1, 2023
    \brief			This header file consists of function declarations for
                    the audio component functions for the imgui.


    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/AeonAPI.h"
#include "IComponent.h"

// user control
// deteremin what my user nned to input to be able to run what he needs to do.


namespace AeonCore
{
    class AEON_API AudioComponent : public IComponent
    {
    public:

        enum class ChannelType
        {
            SFX,
            BGM,
            Voice,
            // Add more channel types as needed
        };

        AudioComponent();
        AudioComponent(std::string soundName);
        //~AudioComponent();
        void Destroy() override;

        inline std::string GetSoundFile()                           { return mAudio;            }
        void setSoundFile(std::string _audioFile);
        //example
        /*inline FMODCoreClass::AudioType GetAudioType()              { return mAudioType;        }
        void setAudioChannel(FMODCoreClass::AudioType _audioType)   { mAudioType = _audioType;  }*/

        inline ChannelType GetChannelType()                         { return mChannelType; }
        void setChannelType(ChannelType _channelType);

        inline float GetVolume()                                   { return mVolume;           }
        void SetVolume(float _Volume)                              { mVolume = _Volume;        }
        
        inline int GetChannelID()                                         { return mChannelID;        }
        void SetChannelID(int _ChannelID)                           { mChannelID = _ChannelID; }

        inline bool GetPlayCheck()                                         { return mPlayCheckBox; }
        void SetPlayCheck(bool _play)                               { mPlayCheckBox = _play; }

        inline bool GetLoop()                                              { return mLoop; }
        void SetLoop(bool _loop)                                    { mLoop = _loop; }

        inline float GetPitch() const                                      { return mPitch; }
        void SetPitch(float _pitch)                                 { mPitch = _pitch; }

        inline float GetPan() const                                        { return mPan; }
        void SetPan(float _pan)                                     { mPan = _pan; }

        inline bool GetMute() const                                        { return mMute; }
        void SetMute(bool _mute)                                    { mMute = _mute; }

        inline bool GetIIIDMode() const                                    { return mIIIDMode; }
        void SetIIIDMode(bool _IIIDMode)                            { mIIIDMode = _IIIDMode; }

        inline bool GetPlayOnAwake() const                                 { return mPlayOnAwake; }
        void SetPlayOnAwake(bool playOnAwake)                       { mPlayOnAwake = playOnAwake; }

		void StopSound();

        void PlaySound();   

        void StopAllSounds();

        void FadeIn(float duration);

        void FadeOut(float duration);

        // Test 
        void TestSetChannelTypeVolume(AudioComponent::ChannelType channelType, float volumePercentage);
        
        void SetPause();
        void SetResume();

     private:
        RTTR_ENABLE(IComponent);
        friend class AudioSystem;

        //FMOD::Channel* mChannel; // or FMOD::ChannelGroup if you want
        float mVolume;
        std::string mAudio;
        ChannelType mChannelType;
        int mChannelID = -1;
        bool mPlayCheckBox;
        bool mLoop = false;
        bool mPlayOnAwake = true;
        float mPitch = 1.0f;
        float mPan = 0.0f;
        bool mMute = false;
        bool mIIIDMode = false;
        float mDuration = 5.0f;
        //Vector3 mPosition;

        // Used only by AudioSystem to see if the audio was paused when game paused or not. Since it's a "private use", no need to add to RTTR
        bool mIsGamePaused = false;
    };
}

/*
 * inline means i return straight
 *
 *
 */