#pragma once
/*!***************************************************************************
    \file			Animator.h
    \author			Leong Wai Kit
    \par			l.waikit\@digipen.edu
    \brief			Definition of the Animator class, responsible for managing
					skeletal animation playback and bone transformations within
					the Aeon game engine. This class declares functions and
					data members for handling animations, playing, pausing,
					and stopping animations, and updating bone matrices.

    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "ECS/Components/Animation.h"

namespace AeonCore
{
    class Animator
    {
    public:
    	Animator(Animation* _animation);

        void UpdateAnimation(float _dt);

        void PlayAnimation(Animation* _pAnimation);

        void CalculateBoneTransform(const AssimpNodeData* _node, glm::mat4 _parentTransform);

        std::vector<glm::mat4> GetFinalBoneMatrices() { return mFinalBoneMatrices; }

    private:
        std::vector<glm::mat4> mFinalBoneMatrices;
        Animation* mCurrentAnimation;
        float mCurrentTime;
        float mDeltaTime;
    };
}
