#include "acpch.h"
/*!***************************************************************************
    \file			Animation.cpp
    \author			Leong Wai Kit
    \par			l.waikit\@digipen.edu
    \brief			Implementation of the Bone and Animation classes. These
					classes are part of the animation system and handle bone
					transformations and animation data for 3D models. The Bone
					class represents individual bones, including their
					positions, rotations, and scales over time. The Animation
					class manages animation data, including keyframes, bones,
					and animation playback. These classes are essential for
					skeletal animation in the Aeon game engine.

    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "ECS/Components/Animation.h"
#include "Resource/AModel.h"

namespace AeonCore
{
    Animation::Animation(const std::string& _animationPath, AModel* _model)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(_animationPath, aiProcess_Triangulate);
        assert(scene && scene->mRootNode);
        auto animation = scene->mAnimations[0];
        mDuration = static_cast<float>(animation->mDuration);
        mTicksPerSecond = static_cast<int>(animation->mTicksPerSecond);
        aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
        globalTransformation = globalTransformation.Inverse();
        ReadHierarchyData(mRootNode, scene->mRootNode);
        ReadMissingBones(animation, *_model);
    }

    Bone* Animation::FindBone(const std::string& _name)
    {
        auto iter = std::find_if(mBones.begin(), mBones.end(),
            [&](const Bone& Bone)
            {
                return Bone.GetBoneName() == _name;
            }
        );
        if (iter == mBones.end()) return nullptr;
        else return &(*iter);
    }

    /*std::shared_ptr<Bone> Animation::FindBone(const std::string& _name)
    {
        auto iter = std::find_if(mBones.begin(), mBones.end(),
            [&](const Bone& bone)
            {
                return bone.GetBoneName() == _name;
            }
        );
        if (iter == mBones.end()) return nullptr;
        return std::make_shared<Bone>(*iter);
    }*/

    //  PRIVATE FUNCTIONS FOR ANIMATION CLASS

    void Animation::ReadMissingBones(const aiAnimation* _animation, AModel& _model)
    {
        int size = _animation->mNumChannels;

        auto& boneInfoMap = _model.GetBoneInfoMap();//getting m_BoneInfoMap from Model class
        int& boneCount = _model.GetBoneCount(); //getting the m_BoneCounter from Model class

        //reading channels(bones engaged in an animation and their keyframes)
        for (int i = 0; i < size; i++)
        {
            auto channel = _animation->mChannels[i];
            std::string boneName = channel->mNodeName.data;

            if (boneInfoMap.find(boneName) == boneInfoMap.end())
            {
                boneInfoMap[boneName].id = boneCount;
                boneCount++;
            }
            mBones.push_back(Bone(channel->mNodeName.data,
                boneInfoMap[channel->mNodeName.data].id, channel));
        }

        mBoneInfoMap = boneInfoMap;
    }

    void Animation::ReadHierarchyData(AssimpNodeData& _dest, const aiNode* _src)
    {
        AeonCore_ASSERT(_src, "src is nullptr")

    	_dest.name = _src->mName.data;
        _dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(_src->mTransformation);
        _dest.childrenCount = static_cast<int>(_src->mNumChildren);

        for (unsigned int i = 0; i < _src->mNumChildren; i++)
        {
            AssimpNodeData newData;
            ReadHierarchyData(newData, _src->mChildren[i]);
            _dest.children.push_back(newData);
        }
    }

    
}

