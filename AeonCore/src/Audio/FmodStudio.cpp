#include "acpch.h"
/*!***************************************************************************
    \file			FmodStudio.cpp
    \author			Wang Yi Long Roy
    \par			w.yilongroy\@digipen.edu
    \date			Oct 1, 2023
    \brief			This source file consists of function definitions for
                    the wrapper functions for FMOD Studio api.


    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Audio/FmodStudio.h"

namespace AeonCore
{
    /*
        Initialize the underlying FMOD system that will allow us to play sounds.
    */

    Implementation::Implementation() : mnNextChannelId(0) 
    {
        mpStudioSystem = NULL;
        //mnNextChannelId = 0; // Initialize mnNextChannelId

        fmodstudioclass::cErrorcheck(FMOD::Studio::System::create(&mpStudioSystem));
        fmodstudioclass::cErrorcheck(mpStudioSystem->initialize(1024, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

        mpSystem = NULL;
        fmodstudioclass::cErrorcheck(mpStudioSystem->getCoreSystem(&mpSystem));
    }

    Implementation::~Implementation() 
    {
        try 
        {
            fmodstudioclass::cErrorcheck(mpStudioSystem->unloadAll());
            fmodstudioclass::cErrorcheck(mpStudioSystem->release());
        }
        catch (const std::exception& ex) 
        {
            std::cerr << "Error during Implementation destructor: " << ex.what() << std::endl;
        }
        /*fmodstudioclass::cErrorcheck(mpStudioSystem->unloadAll());
        fmodstudioclass::cErrorcheck(mpStudioSystem->release());*/
    }

    void Implementation::sUpdate() {
        vector<ChannelMap::iterator> pStoppedChannels;
        for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
        {
            bool bIsPlaying = false;
            it->second->isPlaying(&bIsPlaying);
            if (!bIsPlaying)
            {
                pStoppedChannels.push_back(it);
            }
        }
        for (auto& it : pStoppedChannels)
        {
            mChannels.erase(it);
        }
        fmodstudioclass::cErrorcheck(mpStudioSystem->update());
    }

    Implementation* sgpImplementation = nullptr;

    void fmodstudioclass::cInit() 
    {
        /*if (sgpImplementation) 
        {
            throw std::runtime_error("Implementation already initialized.");
        }*/
        sgpImplementation = new Implementation;
    }

    void fmodstudioclass::cUpdate()
    {
        /*if (!sgpImplementation) 
        {
            throw std::runtime_error("Implementation not initialized.");
        }*/
        sgpImplementation->sUpdate();
    }

    void fmodstudioclass::LoadSound(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream)
    {
        auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
        if (tFoundIt != sgpImplementation->mSounds.end())
            return;

        FMOD_MODE eMode = FMOD_DEFAULT;
        eMode |= b3d ? FMOD_3D : FMOD_2D;
        eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
        eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

        FMOD::Sound* pSound = nullptr;
        fmodstudioclass::cErrorcheck(sgpImplementation->mpSystem->createSound(strSoundName.c_str(), eMode, nullptr, &pSound));
        if (pSound) {
            sgpImplementation->mSounds[strSoundName] = pSound;
        }

    }

    void fmodstudioclass::UnLoadSound(const std::string& strSoundName)
    {
        auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
        if (tFoundIt == sgpImplementation->mSounds.end())
            return;

        fmodstudioclass::cErrorcheck(tFoundIt->second->release());
        sgpImplementation->mSounds.erase(tFoundIt);
    }

    void fmodstudioclass::Set3dListenerAndOrientation(const Vector3& vPos, float fVolumedB)
    {
        (void)fVolumedB;
        (void)vPos;
    }

    int fmodstudioclass::PlaySound(const string& strSoundName, const Vector3& vPosition, float fVolumedB)
    {
        int nChannelId = sgpImplementation->mnNextChannelId++;
        auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
        if (tFoundIt == sgpImplementation->mSounds.end())
        {
            LoadSound(strSoundName);
            tFoundIt = sgpImplementation->mSounds.find(strSoundName);
            if (tFoundIt == sgpImplementation->mSounds.end())
            {
                return nChannelId;
            }
        }
        FMOD::Channel* pChannel = nullptr;
        fmodstudioclass::cErrorcheck(sgpImplementation->mpSystem->playSound(tFoundIt->second, nullptr, true, &pChannel));
        if (pChannel)
        {
            FMOD_MODE currMode;
            tFoundIt->second->getMode(&currMode);
            if (currMode & FMOD_3D) {
                FMOD_VECTOR position = VectorToFmod(vPosition);
                fmodstudioclass::cErrorcheck(pChannel->set3DAttributes(&position, nullptr));
            }
            fmodstudioclass::cErrorcheck(pChannel->setVolume(dbToVolume(fVolumedB)));
            fmodstudioclass::cErrorcheck(pChannel->setPaused(false));
            sgpImplementation->mChannels[nChannelId] = pChannel;
        }
        return nChannelId;
    }

    void fmodstudioclass::SetChannel3dPosition(int nChannelId, const Vector3& vPosition)
    {
        auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
        if (tFoundIt == sgpImplementation->mChannels.end())
            return;

        FMOD_VECTOR position = VectorToFmod(vPosition);
        fmodstudioclass::cErrorcheck(tFoundIt->second->set3DAttributes(&position, NULL));
    }

    void fmodstudioclass::SetChannelVolume(int nChannelId, float fVolumedB)
    {
        auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
        if (tFoundIt == sgpImplementation->mChannels.end())
            return;

        // Ensure the volume is within the valid range [0.0, 1.0]
        fVolumedB = std::min(std::max(fVolumedB, 0.0f), 1.0f);

        // Set the volume for the channel
        fmodstudioclass::cErrorcheck(tFoundIt->second->setVolume(dbToVolume(fVolumedB)));
    }

    void fmodstudioclass::SetEventVolume(const std::string& strEventName, float fVolumeB)
    {
        auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
        if (tFoundIt == sgpImplementation->mEvents.end())
            return;

        // Ensure the volume is within the valid range [0.0, 1.0]
        fVolumeB = std::min(std::max(fVolumeB, 0.0f), 1.0f);

        // Set the volume for the event
        FMOD_RESULT result = tFoundIt->second->setVolume(fVolumeB);
        fmodstudioclass::cErrorcheck(result);
    }

    void fmodstudioclass::LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags) {
        auto tFoundIt = sgpImplementation->mBanks.find(strBankName);
        if (tFoundIt != sgpImplementation->mBanks.end())
            return;
        FMOD::Studio::Bank* pBank;
        fmodstudioclass::cErrorcheck(sgpImplementation->mpStudioSystem->loadBankFile(strBankName.c_str(), flags, &pBank));
        if (pBank) {
            sgpImplementation->mBanks[strBankName] = pBank;
        }
    }

    void fmodstudioclass::LoadEvent(const std::string& strEventName) {
        auto tFoundit = sgpImplementation->mEvents.find(strEventName);
        if (tFoundit != sgpImplementation->mEvents.end())
            return;
        FMOD::Studio::EventDescription* pEventDescription = NULL;
        fmodstudioclass::cErrorcheck(sgpImplementation->mpStudioSystem->getEvent(strEventName.c_str(), &pEventDescription));
        if (pEventDescription) {
            FMOD::Studio::EventInstance* pEventInstance = NULL;
            fmodstudioclass::cErrorcheck(pEventDescription->createInstance(&pEventInstance));
            if (pEventInstance) {
                sgpImplementation->mEvents[strEventName] = pEventInstance;
            }
        }
    }

    void fmodstudioclass::PlayEvent(const string& strEventName) {
        auto tFoundit = sgpImplementation->mEvents.find(strEventName);
        if (tFoundit == sgpImplementation->mEvents.end()) {
            LoadEvent(strEventName);
            tFoundit = sgpImplementation->mEvents.find(strEventName);
            if (tFoundit == sgpImplementation->mEvents.end())
                return;
        }
        tFoundit->second->start();
    }

    void fmodstudioclass::StopChannel(int nChannelId)
    {
        auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
        if (tFoundIt != sgpImplementation->mChannels.end())
        {
            fmodstudioclass::cErrorcheck(tFoundIt->second->stop());
            sgpImplementation->mChannels.erase(tFoundIt);
        }
    }

    void fmodstudioclass::StopEvent(const string& strEventName, bool bImmediate) {
        auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
        if (tFoundIt == sgpImplementation->mEvents.end())
            return;

        FMOD_STUDIO_STOP_MODE eMode;
        eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
        fmodstudioclass::cErrorcheck(tFoundIt->second->stop(eMode));
    }

    bool fmodstudioclass::IsEventPlaying(const string& strEventName) const {
        auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
        if (tFoundIt == sgpImplementation->mEvents.end())
            return false;

        /*FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
        if (tFoundIt->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING) {
            return true;
        }
        return false;*/

        FMOD_STUDIO_PLAYBACK_STATE state; // Initialize the state variable
        FMOD_RESULT result = tFoundIt->second->getPlaybackState(&state);
        if (result == FMOD_OK && state == FMOD_STUDIO_PLAYBACK_PLAYING)
        {
            return true;
        }
        else
        {
            return false; // Return a value in the else block
        }
    }

    void fmodstudioclass::GetEventParameter(const string& strEventName, FMOD_STUDIO_PARAMETER_ID parameterID, float* parameter) {
        auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
        if (tFoundIt == sgpImplementation->mEvents.end())
            return;

        FMOD::Studio::EventInstance* pEventInstance = tFoundIt->second; // Assuming mEvents stores EventInstances
        FMOD_RESULT result = pEventInstance->getParameterByID(parameterID, parameter);
        fmodstudioclass::cErrorcheck(result);
    }

    void fmodstudioclass::SetEventParameter(const string& strEventName, FMOD_STUDIO_PARAMETER_ID parameterID, float fValue) {
        auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
        if (tFoundIt == sgpImplementation->mEvents.end())
            return;

        FMOD::Studio::EventInstance* pEventInstance = tFoundIt->second; // Assuming mEvents stores EventInstances
        FMOD_RESULT result = pEventInstance->setParameterByID(parameterID, fValue);
        fmodstudioclass::cErrorcheck(result);
    }

    FMOD_VECTOR fmodstudioclass::VectorToFmod(const Vector3& vPosition) 
    {
        FMOD_VECTOR fVec;
        fVec.x = vPosition.x;
        fVec.y = vPosition.y;
        fVec.z = vPosition.z;
        return fVec;
    }

    // Function to set the position of a sound channel
    void SetSoundPosition(FMOD::Channel* pChannel, const Vector3& soundPosition)
    {
        // Create an instance of fmodstudioclass
        fmodstudioclass fmodInstance;

        // Convert Vector3 position to FMOD_VECTOR
        FMOD_VECTOR fmodPosition = fmodInstance.VectorToFmod(soundPosition);

        // Set the 3D position of the sound channel
        fmodstudioclass::cErrorcheck(pChannel->set3DAttributes(&fmodPosition, nullptr));
    }

    float fmodstudioclass::dbToVolume(float dB)
    {
        return powf(10.0f, 0.05f * dB);
    }

    float fmodstudioclass::VolumeTodB(float volume)
    {
        return 20.0f * log10f(volume);
    }

    int fmodstudioclass::cErrorcheck(FMOD_RESULT result) 
    {
        if (result != FMOD_OK) 
        {
            std::cerr << "FMOD ERROR " << result << std::endl;
            cout << "FMOD ERROR " << result << endl;
            return 1;
        }
        // cout << "FMOD all good" << endl;
        return 0;
    }

    void fmodstudioclass::cShutdown() 
    {
        //delete sgpImplementation;

        if (sgpImplementation) 
        {
            delete sgpImplementation;
            sgpImplementation = nullptr;
        }
    }
}