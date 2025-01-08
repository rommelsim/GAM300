#pragma once
/*!***************************************************************************
    \file			FmodCore.h
    \author			Wang Yi Long Roy
    \par			w.yilongroy\@digipen.edu
    \date			Oct 1, 2023
    \brief			This header file consists of function declarations for
                    the wrapper functions for FMOD Core api.


    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/


//To access the content inside FMOD folder locate the folder of FMOD with FMOD/

#include "Core/AeonAPI.h"

#include <fmod.hpp>

#include "Core/Singleton.hpp"
#include "Core/Log.h"
#include "ECS/Components/Audio.h"
#include "ECS/Architecture/Coordinator.hpp"
#include "ECS/Components/Transform.h"

namespace AeonCore
{
	using namespace std;

    /*
        Vector 3 implementation for spatial audio
        to be implemented and working soon

        A struct is a container that we can predefine
        what variables are going to be in it.
    */
    struct Vector3
    {
        float x;
        float y;
        float z;
    };

    //struct FMODChannelINFO
    //{
    //    FMOD::Channel* channel;
    //    AudioComponent::ChannelType channelType;
    //    float originalVolume;
    //    // Constructor to initialize the structure
    //    FMODChannelINFO(FMOD::Channel* pChannel, AudioComponent::ChannelType type, float volume)
    //        : channel(pChannel), channelType(type), originalVolume(volume)
    //    {

    //    }
    //};

    class AEON_API FMODCoreClass : public Singleton <FMODCoreClass>
    {
    public:
        enum class AudioState
        {
            STOPPED,
            PLAYING,
            PAUSED
        };

        FMODCoreClass() = default;
        ~FMODCoreClass() = default;

        /*
			ChannelPair:
				Pairs a FMOD channel pointer and AudioComponent Pointer
			Pair:
				Provide a way to store 2 object as a single unit
				- Template Class
				- Struct
			Parameter:
				FMOD::Channel*:
					Pointer to access the FMOD channel class
				AudioComponent*:
					Pointer to access the AudioComponent class
			Usage:
				
         */
        using ChannelPair = pair<FMOD::Channel*, AudioComponent*>;

        /*
			SoundMap:
				Maps a string as a key and FMOD Sound pointer as a value
			Map:
				Is a sorted associative container that contain key-value
				pairs with unique keys.
				- Key-Value
				- Dictionary in some places
			Parameter:
				string:
					Audio name of the files
				FMOD::Sound*:
					Pointer to access the FMOD Sound class
			Usage:
				Calling the string will get the FMOD::Sound* associated
         */
        typedef map<string, FMOD::Sound*> SoundMap;

        /*
			ChannelMap:
				Maps a int as a key and ChannelPair as value
			Map:
			Parameter:
				int:
					Channel number
				ChannelPair:
					channel pointer and AudioComponent pointer
			Usage:
				Calling channel wil get the associated ChannelPair
         */
        typedef map<int, ChannelPair> ChannelMap;

        /*
			ChannelData:
				Maps a AudioComponent::ChannelType as a key and ChannelMap
				as value
			Map:
			Parameter:
				AudioComponent::ChannelType:
					Channel types such as BGM, SFX and VO
				ChannelMap:
					int as the ID and ChannelPair
					
					ChannelPair:
						channel pointer and AudioComponent pointer
			Usage:
				Calling the channel type to get the associated ChannelMap
				which contains the ID to the associated Channel Map
         */
        typedef map<AudioComponent::ChannelType, ChannelMap> ChannelData;
        
        // FMOD Core declarations
        /*
			CInit:
				Initialize the FMOD Core
					- FMOD::System* m_PtrCoreSystem
						Used for FMOD::System_Create to create the system reference
						Once it is created call init in the system
			ChannelData m_ChannelData:
				Used to initialize a empty channel map with audio component channel
				type initialized to BGM, SFX, Voice
         */
        void CInit();    // Declaration for initializing FMOD Core
        /*
            CUpdate:
                Update the FMOD Core
            ChannelData m_ChannelData:
                - typeItr
                    Used for iterating through the m_ChannelData, which is the
                    ChannelData map that has the BGM, SFX, Voice initialized
                    and saved into typeItr
                - _channels
					ChannelMap map
				- it
					ChannelMap second which is channelPair
					ChannelPair first is the Channel pointer
			Usage:
				Check if Channel* is playing, if not playing, push back
                      
         */
        void CUpdate();
        void CShutdown();// Declaration for shutting down FMOD Core

        // Function Overloading For Playsound
        int CPlaySound_Internal(
	        AudioComponent& audioComp);

        int CPlaySound(AudioComponent& audio_comp);

        // Add declarations for FMOD Core features here...
        // Map the data of the sound

        

        void SetChannelVolume(AudioComponent::ChannelType channelType, int nChannelId, float volume);
        void CUnloadSound(const std::string& strSoundName);
        bool IsChannelPlaying(AudioComponent& audio_comp);
        void PauseChannel(AudioComponent& audio_comp);
        void PauseAllChannels();
        void ResumeAllChannels();
        void ResumeChannel(AudioComponent& audio_comp);

        // Wraper for Class System
        void Set3DListenerAttribute(const FMOD_VECTOR& position,
                                    const FMOD_VECTOR& velocity,
                                    const FMOD_VECTOR& forward,
                                    const FMOD_VECTOR& up);

        void Set3DAttributes(FMOD::Channel* pChannel,
                            const FMOD_VECTOR& position,
                            const FMOD_VECTOR& velocity);

        AudioState GetAudioState(FMOD::Channel* pChannel);
        AudioState GetAudioState(AudioComponent& audioComp);

        void StopChannel(AudioComponent& audio_comp);
        void StopAllChannels();

        FMOD::Channel* GetChannel(AudioComponent& audio_comp);
        void UpdateChannelType(AudioComponent& audio_comp, AudioComponent::ChannelType newType);

        /* Fade in and Fade out Function */
        void CFadeIn(AudioComponent::ChannelType channelType, int nChannelId, float fadeDuration, float targetVolume);
        void CFadeOut(AudioComponent::ChannelType channelType, int nChannelId, float fadeDuration, float targetVolume);

        /* AudioType Volume Control */
        void SetChannelTypeVolume(AudioComponent::ChannelType channelType, float volumePercentage);
        /* Master Volume */
        void SetChannelMasterVolume(float masterVolumePercentage);
        /* Getter Function For SetChannelTypeVolume For Protecting Direct Write Access */
        inline float GetBGMChannelVolume() { return bgmVolume; }
        inline float GetSFXChannelVolume() { return sfxVolume; }
        inline float GetVOIChannelVolume() { return voiVolume; }
        inline float GetMasterVolume() { return masterVolume; }

        FMOD::System* GetFMODSystem() { return m_PtrCoreSystem; }
		int cErrorCheck(FMOD_RESULT result);

    private:
        // FMOD Core declarations
        FMOD_VECTOR VectorToFmod(const Vector3& vPosition);
        
        FMOD::System* m_PtrCoreSystem = nullptr;                 // Declaration for FMOD Low-Level System
        int m_nNextChannelId{0};
        //ChannelMap m_Channels;
        ChannelData m_ChannelData; // Store original volume for each channel type

        //overall vol for each channel type
        float bgmVolume = 1.0f;
        float sfxVolume = 1.0f;
        float voiVolume = 1.0f;
        float masterVolume = 1.0f;
    };
}