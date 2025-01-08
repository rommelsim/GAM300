#include "acpch.h"
/*!***************************************************************************
	\file			RenderSystem.cpp
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					create and manage 3D shapes for rendering
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "AeonCore.h"

#include "ECS/Systems/RenderSystem.h"
#include "ECS/Components/Animation.h"
#include "ECS/Systems/Renderer2DSystem.h"		// prevent cyclic ref
#include "Physics/ACPhysicsInternal.hpp"
#include "Physics/ACPhysicsUtility.hpp"
#include "ECS/Components/Lighting.h"
#include "Camera/CameraManager.h"
#include "Animation/Animator.h"
#include "Resource/AModel.h"

namespace AeonCore
{
	AModel* testModel = nullptr;
	Animation* testMove = nullptr;
	Animator* testAnimator = nullptr;

	/// <summary>
	/// For Batching...
	/// </summary>
	std::vector<Vertex>				Batcher::mBatchedVertices;
	std::vector<uint32_t>			Batcher::mBatchedIndicies;
	std::shared_ptr<VertexArray>	Batcher::mVertexArray;
	std::shared_ptr<VertexBuffer>	Batcher::mVertexBuffer;
	std::shared_ptr<IndexBuffer>	Batcher::mIndexBuffer;

	int RenderSystem::OFFSET_SPOT = 1;
	int RenderSystem::OFFSET_DIRECTIONAL = OFFSET_SPOT + 5;
	int RenderSystem::OFFSET_POINT = OFFSET_DIRECTIONAL + 5;

	bool RenderSystem::onShadow = false;
	bool RenderSystem::HDR = false;
	float RenderSystem::mGamma = 1.0f;

	void RenderSystem::RenderDepthMapFromLightPerspective(glm::mat4 light_ProjectionMat, glm::mat4 light_ViewMat, const ShadowMapFBO::MODE& mode)
	{
		glm::mat4 vp = light_ProjectionMat * light_ViewMat;

		AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();
		FrustrumCulling frustumCulling(light_ViewMat, light_ProjectionMat);
		std::vector<Entity> visibleEntities;

		std::vector<glm::mat4> entityTransforms;
		std::vector<FrustrumCullingUtilities::AABB> entityAABBs;

		std::unordered_map<Model*, FrustrumCullingUtilities::AABB> aabbCache;
		for (Entity& e : gCoordinator.GetEntities())
		{
			// Skip light models to be rendered
			if (gCoordinator.HasComponent<Lighting>(e))
				continue;

			if (gCoordinator.HasComponent<Render>(e)) {
				Transform& trans = gCoordinator.GetComponent<Transform>(e);
				entityTransforms.push_back(trans.UpdateMatrix());

				Render& render = gCoordinator.GetComponent<Render>(e);
				Model* modelPtr = render.GetModelPtr();
				// Check if AABB is cached
				if (aabbCache.find(modelPtr) == aabbCache.end()) {
					// Cache miss, calculate and cache the AABB
					aabbCache[modelPtr] = GetAABB(*modelPtr);
				}
				// Use the cached AABB
				entityAABBs.push_back(aabbCache[modelPtr]);
				frustumCulling.cullAABBsAgainstFrustum(entityTransforms, entityAABBs, visibleEntities, e);
			}
		}

		for (auto& v : visibleEntities)
		{
			if (gCoordinator.HasComponent<Lighting>(v))
				continue;

			if (gCoordinator.HasComponent<Render>(v) && gCoordinator.HasComponent<Transform>(v)) {
				Transform& trans = gCoordinator.GetComponent<Transform>(v);
				trans.GetWorldMatrix();

				// Use directional and spot shader pgm
				if (mode == ShadowMapFBO::MODE::DIRECTIONAL || mode == ShadowMapFBO::MODE::SPOT)
				{
					m_shadowMapPgm_SpotDirectional->use();
					m_shadowMapPgm_SpotDirectional->setMat4("vp", vp);

					{
						m_shadowMapPgm_SpotDirectional->setMat4("mdl", trans.GetWorldMatrix());		// check if fail
					}
				}

				// Use point shader pgm
				else
				{
					depthShader_Point->use();
					depthShader_Point->setMat4("vp", vp);

					{
						depthShader_Point->setMat4("mdl", trans.GetWorldMatrix());		// check if fail
					}
				}

				// Render 
				Render& render = gCoordinator.GetComponent<Render>(v);
				Model& m = *render.GetModelPtr();
				for (auto& mesh : m.GetMeshes())
				{
					mesh.Draw();
				}

			}
		}
	}

	void RenderSystem::RenderSceneWithShadows(glm::mat4 camera_ProjectionMat, glm::mat4 camera_ViewMat, ShadowMapFBO* shadowMaps, ShadowMapFBO::MODE mode)
	{

		/*AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();

		for (auto& e : gCoordinator.GetEntities())
		{
			if (gCoordinator.HasComponent<Render>(e) && gCoordinator.HasComponent<Transform>(e))
			{
				{
					Transform& trans = gCoordinator.GetComponent<Transform>(e);
					Render& render = gCoordinator.GetComponent<Render>(e);
					Model& m = *render.GetModelPtr();
					Material& material = *render.GetMaterialPtr();

					Rendering::GetInstance().DrawModel(trans, camera_ViewMat, camera_ProjectionMat, m, material, shadowMaps->GetID(), mode);

				}
			}
			if (gCoordinator.HasComponent<Particles>(e))
			{

				Particles* particles = &gCoordinator.GetComponent<Particles>(e);
				for (auto& p : particles->GetParticleContainer())
				{
					if (p.life > 0.0f)
					{
						Transform particles_transform;
						particles_transform.SetPosition(p.position);
						particles_transform.SetScale(p.scale);

						Rendering::GetInstance().DrawModel(particles_transform, camera_ViewMat, camera_ProjectionMat, *particles->GetModelPtr(), *particles->GetMaterialPtr(), shadowMaps->GetID(), mode);
					}
				}
			}
		}*/

	}

	void RenderSystem::RenderScene(Shader& shader)
	{
		AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();
		for (Entity& e : gCoordinator.GetEntities())
		{
			if (gCoordinator.HasComponent<Lighting>(e)) continue;

			Transform& transform = gCoordinator.GetComponent<Transform>(e);
			if (gCoordinator.HasComponent<Render>(e))
			{
				shader.use();
				shader.setMat4("mdl", transform.UpdateMatrix());
				Render& render = gCoordinator.GetComponent<Render>(e);
				Model& m = *render.GetModelPtr();
				for (auto& mesh : m.GetMeshes())
				{
					mesh.Draw();
				}
			}
		}
	}

	FrustrumCullingUtilities::AABB RenderSystem::GetAABB(Model& model)
	{
#if 0
		// Initialize SIMD variables for min/max with extreme values
		__m128 minVec = _mm_set1_ps(std::numeric_limits<float>::max());
		__m128 maxVec = _mm_set1_ps(std::numeric_limits<float>::lowest());

		for (auto& mesh : model.GetMeshes()) {
			for (const auto& vertex : mesh.GetVertices()) {
				// Load vertex positions into SIMD variables
				__m128 vertexPos = _mm_setr_ps(vertex.Position.x, vertex.Position.y, vertex.Position.z, 0.0f);

				// Compute min/max
				minVec = _mm_min_ps(minVec, vertexPos);
				maxVec = _mm_max_ps(maxVec, vertexPos);
			}
		}

		// Extract the min/max values from SIMD variables
		glm::vec3 min, max;
		_mm_store_ss(&min.x, minVec);
		_mm_store_ss(&min.y, _mm_shuffle_ps(minVec, minVec, _MM_SHUFFLE(1, 1, 1, 1)));
		_mm_store_ss(&min.z, _mm_shuffle_ps(minVec, minVec, _MM_SHUFFLE(2, 2, 2, 2)));
		_mm_store_ss(&max.x, maxVec);
		_mm_store_ss(&max.y, _mm_shuffle_ps(maxVec, maxVec, _MM_SHUFFLE(1, 1, 1, 1)));
		_mm_store_ss(&max.z, _mm_shuffle_ps(maxVec, maxVec, _MM_SHUFFLE(2, 2, 2, 2)));

		return FrustrumCullingUtilities::AABB(min, max);
#endif

		glm::vec3 min(std::numeric_limits<float>::max());
		glm::vec3 max(std::numeric_limits<float>::lowest());

		for (auto& mesh : model.GetMeshes()) {
			const auto& vertices = mesh.GetVertices();
			// Assuming vertices.size() is always a multiple of 4 for simplicity.
			// In practice, you'd need to handle the remainder.
			for (size_t i = 0; i < vertices.size(); i += 4) {
				min = glm::min(min, vertices[i].Position);
				max = glm::max(max, vertices[i].Position);

				min = glm::min(min, vertices[i + 1].Position);
				max = glm::max(max, vertices[i + 1].Position);

				min = glm::min(min, vertices[i + 2].Position);
				max = glm::max(max, vertices[i + 2].Position);

				min = glm::min(min, vertices[i + 3].Position);
				max = glm::max(max, vertices[i + 3].Position);
			}

			// Handle the remainder if vertices.size() is not a multiple of 4
			// This part is necessary to avoid missing vertices in models with a vertex count not divisible by 4.
			for (size_t i = vertices.size() - (vertices.size() % 4); i < vertices.size(); ++i) {
				min = glm::min(min, vertices[i].Position);
				max = glm::max(max, vertices[i].Position);
			}
		}

		return FrustrumCullingUtilities::AABB(min, max);
	}

	static int mapIndex_Point = 0;
	static int mapIndex_Spotlight = 0;
	static int mapIndex_Directional = 0;
	void RenderSystem::ShadowMapPass()
	{

		mapIndex_Point = 0;
		mapIndex_Spotlight = 0;
		mapIndex_Directional = 0;

		AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();
		for (auto& light : LightManager::GetInstance().GetLights())
		{
			float near_plane = 0.1f, far_plane = 1000.0f;
			glm::mat4 projection, view;

			switch (light.second->GetLightType())
			{
			case Lighting::LightType::POINT:
			{
				float near_plane = 0.1f;
				float far_plane = 50.0f;
				glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_MAP_WIDTH / (float)SHADOW_MAP_HEIGHT, near_plane, far_plane);
				std::vector<glm::mat4> view;
				std::array<glm::mat4, 6> vp;

				view.push_back(glm::lookAt(light.second->GetPosition(), light.second->GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
				view.push_back(glm::lookAt(light.second->GetPosition(), light.second->GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
				view.push_back(glm::lookAt(light.second->GetPosition(), light.second->GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
				view.push_back(glm::lookAt(light.second->GetPosition(), light.second->GetPosition() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
				view.push_back(glm::lookAt(light.second->GetPosition(), light.second->GetPosition() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
				view.push_back(glm::lookAt(light.second->GetPosition(), light.second->GetPosition() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

				for (int i = 0; i < vp.size(); ++i)
				{
					vp[i] = shadowProj * view[i];
				}

				light.second->SetVPMatrixPoint(vp);
				light.second->SetShadowMapIndex(mapIndex_Point);
				m_shadowMaps_Point[mapIndex_Point].BindForWriting();

				m_colorPassProgram->use();
				m_colorPassProgram->setFloat("far_plane", far_plane);

				depthShader_Point->use();
				for (unsigned int k = 0; k < 6; k++)
					depthShader_Point->setMat4("shadowMatrices[" + std::to_string(k) + "]", vp[k]);

				depthShader_Point->setVec3("lightPos", light.second->GetPosition());
				depthShader_Point->setFloat("far_plane", far_plane);

				// RenderScene(*depthShader_Point);

				for (Entity& e : gCoordinator.GetEntities())
				{
					if (gCoordinator.HasComponent<Lighting>(e)) continue;

					Transform& transform = gCoordinator.GetComponent<Transform>(e);
					if (gCoordinator.HasComponent<Render>(e))
					{
						depthShader_Point->use();
						depthShader_Point->setMat4("mdl", transform.UpdateMatrix());
						Render& render = gCoordinator.GetComponent<Render>(e);
						Model& m = *render.GetModelPtr();
						for (auto& mesh : m.GetMeshes())
						{
							mesh.Draw();
						}
					}
				}


				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				mapIndex_Point++;
				break;
			}
			case Lighting::LightType::SPOTLIGHT:
			{
				projection = glm::perspective(glm::radians(90.0f), 1.0f, near_plane, far_plane);

				// Calculate direction vector
				glm::vec3 direction = light.second->GetLightDirection();
				// Normalize the direction vector if it's not zero
				if (glm::length(direction) > 0.0001) { // Check if the direction vector is not effectively zero
					direction = glm::normalize(direction);
				}
				else {
					// Handle zero or near-zero direction vector case
					// For example, skip this light or set a default direction
					direction = glm::vec3(0, -1, 0); // Example fallback direction
				}

				// Calculate view matrix with safety checks
				glm::vec3 position = light.second->GetPosition();
				glm::vec3 target = position + direction;
				glm::vec3 upVector = glm::vec3(0, 1, 0);

				// Adjust up vector if it's parallel or nearly parallel to direction
				if (glm::abs(glm::dot(direction, upVector)) > 0.9999) {
					upVector = glm::vec3(1, 0, 0); // Choose a different up vector
				}

				glm::mat4 view = glm::lookAt(position, target, upVector);


				m_shadowMaps_Spot[mapIndex_Spotlight].BindForWriting();
				Lighting& lightComp = *light.second;//gCoordinator.GetComponent<Lighting>(light.secon);
				{
					glm::mat4 vp = projection * view;
					lightComp.SetVPMatrix(vp);
					lightComp.SetShadowMapIndex(mapIndex_Spotlight);

					//continue;
				}
				for (Entity& e : gCoordinator.GetEntities())
				{

					Transform& transform = gCoordinator.GetComponent<Transform>(e);
					if (gCoordinator.HasComponent<Render>(e) && e.GetEntityID() != lightComp.GetEntityID())
					{

						glm::mat4 model = transform.UpdateMatrix();
						glm::mat4 MVP = projection * view * model;
						m_shadowMapPgm_SpotDirectional->use();
						m_shadowMapPgm_SpotDirectional->setMat4("MVP", MVP);

						Render& render = gCoordinator.GetComponent<Render>(e);
						Model& m = *render.GetModelPtr();
						for (auto& mesh : m.GetMeshes())
						{
							mesh.Draw();
						}
					}
				}

				mapIndex_Spotlight++;
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				break;
			}

			case Lighting::LightType::DIRECTIONAL:
			{
				projection = glm::ortho(-65.0f, 65.0f, -65.0f, 65.0f, near_plane, far_plane);
				view = glm::lookAt(light.second->GetPosition(), light.second->GetPosition() + light.second->GetLightDirection(), glm::vec3(0, 1, 0));

				m_shadowMaps_Directional[mapIndex_Directional].BindForWriting();
				Lighting& lightComp = *light.second;
				{
					glm::mat4 vp = projection * view;
					lightComp.SetVPMatrix(vp);
					lightComp.SetShadowMapIndex(mapIndex_Directional);
				}

				for (Entity& e : gCoordinator.GetEntities())
				{

					Transform& transform = gCoordinator.GetComponent<Transform>(e);
					if (gCoordinator.HasComponent<Render>(e) && e.GetEntityID() != lightComp.GetEntityID())
					{

						glm::mat4 model = transform.UpdateMatrix();
						glm::mat4 MVP = projection * view * model;
						m_shadowMapPgm_SpotDirectional->use();
						m_shadowMapPgm_SpotDirectional->setMat4("MVP", MVP);

						Render& render = gCoordinator.GetComponent<Render>(e);
						Model& m = *render.GetModelPtr();
						for (auto& mesh : m.GetMeshes())
						{
							mesh.Draw();
						}
					}
				}

				mapIndex_Directional++;
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				break;

				// projection = glm::perspective(glm::radians(90.0f), 1.0f, near_plane, far_plane);
				// glm::vec3 direction = light.second->GetLightDirection();
				// // Normalize the direction vector if it's not zero
				// if (glm::length(direction) > 0.0001) { // Check if the direction vector is not effectively zero
				// 	direction = glm::normalize(direction);
				// }
				// else {
				// 	// Handle zero or near-zero direction vector case
				// 	// For example, skip this light or set a default direction
				// 	direction = glm::vec3(0, -1, 0); // Example fallback direction
				// }
				// 
				// // Calculate view matrix with safety checks
				// glm::vec3 position = light.second->GetPosition();
				// glm::vec3 target = position + direction;
				// glm::vec3 upVector = glm::vec3(0, 1, 0);
				// 
				// // Adjust up vector if it's parallel or nearly parallel to direction
				// if (glm::abs(glm::dot(direction, upVector)) > 0.9999) {
				// 	upVector = glm::vec3(1, 0, 0); // Choose a different up vector
				// }
				// 
				// glm::mat4 view = glm::lookAt(position, target, upVector);
				// m_shadowMaps_Directional[mapIndex_Directional].BindForWriting();
				// 
				// Lighting& lightComp = *light.second;//gCoordinator.GetComponent<Lighting>(light.secon);
				// {
				// 	glm::mat4 vp = projection * view;
				// 	lightComp.SetVPMatrix(vp);
				// 	lightComp.SetShadowMapIndex(mapIndex_Spotlight);
				// 
				// 	//continue;
				// }
				// for (Entity& e : gCoordinator.GetEntities())
				// {
				// 
				// 	Transform& transform = gCoordinator.GetComponent<Transform>(e);
				// 	if (gCoordinator.HasComponent<Render>(e) && e.GetEntityID() != lightComp.GetEntityID())
				// 	{
				// 
				// 		glm::mat4 model = transform.UpdateMatrix();
				// 		glm::mat4 MVP = projection * view * model;
				// 		m_shadowMapPgm_SpotDirectional->use();
				// 		m_shadowMapPgm_SpotDirectional->setMat4("MVP", MVP);
				// 
				// 		Render& render = gCoordinator.GetComponent<Render>(e);
				// 		Model& m = *render.GetModelPtr();
				// 		for (auto& mesh : m.GetMeshes())
				// 		{
				// 			mesh.Draw();
				// 		}
				// 	}
				// }
				// mapIndex_Directional++;
				// glBindFramebuffer(GL_FRAMEBUFFER, 0);
				// break;
			}


			}
		}
	}
	void RenderSystem::LightingPass()
	{
		// for each entity:
		// 	render entity

		/* glBindFramebuffer(GL_FRAMEBUFFER, 0);
		 glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

		AeonCore::Camera& cam = AeonCore::CameraManager::GetInstance().GetActiveCamera();
		glm::mat4 view = cam.GetCameraViewMatrix();
		glm::mat4 proj = cam.GetCameraProjectionMatrix();

		// Rneder all entities
		auto fb = AeonCore::FramebufferManager::GetInstance().GetFramebuffer("f1");

		fb->Resize(mwindowHandle->GetWidth(), mwindowHandle->GetHeight());
		fb->Bind();

		unsigned int k = 0;

		// if in level 1
		//Rendering::GetInstance().DrawSkyBox("SkyBox", view, proj);

		AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();

		auto& layers = AeonCore::Renderer2DSystem::GetLayers();

		Shader* shader_LightOnto = ResourceManager::GetInstance().GetShader("AEON_MODEL_SHADER");
		shader_LightOnto->use();
		shader_LightOnto->setFloat("gamma", mGamma);

		for (int i = 0; i < Lighting::MAX_LIGHTS; ++i)
		{
			m_shadowMaps_Directional[i].BindForReading(GL_TEXTURE0 + i + OFFSET_DIRECTIONAL);
			m_shadowMaps_Spot[i].BindForReading(GL_TEXTURE0 + i + OFFSET_SPOT);
			m_shadowMaps_Point[i].BindForReading(GL_TEXTURE0 + i + OFFSET_POINT);

			shader_LightOnto->use();
			shader_LightOnto->setInt("shadowMap_Directionallights[" + std::to_string(i) + "]", i + OFFSET_DIRECTIONAL);
			shader_LightOnto->setInt("shadowMap_Spotlights[" + std::to_string(i) + "]", i + OFFSET_SPOT);
			shader_LightOnto->setInt("shadowMap_Pointlights[" + std::to_string(i) + "]", i + OFFSET_POINT);
		}

		/*for (auto& light : LightManager::GetInstance().GetLights())
		{
			switch (light.second->GetLightType())
			{
			case Lighting::LightType::DIRECTIONAL:
			{
				for (int i = 0; i < Lighting::MAX_LIGHTS; ++i)
				{
					m_shadowMaps_Directional[i].BindForReading(GL_TEXTURE0 + i + OFFSET_DIRECTIONAL);
					m_shadowMaps_Spot[i].BindForReading(GL_TEXTURE0 + i + OFFSET_SPOT);
					m_shadowMaps_Point[i].BindForReading(GL_TEXTURE0 + i + OFFSET_POINT);

					shader_LightOnto->use();
					shader_LightOnto->setInt("shadowMap_Directionallights[" + std::to_string(i) + "]", i + OFFSET_DIRECTIONAL);
					shader_LightOnto->setInt("shadowMap_Spotlights[" + std::to_string(i) + "]", i + OFFSET_SPOT);
					shader_LightOnto->setInt("shadowMap_Pointlights[" + std::to_string(i) + "]", i + OFFSET_POINT);
				}

				for (auto& e : gCoordinator.GetEntities())
				{
					if (gCoordinator.HasComponent<Render>(e) && gCoordinator.HasComponent<Transform>(e))
					{
						{
							Transform& trans = gCoordinator.GetComponent<Transform>(e);
							Render& render = gCoordinator.GetComponent<Render>(e);
							Model& m = *render.GetModelPtr();
							Material& material = *render.GetMaterialPtr();
							Rendering::GetInstance().DrawModel(trans, view, proj, m, material, ShadowMapFBO::MODE::DIRECTIONAL);
						}
					}
				}
				break;
			}

			case Lighting::LightType::SPOTLIGHT:
			{
				for (int i = 0; i < Lighting::MAX_LIGHTS; ++i)
				{
					m_shadowMaps_Spot[i].BindForReading(GL_TEXTURE0 + i + OFFSET_SPOT);
					shader_LightOnto->use();
					shader_LightOnto->setInt("shadowMap_Directionallights[" + std::to_string(i) + "]", i + OFFSET_DIRECTIONAL);
					shader_LightOnto->setInt("shadowMap_Spotlights[" + std::to_string(i) + "]", i + OFFSET_SPOT);
					shader_LightOnto->setInt("shadowMap_Pointlights[" + std::to_string(i) + "]", i + OFFSET_POINT);
				}

				for (auto& e : gCoordinator.GetEntities())
				{
					if (gCoordinator.HasComponent<Render>(e) && gCoordinator.HasComponent<Transform>(e))
					{
						{
							Transform& trans = gCoordinator.GetComponent<Transform>(e);
							Render& render = gCoordinator.GetComponent<Render>(e);
							Model& m = *render.GetModelPtr();
							Material& material = *render.GetMaterialPtr();
							Rendering::GetInstance().DrawModel(trans, view, proj, m, material, ShadowMapFBO::MODE::DIRECTIONAL);
						}
					}
				}

				break;
			}

			case Lighting::LightType::POINT:
			{
				for (int i = 0; i < Lighting::MAX_LIGHTS; ++i)
				{
					m_shadowMaps_Point[i].BindForReading(GL_TEXTURE0 + i + OFFSET_POINT);
					shader_LightOnto->use();
					shader_LightOnto->setInt("shadowMap_Directionallights[" + std::to_string(i) + "]", i + OFFSET_DIRECTIONAL);
					shader_LightOnto->setInt("shadowMap_Spotlights[" + std::to_string(i) + "]", i + OFFSET_SPOT);
					shader_LightOnto->setInt("shadowMap_Pointlights[" + std::to_string(i) + "]", i + OFFSET_POINT);
				}

				for (auto& e : gCoordinator.GetEntities())
				{
					if (gCoordinator.HasComponent<Render>(e) && gCoordinator.HasComponent<Transform>(e))
					{
						{
							Transform& trans = gCoordinator.GetComponent<Transform>(e);
							Render& render = gCoordinator.GetComponent<Render>(e);
							Model& m = *render.GetModelPtr();
							Material& material = *render.GetMaterialPtr();
							Rendering::GetInstance().DrawModel(trans, view, proj, m, material, ShadowMapFBO::MODE::SPOT);
						}
					}
				}

				break;
			}
			}
		}*/

		//for (int i = 0; i < Lighting::MAX_LIGHTS; ++i)
		//{
		//	m_shadowMaps_Point[i].BindForReading(GL_TEXTURE0 + i + OFFSET_POINT);
		//	m_shadowMaps_Directional[i].BindForReading(GL_TEXTURE0 + i + OFFSET_DIRECTIONAL);
		//	m_shadowMaps_Spot[i].BindForReading(GL_TEXTURE0 + i + OFFSET_SPOT);			// why texture 144 which is the first shadow map is not sent to fragment shader?
		//	shader_LightOnto->use();
		//	shader_LightOnto->setInt("shadowMap_Directionallights[" + std::to_string(i) + "]", i + OFFSET_DIRECTIONAL);
		//	shader_LightOnto->setInt("shadowMap_Spotlights[" + std::to_string(i) + "]", i + OFFSET_SPOT);
		//	shader_LightOnto->setInt("shadowMap_Pointlights[" + std::to_string(i) + "]", i + OFFSET_POINT);
		//}
		//for (auto& e : gCoordinator.GetEntities())
		//{
		//	if (gCoordinator.HasComponent<Render>(e) && gCoordinator.HasComponent<Transform>(e))
		//	{
		//		{
		//			Transform& trans = gCoordinator.GetComponent<Transform>(e);
		//			Render& render = gCoordinator.GetComponent<Render>(e);
		//			Model& m = *render.GetModelPtr();
		//			Material& material = *render.GetMaterialPtr();
		//			Rendering::GetInstance().DrawModel(trans, view, proj, m, material, ShadowMapFBO::MODE::SPOT);
		//		}
		//	}
		//	if (gCoordinator.HasComponent<Particles>(e))
		//	{

		//		Particles* particles = &gCoordinator.GetComponent<Particles>(e);
		//		for (auto& p : particles->GetParticleContainer())
		//		{
		//			if (p.life > 0.0f)
		//			{
		//				Transform particles_transform;
		//				particles_transform.SetPosition(p.position);
		//				particles_transform.SetScale(p.scale);

		//				Rendering::GetInstance().DrawModel(particles_transform, view, proj, *particles->GetModelPtr(), *particles->GetMaterialPtr(), ShadowMapFBO::MODE::SPOT);
		//			}
		//		}
		//	}
		//}

		for (auto& e : gCoordinator.GetEntities())
		{
			if (gCoordinator.HasComponent<Render>(e) && gCoordinator.HasComponent<Transform>(e))
			{
				{
					Transform& trans = gCoordinator.GetComponent<Transform>(e);
					Render& render = gCoordinator.GetComponent<Render>(e);
					Model& m = *render.GetModelPtr();
					Material& material = *render.GetMaterialPtr();
					Rendering::GetInstance().DrawModel(trans, view, proj, m, material, ShadowMapFBO::MODE::SPOT);
				}
			}

			if (gCoordinator.HasComponent<Particles>(e))
			{
				Particles* particles = &gCoordinator.GetComponent<Particles>(e);
				for (auto& p : particles->GetParticleContainer())
				{
					if (p.life > 0.0f)
					{
						Transform particles_transform;
						particles_transform.SetPosition(p.position);
						particles_transform.SetScale(p.scale);

						Rendering::GetInstance().DrawModel(particles_transform, view, proj, *particles->GetModelPtr(), *particles->GetMaterialPtr(), ShadowMapFBO::MODE::SPOT);
					}
				}
			}
		}


		if (AeonCore::Rendering::GetInstance().RenderUIEditor)
		{
			for (const auto& layer : layers)
			{
				for (const auto& subLayer : layer.second)
				{
					AeonCore::UI& ui = gCoordinator.GetComponent<UI>(subLayer.first);
					AeonCore::Transform& trans = gCoordinator.GetComponent<AeonCore::Transform>(ui.GetEntityID());

					switch (ui.GetFormat())
					{
					case AeonCore::UI::Format::Image:
						AeonCore::Rendering::GetInstance().DrawTexture2D(trans, cam, *subLayer.second);
						break;

					case AeonCore::UI::Format::Word:
						AeonCore::Rendering::GetInstance().DrawFont(ui.GetText(), ui, trans, cam);
						break;

					}
				}
			}
		}

		/////////////////////////////////////////////////////// PHYSICS DEBUG DRAW HI ROMMEL ///////////////////////////////////////////
		if (Application::GetApplication().IsEditorOn())
		{
			auto sceneTemp = &AeonCore::ACPhysics::PhysicsInternal::GetInstance().GetPhysicsScene();

			sceneTemp->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);
			sceneTemp->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);

			//Camera &cam = CameraManager::GetInstance().GetActiveCamera();

			const physx::PxRenderBuffer& rb = sceneTemp->getRenderBuffer();
			for (physx::PxU32 i = 0; i < rb.getNbLines(); i++)
			{
				const physx::PxDebugLine& line = rb.getLines()[i];
				// render the line

				glm::vec3 grey(0.71f, 0.71f, 0.71f);

				if (AeonCore::Application::GetApplication().GetRuntimeState() == AeonCore::RuntimeState::Start)
				{
					AeonCore::Rendering::GetInstance().DrawLine(
						AeonCore::ACPhysicsUtility::FromPhysXVector(line.pos0), AeonCore::ACPhysicsUtility::FromPhysXVector(line.pos1), grey, cam);
				}
			}
		}

		// Green is not-play mode
		if (AeonCore::Application::GetApplication().GetRuntimeState() != AeonCore::RuntimeState::Start)
		{
			// AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();
			for (auto& e : gCoordinator.GetEntities())
			{
				if (gCoordinator.HasComponent<AeonCore::Collider>(e) && gCoordinator.HasComponent<AeonCore::RigidBody>(e))
				{
					AeonCore::Transform* trans = &gCoordinator.GetComponent<AeonCore::Transform>(e);
					AeonCore::Collider& collider = gCoordinator.GetComponent<AeonCore::Collider>(e);
					AeonCore::RigidBody& rigidbody = gCoordinator.GetComponent<AeonCore::RigidBody>(e);
					AeonCore::ACPhysics::CubeGeometry* cube = nullptr;
					AeonCore::ACPhysics::SphereGeometry* sphere = nullptr;
					if (rigidbody.GetPhysxActor())
					{
						switch (collider.GetColliderType())
						{
						case AeonCore::Collider::ColliderType::CUBE:
							cube = static_cast<AeonCore::ACPhysics::CubeGeometry*>(collider.GetGeometry());
							AeonCore::Rendering::GetInstance().DrawColliderBox(*trans, rigidbody, collider, *cube, cam);
							break;

						case AeonCore::Collider::ColliderType::SPHERE:
							sphere = static_cast<AeonCore::ACPhysics::SphereGeometry*>(collider.GetGeometry());
							AeonCore::Rendering::GetInstance().DrawColliderSphere(*trans, rigidbody, collider, *sphere, cam);
							break;

						default:
							break;
						}
					}
				}
			}
		}
		/////////////////////////////////////////////////////// PHYSICS DEBUG DRAW HI ROMMEL ///////////////////////////////////////////
		fb->Unbind();

	}

	void RenderSystem::CreateShadowMap()
	{
		for (int i = 0; i < m_shadowMaps_Spot.size(); ++i)
		{
			if (!m_shadowMaps_Spot[i].Init(SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT)) AeonCore_ERROR("ERROR");
		}

		for (int j = 0; j < m_shadowMaps_Directional.size(); ++j)
		{
			if (!m_shadowMaps_Directional[j].Init(SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT)) AeonCore_ERROR("ERROR");
		}

		for (int k = 0; k < m_shadowMaps_Point.size(); ++k)
		{
			if (!m_shadowMaps_Point[k].Init(SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, ShadowMapFBO::MODE::POINT)) AeonCore_ERROR("ERROR");
		}
	}


	void RenderSystem::Init()
	{
		Application& app = Application::GetApplication();
		mwindowHandle = &app.GetWindow();

		Rendering::GetInstance().OnStart(&app.GetWindow());

		//std::string path = "/resources/objects/FBXTEST/CallAllAnimsNoArm.fbx";
		std::string path = "/resources/objects/FBXTEST/Catwalk Walk Forward.fbx";


		// testModel = new AModel(parent.string() + path);
		// testMove = new Animation(parent.string() + path, testModel);
		// testAnimator = new Animator(testMove);
		Batcher::StartUp();

		AeonCore::FramebufferManager::GetInstance().CreateFramebuffer("f1",
			mwindowHandle->GetWidth(), mwindowHandle->GetHeight());


		std::filesystem::path working_dir(std::filesystem::current_path());
		auto parent_path = working_dir;

		// Point light depth map shader init
		depthShader_Point = ResourceManager::GetInstance().LoadShader("AEON_POINT_SHADOW_MAP",
			parent_path.string() + std::string("/resources/shaders/AEON_POINT_SHADOW_MAP.vert"),
			parent_path.string() + std::string("/resources/shaders/AEON_POINT_SHADOW_MAP.frag"),
			parent_path.string() + std::string("/resources/shaders/AEON_POINT_SHADOW_MAP.geom"));

		// Directional and spot light depth map shader init
		m_shadowMapPgm_SpotDirectional = ResourceManager::GetInstance().LoadShader("AEON_SPOT_DIRECTIONAL_SHADOW_MAP",
			parent_path.string() + std::string("/resources/shaders/AEON_SPOT_DIRECTIONAL_SHADOW_MAP.vert"),
			parent_path.string() + std::string("/resources/shaders/AEON_SPOT_DIRECTIONAL_SHADOW_MAP.frag"));

		// Color (render pass) shader init (Init @ Rendering.cpp)
		m_colorPassProgram = ResourceManager::GetInstance().GetShader("AEON_MODEL_SHADER");

		ResourceManager::GetInstance().LoadShader("AEON_HDR",
			parent_path.string() + std::string("/resources/shaders/AEON_HDR.vert"),
			parent_path.string() + std::string("/resources/shaders/AEON_HDR.frag"));

		CreateShadowMap();
	}


	// TODO : What does the render system update?
	void RenderSystem::Update()
	{
		ZoneScopedN("Render System Update");

		//	fustrum calling
		Coordinator& coord = Coordinator::GetInstance();
		for (auto entity : mEntities)
		{
			if (!coord.HasComponent<Transform>(entity))
				continue;

			Transform& trans = coord.GetComponent<Transform>(entity);

			if (!coord.HasComponent<Render>(entity))
				continue;

			Render& rend = coord.GetComponent<Render>(entity);
			trans.boundingVolume = generateAABB(rend.GetModelPtr());
		}

		/*
		* Main stage of rendering ent here
		* Consist of 2 stages: Shadow Pass + Color Pass
		* Shadow Pass will generate shadow maps for point, spot and directional lights
		* Color Pass then renders accordingly
		*/

		if (HDR)
		{
			m_colorPassProgram->use();
			m_colorPassProgram->setBool("hdr", true);
		}
		else
		{
			m_colorPassProgram->use();
			m_colorPassProgram->setBool("hdr", false);
		}

		glCullFace(GL_FRONT);
		ShadowMapPass();
		glCullFace(GL_FRONT);
		LightingPass();


	}

	void RenderSystem::Destroy()
	{
		/*delete testAnimator;
		delete testMove;
		delete testModel;*/
	}

	void RenderSystem::DrawIndexed(std::shared_ptr<VertexArray> vertexArray, Shader& shader, uint32_t indexCount)
	{
		shader.use();
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void RenderSystem::ClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void RenderSystem::ClearBits()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	void Batcher::StartUp()
	{
		// VAO
		// Create a huge amount, then per frame update buffer.
		mVertexArray.reset(VertexArray::Create());
		mVertexBuffer.reset(VertexBuffer::Create(MAX_VERTICES * sizeof(Vertex)));

		// VBO
		BufferLayout layout =
		{
			{ShaderDataType::Float3, "aPos"},
			{ShaderDataType::Float3, "aNormal"},
			{ShaderDataType::Float2, "aUV"},
			{ShaderDataType::Float3, "aTangent"},
			{ShaderDataType::Float3, "aBitTangent"},
			{ShaderDataType::Int4,   "aBoneId"},
			{ShaderDataType::Float4, "aWeights"},
			{ShaderDataType::Int,    "aEntityID"},

		};

		mVertexBuffer->SetLayout(layout);
		mVertexArray->AddVertexBuffer(mVertexBuffer);

		mIndexBuffer.reset(IndexBuffer::Create(MAX_INDICES));
		mVertexArray->SetIndexBuffer(mIndexBuffer);
		mVertexArray->UnBind();
	}

	void Batcher::StartBatching()
	{
		uint32_t lastTotalVertices = 0;
		AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();
		for (auto& e : gCoordinator.GetEntities())
		{
			if (gCoordinator.HasComponent<Render>(e) && gCoordinator.HasComponent<Transform>(e))
			{
				Transform& transform = gCoordinator.GetComponent<Transform>(e);
				Render& render = gCoordinator.GetComponent<Render>(e);
				Model* model = render.GetModelPtr();

				for (Mesh& mesh : model->GetMeshes())
				{
					std::vector<Vertex> vertices = mesh.GetWorldSpaceVertices(transform);
					std::vector<uint32_t>& indices = mesh.GetIndices();

					for (Vertex& vertex : vertices)
					{
						mBatchedVertices.push_back(vertex);
					}

					for (const uint32_t& index : indices)
					{
						mBatchedIndicies.push_back(index + lastTotalVertices);
					}
					lastTotalVertices += static_cast<uint32_t>(vertices.size());
				}
			}
		}


		if (mBatchedVertices.size() > 0)
			mVertexBuffer->UpdateBufferStore((void*)mBatchedVertices.data(), static_cast<uint32_t>(sizeof(Vertex) * mBatchedVertices.size()));


	}
	void Batcher::DrawBatch()
	{
		Shader* shader_LightOnto = ResourceManager::GetInstance().GetShader("AEON_MODEL_SHADER_BATCHING");
		//Shader* shader_LightSrc = ResourceManager::GetInstance().GetShader("AEON_LIGHT_SRC_SHADER");

		RenderSystem::DrawIndexed(mVertexArray, *shader_LightOnto, static_cast<uint32_t>(mBatchedIndicies.size()));
	}
	void Batcher::FlushBatch()
	{
		mBatchedVertices.clear();
		mBatchedIndicies.clear();
	}




}

