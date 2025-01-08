#include "acpch.h"
/*!***************************************************************************
	\file            ScriptingSystem.cpp
	\author          Lam En Qing
	\par             enqing.lam\@digipen.edu
	\brief           This file defines the functions in the ScriptingSystem
					 class
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Systems/ScriptingSystem.h"

#include "Core/Application.h"
#include "ECS/Architecture/Coordinator.hpp"
#include "ECS/Components/Behaviour.h"
#include "Scripting/Scripting.h"

namespace AeonCore {
	void ScriptingSystem::Init() {
	}

	void ScriptingSystem::Update() {
		ZoneScopedN("Scripting System Update");

		Application& app = Application::GetApplication();

		if (app.GetRuntimeState() != RuntimeState::Start /*|| app.GetLoadStatus() != LoadStatus::NotRunning*/) {
			return;
		}

		// Invoke Update function of the Behaviour classes
		for (EntityID entity : mEntities) {
			Behaviour& behaviourComp = Coordinator::GetInstance().GetComponent<Behaviour>(entity);
			behaviourComp.InvokeUpdate();
		}

		Scripting::InvokeEventFunction("ScriptPostUpdate");
	}

	void ScriptingSystem::Destroy() {
	}
}