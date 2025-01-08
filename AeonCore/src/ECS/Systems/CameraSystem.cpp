#include "acpch.h"
/*!***************************************************************************
	\file			CameraSystem.cpp
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\brief			Implementation of the CameraSystem responsible for
					managing cameras. This file contains the implementation of
					the CameraSystem, which is responsible for managing
					cameras. It initializes the default active camera as the
					editor camera and updates the positions of cameras based on
					associated entities and transformations.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "ECS/Systems/CameraSystem.h"

#include "Core/Application.h"
#include "Scripting/Scripting.h"

namespace AeonCore
{
	CameraManager& CamManager = CameraManager::GetInstance();
	EditorCamera& editorCam = EditorCamera::GetInstance();

	void CameraSystem::Init()
	{
		//for (auto& entity : coord.GetEntities())
		//{
		//	//if (coord.HasComponent<Camera>(entity))
		//	//{
		//	//	//	Add in any camera added into camera manager
		//	//	Camera& cam = coord.GetComponent<Camera>(entity);
		//	//	CamManager.AddCamera(entity.GetEntityID(), &cam);

		//	//	//if (!coord.HasComponent<Transform>(entity))
		//	//	//	continue;

		//	//	////	Update Camera to follow entity
		//	//	//Transform& trans = coord.GetComponent<Transform>(entity);
		//	//	//cam.SetCameraTarget(trans.GetPosition());
		//	//	//cam.SetCameraPosition(trans.GetPosition());
		//	//}
		//}

		// only if the game is running, then we request to switdch
		if (Application::GetApplication().GetRuntimeState() == RuntimeState::Start)
		{
			CamManager.SetRequestSwitch(true);
		}
		else
		{
			CamManager.SetActiveCamera(&editorCam);
		}
	}

	void CameraSystem::Update()
	{
		////	reset back to editor camera from runtime
		//if (Application::GetApplication().GetRuntimeState() != RuntimeState::Start)
		//{
		//	CamManager.SetActiveCamera(&editorCam);
		//}
		ZoneScopedN("Camera Update");

		/*if (Application::GetApplication().GetLoadStatus() != LoadStatus::NotRunning)
			return;*/

		Coordinator& coord = Coordinator::GetInstance();
		for (auto entity : mEntities)
		{
			//	Add in any camera added into camera manager
			Camera& cam = coord.GetComponent<Camera>(entity);
			CamManager.AddCamera(entity, &cam);

			if (!coord.HasComponent<Transform>(entity))
				continue;

			//	Update Camera to follow entity
			Transform& trans = coord.GetComponent<Transform>(entity);
			cam.SetCameraPosition(trans.GetPosition());

			//	update camera inside camera manager
			for (auto cam1 : CamManager.GetCameraList())
			{
				if (cam1.first == entity)
				{
					cam1.second->SetCameraPosition(trans.GetPosition());
					break;
				}
			}
		}

		/*Camera& activeCam = CamManager.GetActiveCamera();
		activeCam.UpdateViewMatrix();
		activeCam.UpdateProjectionMatrix();*/

		//for (auto& entity : coord.GetEntities())
		//{
		//	if (coord.HasComponent<Camera>(entity))
		//	{
		//		//	Add in any camera added into camera manager
		//		Camera& cam = coord.GetComponent<Camera>(entity);
		//		CamManager.AddCamera(entity.GetEntityID(), &cam);

		//		if (!coord.HasComponent<Transform>(entity))
		//			continue;

		//		//	Update Camera to follow entity
		//		Transform& trans = coord.GetComponent<Transform>(entity);

		//		if(coord.HasComponent<Behaviour>(entity))
		//		{
		//			Behaviour& beh = coord.GetComponent<Behaviour>(entity);
		//			if (Scripting::GetBehaviourInstances(entity.GetEntityID()) == nullptr)
		//				continue;

		//			for(auto& player : *Scripting::GetBehaviourInstances(entity.GetEntityID()))
		//			{
		//				if(player->GetFieldValueStr("PlayerName") != "")
		//				{
		//					//	valid player here
		//					if(coord.GetEntityByName(player->GetFieldValueStr("PlayerName")) != nullptr)
		//					{
		//						Entity player_1 = *coord.GetEntityByName(player->GetFieldValueStr("PlayerName"));
		//						Transform& transp1 = coord.GetComponent<Transform>(player_1);
		//						cam = CamManager.GetActiveCamera();
		//						cam.SetCameraTarget(transp1.GetPosition());
		//						//cam.SetCameraPosition(transp1.GetPosition());
		//					}
		//				}
		//			}
		//		}
		//	}
		//}

		//	Game Camera Functionality
		/*----------------------------------------------------------------------------------------------------
		 *	In order for game camera to function, requires 2 players
		----------------------------------------------------------------------------------------------------*/
		{
			for (auto& camera : CamManager.GetCameraList())
			{
				Camera& cam = *camera.second;

				switch (cam.GetCameraRole())
				{
				case CameraRole::CAMERA:
					mGameCamera = &cam;
					cam.SetCameraRoleSelected(true);
					break;

				/*case CameraRole::NONE:
					mGameCamera = nullptr;
					cam.SetCameraRoleSelected(false);
					break;*/
				}
			}

			
			

			if(Application::GetApplication().GetRuntimeState() == RuntimeState::Start)
			{
				if (CamManager.GetRequestSwitch())
				{
					if (mGameCamera != nullptr)
					{
						CamManager.SetActiveCamera(mGameCamera);
						CamManager.SetRequestSwitch(false);
					}
					else
					{
						CamManager.SetActiveCamera(&editorCam);
						CamManager.SetRequestSwitch(false);
					}
				}
				
				//if(mFollowCam != nullptr &&
				//	mPlayer1 != nullptr &&
				//	mPlayer2 != nullptr)
				//{
				//	
				//	//mFollowCam->Follow2Players(mPlayer1->GetCameraPosition(), mPlayer2->GetCameraPosition());
				//}
			}
		}
	}

	void CameraSystem::Destroy()
	{
		CamManager.ClearCameraList();
		mGameCamera = nullptr;
		CamManager.SetActiveCamera(&editorCam);

		// only if the game is running, then we request to switch
		if (Application::GetApplication().GetRuntimeState() == RuntimeState::Start)
			CamManager.SetRequestSwitch(true);
	}
}
