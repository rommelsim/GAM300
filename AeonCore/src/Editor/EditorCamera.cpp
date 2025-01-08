#include "acpch.h"
/*****************************************************************//**
 * \file   EditorCamera.h
 * \brief  Basic camera class
 *
 *
 *
 * \author Leong Wai Kit
 * \date   October 2023
 *
 * Copyright (C) 2023 Digipen Insitute Of Technology
 * Reproduction or disclosure of this file or its contents without
 * the prior consent of Digipen Insitute Of Technology is prohibited.
 *********************************************************************/

#include "Editor/EditorCamera.h"
#include "Core/Log.h"

namespace AeonCore
{
    //EditorCamera::EditorCamera(glm::vec3 _position, glm::vec3 _up, float _yaw, float _pitch)
    //    : mFront(glm::vec3(0.f, 0.f, -1.f)), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY), mZoom(ZOOM)
    //{
    //    mPosition = _position;
    //    mWorldUp = _up;
    //    mYaw = _yaw;
    //    mPitch = _pitch;

    //    UpdateCameraVectors();
    //}

    //void EditorCamera::UpdateCameraVectors()
    //{
    //    // calculate the new Front vector
    //    glm::vec3 front;
    //    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    //    front.y = sin(glm::radians(mPitch));
    //    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    //    mFront = glm::normalize(front);
    //    // also re-calculate the Right and Up vector
    //    mRight = glm::normalize(glm::cross(mFront, mWorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    //    mUp = glm::normalize(glm::cross(mRight, mFront));
    //}
    EditorCamera::EditorCamera(glm::vec3 _position, glm::vec3 _up, float _yaw, float _pitch,
        float _fov, float _ar, float _near, float _far)
        : mFront(glm::vec3(0.f, 0.f, -1.f)), mMovementSpeed(20.f), mMouseSensitivity(0.1f), mZoom(45.0f)
    {
        mPosition = _position;
        mWorldUp = _up;
        mYaw = _yaw;
        mPitch = _pitch;

        mFov = _fov;
        mAspectRatio = _ar;
        mNearClip = _near;
        mFarClip = _far;

        UpdateCameraVectors();
    }

    void EditorCamera::SetCameraPosition(glm::vec3 _position)
    {
        mPosition = _position;
        UpdateCameraVectors();
    }

    void EditorCamera::SetCameraTarget(glm::vec3 _target)
    {
        mTarget = _target;
        UpdateViewMatrix();
    }

    void EditorCamera::SetCameraPitch(float _pitch)
    {
        mPitch = _pitch;
        UpdateCameraVectors();
    }

    void EditorCamera::SetCameraYaw(float _yaw)
    {
        mYaw = _yaw;
        UpdateCameraVectors();
    }

    void EditorCamera::UpdateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
        front.y = sin(glm::radians(mPitch));
        front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
        mFront = glm::normalize(front);
        // also re-calculate the Right and Up vector
        mRight = glm::normalize(glm::cross(mFront, mWorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        mUp = glm::normalize(glm::cross(mRight, mFront));

        mTarget = mPosition + mFront;

        UpdateViewMatrix();             // Update view
        UpdateProjectionMatrix();       // update projection
    }
    inline void EditorCamera::SetWindowWidthAndHeight(int width, int height)
    {
        m_width = width;
        m_height = height;
        mAspectRatio = (float)m_width / (float)m_height;
        UpdateProjectionMatrix();
        //UpdateCameraVectors();
    }
    RTTR_REGISTRATION
    {
        rttr::registration::class_<EditorCamera>("EditorCamera")
        .constructor()()
        .property("editorcamposition", &EditorCamera::GetCameraPosition, &EditorCamera::SetCameraPosition);
    }
}



    //EditorCamera::EditorCamera()
    //{
    //    mPosition   = glm::vec3(0,0,3);
    //    mWorldUp    = glm::vec3(0, 1, 0);
    //    mYaw        = YAW;
    //    mPitch      = PITCH;
    //    updateCameraVectors();

    //    mFront              = glm::vec3(0, 0, -1);
    //    mMovementSpeed      = SPEED;
    //    mMouseSensitivity   = SENSITIVITY;
    //    mZoom               = ZOOM;
    //    mViewMatrix         = GetViewMatrix();

    //    mCamProps.sZoom             = mZoom;
    //    mCamProps.sViewProjection   = mViewMatrix;
    //}

    //EditorCamera::EditorCamera(glm::vec3 _position)
	   // : mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY), mZoom(ZOOM)
    //{
    //    mPosition   = _position;
    //    mWorldUp    = glm::vec3(0, 1, 0);
    //    mYaw        = YAW;
    //    mPitch      = PITCH;
    //    updateCameraVectors();

    //    mCamProps.sZoom             = mZoom;
    //    mCamProps.sViewProjection   = mViewMatrix;
    //}

    //void EditorCamera::UpdateViewMatrix()
    //{
    //    mViewMatrix = glm::lookAt(mPosition, mPosition + mFront, mUp);
    //    mCamProps.sViewProjection = mViewMatrix;
    //}

    //void EditorCamera::SetCameraPosition(glm::vec3 _position)
    //{
    //    mPosition = _position;
    //    UpdateViewMatrix();
    //}

    //void EditorCamera::SetCamerazoom(float _zoom)
    //{
    //    mZoom = _zoom;
    //    mCamProps.sZoom = _zoom;
    //}

    //void EditorCamera::ProcessKeyboard(Camera_Movement _direction, float _deltaTime)
    //{
    //    float velocity = mMovementSpeed * _deltaTime;
    //    if (_direction == Camera_Movement::FORWARD)
    //    	mPosition += mFront * velocity;
    //    if (_direction == Camera_Movement::BACKWARD)
    //    	mPosition -= mFront * velocity;
    //    if (_direction == Camera_Movement::LEFT)
    //    	mPosition -= mRight * velocity;
    //    if (_direction == Camera_Movement::RIGHT)
    //    	mPosition += mRight * velocity;

    //    UpdateViewMatrix();
    //}

    //void EditorCamera::ProcessMouseMovement(float _xoffset, float _yoffset, GLboolean _constrainPitch)
    //{
    //    _xoffset *= mMouseSensitivity;
    //    _yoffset *= mMouseSensitivity;

    //    mYaw += _xoffset;
    //    mPitch += _yoffset;

    //    // make sure that when pitch is out of bounds, screen doesn't get flipped
    //    if (_constrainPitch)
    //    {
    //    	if (mPitch > 89.0f)
    //    		mPitch = 89.0f;
    //    	if (mPitch < -89.0f)
    //    		mPitch = -89.0f;
    //    }

    //    // update Front, Right and Up Vectors using the updated Euler angles
    //    updateCameraVectors();

    //    UpdateViewMatrix();
    //}

    //void EditorCamera::ProcessMouseScroll(float _yoffset)
    //{
    //    mZoom -= (float)_yoffset;
    //    if (mZoom < 1.0f)
    //    	mZoom = 1.0f;
    //    if (mZoom > 45.0f)
    //    	mZoom = 45.0f;

    //    SetCamerazoom(mZoom);
    //}

    ////  PRIVATE FUNCTION
    //void EditorCamera::updateCameraVectors()
    //{
    //    // calculate the new Front vector
    //    glm::vec3 front;
    //    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    //    front.y = sin(glm::radians(mPitch));
    //    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    //    mFront = glm::normalize(front);
    //    // Recompute the Right and Up vectors
    //    mRight = glm::normalize(glm::cross(mFront, mWorldUp));  // Normalize the vectors to maintain consistent movement speed, especially when looking up or down
    //    mUp = glm::normalize(glm::cross(mRight, mFront));
    //}

