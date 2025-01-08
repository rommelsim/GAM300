#include "acpch.h"
/*****************************************************************//**
 * \file   Camera.cpp
 * \brief  Basic camera class
 *
 *
 *
 * \author Rommel
 * \date   October 2023
 *
 * Copyright (C) 2023 Digipen Insitute Of Technology
 * Reproduction or disclosure of this file or its contents without
 * the prior consent of Digipen Insitute Of Technology is prohibited.
 *********************************************************************/

#include "ECS/Components/Camera.h"

#include "Camera/CameraManager.h"
#include "Serialization/RegisterManager.hpp"

namespace AeonCore
{
	//Camera::Camera(glm::vec3 _target, glm::vec3 _position, glm::vec3 _up, float _yaw, float _pitch, float _radius)
	//	: mFront(glm::vec3(0.f, 0.f, -1.f)), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY), mZoom(ZOOM)
	//{
	//	mPosition = _position;
	//	mWorldUp = _up;
	//	mYaw = _yaw;
	//	mPitch = _pitch;

	//	Camera::UpdateCameraVectors();	//	update vectors for camera

	//	mTarget = _target;
	//	mRadius = _radius;

	//	Camera::UpdateCameraPosition();	
	//}

	//void Camera::UpdateCameraVectors()
	//{
	//	// calculate the new Front vector
	//	glm::vec3 front;
	//	front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	//	front.y = sin(glm::radians(mPitch));
	//	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	//	mFront = glm::normalize(front);
	//	// also re-calculate the Right and Up vector
	//	mRight = glm::normalize(glm::cross(mFront, mWorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	//	mUp = glm::normalize(glm::cross(mRight, mFront));
	//}
	//RTTR_REGISTRATION
	//{
	//	RegisterManager::instance().addRegister([]() {
	//	rttr::registration::class_<Camera>("Camera")
	//	.constructor<glm::vec3 const&, glm::vec3 const&, glm::vec3 const&, float const&, float const&, float const&>()()
	//	.property_readonly("id", &Camera::GetComponentID)
	//	.property("front", &Camera::GetFront, &Camera::SetFront)
	//	.property("up", &Camera::GetUp, &Camera::SetUp)
	//	.property("right", &Camera::GetRight, &Camera::SetRight)
	//	.property("world up", &Camera::GetWorldUp, &Camera::SetWorldUp)
	//	.property("movement speed", &Camera::GetMovementSpeed, &Camera::SetMovementSpeed)
	//	.property("sensitivity", &Camera::GetMouseSensitivity, &Camera::SetMouseSensitivity)
	//	.property("zoom", &Camera::GetZoom, &Camera::SetZoom)
	//	.property("hover", &Camera::GetIsHovered, &Camera::SetIsHovered)
	//	.property("target", &Camera::GetTarget, &Camera::SetTarget)
	//	.property("yaw", &Camera::GetYaw, &Camera::SetYaw)
	//	.property("pitch", &Camera::GetPitch, &Camera::SetPitch)
	//	.property("radius", &Camera::GetRadius, &Camera::SetRadius)
	//	.property("enabled", &Camera::GetEnabled, &Camera::SetEnabled); }, 5);
	//}

	Camera::Camera(glm::vec3 _position, glm::vec3 _up, float _yaw, float _pitch, float _fov, float _ar, float _near, float _far)
		: mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(20.f), mMouseSensitivity(0.1f), mZoom(20.0f)
	{
		mPosition = _position;
		mWorldUp = _up;
		mYaw = _yaw;
		mPitch = _pitch;
	
		mFov = _fov;
		mAspectRatio = _ar;
		mNearClip = _near;
		mFarClip = _far;

		m_width = 1920;
		m_height = 1080;

		UpdateCameraVectors();
	}

	void Camera::Destroy()
	{
		CameraManager::GetInstance().RemoveCamera(this);
	}

	void Camera::SetCameraPosition(glm::vec3 _position)
	{
		mPosition = _position;
		UpdateCameraVectors();
	}

	void Camera::SetCameraTarget(glm::vec3 _target)
	{
		mTarget = _target;
		UpdateViewMatrix();
	}

	void Camera::SetCameraPitch(float _pitch)
	{
		mPitch = _pitch;
		UpdateCameraVectors();
	}

	void Camera::SetCameraYaw(float _yaw)
	{
		mYaw = _yaw;
		UpdateCameraVectors();
	}

	void Camera::SetWindowWidthAndHeight(int width, int height)
	{
		m_width = width;
		m_height = height;
		mAspectRatio = (float)m_width / (float)m_height;
		UpdateCameraVectors();
	}

	void Camera::UpdateCameraVectors()
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

	RTTR_REGISTRATION
	{
		RegisterManager::Instance().AddRegister([]() {
		rttr::registration::class_<Camera>("Camera")
		.constructor<glm::vec3 const&, glm::vec3 const&, float const&, float const&>()()
		.property_readonly("id", &Camera::GetComponentID)
		.property("movement speed", &Camera::GetCameraMovementSpeed, &Camera::SetCameraMovementSpeed)
		.property("zoom", &Camera::GetCameraZoom, &Camera::SetCameraZoom)
		.property("target", &Camera::GetCameraTarget, &Camera::SetCameraTarget)
		.property("yaw", &Camera::GetCameraYaw, &Camera::SetCameraYaw)
		.property("pitch", &Camera::GetCameraPitch, &Camera::SetCameraPitch)
		.property("role", &Camera::GetCameraRole, &Camera::SetCameraRole); });
		/*.property("enabled", &Camera::GetEnabled, &Camera::SetEnabled); }, 5);*/
	}
}





	//////	TODO: Transfer over to Input for camera
	////void Camera::ProcessKeyboard(Camera_Movement _direction, float _deltaTime)
	////{
	////	float velocity = mMovementSpeed * _deltaTime;
	////	if (_direction == FORWARD)
	////		mPosition += mFront * velocity;
	////	if (_direction == BACKWARD)
	////		mPosition -= mFront * velocity;
	////	if (_direction == LEFT)
	////		mPosition -= mRight * velocity;
	////	if (_direction == RIGHT)
	////		mPosition += mRight * velocity;
	////}

	////void Camera::ProcessMouseMovement(float _xoffset, float _yoffset, GLboolean _constrainPitch)
	////{
	////	_xoffset *= mMouseSensitivity;
	////	_yoffset *= mMouseSensitivity;

	////	mYaw += _xoffset;
	////	mPitch += _yoffset;

	////	// make sure that when pitch is out of bounds, screen doesn't get flipped
	////	if (_constrainPitch)
	////	{
	////		if (mPitch > 89.0f)
	////			mPitch = 89.0f;
	////		if (mPitch < -89.0f)
	////			mPitch = -89.0f;
	////	}

	////	// update Front, Right and Up Vectors using the updated Euler angles
	////	updateCameraVectors();
	////}

	////void Camera::ProcessMouseScroll(float _yoffset)
	////{
	////	mZoom -= (float)_yoffset;
	////	if (mZoom < 1.0f)
	////		mZoom = 1.0f;
	////	if (mZoom > 45.0f)
	////		mZoom = 45.0f;
	////}

	//Camera::Camera(glm::vec3 _position, glm::vec3 _up, float _yaw, float _pitch)
	//	: mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY), mZoom(ZOOM)
	//{
	//	mPosition = _position;
	//	mWorldUp = _up;
	//	mYaw = _yaw;
	//	mPitch = _pitch;
	//	updateCameraVectors();

	//	mCamProps.sZoom = mZoom;
	//	mCamProps.sViewProjection = mViewMatrix;
	//}

	//void Camera::UpdateViewMatrix()
	//{
	//	mViewMatrix = glm::lookAt(mPosition, mPosition + mFront, mUp);
	//	mCamProps.sViewProjection = mViewMatrix;
	//}

	//void Camera::SetCameraPosition(glm::vec3 _position)
	//{
	//	mPosition = _position;
	//	//UpdateViewMatrix();
	//}

	//void Camera::SetCamerazoom(float _zoom)
	//{
	//	mZoom = _zoom;
	//	UpdateViewMatrix();
	//	mCamProps.sZoom = _zoom;
	//}

	//void Camera::updateCameraVectors()
	//{
	//	// calculate the new Front vector
	//	glm::vec3 front;
	//	front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	//	front.y = sin(glm::radians(mPitch));
	//	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	//	mFront = glm::normalize(front);
	//	// Recompute the Right and Up vectors
	//	mRight = glm::normalize(glm::cross(mFront, mWorldUp));  // Normalize the vectors to maintain consistent movement speed, especially when looking up or down
	//	mUp = glm::normalize(glm::cross(mRight, mFront));
	//}

