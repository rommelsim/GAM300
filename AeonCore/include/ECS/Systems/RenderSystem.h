#pragma once
/*!***************************************************************************
	\file			RenderSystem.h
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					create and manage 3D shapes for rendering
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Architecture/System.hpp"
#include "ECS/Architecture/Coordinator.hpp"

#include "ECS/Components/Render.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Font.h"
#include "ECS/Components/Collider.h"
//#include "ECS/Components/Collision.h"
#include "ECS/Components/UI.h"

#include "Core/Application.h"
#include "Editor/EditorCamera.h"
#include "Camera/CameraManager.h"

#include "Rendering/Rendering.h"
#include "Rendering/Framebuffer/FramebufferManager.h"

#include "Rendering/Buffers/OpenGLBuffers.h"
#include "Rendering/Arrays/OpenGLVertexArray.h"

#include "Rendering/Shadows/ShadowMapFBO.h"

#define SHADOW_MAP_WIDTH 512		
#define SHADOW_MAP_HEIGHT 512


namespace AeonCore
{
	namespace FrustrumCullingUtilities
	{
		struct AABB {
			glm::vec3 min = {};
			glm::vec3 max = {};
		};

		// Utility function to check if a value falls within a specified range
		inline bool within(float min, float val, float max) {
			return val >= min && val <= max;

		}

	}

	class FrustrumCulling {
	public:
		FrustrumCulling(const glm::mat4& view, const glm::mat4& projection)
			: camera_view(view), camera_projection(projection) {}

		void setCullingParameters(const glm::mat4& view, const glm::mat4& projection) {
			camera_view = view;
			camera_projection = projection;
		}

		void cullAABBsAgainstFrustum(const std::vector<glm::mat4>& transforms,
			const std::vector<FrustrumCullingUtilities::AABB>& aabbList,
			std::vector<Entity>& outVisibleList, Entity& ent) const 
		{
			glm::mat4 VP = camera_projection * camera_view;

			for (size_t i = 0; i < aabbList.size(); ++i) {
				glm::mat4 MVP = VP * transforms[i];

				const FrustrumCullingUtilities::AABB& aabb = aabbList[i];
				if (test_AABB_against_frustum(MVP, aabb)) {
					outVisibleList.push_back(ent);
				}
			}
		}

	private:
		glm::mat4 camera_view;
		glm::mat4 camera_projection;

		// Function to test AABB against frustum
		bool test_AABB_against_frustum(const glm::mat4& MVP, const FrustrumCullingUtilities::AABB& aabb) const {
			glm::vec4 corners[8] = {
				{aabb.min.x, aabb.min.y, aabb.min.z, 1.0}, // x y z
				{aabb.max.x, aabb.min.y, aabb.min.z, 1.0}, // X y z
				{aabb.min.x, aabb.max.y, aabb.min.z, 1.0}, // x Y z
				{aabb.max.x, aabb.max.y, aabb.min.z, 1.0}, // X Y z
				{aabb.min.x, aabb.min.y, aabb.max.z, 1.0}, // x y Z
				{aabb.max.x, aabb.min.y, aabb.max.z, 1.0}, // X y Z
				{aabb.min.x, aabb.max.y, aabb.max.z, 1.0}, // x Y Z
				{aabb.max.x, aabb.max.y, aabb.max.z, 1.0}, // X Y Z
			};

			bool inside = false;

			for (size_t corner_idx = 0; corner_idx < 8; corner_idx++) {
				glm::vec4 corner = MVP * corners[corner_idx];
				inside = inside ||
					FrustrumCullingUtilities::within(-corner.w, corner.x, corner.w) &&
					FrustrumCullingUtilities::within(-corner.w, corner.y, corner.w) &&
					FrustrumCullingUtilities::within(0.0f, corner.z, corner.w);
			}
			return inside;
		}
	};


	class AEON_API RenderSystem : public System
	{
	private:
		Window* mwindowHandle;
	
		void ShadowMapPass();
		void LightingPass();
		void CreateShadowMap();

		std::array<ShadowMapFBO, Lighting::MAX_LIGHTS> m_shadowMaps_Spot;
		std::array<ShadowMapFBO, Lighting::MAX_LIGHTS> m_shadowMaps_Directional;
		std::array<ShadowMapFBO, Lighting::MAX_LIGHTS> m_shadowMaps_Point;
		Shader* depthShader_Point;				// shd pgm for point light depth map 
		Shader* m_shadowMapPgm_SpotDirectional;			// shader pgm for directional and spot light depth map
		Shader* m_colorPassProgram;						// shdr pgm for color pass (usual rendering)

		void RenderDepthMapFromLightPerspective(glm::mat4 light_ProjectionMat, glm::mat4 light_ViewMat, const ShadowMapFBO::MODE& mode);
		void RenderSceneWithShadows(glm::mat4 camera_ProjectionMat, glm::mat4 camera_ViewMat, ShadowMapFBO* shadowMaps, ShadowMapFBO::MODE);
		void RenderScene(Shader& shader);
		FrustrumCullingUtilities::AABB GetAABB(Model& model);
	public:
		RenderSystem() = default;

		void Init() override;

		void Update() override;
		
		void Destroy() override;

		static void DrawIndexed(std::shared_ptr<VertexArray> vertexArray, Shader& shader, uint32_t indexCount = 0);

		static void ClearColor(float r, float g, float b, float a);
		static void ClearBits();

		static bool onShadow;
		static bool HDR;

		static int OFFSET_POINT;
		static int OFFSET_SPOT;
		static int OFFSET_DIRECTIONAL;
		static float mGamma;
		/*void SetShadowFlag(bool flag) { onShadow = flag; }
		bool GetShadowFlag()		  { return onShadow; }*/
	};

	

	class Batcher
	{
	private:

		
		static std::vector<Vertex> mBatchedVertices;
		static std::vector<uint32_t> mBatchedIndicies;

		static std::shared_ptr<VertexArray> mVertexArray;
		static std::shared_ptr<VertexBuffer> mVertexBuffer;
		static std::shared_ptr<IndexBuffer> mIndexBuffer;

	public:

		// 
		static const int MAX_VERTICES = 1000000;
		static const int MAX_INDICES = 6 * MAX_VERTICES;

		// Called once
		static void StartUp();

		// Called per frame
		static void StartBatching();
		static void DrawBatch();
		static void FlushBatch();
	};
}
