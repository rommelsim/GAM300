
#ifndef RENDERING_H
#define RENDERING_H
/*****************************************************************//**
 * \file   Rendering.h
 * \brief  Contains entry point for drawing.
 *		   Has basic draw calls...
 *
 *
 *
 * \author Rommel, Waikit
 * \date   October 2023
 *
 * Copyright (C) 2023 Digipen Insitute Of Technology
 * Reproduction or disclosure of this file or its contents without
 * the prior consent of Digipen Insitute Of Technology is prohibited.
 *********************************************************************/

#include "Core/Singleton.hpp"
#include "Core/AeonAPI.h"
#include "Core/TimeStep.h"

#include "Resource/ResourceManager.h"
#include "Camera.h"
#include "Core/Log.h"
#include "Rendering/Framebuffer/FramebufferManager.h"
#include "Rendering/Framebuffer/OpenGLFramebuffer.h"
#include "Window/WindowsWindow.h"

#include "ECS/Components/Camera.h"
#include "ECS/Components/Render.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "ECS/Components/Transform.h"
#include "ECS/Components/Font.h"
#include <ECS/Components/UI.h>

#include <unordered_map>

#include "ECS/Components/Collider.h"
#include "ECS/Components/Particle.h"
#include "ECS/Components/RigidBody.h"
#include "Physics/ACPhysicsGeom.hpp"

#include <Rendering/Shadows/ShadowMapFBO.h>
#include "rttr/registration"

namespace AeonCore
{
	class AEON_API Rendering : public Singleton<Rendering>
	{
	private:
		//static std::unique_ptr<Camera> default_camera;

		static float		lastX;
		static float		lastY;
		static bool			firstMouse;
		static int			m_winWidth;
		static int			m_winHeight;
		std::string			m_winTitle;

		// OpenGL Callbacks
		static void			framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void			processInput(GLFWwindow* window, float dt);
		static void			mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
		static void			scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void			error_callback(int error_code, const char* description);

		std::string GetGLVersion	();
		std::string GetGLVendor		();
		std::string GetGLRenderer	();

		

	public:

		//Camera				GetCamera();

		void				Exit();
		static void			FrameStart();
		static void			FrameEnd();
		bool				CheckWindowIsClose();
		//glm::vec3			GetUniversalLightColor();// { return m_universal_light_pos; }
		//void				SetUniversalLightColor(glm::vec3 _pos);// { m_universal_light_pos = _pos; }


		void				DrawWireframe	(bool isOn = false);
		void				DrawLine		(glm::vec3 pos0, glm::vec3 pos1, glm::vec3 color, Camera& camera);
		void				DrawQuad		(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color, Camera& camera);
		void				DrawCubeFrame	(Transform* transform, glm::vec3 color, Camera& camera) const;
		void			    DrawTriangle	(Transform* transform, glm::vec3 color, Camera& camera) const;
		void				DrawCircle		(Transform* transform, glm::vec3 color, Camera& camera, int numSegments = 360) const;
		void				DrawCircleFrame	(Transform* transform, glm::vec3 color, Camera& camera, int numSegments = 360) const;
		void				DrawSphereFrame	(Transform* transform, glm::vec3 color, Camera& camera) const;

		void				DrawColliderBox	(Transform& _trans, RigidBody& _rigid, Collider& _coll, ACPhysics::CubeGeometry& _geom, Camera& _cam) const;
		void				DrawColliderSphere(Transform& _trans, RigidBody& _rigid, Collider& _coll, ACPhysics::SphereGeometry& _geom, Camera& _cam);
		// 3D font 
		void				DrawFont		(std::string text, Font& font, Transform& transform, Camera& camera, glm::vec3 color) const;		

		// 2D Font
		void				DrawFont		(std::string text, UI& ui, Transform& transform, Camera& camera) const;	

		//	3D Shapes for lighting
		void				DrawTexture2D	(Transform& transform, Camera& camera, UI& ui) const;
		void				DrawModel		(Transform& transform, Camera& camera, Model& model, Material& material) const;
		void				DrawModel		(Transform& transform, glm::mat4 view, glm::mat4 projection, Model& model, Material& material, ShadowMapFBO::MODE mode) const;
		void				DrawModel		(Transform& transform, glm::mat4 view, glm::mat4 projection, Model& model, Material& material) const;
		void				DrawCubeFilled	(Transform& transform, Camera& camera, Material& material, Lighting& lighting) const;
		void				DrawSphere		(Transform& transform, Camera& camera, Material& material, Lighting& lighting) const;
		void				DrawCylinder	(Transform& transform, Camera& camera, Material& material, Lighting& lighting) const;
		void				DrawSkyBox		(Camera& camera);
		void				DrawSkyBox		(const char* texture_name, glm::mat4 view, glm::mat4 proj);
		void				DrawShapeObject	(Shape3D _shape, Transform* _transform, glm::vec3 _col, Camera& cam, glm::vec3 _lightpos = glm::vec3(), glm::vec3 _lightcol = glm::vec3());

		void				DrawPlane(Transform& _transform, glm::vec3 _color, Camera& _camera, Lighting& lighting)
		{
			const glm::mat4 projection = glm::perspective(glm::radians(_camera.GetCameraZoom()), static_cast<float>(m_winWidth) / static_cast<float>(m_winHeight), 0.1f, 100.0f);
			const glm::mat4 view = _camera.GetCameraViewMatrix();

			glm::mat4 mdl(1.0f);
			mdl = glm::translate(mdl, _transform.GetPosition());

			// Apply Euler angle rotations
			mdl = glm::rotate(mdl, _transform.GetEulerAngle().x, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
			mdl = glm::rotate(mdl, _transform.GetEulerAngle().y, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
			mdl = glm::rotate(mdl, _transform.GetEulerAngle().z, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis

			mdl = glm::scale(mdl, _transform.GetScale());

			Shader* shader = ResourceManager::GetInstance().GetShader("AEON_PLANE_SHADER");
			AeonCore_ASSERT(shader, "Failed to retrieve Shader from ResourceManager...")

			Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_PLANE_MESH");
			AeonCore_ASSERT(mesh, "Failed to retrieve Mesh from ResourceManager...")

			shader->use();
			shader->setMat4("model", mdl);
			shader->setMat4("view", view);
			shader->setMat4("projection", projection);
			shader->setVec3("color", _color);

			// Lighting...
			//if (lighting.getLightColour() != glm::vec3())	//	lighting enable
			//{
			//	shader->setInt("lightMode", 1);
			//	shader->setVec3("lightPos", lighting.getLightPos());
			//	shader->setVec3("lightColor", lighting.getLightColour());
			//}
			//else
			//{
			//	shader->setInt("lightMode", 0);		//	lighting disable
			//}

			mesh->m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->m_IndexBuffer->GetCount()), GL_UNSIGNED_INT, nullptr);
		}

		void				DrawParticle(Particles& _particles, Camera& _camera, Model& _model, Material& _material, std::unordered_map<EntityID, Lighting*>& _lights) const;
		

		Rendering() {}
		//Render(Window* win);
		//~Render();

		void				OnStart(Window* win);

		void				init(int width, int height);
		void				swap_buffers();

		GLFWwindow*			get_window() { return m_windowHandle; }



		//std::string GetGLVersion();
		//std::string GetGLVendor();
		////const char* GetGLShader();
		//std::string GetGLRenderer();
		static bool RenderUIEditor;

	private:
		RTTR_ENABLE();
		static GLFWwindow*	m_windowHandle;
		static Rendering*	m_InstancePtr;
		std::string			m_version;
		std::string			m_vendor;
		std::string			m_renderer;

		// glm::vec3			m_universal_light_color;
		
	};
}

#endif // !RENDERING_H

//
//* ARCHIVED FOR FUTURE REFERENCE !!! DO NOT DELETE
//*
//		// view/projection transformations
//		glm::mat4 projection = glm::perspective(glm::radians(RenderingComponent->GetCamera()->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//		glm::mat4 view = RenderingComponent->GetCamera()->GetViewMatrix();
//		glm::mat4 model_cube = glm::mat4(1.0f);
//		glm::mat4 model_lamp = glm::mat4(1.0f);
//		glm::mat4 model_2nd_lamp = glm::mat4(1.0f);
//
//		std::vector<glm::vec3> lights
//		{
//			glm::vec3(0.6f, 0.5f, 2.0f),
//			glm::vec3(3.0f, 2.0f, -10.0f)
//		};
//		std::vector<glm::vec3> lightColors
//		{
//			glm::vec3(0, 1, 0),
//			glm::vec3(1, 0, 0)
//		};
//
//		// Cube source
//		// cubeShader->use();
//		// cubeShader->setVec3("objectColor", 1.0f, 1.0f, 1.0f);
//		//
//		// for (int i = 0; i < lights.size(); ++i)
//		// {
//		// 	lights[i].x = glm::sin(RenderingComponent->m_lastFrameTime * 3.0f);
//		// 	lights[i].y = glm::cos(RenderingComponent->m_lastFrameTime * 1.0f);
//		//
//		// 	cubeShader->setVec3("lightPos", lights[i]);           // recieve light info from light source.
//		// 	cubeShader->setVec3("lightColor", lightColors[i]);
//		// }
//		// cubeShader->setVec3("viewPos", ourCamera->Position.x, ourCamera->Position.y, ourCamera->Position.z);
//		// cubeShader->setMat4("projection", projection);
//		// cubeShader->setMat4("view", view);
//		// cubeShader->setMat4("model", model_cube);
//		// // cubeMesh->Draw(*cubeShader);
//		//
//		// // Lamp source
//		// lightShader->use();
//		// lightShader->setVec3("lightColor", lightColors[0]);
//		// lightShader->setMat4("projection", projection);
//		// lightShader->setMat4("view", view);
//		// model_lamp = glm::translate(model_lamp, lights[0]);
//		// model_lamp = glm::scale(model_lamp, glm::vec3(0.2f)); // a smaller cube
//		// lightShader->setMat4("model", model_lamp);
//		// lightMesh->Draw(*lightShader);
//		// Lamp source
//		// lightShader->use();
//		// lightShader->setVec3("lightColor", lightColors[1]);
//		// lightShader->setMat4("projection", projection);
//		// lightShader->setMat4("view", view);
//		// model_lamp = glm::translate(model_lamp, lights[1]);
//		// model_lamp = glm::scale(model_lamp, glm::vec3(2.0f)); // a smaller cube
//		// lightShader->setMat4("model", model_lamp);
//		// lightMesh2->Draw(*lightShader);
//		// DrawQuad(glm::vec3 pos, vec2 uv, vec3 color);
