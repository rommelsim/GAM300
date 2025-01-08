#pragma once
/*!***************************************************************************
    \file			FmodStudio.h
    \author			Wang Yi Long Roy
    \par			w.yilongroy\@digipen.edu
    \date			Oct 1, 2023
    \brief			This header file consists of function declarations for
                    the wrapper functions for FMOD Core api.


    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

//#include "fmod_studio.hpp"
#include "fmod.hpp"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>

#include "fmod_studio.hpp"

namespace AeonCore
{
    using namespace std;

    /*
        A struct is a container that we can predefine
        what variables are going to be in it.
    */
    struct CoreVector3
    {
        float x;
        float y;
        float z;
    };

    /*
        The struct to contain the code for initializing 
        and shutting down the FMOD engine as well as hold 
        instances of both the Studio and Low-Level system 
        objects for FMOD. 

        Implementation will also hold a map of all the sounds 
        and events we've played in our projects. 
    */
    struct Implementation 
    {
        // Existing FMOD Studio code...
        Implementation();
        ~Implementation();

        void sUpdate();

        FMOD::Studio::System* mpStudioSystem;
        // FMOD Core declarations
        FMOD::System* mpSystem;// Declaration for FMOD Low-Level System
        // Add declarations for FMOD Core features here...

        int mnNextChannelId;

        typedef map<string, FMOD::Sound*> SoundMap;
        typedef map<int, FMOD::Channel*> ChannelMap;
        typedef map<string, FMOD::Studio::EventInstance*> EventMap;
        typedef map<string, FMOD::Studio::Bank*> BankMap;

        BankMap mBanks;
        EventMap mEvents;
        SoundMap mSounds;
        ChannelMap mChannels;
    };

    /*
        The engine class will do calls to the Implementation 
        struct to start, stop, and update FMOD.
    */

    class fmodstudioclass
    {
    public:

        FMOD::Channel* SfxChannel = nullptr;
        FMOD::Channel* MusicChannel = nullptr;
        FMOD::Channel* VoiceChannel = nullptr;
        FMOD::ChannelGroup* ChannelGroup = nullptr;

        typedef map<string, FMOD::Sound*> SoundDb;
        typedef map<string, FMOD::Sound*> MusicDb;
        typedef map<string, FMOD::Sound*> VoiceDb;
        // FMOD Core declarations
        static void cInit();    // Declaration for initializing FMOD Core
        static void cUpdate();  // Declaration for updating FMOD Core
        static void cShutdown();// Declaration for shutting down FMOD Core
        static int cErrorcheck(FMOD_RESULT result);
        // New member variable to store the volume level for each sound channel
        //std::map<int, float> mChannelVolumes;

        void LoadBank(const string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
        void LoadEvent(const string& strEventName);
        void LoadSound(const string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false);
        void UnLoadSound(const string& strSoundName);
        void Set3dListenerAndOrientation(const Vector3& vPos = Vector3{ 0, 0, 0 }, float fVolumedB = 0.0f);
        int PlaySound(const string& strSoundName, const Vector3& vPos = Vector3{ 0, 0, 0 }, float fVolumedB = 0.0f);
        void PlayEvent(const string& strEventName);
        void StopChannel(int nChannelId);
        void StopEvent(const string& strEventName, bool bImmediate = false);
        void GetEventParameter(const string& strEventName, FMOD_STUDIO_PARAMETER_ID parameterID, float* parameter);
        void SetEventParameter(const string& strEventName, FMOD_STUDIO_PARAMETER_ID parameterID, float fValue);
        void StopAllChannels();
        void SetChannel3dPosition(int nChannelId, const Vector3& vPosition);
        void SetChannelVolume(int nChannelId, float fVolumedB);

        
        /*
        */
        void SetEventVolume(const std::string& strEventName, float fVolumeB);

        bool IsPlaying(int nChannelId) const;
        bool IsEventPlaying(const string& strEventName) const;

        
        // New method to set the volume of a specific channel
        //void SetChannelVolume(int nChannelId, float fVolumedB);

        /*
            This function takes a volume level in decibels (dB) as input and 
            returns the corresponding linear amplitude value. 

            It converts from a logarithmic dB scale to a linear scale. 
            You can use this function to set the volume of a sound channel 
            when you want to specify the volume in dB.

            TLDR: converts dB to linear amplitude and can be used to set the volume.
        */
        float dbToVolume(float db);
        /*
            This function takes a linear amplitude value as input and 
            returns the corresponding volume level in decibels (dB). 

            It converts from a linear scale to a logarithmic dB scale. 
            You can use this function to query the current volume level 
            of a sound channel in dB.

            TLDR: converts linear amplitude to dB and can be used to query the current volume.
        */
        float VolumeTodB(float volume);
        static FMOD_VECTOR VectorToFmod(const Vector3& vPosition);
        void SetSoundPosition(FMOD::Channel* pChannel, const Vector3& soundPosition);
    };

}