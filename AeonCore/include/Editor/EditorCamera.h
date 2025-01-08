#pragma once
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
#include "Core/AeonAPI.h"
#include "Core/Singleton.hpp"
#include "ECS/Components/Camera.h"
#include "Core/Log.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "rttr/registration"

namespace AeonCore
{
	class AEON_API EditorCamera : public Camera, public Singleton<EditorCamera>
	{
	public:
		EditorCamera(glm::vec3 _position = glm::vec3(0, 30, 20), glm::vec3 _up = glm::vec3(0, 1, 0), float _yaw = -85.f, float _pitch = -55.f,
			float _fov = 75.0f, float _ar = 16.0f / 9.0f, float _near = 0.1f, float _far = 100000000.0f);
		~EditorCamera() override = default;

		void SetCameraPosition(glm::vec3 _position) override;
		void SetCameraTarget(glm::vec3 _target) override;
		void SetCameraPitch(float _pitch) override;
		void SetCameraYaw(float _yaw) override;

		inline void SetViewPortSelected(bool _selected) override	{ mViewPortSelected = _selected; }
		inline void SetCameraMovementSpeed(float _speed) override	{ mMovementSpeed = _speed; }
		inline void SetCameraZoom(float _zoom) override				{ mZoom = _zoom; }

		inline bool GetViewPortSelected() override					{ return mViewPortSelected; }
		inline float GetCameraZoom() override						{ return mZoom; }
		inline float GetCameraYaw() override						{ return mYaw; }
		inline float GetCameraMovementSpeed() override				{ return mMovementSpeed; }
		inline float GetCameraPitch() override						{ return mPitch; }
		inline float GetCameraSensitivity() override				{ return mMouseSensitivity; }
		inline glm::vec3 GetCameraPosition() override				{ return mPosition; }
		inline glm::vec3 GetCameraTarget() override					{ return mTarget; }
		inline glm::vec3 GetCameraRight() override					{ return mRight; }
		inline glm::vec3 GetCameraFront() override					{ return mFront; }
		inline glm::vec3 GetCameraUp() override						{ return mUp; }
		inline glm::mat4 GetCameraViewMatrix() override				{ return mViewMatrix; }
		inline glm::mat4 GetCameraProjectionMatrix() override		{ return mProjectionMatrix; }
		void UpdateCameraVectors() override;

		inline void UpdateViewMatrix() override { mViewMatrix = glm::lookAt(mPosition, mTarget, mUp); }
		inline void UpdateProjectionMatrix() override { mProjectionMatrix = glm::perspective(glm::radians(mZoom), mAspectRatio, mNearClip, mFarClip); }

		inline void SetWindowWidthAndHeight(int width, int height) override;
		inline void SetFar(float _far) override { mFarClip = _far; }
		inline void SetNear(float _near) override { mNearClip = _near; }
	private:
		RTTR_ENABLE();
		// camera Attributes
		glm::vec3 mFront;
		glm::vec3 mRight;
		glm::vec3 mWorldUp;
		glm::vec3 mUp;

		glm::vec3 mPosition;
		glm::vec3 mTarget;
		glm::mat4 mViewMatrix;
		glm::mat4 mProjectionMatrix;

		// euler Angles
		float mYaw;
		float mPitch;

		// camera options
		float mMovementSpeed;
		float mMouseSensitivity;
		float mZoom;

		bool mViewPortSelected{ false };

		float mFov;
		float mAspectRatio;
		float mNearClip;
		float mFarClip;

		int m_width;
		int m_height;
	};



	//class AEON_API EditorCamera : public Camera, public Singleton<EditorCamera>
	//{
	//public:
	//	/*-----------------------------------------------------------------------------------------------
	//	 *	Constructor
	//	 -----------------------------------------------------------------------------------------------*/
	//	EditorCamera(glm::vec3 _position = glm::vec3(0, 10, 10), glm::vec3 _up = glm::vec3(0.f, 1.f, 0.f),
	//				 float _yaw = YAW, float _pitch = -45.f);

	//	/*-----------------------------------------------------------------------------------------------
	//	 *	Destructor
	//	 -----------------------------------------------------------------------------------------------*/

	//	~EditorCamera() = default;

	//	/*-----------------------------------------------------------------------------------------------
	//	 *	Get Functions
	//	 -----------------------------------------------------------------------------------------------*/

	//	inline glm::mat4	GetViewMatrix()										override { return mViewMatrix = glm::lookAt(mPosition, mPosition + mFront, mUp); }
	//	inline glm::vec3	GetPosition()										override { return mPosition; }
	//	inline glm::vec3	GetFront()											override { return mFront; }
	//	inline glm::vec3	GetUp()												override { return mUp; }
	//	inline glm::vec3	GetRight()											override { return mRight; }
	//	inline glm::vec3	GetWorldUp()										override { return mWorldUp; }
	//	inline float		GetYaw()											override { return mYaw; }
	//	inline float		GetPitch()											override { return mPitch; }
	//	inline float		GetMovementSpeed()									override { return mMovementSpeed; }
	//	inline float		GetMouseSensitivity()								override { return mMouseSensitivity; }
	//	inline float		GetZoom()											override { return mZoom; }
	//	inline bool			GetIsHovered()										override { return mIsHovered; }

	//	/*-----------------------------------------------------------------------------------------------
	//	 *	Set Functions
	//	 -----------------------------------------------------------------------------------------------*/

	//	inline void			SetPosition				(glm::vec3 _position)		override { mPosition = _position; }
	//	inline void			SetFront				(glm::vec3 _front)			override { mFront = _front; }
	//	inline void			SetUp					(glm::vec3 _up)				override { mUp = _up; }
	//	inline void			SetRight				(glm::vec3 _right)			override { mRight = _right; }
	//	inline void			SetWorldUp				(glm::vec3 _worldUp)		override { mWorldUp = _worldUp; }
	//	inline void			SetYaw					(float _yaw)				override { mYaw = _yaw; }
	//	inline void			SetPitch				(float _pitch)				override { mPitch = _pitch; }
	//	inline void			SetMovementSpeed		(float _movementSpeed)		override { mMovementSpeed = _movementSpeed; }
	//	inline void			SetMouseSensitivity		(float _mouseSensitivity)	override { mMouseSensitivity = _mouseSensitivity; }
	//	inline void			SetZoom					(float _zoom)				override { mZoom = _zoom; }
	//	inline void			SetIsHovered			(bool _isHovered)			override { mIsHovered = _isHovered; }

	//	/*-----------------------------------------------------------------------------------------------
	//	 *	Update Functions
	//	 -----------------------------------------------------------------------------------------------*/
	//	void				UpdateCameraVectors()								override;

	//private:
	//	//	camera attributes
	//	glm::vec3 mPosition;
	//	glm::vec3 mFront;
	//	glm::vec3 mUp;
	//	glm::vec3 mRight;
	//	glm::vec3 mWorldUp;

	//	//	euler angles
	//	float mYaw;
	//	float mPitch;

	//	//	camera options
	//	float mMovementSpeed;
	//	float mMouseSensitivity;
	//	float mZoom;

	//	//	view matrix of camera
	//	glm::mat4 mViewMatrix;

	//	bool mIsHovered{ false };
	//};


}




//#include "Core/Singleton.hpp"
//#include "Core/AeonAPI.h"
//#include "ECS/Components/Camera.h"
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glad/glad.h>
//
//namespace AeonCore
//{
//    class AEON_API EditorCamera : public Singleton<EditorCamera>
//    {
//    public:
//        EditorCamera();
//
//        EditorCamera(glm::vec3 _position);
//
//        inline glm::mat4 GetViewMatrix()        { return glm::lookAt(mPosition, mPosition + mFront, mUp); }
//        inline CameraProps GetCamProp()    { return mCamProps; }
//
//        //  For Camera Inputs
//        inline glm::vec3 GetCameraPosition()    { return mPosition; }
//        inline float GetCameraPitch()           { return mPitch; }
//        inline float GetCameraZoom()            { return mZoom; }
//
//        void UpdateViewMatrix();
//        void SetCameraPosition(glm::vec3 _position);
//        void SetCamerazoom(float _zoom);
//
//        void ProcessKeyboard(Camera_Movement _direction, float _deltaTime);                           // Processes input from a keyboard-like system, accepting input as a camera-defined ENUM to abstract it from windowing systems.
//		void ProcessMouseMovement(float _xoffset, float _yoffset, GLboolean _constrainPitch = true);  // Processes input from a mouse system, expecting offset values in both x and y directions.
//		void ProcessMouseScroll(float _yoffset);
//
//    private:
//        void updateCameraVectors();
//
//        // camera Attributes
//        glm::vec3 mPosition{};
//        glm::vec3 mFront{};
//        glm::vec3 mUp{};
//        glm::vec3 mRight{};
//        glm::vec3 mWorldUp{};
//
//        // euler Angles
//        float mYaw;
//        float mPitch;
//        // camera options
//        float mMovementSpeed;
//        float mMouseSensitivity;
//        float mZoom;
//
//        // view matrix
//        glm::mat4 mViewMatrix{};
//
//        //  camera properties for draw calls
//        CameraProps mCamProps{};
//    };
//}