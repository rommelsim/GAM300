#pragma once
/*!***************************************************************************
	\file			AudioSystem.h
	\author			Wang Yi Long Roy
	\par			w.yilongroy\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of function declarations for
                    calling audio component functions.


	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "ECS/Architecture/System.hpp"

// will look at all the object in aundio component so the system can update


namespace AeonCore
{
    class AudioSystem : public System
    {
    public:
        AudioSystem() = default;

        void Init() override;
        void Update() override;
        void Destroy() override;

        // Used by scripting to pause any playing sounds, and then unpause them
        void OnGamePause();
        void OnGameUnpause();

        // Used by scenemanager when a cached scene is active & inactive
        void OnCachedSceneActive();
        void OnScenedSceneInactive();
    private:
        //FMODCoreClass audioInstance; // Declare audioInstance as a member variable
        bool hasOnAwakeRun;
    };
}
