#pragma once
/*!***************************************************************************
	\file			CameraManager.h
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\brief			Declaration of the CameraManager class, responsible for
					managing and switching between cameras. This class defines
					functions to access, add, and switch cameras.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Core/AeonAPI.h"
#include "core/Singleton.hpp"
#include "Core/Log.h"

#include "ECS/Architecture/Coordinator.hpp"
#include "ECS/Components/Camera.h"

#include "Editor/EditorCamera.h"

namespace AeonCore
{
	class AEON_API CameraManager : public Singleton<CameraManager>
	{
	public:
		/*-----------------------------------------------------------------------------------------------
		 *	Constructor
		 -----------------------------------------------------------------------------------------------*/

		CameraManager() = default;

		/*-----------------------------------------------------------------------------------------------
		 *	Destructor
		 -----------------------------------------------------------------------------------------------*/

		~CameraManager() = default;

		/*-----------------------------------------------------------------------------------------------
		 *	Get Functions
		 -----------------------------------------------------------------------------------------------*/

		inline std::map<EntityID, Camera*>	GetCameraList()					{ return mCameraList; }
		inline Camera&						GetActiveCamera()				{ return *mActiveCamera; }
		inline static bool					GetEnableEntityCam()			{ return mEnableEntityCam; }
		inline static bool					GetSwitchEntityCam()			{ return mSwitchEntityCam; }
		inline EntityID						GetActiveCameraID()				{ return mCurrentCamID; }

		Camera&								GetCamera(EntityID _id);

		inline static bool GetRequestSwitch() { return mRequestSwitch;}

		void SwitchGameCamera(EntityID _id)
		{
			auto it = mCameraList.find(_id);
			if (it != mCameraList.end())
			{
				GetActiveCamera().SetCameraRole(CameraRole::NONE);
				it->second->SetCameraRole(CameraRole::CAMERA);
			}
		}

		/*-----------------------------------------------------------------------------------------------
		 *	Set Functions
		 -----------------------------------------------------------------------------------------------*/

		inline void							SetActiveCamera(Camera* _cam)				{ mActiveCamera = _cam; }
		inline void							SetEnableEntityCam(const bool _EnableEntityCam)	{ mEnableEntityCam = _EnableEntityCam; }
		inline void							SetSwitchEntityCam(bool _SwitchEntityCam)	{ mSwitchEntityCam = _SwitchEntityCam; }
		inline void	SetRequestSwitch(bool _flag) { mRequestSwitch = _flag; }

		/*-----------------------------------------------------------------------------------------------
		 *	Other Functions
		 -----------------------------------------------------------------------------------------------*/

		void AddCamera(EntityID _id, Camera* _camera);
		void SwitchCamera(EntityID _id);
		void RemoveCamera(Camera* _camera);
		void ClearCameraList();

	private:
		std::map<EntityID, Camera*> mCameraList;
		Camera* mActiveCamera;

		inline static bool mEnableEntityCam = false;
		inline static bool mSwitchEntityCam = false;

		EntityID mCurrentCamID;

		inline static bool mRequestSwitch{ false };
	};
}