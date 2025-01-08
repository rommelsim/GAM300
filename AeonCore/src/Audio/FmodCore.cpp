#include "acpch.h"
/*!***************************************************************************
	\file			FmodCore.cpp
	\author			Wang Yi Long Roy
	\par			w.yilongroy\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function definitions for
					the wrapper functions for FMOD Core api.


	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Audio/FmodCore.h"

#include "Camera/CameraManager.h"
#include "Resource/ResourceManager.h"
namespace AeonCore
{
	void FMODCoreClass::CInit()
	{
		cErrorCheck(FMOD::System_Create(&m_PtrCoreSystem));
		cErrorCheck(m_PtrCoreSystem->init(1024, FMOD_INIT_3D_RIGHTHANDED, NULL));

		m_ChannelData.insert({ AudioComponent::ChannelType::BGM, ChannelMap{} });
		m_ChannelData.insert({ AudioComponent::ChannelType::SFX, ChannelMap{} });
		m_ChannelData.insert({ AudioComponent::ChannelType::Voice, ChannelMap{} });
	}

	void FMODCoreClass::CUpdate()
	{
		for (auto typeItr = m_ChannelData.begin(); typeItr != m_ChannelData.end(); ++typeItr)
		{
			auto& _channels = typeItr->second;
			vector<ChannelMap::iterator> pStoppedChannels;
			for (auto it = _channels.begin(), itEnd = _channels.end(); it != itEnd; ++it)
			{
				bool bIsPlaying = false;
				it->second.first->isPlaying(&bIsPlaying);
				if (!bIsPlaying)
				{
					pStoppedChannels.push_back(it);
				}
			}
			for (auto& it : pStoppedChannels)
			{
				_channels.erase(it);
			}
		}
		

		cErrorCheck(m_PtrCoreSystem->update());
	}

	void FMODCoreClass::CShutdown()
	{
		// Release all loaded sounds
		SoundMap mSounds = ResourceManager::GetInstance().GetSound();
		for (auto& soundPair : mSounds)
		{
			FMOD::Sound* sound = soundPair.second;
			sound->release();
		}
		mSounds.clear(); // Clear the map

		if (m_PtrCoreSystem) {
			m_PtrCoreSystem->release();
			m_PtrCoreSystem = nullptr;
		}
	}

	int FMODCoreClass::CPlaySound_Internal(AudioComponent& audioComp)
	{
		string strSoundName = audioComp.GetSoundFile();
		AudioComponent::ChannelType channelType = audioComp.GetChannelType();
		float volume = audioComp.GetVolume();
			bool bLoop = audioComp.GetLoop();
		float pitch = audioComp.GetPitch();
		float pan = audioComp.GetPan();
		bool mute = audioComp.GetMute();
		bool IIIDMode = audioComp.GetIIIDMode();
		int channelID = audioComp.GetChannelID();

		//int channelvolume = 

		// Increment the channel ID.
		if (channelID == -1) // if the channelID is invalid (we pick -1 is the invalid value)
		{
			channelID = m_nNextChannelId++;
		}
			

		// Try to find the sound in the loaded sounds.
		SoundMap mSounds = ResourceManager::GetInstance().GetSound();
		auto tFoundIt = mSounds.find(strSoundName);

		if (tFoundIt == mSounds.end())
		{
			// If the sound is not found, return without playing.
			return channelID;
		}

		// Create a channel for the sound.
		FMOD::Channel* pChannel = nullptr;
		FMOD_RESULT result = (m_PtrCoreSystem->playSound(tFoundIt->second, nullptr, false, &pChannel));

		//m_ChannelData[channelType][channelID] = volume;

		pChannel->setVolume(volume);
		pChannel->setPitch(pitch);
		pChannel->setPan(pan);
		pChannel->setMute(mute);

		/*Entity* TestEntity2 = gCoordinator.GetEntityByName("Font");
		if ()*/

		//pChannel->set3DAttributes()
		
		// Depending on ChannelType, set the mode, so like copy that AudioType code to set the mode right? Put it here? So like Sound itself is just a sound, no settings tied to it, yeah?
		// Yeah so like, when you play sound, the channel itself decides how it should play the sound, should it be 3d, or looping or.. etc? Maybe this is better? Idk
		//pChannel->setMode();

		//	Check for errors when playing the sound.
		if (cErrorCheck(result) != 0)
		{
			// Handle the error, return the channel ID without playing.
			return channelID;
		}

		/*
			Setting FMOD Modes in Play Sound Function:
			Advantages:
				More flexible: You can adjust the settings for each
				instance of the sound at the time of playback.
				This can be useful when you need different instances
				of the same sound with different settings.
				
				Simplified loading:You can load sounds with default
				settings and modify those settings at runtime based
				on your application's logic.

			Trade-offs:
				Slightly less efficient: It may result in a few extra
				function calls when you play the sound since you need
				to set modes for each instance individually.
				
		*/

		// Determine the appropriate FMOD mode based on ChannelType.
		FMOD_MODE channelMode = FMOD_DEFAULT; // Start with default mode
		channelMode |= bLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		channelMode |= IIIDMode ? FMOD_3D : FMOD_2D;

		if (IIIDMode)
		{
			/*float minDistance = 1.0f;
			float maxDistance = 100.0f;

			pChannel->set3DMinMaxDistance(minDistance, maxDistance);*/
			// Set volume rolloff model to linear
			//pChannel->setMode(FMOD_3D_LINEARROLLOFF);
			channelMode |= FMOD_3D_LINEARROLLOFF | FMOD_3D_WORLDRELATIVE;
		}

		

		// Store the channel in the map.
		const ChannelPair channelPair(pChannel, &audioComp);


		auto channelTypeItr = m_ChannelData.find(channelType);
		if (channelTypeItr != m_ChannelData.end())
		{
			if (!channelTypeItr->second.contains(channelID))
				channelTypeItr->second.insert({ channelID,channelPair });
		}
		else // create if don't have
		{
			ChannelMap channel_map{ {channelID, channelPair} };
			m_ChannelData[channelType] = channel_map;
		}

		switch (channelType)
		{
		case AudioComponent::ChannelType::SFX:
			// Set appropriate modes for sound effects (e.g., non-looping, 2D)
			//SetChannelTypeVolume(AudioComponent::ChannelType::SFX, sfx);
			pChannel->setVolume(volume * GetSFXChannelVolume() * GetMasterVolume());

			break;
		case AudioComponent::ChannelType::BGM:
			// Set modes for background music (e.g., looping, streaming, 2D or 3D based on the type)
			channelMode |= FMOD_CREATESTREAM; // FMOD will load and decode audio data from a file on-the-fly as it's needed for playback.
			pChannel->setVolume(volume * GetBGMChannelVolume() * GetMasterVolume());
			break;
		case AudioComponent::ChannelType::Voice:
			// Set modes for voice (e.g., non-looping, 2D)
			channelMode |= FMOD_CREATESTREAM; // FMOD will load and decode audio data from a file on-the-fly as it's needed for playback.
			pChannel->setVolume(volume* GetVOIChannelVolume()* GetMasterVolume());
			break;
			// Add more cases for other channel types
		}

		pChannel->setMode(channelMode);

		// Return the channel ID for reference.
		return channelID;
	}

	// Function overloading
	int FMODCoreClass::CPlaySound(AudioComponent& audio_comp)
	{
		int channelID = audio_comp.GetChannelID(); // save to variable since we will be getting it a few times
		if (channelID == -1)
		{
			// AudioComponent has no channel, so delegate the work to the other CPlaySound
			channelID = CPlaySound_Internal(audio_comp);

			audio_comp.SetChannelID(channelID);
			return channelID;
		}
		else
		{
			// AudioComponent already has a channel, so stop and play the channel.
			AeonCore_INFO("Channel ID {}", audio_comp.GetChannelID());

			if (IsChannelPlaying(audio_comp))
			{
				// Stop the channel
				StopChannel(audio_comp);
			}

			/*	
				FMOD::Channel* becomes invalid in two ways:
				1. When the channel finishes playing the sound
				2. When the channel is asked to stopped
				Because the pointer becomes invalid, we must recreate the pointer
			*/

			// Recreate the FMOD::Channel* by delegating the work to the other CPlaySound
			channelID = CPlaySound_Internal(audio_comp);
			audio_comp.SetChannelID(channelID);

			// Return the channel ID or an error code if needed
			return audio_comp.GetChannelID();
		}
	}

	void FMODCoreClass::Set3DListenerAttribute(	const FMOD_VECTOR& position,
												const FMOD_VECTOR& velocity,
												const FMOD_VECTOR& forward,
												const FMOD_VECTOR& up)
	{
		// Check if the FMOD system has been initialized
		if (!m_PtrCoreSystem)
		{
			// Handle error or return, depending on your requirements
			return;
		}
		// Set the 3D listener attributes
		cErrorCheck(m_PtrCoreSystem->set3DListenerAttributes(0, &position, &velocity, &forward, &up));
	}

	void FMODCoreClass::Set3DAttributes(FMOD::Channel* pChannel, 
										const FMOD_VECTOR& position, 
										const FMOD_VECTOR& velocity)
	{
		// Check if the channel and system pointers are valid
		if (!pChannel || !m_PtrCoreSystem)
		{
			// Handle error or return
			AeonCore_ERROR("No Vaild Channel or Sys Ptr!");
			return;
		}

		// Set 3D attributes for the channel
		pChannel->set3DAttributes(&position, &velocity);
	}



	//FMOD::Channel* pChannel,
	void FMODCoreClass::CFadeIn(AudioComponent::ChannelType channelType, int nChannelId, float fadeDuration, float targetVolume)
	{
		auto& _channels = m_ChannelData.at(channelType);
		auto it = _channels.find(nChannelId);
		if (it != _channels.end())
		{
			FMOD::Channel* pChannel = it->second.first;

			// Get the current DSP clock and rate
			unsigned long long dspclock = 0;
			int rate = 0;
			FMOD_RESULT result = pChannel->getSystemObject(&m_PtrCoreSystem);
			result = m_PtrCoreSystem->getSoftwareFormat(&rate, 0, 0);				// Get mixer rate
			result = pChannel->getDSPClock(0, &dspclock);							// Get the reference clock, which is the parent channel group
			result = pChannel->addFadePoint(dspclock, 0.0f);						// Add a fade point at 'now' with full volume.
			result = pChannel->addFadePoint(static_cast<long long>(dspclock + (rate * fadeDuration)), targetVolume);// Add a fade point 5 seconds later at 0 volume.
			//result = pChannel->setPaused(false);
		}
	}

	void FMODCoreClass::CFadeOut(AudioComponent::ChannelType channelType, int nChannelId, float fadeDuration, float targetVolume)
	{
		auto& _channels = m_ChannelData.at(channelType);
		auto it = _channels.find(nChannelId);
		if (it != _channels.end())
		{
			FMOD::Channel* pChannel = it->second.first;

			// Get the current DSP clock and rate
			unsigned long long dspclock = 0;
			int rate = 0;

			FMOD_RESULT result = pChannel->getSystemObject(&m_PtrCoreSystem);
			result = m_PtrCoreSystem->getSoftwareFormat(&rate, 0, 0);				// Get mixer rate
			result = pChannel->getDSPClock(0, &dspclock);							// Get the reference clock, which is the parent channel group
			// Add fade points for fading out
			result = pChannel->addFadePoint(dspclock, targetVolume);						// Add a fade point at 'now' with full volume.
			result = pChannel->addFadePoint(static_cast<long long>(dspclock + (rate * fadeDuration)), 0.0f);// Add a fade point 5 seconds later at 0 volume.
			//result = pChannel->setDelay(0, dspclock + (rate + fadeDuration), true);	// Add a delayed stop command at 5 seconds ('stopchannels = true')
		}
	}

	void FMODCoreClass::SetChannelTypeVolume(AudioComponent::ChannelType channelType, float volumePercentage)
	{
		// Check if the FMOD system has been initialized
		if (!m_PtrCoreSystem)
		{
			// Handle error or return, depending on your requirements
			return;
		}

		// Ensure volumeStage is within valid range (0 to 5)
		//volumeStage = std::clamp(volumeStage, 0, 10);

		// Define volume percentages for each stage
		//const float volumePercentages[] = { 0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f, 1.2f, 1.4f, 1.6f, 1.8f, 2.0f };

		// Ensure volumePercentage is within valid range (0 to 2, 1 is default)
		volumePercentage = std::clamp(volumePercentage, 0.0f, 1.0f);

		if (channelType == AudioComponent::ChannelType::BGM)
		{
			bgmVolume = volumePercentage;
		}
		else if (channelType == AudioComponent::ChannelType::SFX)
		{
			sfxVolume = volumePercentage;
		}
		else if (channelType == AudioComponent::ChannelType::Voice)
		{
			voiVolume = volumePercentage;
		}

		// Iterate through all channels and adjust volume for the specified channel type
		auto& _channels = m_ChannelData.at(channelType);
		for (auto& channelIDPair : _channels)
		{
			FMOD::Channel* pChannel = channelIDPair.second.first;
			AudioComponent* audioComp = channelIDPair.second.second;

			switch (channelType)
			{
			case AudioComponent::ChannelType::SFX:
				pChannel->setVolume(audioComp->GetVolume() * sfxVolume * masterVolume);
				break;
			case AudioComponent::ChannelType::BGM:
				pChannel->setVolume(audioComp->GetVolume() * bgmVolume * masterVolume);
				break;
			case AudioComponent::ChannelType::Voice:
				pChannel->setVolume(audioComp->GetVolume() * voiVolume * masterVolume);
				break;
			}

			//if (channelTypeOfChannel == channelType)
			{
				//// Retrieve the current volume for the channel
				//float currentVolume;
				//pChannel->getVolume(&currentVolume);
				//AeonCore_INFO("current volume in channel type{}", currentVolume);

				//// Adjust volume for the specified channel type by multiplying with the percentage
				//float adjustedVolume = currentVolume * volumePercentage;
				//AeonCore_INFO("adjusted volume in channel type{}", adjustedVolume);
				//// Set the adjusted volume for the channel
				//pChannel->setVolume(adjustedVolume);

				// Retrieve the original volume from m_ChannelData
				//map<int, pair<FMOD::Channel*, AudioComponent::ChannelType>>::mapped_type originalVolume = m_ChannelData[
					//channelType][channelIDPair.first];

				// Adjust volume for the specified channel type by multiplying with the percentage
				//float adjustedVolume = originalVolume * volumePercentage;

				// Set the adjusted volume for the channel
				//pChannel->setVolume(audioComp->GetVolume());
			}
		}
	}

	void FMODCoreClass::SetChannelMasterVolume(float masterVolumePercentage)
	{
		// Check if the FMOD system has been initialized
		if (!m_PtrCoreSystem)
		{
			// Handle error or return, depending on your requirements
			return;
		}

		masterVolume = std::clamp(masterVolumePercentage, 0.0f, 1.0f);

		SetChannelTypeVolume(AudioComponent::ChannelType::BGM, GetBGMChannelVolume());
		SetChannelTypeVolume(AudioComponent::ChannelType::SFX, GetSFXChannelVolume());
		SetChannelTypeVolume(AudioComponent::ChannelType::Voice, GetVOIChannelVolume());
	}

	// UNUSED - For Now
	void FMODCoreClass::SetChannelVolume(AudioComponent::ChannelType channelType, int nChannelId, float volume)
	{
		auto& _channels = m_ChannelData.at(channelType);
		auto it = _channels.find(nChannelId);
		if (it != _channels.end())
		{
			FMOD::Channel* pChannel = it->second.first;
			pChannel->setVolume(volume);
		}
	}

	// UNUSED - For Now
	void FMODCoreClass::CUnloadSound(const std::string& strSoundName)
	{
		SoundMap mSounds = ResourceManager::GetInstance().GetSound();
		auto tFoundIt = mSounds.find(strSoundName);
		if (tFoundIt != mSounds.end())
		{
			FMOD::Sound* pSound = tFoundIt->second;
			pSound->release(); // Release the sound resource
			mSounds.erase(tFoundIt); // Remove the sound entry from the map
		}
	}

	// USED
	void FMODCoreClass::StopChannel(AudioComponent& audio_comp)
	{
		auto& channelmap = m_ChannelData.at(audio_comp.GetChannelType());
		auto findchannel = channelmap.find(audio_comp.GetChannelID());
		if (findchannel != channelmap.end())
		{
			FMOD::Channel* pChannel = findchannel->second.first;
			pChannel->stop();
			channelmap.erase(findchannel);
		}
	}

	void FMODCoreClass::StopAllChannels()
	{
		for (auto typeItr = m_ChannelData.begin(); typeItr != m_ChannelData.end(); ++typeItr)
		{
			auto& _channels = typeItr->second;
			for (auto it = _channels.begin(), itEnd = _channels.end(); it != itEnd; ++it)
			{
				FMOD::Channel* pChannel = it->second.first;
				pChannel->stop();
			}
			_channels.clear();
		}
	}

	FMOD::Channel* FMODCoreClass::GetChannel(AudioComponent& audio_comp)
	{
		auto& channelmap = m_ChannelData.at(audio_comp.GetChannelType());
		auto findchannel = channelmap.find(audio_comp.GetChannelID());
		//auto it = m_Channels.find(nChannelId);
		FMOD::Channel* pChannel;
		if (findchannel != channelmap.end())
		{
			pChannel = findchannel->second.first;
		}
		else
		{
			pChannel = nullptr;
		}
		return pChannel;
	}

	void FMODCoreClass::UpdateChannelType(AudioComponent& audio_comp, AudioComponent::ChannelType newType) {
		auto& oldChannelTypeMap = m_ChannelData.at(audio_comp.GetChannelType());
		auto findchannel = oldChannelTypeMap.find(audio_comp.GetChannelID());

		FMOD::Channel* pChannel{nullptr};
		if (findchannel != oldChannelTypeMap.end())
		{
			pChannel = findchannel->second.first;
		}

		if (pChannel) {
			// remove the channel entry in the old channel type
			oldChannelTypeMap.erase(findchannel);

			auto& newChannelTypeMap = m_ChannelData.at(newType);
			newChannelTypeMap.insert({ audio_comp.GetChannelID(), { pChannel, &audio_comp } });
		}
	}

	bool FMODCoreClass::IsChannelPlaying(AudioComponent& audio_comp)
	{
		auto& channelmap = m_ChannelData.at(audio_comp.GetChannelType());
		auto findchannel = channelmap.find(audio_comp.GetChannelID());
		if (findchannel != channelmap.end())
		{
			FMOD::Channel* pChannel = findchannel->second.first;
			bool isPlaying = false;
			pChannel->isPlaying(&isPlaying);
			return isPlaying;
		}

		// If the channel is not found, return false (not playing).
		return false;
	}

	

	FMODCoreClass::AudioState FMODCoreClass::GetAudioState(FMOD::Channel* pChannel)
	{
		bool isPlaying = false;
		bool isPaused = false;

		pChannel->isPlaying(&isPlaying);
		pChannel->getPaused(&isPaused);

		if (isPlaying)
		{
			if (isPaused)
			{
				return FMODCoreClass::AudioState::PAUSED;
			}
			else
			{
				return FMODCoreClass::AudioState::PLAYING;
			}
		}
		else
		{
			return FMODCoreClass::AudioState::STOPPED;
		}
	}

	FMODCoreClass::AudioState FMODCoreClass::GetAudioState(AudioComponent& audioComp) {
		FMOD::Channel* channel = GetChannel(audioComp);

		if (channel) {
			return GetAudioState(channel);
		}

		return AudioState::STOPPED;
	}

	void FMODCoreClass::PauseChannel(AudioComponent& audio_comp)
	{
		auto& channelmap = m_ChannelData.at(audio_comp.GetChannelType());
		auto findchannel = channelmap.find(audio_comp.GetChannelID());
		if (findchannel != channelmap.end())
		{
			FMOD::Channel* pChannel = findchannel->second.first;
			pChannel->setPaused(true);
		}
	}

	void FMODCoreClass::PauseAllChannels()
	{
		for (auto typeItr = m_ChannelData.begin(); typeItr != m_ChannelData.end(); ++typeItr)
		{
			auto& _channels = typeItr->second;
			for (auto it = _channels.begin(), itEnd = _channels.end(); it != itEnd; ++it)
			{
				FMOD::Channel* pChannel = it->second.first;
				pChannel->setPaused(true);
			}
		}
	}

	void FMODCoreClass::ResumeAllChannels()
	{
		for (auto typeItr = m_ChannelData.begin(); typeItr != m_ChannelData.end(); ++typeItr)
		{
			auto& _channels = typeItr->second;
			for (auto it = _channels.begin(), itEnd = _channels.end(); it != itEnd; ++it)
			{
				FMOD::Channel* pChannel = it->second.first;
				pChannel->setPaused(false);
			}
		}
	}

	void FMODCoreClass::ResumeChannel(AudioComponent& audio_comp)
	{
		auto& channelmap = m_ChannelData.at(audio_comp.GetChannelType());
		auto findchannel = channelmap.find(audio_comp.GetChannelID());
		if (findchannel != channelmap.end())
		{
			FMOD::Channel* pChannel = findchannel->second.first;
			pChannel->setPaused(false);
		}
	}

	/*
		This function is to use given vector and convert it to 
		FMOD_VECTOR version of vector to use fmod vector version 
		of calculation.

		One day this will be done but not today.
	*/
	FMOD_VECTOR FMODCoreClass::VectorToFmod(const Vector3& vPosition)
	{
		FMOD_VECTOR fVec;
		fVec.x = vPosition.x;
		fVec.y = vPosition.y;
		fVec.z = vPosition.z;
		return fVec;

		
	}
	/*
		This functions helps with telling me what kinds of error
		I am suppose to recieve when there is an error by returning 
		the error numbers.
	*/
	int FMODCoreClass::cErrorCheck(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			cout << "FMOD ERROR " << result << endl;
			AeonCore_ASSERT(false, "FMOD ERROR {}", static_cast<int>(result))
			return 1;
		}
		// cout << "FMOD all good" << endl;
		return 0;
	}
}


/* Notes To Self todo list
d	static void Init();
d	static void Update();
d	static void Shutdown();
/m	static int ErrorCheck(FMOD_RESULT result);

d	void Loadsound(const string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false);
/m	void UnLoadSound(const string& strSoundName);
td	void Set3dListenerAndOrientation(const Vector3& vPos = Vector3{ 0, 0, 0 }, float fVolumedB = 0.0f); camera
wip	void PlaySound(const string& strSoundName, const Vector3& vPos = Vector3{ 0, 0, 0 }, float fVolumedB = 0.0f);
	void StopChannel(int nChannelId);
	void StopAllChannels();
	void SetChannel3dPosition(int nChannelId, const Vector3& vPosition); object
	void SetChannelvolume(int nChannelId, float fVolumedB);
	bool IsPlaying(int nChannelId) const;
	float dbToVolume(float db);
	float VolumeTodb(float volume);
d	FMOD_VECTOR VectorToFmod(const Vector& vPosition);

*/