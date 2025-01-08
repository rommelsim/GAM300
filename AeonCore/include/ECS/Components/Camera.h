/*****************************************************************//**
 * \file   Camera.h
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
#pragma once
#include "Core/AeonAPI.h"
#include "ECS/Components/IComponent.h"
#include "ECS/Components/Transform.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace AeonCore
{
	//	Defines several possible options for camera movement.
	//	Used as abstraction to stay away from window-system specific input methods
	enum class CameraMovement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		STOP
	};

	enum class CameraType
	{
		NONE,
		FOCUS,	//	Focus camera on object
		GAME	//	Game camera to follow 2 players
	};

	enum class CameraRole	//	for game only
	{
		NONE,
		CAMERA
	};

	////	Default camera values
	//const float YAW = -90.0f;
	//const float PITCH = 0.0f;
	//const float SPEED = 5.f;
	//const float SENSITIVITY = 0.1f;
	//const float ZOOM = 45.0f;

	class AEON_API Camera : public IComponent
	{
	public:
		Camera(glm::vec3 _position = glm::vec3(), glm::vec3 _up = glm::vec3(0, 1, 0), float _yaw = -90.f, float _pitch = 0,
			float _fov = 0.0f, float _ar = 16.0f/9.0f, float _near = 0.1f, float _far = 1000.0f);
		virtual ~Camera() = default;
		virtual void Destroy() override;

		virtual void SetCameraPosition(glm::vec3 _position);
		virtual void SetCameraTarget(glm::vec3 _target);
		virtual void SetCameraPitch(float _pitch);
		virtual void SetCameraYaw(float _yaw);

		virtual inline void SetViewPortSelected(bool _selected)		{ mViewPortSelected = _selected; }
		virtual inline void SetCameraMovementSpeed(float _speed)	{ mMovementSpeed = _speed; }
		virtual inline void SetCameraZoom(float _zoom)				{ mZoom = _zoom; }
		virtual inline void SetCameraRole(CameraRole _role)			{ mRole = _role; }

		virtual inline void SetCameraRoleSelected(bool _selected)	 { CameraSelected = _selected; }
		virtual inline void SetCameraPlayer1Selected(bool _selected) { Player1Selected = _selected; }
		virtual inline void SetCameraPlayer2Selected(bool _selected) { Player2Selected = _selected; }

		virtual inline bool GetViewPortSelected()					{ return mViewPortSelected; }
		virtual inline float GetCameraZoom()						{ return mZoom; }
		virtual inline float GetCameraYaw()							{ return mYaw; }
		virtual inline float GetCameraPitch()						{ return mPitch; }
		virtual inline float GetCameraMovementSpeed()				{ return mMovementSpeed; }
		virtual inline float GetCameraSensitivity()					{ return mMouseSensitivity; }
		virtual inline glm::vec3 GetCameraPosition()				{ return mPosition; }
		virtual inline glm::vec3 GetCameraTarget()					{ return mTarget; }
		virtual inline glm::vec3 GetCameraRight()					{ return mRight; }
		virtual inline glm::vec3 GetCameraFront()					{ return mFront; }
		virtual inline glm::vec3 GetCameraUp()						{ return mUp; }
		virtual inline glm::mat4 GetCameraViewMatrix()				{ return mViewMatrix; }
		virtual inline glm::mat4 GetCameraProjectionMatrix()		{ return mProjectionMatrix; }

		virtual inline bool GetCameraRoleSelected()	   { return CameraSelected; }
		virtual inline bool GetCameraPlayer1Selected() { return Player1Selected; }
		virtual inline bool GetCameraPlayer2Selected() { return Player2Selected; }

		virtual inline CameraRole GetCameraRole()					{ return mRole; }

		virtual void UpdateCameraVectors();	// calculates the front vector from the Camera's (updated) Euler Angles

		virtual inline void UpdateViewMatrix()				{ mViewMatrix = glm::lookAt(mPosition, mTarget, mUp); }
		virtual inline void UpdateProjectionMatrix()		{ mProjectionMatrix = glm::perspective(glm::radians(mZoom), mAspectRatio, mNearClip, mFarClip); }
		
		virtual inline void SetFar(float _far) { mFarClip = _far; }
		virtual inline float GetFar() { return mFarClip; }
		virtual inline void SetNear(float _near) { mNearClip = _near; }
		virtual inline float GetNear() { return mNearClip; }
		virtual void SetWindowWidthAndHeight(int width, int height);
		virtual int GetWindowWidth() { return m_width; }
		virtual int GetWindowHeight() { return m_height; }

		void Follow2Players(glm::vec3 _p1pos, glm::vec3 _p2pos)
		{
			// Calculate the midpoint between the two players
			glm::vec3 midpoint = (_p1pos + _p2pos) * 0.5f;

			SetCameraZoom(10.f);

			// Set the camera's target to the midpoint
			SetCameraTarget(midpoint);
		}
	
	private:
		RTTR_ENABLE(IComponent)

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

		CameraRole mRole{ CameraRole::NONE };

		bool CameraSelected		{ false };
		bool Player1Selected	{ false };
		bool Player2Selected	{ false };
	};

	//class AEON_API Camera : public IComponent
	//{
	//public:
	//	/*-----------------------------------------------------------------------------------------------
	//	 *	Constructor
	//	 -----------------------------------------------------------------------------------------------*/

	//	Camera(glm::vec3 _target = glm::vec3(), glm::vec3 _position = glm::vec3(), glm::vec3 _up = glm::vec3(0.f, 1.f, 0.f),
	//	       float _yaw = 90.f, float _pitch = PITCH, float radius = 2.0f);

	//	/*-----------------------------------------------------------------------------------------------
	//	 *	Destructor
	//	 -----------------------------------------------------------------------------------------------*/

	//	~Camera() = default;

	//	/*-----------------------------------------------------------------------------------------------
	//	 *	Get Functions
	//	 -----------------------------------------------------------------------------------------------*/
	//	virtual inline glm::mat4	GetOrientation()									{ return mViewMatrix = glm::lookAt(mPosition, mTarget, mUp); }
	//	virtual inline glm::mat4	GetViewMatrix()										{ return mViewMatrix = glm::lookAt(mPosition, mTarget, mUp); }
	//	virtual inline glm::vec3	GetPosition()										{ return mPosition; }
	//	virtual inline glm::vec3	GetFront()											{ return mFront; }
	//	virtual inline glm::vec3	GetUp()												{ return mUp; }
	//	virtual inline glm::vec3	GetRight()											{ return mRight; }
	//	virtual inline glm::vec3	GetWorldUp()										{ return mWorldUp; }
	//	virtual inline glm::vec3	GetTarget()											{ return mTarget; }
	//	virtual inline float		GetYaw()											{ return mYaw; }
	//	virtual inline float		GetPitch()											{ return mPitch; }
	//	virtual inline float		GetMovementSpeed()									{ return mMovementSpeed; }
	//	virtual inline float		GetMouseSensitivity()								{ return mMouseSensitivity; }
	//	virtual inline float		GetZoom()											{ return mZoom; }
	//	virtual inline bool			GetIsHovered()										{ return mIsHovered; }
	//	inline float				GetRadius()											{ return mRadius; }
	//	inline bool					GetEnabled()										{ return mEnabled; }

	//	/*-----------------------------------------------------------------------------------------------
	//	 *	Set Functions
	//	 -----------------------------------------------------------------------------------------------*/

	//	virtual inline void			SetPosition				(glm::vec3 _position)		{ mPosition = _position; }
	//	virtual inline void			SetFront				(glm::vec3 _front)			{ mFront = _front; }
	//	virtual inline void			SetUp					(glm::vec3 _up)				{ mUp = _up; }
	//	virtual inline void			SetRight				(glm::vec3 _right)			{ mRight = _right; }
	//	virtual inline void			SetWorldUp				(glm::vec3 _worldUp)		{ mWorldUp = _worldUp; }
	//	virtual inline void			SetMovementSpeed		(float _movementSpeed)		{ mMovementSpeed = _movementSpeed; }
	//	virtual inline void			SetMouseSensitivity		(float _mouseSensitivity)	{ mMouseSensitivity = _mouseSensitivity; }
	//	virtual inline void			SetZoom					(float _zoom)				{ mZoom = _zoom; }
	//	virtual inline void			SetIsHovered			(bool _isHovered)			{ mIsHovered = _isHovered; }

	//	//	functions that affects the camera
	//	virtual inline void			SetTarget				(glm::vec3 _target)
	//	{
	//		mTarget = _target;
	//		UpdateCameraPosition();
	//	}
	//	virtual inline void			SetYaw					(float _yaw)
	//	{
	//		mYaw = _yaw;
	//		UpdateCameraPosition();
	//	}
	//	
	//	virtual inline void			SetPitch				(float _pitch)
	//	{
	//		mPitch = _pitch;
	//		UpdateCameraPosition();
	//	}
	//	inline void					SetRadius				(float _radius)
	//	{
	//		mRadius = _radius;
	//		UpdateCameraPosition();
	//	}

	//	inline void					SetEnabled				(bool _enabled)				{ mEnabled = _enabled; }
	//	/*-----------------------------------------------------------------------------------------------
	//	 *	Update Functions
	//	 -----------------------------------------------------------------------------------------------*/

	//	virtual void				UpdateCameraVectors();
	//	void						UpdateCameraPosition()
	//	{
	//		mPosition.x = mTarget.x + mRadius * cos(glm::radians(mPitch)) * cos(glm::radians(mYaw));
	//		mPosition.y = mTarget.y + mRadius * sin(glm::radians(mPitch));
	//		mPosition.z = mTarget.z + mRadius * cos(glm::radians(mPitch)) * sin(glm::radians(mYaw));

	//		GetViewMatrix();
	//	}
	//	
	//private:
	//	RTTR_ENABLE(IComponent);
	//	//	camera attributes
	//	glm::vec3 mPosition;
	//	glm::vec3 mFront;
	//	glm::vec3 mUp;
	//	glm::vec3 mRight;
	//	glm::vec3 mWorldUp;
	//	glm::vec3 mTarget;

	//	//	euler angles
	//	float mYaw;
	//	float mPitch;

	//	//	camera options
	//	float mMovementSpeed;
	//	float mMouseSensitivity;
	//	float mZoom;

	//	//	distance to rotate about object
	//	float mRadius;

	//	//	view matrix of camera
	//	glm::mat4 mViewMatrix;

	//	bool mIsHovered{ false };
	//	bool mEnabled{ false };
	//};
}


//#include "Core/Log.h"
//#include "IComponent.h"
//#include "Input/Input.h"
//#include "Core/AeonAPI.h"
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glad/glad.h>


//// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
//namespace AeonCore
//{
//  //  class Camera : IComponent
//  //  {
//  //  public:
//  //      /*Camera(): mPosition(), mFront(), mUp(), mRight(), mWorldUp(), mYaw(0), mPitch(0), mMovementSpeed(0),
//  //                mMouseSensitivity(0),
//  //                mZoom(0)
//  //      {
//  //      }*/
//
//  //  	// constructor with vectors
//		//Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f), float _yaw = YAW, float _pitch = PITCH)
//		//	: mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY), mZoom(ZOOM)
//		//{
//		//	mPosition = _position;
//		//	mWorldUp = _up;
//		//	mYaw = _yaw;
//		//	mPitch = _pitch;
//		//	updateCameraVectors();
//		//}
//
//		////// constructor with scalar values
//		////Camera(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch)
//		////	:mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY), mZoom(ZOOM)
//		////{
//		////	mPosition = glm::vec3(_posX, _posY, _posZ);
//		////	mWorldUp = glm::vec3(_upX, _upY, _upZ);
//		////	mYaw = _yaw;
//		////	mPitch = _pitch;
//		////	updateCameraVectors();
//		////	AeonCore_INFO("Camera Created");
//		////}
//
//		//inline glm::mat4 GetViewMatrix()         { return glm::lookAt(mPosition, mPosition + mFront, mUp); } // returns the view matrix calculated using Euler Angles and the LookAt Matrix
//  //      inline void SetPosition(glm::vec3 _pos)  { mPosition = _pos;}                                                   // set camera pos
//  //      inline float GetZoom()                   { return mZoom; }
//
//  //      void ProcessKeyboard(Camera_Movement _direction, float _deltaTime);                           // Processes input from a keyboard-like system, accepting input as a camera-defined ENUM to abstract it from windowing systems.
//  //      void ProcessMouseMovement(float _xoffset, float _yoffset, GLboolean _constrainPitch = true);  // Processes input from a mouse system, expecting offset values in both x and y directions.
//  //      void ProcessMouseScroll(float _yoffset);                                                      // Handles mouse scroll-wheel events, requiring input exclusively on the vertical wheel axis.
//
//  //  private:
//  //      // calculates the front vector from the Camera's (updated) Euler Angles
//  //      void updateCameraVectors()
//  //      {
//  //          // calculate the new Front vector
//  //          glm::vec3 front;
//  //          front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
//  //          front.y = sin(glm::radians(mPitch));
//  //          front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
//  //          mFront = glm::normalize(front);
//  //          // also re-calculate the Right and Up vector
//  //          mRight = glm::normalize(glm::cross(mFront, mWorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
//  //          mUp = glm::normalize(glm::cross(mRight, mFront));
//  //      }
//
//  //      // camera Attributes
//  //      glm::vec3 mPosition;
//  //      glm::vec3 mFront;
//  //      glm::vec3 mUp;
//  //      glm::vec3 mRight;
//  //      glm::vec3 mWorldUp;
//  //      // euler Angles
//  //      float mYaw;
//  //      float mPitch;
//  //      // camera options
//  //      float mMovementSpeed;
//  //      float mMouseSensitivity;
//  //      float mZoom;
//  //  };
//
//    // Defines several possible options for camera movement.
//    // Used as abstraction to stay away from window-system specific input methods
//    enum class Camera_Movement
//    {
//        FORWARD,
//        BACKWARD,
//        LEFT,
//        RIGHT
//    };
//
//    enum class Camera_Type
//    {
//	    Main,
//        Target
//    };
//
//    struct CameraProps
//    {
//        float sZoom;
//        glm::mat4 sViewProjection;
//    };
//
//    // Default camera values
//    /*const float YAW = -90.0f;
//    const float PITCH = 0.0f;
//    const float SPEED = 10.0f;
//    const float SENSITIVITY = 0.1f;
//    const float ZOOM = 45.0f;*/
//
//    class AEON_API Camera : public IComponent
//    {
//    public:
//        Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 5.0f),
//			   glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f),
//			   float _yaw = YAW, float _pitch = PITCH);
//
//        inline glm::mat4 GetViewMatrix()        { return glm::lookAt(mPosition, mPosition + mFront, mUp); }
//        inline CameraProps& GetCamProp()           { return mCamProps; }
//
//        //  For Camera Inputs
//        inline glm::vec3 GetCameraPosition()    { return mPosition; }
//        inline float GetCameraPitch()           { return mPitch; }
//        inline float GetCameraZoom()            { return mZoom; }
//        inline bool GetSwitchView()             { return mSwitchView; }
//        inline Camera_Type GetCameraType()      { return mType; }
//
//        void UpdateViewMatrix();
//        void updateCameraMatrix()
//        {
//            mViewMatrix = glm::lookAt(mPosition, mTarget, mUp);
//            mCamProps.sViewProjection = mViewMatrix;
//        }
//    	void SetCameraPosition(glm::vec3 _position);
//    	void SetCamerazoom(float _zoom);
//        void SetCameraTarget(glm::vec3 _target) { mTarget = _target; }
//        void SetSwitchView(bool _switchview)    { mSwitchView = _switchview; }
//        void SetCameraType(Camera_Type _type)   { mType = _type; }
//
//        void ProcessMouseMovement(float _xoffset, float _yoffset,glm::vec3 objectPosition, GLboolean _constrainPitch = true)
//        {
//            _xoffset *= mMouseSensitivity;
//            _yoffset *= mMouseSensitivity;
//
//            mYaw += _xoffset;
//            mPitch += _yoffset;
//
//
//            // make sure that when pitch is out of bounds, screen doesn't get flipped
//            if (_constrainPitch)
//            {
//                if (mPitch > 89.0f)
//                    mPitch = 89.0f;
//                if (mPitch < -89.0f)
//                    mPitch = -89.0f;
//            }
//
//            float radius = 5.0f;
//            glm::vec3 cameraPosition;
//            cameraPosition.x = objectPosition.x + radius * cos(glm::radians(mPitch)) * cos(glm::radians(mYaw));
//            cameraPosition.y = objectPosition.y + radius * sin(glm::radians(mPitch));
//            cameraPosition.z = objectPosition.z + radius * cos(glm::radians(mPitch)) * sin(glm::radians(mYaw));
//            SetCameraPosition(cameraPosition);
//            SetCameraTarget(objectPosition);
//            updateCameraMatrix();
//        }
//
//        void EnableCamera(glm::vec3 _ObjPosition)
//        {
//            static bool firstmouse = true;
//            if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
//            {
//            	float xpos = static_cast<float>(Input::GetMouseX());
//            	float ypos = static_cast<float>(Input::GetMouseY());
//            	static float lastx = 0.0f;
//            	static float lasty = 0.0f;
//            	static float lastz = 3;
//            	if (firstmouse)
//            	{
//
//            		lastx = xpos;
//            		lasty = ypos;
//            		firstmouse = false;
//            	}
//
//            	//float xoffset = xpos - lastx;
//            	float xoffset = xpos - lastx;
//            	float yoffset = lasty - ypos; // reversed since y-coordinates go from bottom to top
//
//            	lastx = xpos;
//            	lasty = ypos;
//
//            	//editorvp_camera->SetPosition(glm::vec3(xoffset, yoffset, 3));
//
//            	ProcessMouseMovement(xoffset, yoffset, _ObjPosition);
//            }
//            if (Input::IsMouseButtonReleased(GLFW_MOUSE_BUTTON_1))
//            {
//            	firstmouse = true;
//            }
//        }
//
//    private:
//
//        void updateCameraVectors();
//
//        // camera Attributes
//        glm::vec3 mPosition;
//        glm::vec3 mFront;
//        glm::vec3 mUp;
//        glm::vec3 mRight;
//        glm::vec3 mWorldUp;
//        glm::vec3 mTarget;
//        // euler Angles
//        float mYaw;
//        float mPitch;
//        // camera options
//        float mMovementSpeed;
//        float mMouseSensitivity;
//        float mZoom;
//
//        // view matrix
//        glm::mat4 mViewMatrix;
//
//        CameraProps mCamProps;
//
//        bool mSwitchView{ false };
//
//        Camera_Type mType;
//    };
//}
