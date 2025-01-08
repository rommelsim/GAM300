#include "acpch.h"
/*!***************************************************************************
    \file			Animator.cpp
    \author			Leong Wai Kit
    \par			l.waikit\@digipen.edu
    \brief			Implementation of the Animator class responsible for
					managing skeletal animation playback and bone
					transformations. This class handles the updating of bone
					matrices based on animations and provides the functionality
					to play, pause, and stop animations.

    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Animation/Animator.h"

namespace AeonCore
{
    Animator::Animator(Animation* _animation): mDeltaTime(0)
    {
	    mCurrentTime = 0.0f;
	    mCurrentAnimation = _animation;

	    mFinalBoneMatrices.reserve(100);

	    for (int i = 0; i < 100; i++)
		    mFinalBoneMatrices.push_back(glm::mat4(1.0f));
    }

    void Animator::UpdateAnimation(float _dt)
    {
        mDeltaTime = _dt;
        if (mCurrentAnimation)
        {
            mCurrentTime += mCurrentAnimation->GetTicksPerSecond() * _dt;
            mCurrentTime = fmod(mCurrentTime, mCurrentAnimation->GetDuration());
            CalculateBoneTransform(&mCurrentAnimation->GetRootNode(), glm::mat4(1.0f));
        }
    }

    void Animator::PlayAnimation(Animation * _pAnimation)
    {
        mCurrentAnimation = _pAnimation;
        mCurrentTime = 0.0f;
    }

    void Animator::CalculateBoneTransform(const AssimpNodeData * _node, glm::mat4 _parentTransform)
    {
        std::string nodeName = _node->name;
        glm::mat4 nodeTransform = _node->transformation;

        Bone* Bone = mCurrentAnimation->FindBone(nodeName);

        if (Bone)
        {
            Bone->Update(mCurrentTime);
            nodeTransform = Bone->GetLocalTransform();
        }

        glm::mat4 globalTransformation = _parentTransform * nodeTransform;

        auto boneInfoMap = mCurrentAnimation->GetBoneIDMap();
        if (boneInfoMap.find(nodeName) != boneInfoMap.end())
        {
            int index = boneInfoMap[nodeName].id;
            glm::mat4 offset = boneInfoMap[nodeName].offset;
            mFinalBoneMatrices[index] = globalTransformation * offset;
        }

        for (int i = 0; i < _node->childrenCount; i++)
            CalculateBoneTransform(&_node->children[i], globalTransformation);

        /*const std::string& nodeName = _node->name;
        glm::mat4 nodeTransform = _node->transformation;

        std::shared_ptr<Bone> Bone = mCurrentAnimation->FindBone(nodeName);

        if (Bone)
        {
            Bone->Update(mCurrentTime);
            nodeTransform = Bone->GetLocalTransform();
        }

        glm::mat4 globalTransformation = _parentTransform * nodeTransform;

        const auto& boneInfoMap = mCurrentAnimation->GetBoneIDMap();
        if (boneInfoMap.find(nodeName) != boneInfoMap.end())
        {
            int index = boneInfoMap.at(nodeName).id;
            mFinalBoneMatrices[index] = globalTransformation * boneInfoMap.at(nodeName).offset;
        }

        for (int i = 0; i < _node->childrenCount; i++)
            CalculateBoneTransform(&_node->children[i], globalTransformation);*/
    
    }
}