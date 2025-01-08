#pragma once
/*!***************************************************************************
    \file			Bone.h
    \author			Leong Wai Kit
    \par			l.waikit\@digipen.edu
    \brief			Declaration of the Bone class, responsible for managing
                    skeletal animation playback and bone transformations within
                    the Aeon game engine. This class declares functions and
                    data members for handling animations, playing, pausing,
                    and stopping animations, and updating bone matrices.

    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Core/Log.h"

#include <Geom/geom_compiler.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace AeonCore
{
    struct KeyPosition
    {
        glm::vec3   position;
        float       timeStamp;
    };

    struct KeyRotation
    {
        glm::quat   orientation;
        float       timeStamp;
    };

    struct KeyScale
    {
        glm::vec3   scale;
        float       timeStamp;
    };

    class Bone
    {
    public:
        Bone(const std::string& _name, int _ID, const aiNodeAnim* _channel);

        /*
         *  interpolates b/w positions,rotations & scaling keys based on the curren time of
            the animation and prepares the local transformation matrix by combining all keys
            tranformations
         */
        void Update(float _animationTime);

        glm::mat4 GetLocalTransform() { return mLocalTransform; }
        std::string GetBoneName() const { return mName; }
        int GetBoneID() { return mID; }

        /*
         *  Gets the current index on mKeyPositions to interpolate to based on
            the current animation time
        */
        int GetPositionIndex(float _animationTime);

        /*
         *  Gets the current index on mKeyRotations to interpolate to based on the
            current animation time
         */
        int GetRotationIndex(float _animationTime);

        /*
         *  Gets the current index on mKeyScalings to interpolate to based on the
            current animation time
         */
        int GetScaleIndex(float _animationTime);

    private:
        std::vector<KeyPosition> mPositions;
        std::vector<KeyRotation> mRotations;
        std::vector<KeyScale> mScales;
        int mNumPositions;
        int mNumRotations;
        int mNumScalings;

        glm::mat4 mLocalTransform;
        std::string mName;
        int mID;

        //  Gets normalized value for Lerp & Slerp
        float GetScaleFactor(float _lastTimeStamp, float _nextTimeStamp, float _animationTime);

        //  Figures out which position keys to interpolate b/w and performs the interpolation
        //  and returns the translation matrix
        glm::mat4 InterpolatePosition(float _animationTime);

        //  Figures out which rotations keys to interpolate b/w and performs the interpolation and returns the rotation matrix
        glm::mat4 InterpolateRotation(float _animationTime);

        //  Figures out which scaling keys to interpolate b/w and performs the interpolation and returns the scale matrix
        glm::mat4 InterpolateScaling(float _animationTime);
    };

    struct AssimpNodeData
    {
        glm::mat4 transformation{};
        std::string name;
        int childrenCount{};
        std::vector<AssimpNodeData> children;
    };
}