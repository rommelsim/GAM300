#pragma once
/*!***************************************************************************
    \file			Animation.cpp
    \author			Leong Wai Kit
    \par			l.waikit\@digipen.edu
    \brief			Header file for the Bone and Animation classes. These
					classes are essential components of the animation system,
					handling bone transformations and animation data for 3D
					models. The Bone class represents individual bones,
					including their positions, rotations, and scales over time.
					The Animation class manages animation data, including
					keyframes, bones, and animation playback. These classes are
					integral to enabling skeletal animation in the Aeon game
					engine.

    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Core/Log.h"

#include "Animation/Bone.h"

#include <assimp/anim.h>
#include <glm/glm.hpp>
#include <Geom/geom_compiler.h>
#include <glm/gtx/quaternion.hpp>

namespace AeonCore
{
	class AModel;

    class Animation
    {
    public:
        Animation(const std::string& _animationPath, AModel* _model);

        Bone* FindBone(const std::string& _name);
        //std::shared_ptr<Bone> FindBone(const std::string& _name);

        inline float GetTicksPerSecond() { return static_cast<float>(mTicksPerSecond); }
        inline float GetDuration() { return mDuration; }
        inline const AssimpNodeData& GetRootNode() { return mRootNode; }
        inline const std::map<std::string, Geometry::BoneInfo>& GetBoneIDMap() { return mBoneInfoMap; }

    private:
        float mDuration;
        int mTicksPerSecond;
        std::vector<Bone> mBones;

        AssimpNodeData mRootNode;
        std::map<std::string, Geometry::BoneInfo> mBoneInfoMap;

        void ReadMissingBones(const aiAnimation* _animation, AModel& _model);
        void ReadHierarchyData(AssimpNodeData& _dest, const aiNode* _src);
    };

    
}
