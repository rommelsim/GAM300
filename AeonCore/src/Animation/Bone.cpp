#include "acpch.h"
/*!***************************************************************************
    \file			Bone.cpp
    \author			Leong Wai Kit
    \par			l.waikit\@digipen.edu
    \brief			Definition of the Bone class, responsible for managing
                    skeletal animation playback and bone transformations within
                    the Aeon game engine. This class declares functions and
                    data members for handling animations, playing, pausing,
                    and stopping animations, and updating bone matrices.

    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Animation/Bone.h"

namespace AeonCore
{
    //  FUNCTIONS FOR BONE CLASS

    Bone::Bone(const std::string& _name, int _ID, const aiNodeAnim* _channel)
        : mName(_name), mID(_ID), mLocalTransform(1.0f)
    {
        mNumPositions = _channel->mNumPositionKeys;

        for (int positionIndex = 0; positionIndex < mNumPositions; ++positionIndex)
        {
            aiVector3D aiPosition = _channel->mPositionKeys[positionIndex].mValue;
            float timeStamp = static_cast<float>(_channel->mPositionKeys[positionIndex].mTime);
            KeyPosition data;
            data.position = AssimpGLMHelpers::GetGLMVec(aiPosition);
            data.timeStamp = timeStamp;
            mPositions.push_back(data);
        }

        mNumRotations = _channel->mNumRotationKeys;
        for (int rotationIndex = 0; rotationIndex < mNumRotations; ++rotationIndex)
        {
            aiQuaternion aiOrientation = _channel->mRotationKeys[rotationIndex].mValue;
            float timeStamp = static_cast<float>(_channel->mRotationKeys[rotationIndex].mTime);
            KeyRotation data;
            data.orientation = AssimpGLMHelpers::GetGLMQuat(aiOrientation);
            data.timeStamp = timeStamp;
            mRotations.push_back(data);
        }

        mNumScalings = _channel->mNumScalingKeys;
        for (int keyIndex = 0; keyIndex < mNumScalings; ++keyIndex)
        {
            aiVector3D scale = _channel->mScalingKeys[keyIndex].mValue;
            float timeStamp = static_cast<float>(_channel->mScalingKeys[keyIndex].mTime);
            KeyScale data;
            data.scale = AssimpGLMHelpers::GetGLMVec(scale);
            data.timeStamp = timeStamp;
            mScales.push_back(data);
        }
    }

    void Bone::Update(float _animationTime)
    {
        glm::mat4 translation = InterpolatePosition(_animationTime);
        glm::mat4 rotation = InterpolateRotation(_animationTime);
        glm::mat4 scale = InterpolateScaling(_animationTime);
        mLocalTransform = translation * rotation * scale;
    }

    int Bone::GetPositionIndex(float _animationTime)
    {
        for (int index = 0; index < mNumPositions - 1; ++index)
        {
            if (_animationTime < mPositions[index + 1].timeStamp)
                return index;
        }
        //AeonCore_ASSERT(false, "No position index found for the given animation time")
    	return 0;
    }

    int Bone::GetRotationIndex(float _animationTime)
    {
        for (int index = 0; index < mNumRotations - 1; ++index)
        {
            if (_animationTime < mRotations[index + 1].timeStamp)
                return index;
        }
        //AeonCore_ASSERT(false, "No rotation index found for the given animation time")
    	return 0;
    }

    int Bone::GetScaleIndex(float _animationTime)
    {
        for (int index = 0; index < mNumScalings - 1; ++index)
        {
            if (_animationTime < mScales[index + 1].timeStamp)
                return index;
        }
        //AeonCore_ASSERT(false, "No scale index found for the given animation time")
    	return 0;
    }

    //  PRIVATE FUNCTIONS FOR BONE CLASS

    float Bone::GetScaleFactor(float _lastTimeStamp, float _nextTimeStamp, float _animationTime)
    {
        float scaleFactor = 0.0f;
        float midWayLength = _animationTime - _lastTimeStamp;
        float framesDiff = _nextTimeStamp - _lastTimeStamp;
        scaleFactor = midWayLength / framesDiff;
        return scaleFactor;
    }

    glm::mat4 Bone::InterpolatePosition(float _animationTime)
    {
        if (1 == mNumPositions)
            return glm::translate(glm::mat4(1.0f), mPositions[0].position);

        int p0Index = GetPositionIndex(_animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(mPositions[p0Index].timeStamp,
            mPositions[p1Index].timeStamp, _animationTime);
        glm::vec3 finalPosition = glm::mix(mPositions[p0Index].position, mPositions[p1Index].position
            , scaleFactor);
        return glm::translate(glm::mat4(1.0f), finalPosition);
    }

    glm::mat4 Bone::InterpolateRotation(float _animationTime)
    {
        if (1 == mNumRotations)
        {
            auto rotation = glm::normalize(mRotations[0].orientation);
            return glm::toMat4(rotation);
        }

        int p0Index = GetRotationIndex(_animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(mRotations[p0Index].timeStamp, mRotations[p1Index].timeStamp, _animationTime);
        glm::quat finalRotation = glm::slerp(mRotations[p0Index].orientation, mRotations[p1Index].orientation, scaleFactor);
        finalRotation = glm::normalize(finalRotation);
        return glm::toMat4(finalRotation);
    }

    glm::mat4 Bone::InterpolateScaling(float _animationTime)
    {
        if (1 == mNumScalings)
            return glm::scale(glm::mat4(1.0f), mScales[0].scale);

        int p0Index = GetScaleIndex(_animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(mScales[p0Index].timeStamp, mScales[p1Index].timeStamp, _animationTime);
        glm::vec3 finalScale = glm::mix(mScales[p0Index].scale, mScales[p1Index].scale, scaleFactor);
        return glm::scale(glm::mat4(1.0f), finalScale);
    }
}