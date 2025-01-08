#include "acpch.h"
/*!***************************************************************************
	\file			CameraManager.cpp
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\brief			Implementation of the CameraManager class, responsible for
					managing and switching between cameras. This class defines
					functions to access, add, and switch cameras, as well as
					handling warnings for camera not found.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Camera/CameraManager.h"

namespace AeonCore
{
	Camera& CameraManager::GetCamera(EntityID _id)
	{
		auto it = mCameraList.find(_id);
		if (it != mCameraList.end())
			return *it->second;
		else
		{
			AeonCore_WARN("Warning : Camera {0} not found in Camera manager...", Coordinator::GetInstance().GetEntity(_id)->GetEntityName());
			return EditorCamera::GetInstance();
		}
	}

	void CameraManager::AddCamera(EntityID _id, Camera* _camera)
	{
		auto camera = mCameraList.find(_id);
		if (camera == mCameraList.end())
		{
			//	no camera found, add in new camera
			mCameraList[_id] = _camera;
			AeonCore_INFO("Camera {0} added...", Coordinator::GetInstance().GetEntity(_id)->GetEntityName());
		}
	}

	void CameraManager::SwitchCamera(EntityID _id)
	{
		auto it = mCameraList.find(_id);
		if (it != mCameraList.end())
		{
			mActiveCamera = it->second;
			mCurrentCamID = _id;
		}
		
			//AeonCore_WARN("Warning : Camera {0} not found in Camera manager...", Coordinator::GetInstance().GetEntity(_id)->GetEntityName());
	}

	void CameraManager::RemoveCamera(Camera* _camera)
	{
		if (_camera == nullptr) return;

		for (auto it = mCameraList.begin(); it != mCameraList.end();)
		{
			if (it->second == _camera)
			{
				it = mCameraList.erase(it); // erase returns the next valid iterator
				break; // exit the loop after erasing the camera
			}
			else
			{
				++it; // move to the next element
			}
		}
	}

	void CameraManager::ClearCameraList() {
		mCameraList.clear();
	}
}
