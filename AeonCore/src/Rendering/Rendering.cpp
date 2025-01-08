#include "acpch.h"
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


#include "Rendering/Rendering.h"
#include "Input/Input.h"
#include "Rendering/GeomLib.h"
#include "Core/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ECS/Components/Collider.h"
#include "ECS/Components/Particle.h"
#include "ECS/Components/RigidBody.h"
#include <Rendering/Buffers/OpenGLBuffers.h>




namespace AeonCore
{

	std::shared_ptr<OpenGLUniformBuffer> UBO_Matrices;
	std::shared_ptr<OpenGLUniformBuffer> UBO_Light;

	float Rendering::lastX; // = SCR_WIDTH / 2.0f;
	float Rendering::lastY; // = SCR_HEIGHT / 2.0f;
	int Rendering::m_winWidth;
	int Rendering::m_winHeight;

	GLFWwindow* Rendering::m_windowHandle = NULL;
	bool Rendering::RenderUIEditor = true;

	bool Rendering::firstMouse = true;
	//std::unique_ptr<Camera> Rendering::default_camera;

	void Rendering::FrameStart()
	{	
		processInput(m_windowHandle, TimerStep::GetInstance().get_delta_time());
	}
	void Rendering::FrameEnd()
	{
		glfwSwapBuffers(m_windowHandle);
		glfwPollEvents();
	}
	bool Rendering::CheckWindowIsClose()
	{
		if (glfwWindowShouldClose(m_windowHandle)) return false;
		return true;
	}

	// glm::vec3 Rendering::GetUniversalLightColor()
	// {
	// 	return m_universal_light_color;
	// }
	// 
	// void Rendering::SetUniversalLightColor(glm::vec3 _pos)
	// {
	// 	m_universal_light_color = _pos;
	// }

	void Rendering::Exit()
	{
		glfwTerminate();
	}

	void Rendering::OnStart(Window *win)
	{
		m_windowHandle = (GLFWwindow *)win->GetNativeWindow();
		init(win->GetWidth(), win->GetHeight());
	}

	void Rendering::init(int width, int height)
	{
		// m_universal_light_color	= glm::vec3(1,1,1);

		// Use glGetString to get OpenGL information
		m_version		=	reinterpret_cast<const char*>(glGetString(GL_VERSION));
		m_vendor		=	reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		m_renderer		=	reinterpret_cast<const char*>(glGetString(GL_RENDERER));

		AeonCore_INFO("VERSION: {}", m_version);
		AeonCore_INFO("VENDOR: {}", m_vendor);
		AeonCore_INFO("RENDERER: {}", m_renderer);

		glfwSetFramebufferSizeCallback(m_windowHandle, framebuffer_size_callback);
		glfwSetCursorPosCallback(m_windowHandle, mouse_callback);
		glfwSetScrollCallback(m_windowHandle, scroll_callback);
		glfwSetErrorCallback(error_callback);

		// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
		stbi_set_flip_vertically_on_load(true);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		m_winWidth = width;
		m_winHeight = height;

		lastX = width / 2.0f;
		lastY = height / 2.0f;

		std::filesystem::path working_dir(std::filesystem::current_path());
		auto parent_path = working_dir;

		///////////////////////////////////////////////////////////////////////////////
		///		Setup of Vertices and Indices for loading of meshes and shaders		///
		///////////////////////////////////////////////////////////////////////////////
		/*-----------------------------------------------------------------------
		 *	CUBE
		 -----------------------------------------------------------------------*/
		{
			std::vector<float>	CUBE_VERTICES2	// vec3 pos, vec3 normal
			{
				/*-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
				0.5f,  -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
				0.5f,   0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
				0.5f,   0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
				-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
				-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,

				-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
				0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,
				0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,
				0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

				-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
				-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
				-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,

				0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
				0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
				0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
				0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
				0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
				0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
				0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
				0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
				0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

				-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
				0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,
				0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
				0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f*/

				 -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
				  0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
				  0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
				 -0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
				 -0.5f, -0.5f,  0.5f, 0.0f,  0.0f, -1.0f,
				  0.5f, -0.5f,  0.5f, 0.0f,  0.0f, -1.0f,
				  0.5f,  0.5f,  0.5f, 0.0f,  0.0f, -1.0f,
				 -0.5f,  0.5f,  0.5f, 0.0f,  0.0f, -1.0f
			};
			

			std::vector<Vertex> CUBE_VERTICES
			{
				// Pos, Normal
				Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f)),
				Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f)),
				Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f)),
				Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f)),
				Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f, -1.0f)),
				Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f, -1.0f)),
				Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f, -1.0f)),
				Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f, -1.0f))
			};

			std::vector<uint32_t> CUBE_INDICES =
			{
				0, 1, 2, 2, 3, 0,
				4, 5, 6, 6, 7, 4,
				0, 3, 7, 7, 4, 0,
				1, 2, 6, 6, 5, 1,
				0, 1, 5, 5, 4, 0,
				2, 3, 7, 7, 6, 2
			};

			BufferLayout layout =
			{
				{ShaderDataType::Float3, "aPos"},
				{ShaderDataType::Float3, "aNormal"}
			};

			ResourceManager::GetInstance().LoadMesh("AEON_CUBE_MESH", CUBE_VERTICES, CUBE_INDICES, layout);
			ResourceManager::GetInstance().LoadShader("AEON_CUBE_SHADER",
				parent_path.string() + std::string("/resources/shaders/AEON_CUBE.vert"),
				parent_path.string() + std::string("/resources/shaders/AEON_CUBE.frag"));
		}

		/*-----------------------------------------------------------------------
		 *	WIREFRAME CUBE
		 -----------------------------------------------------------------------*/
		{
			std::vector<float>	CUBE_WIREFRAME_VERTICES	// vec3 pos, vec3 normal
			{
				-0.5f, -0.5f, -0.5f, // Vertex 0 (front bottom left)
				 0.5f, -0.5f, -0.5f, // Vertex 1 (front bottom right)
				 0.5f,  0.5f, -0.5f, // Vertex 2 (front top right)
				-0.5f,  0.5f, -0.5f, // Vertex 3 (front top left)
				-0.5f, -0.5f,  0.5f, // Vertex 4 (back bottom left)
				 0.5f, -0.5f,  0.5f, // Vertex 5 (back bottom right)
				 0.5f,  0.5f,  0.5f, // Vertex 6 (back top right)
				-0.5f,  0.5f,  0.5f  // Vertex 7 (back top left)
			};

			std::vector<uint32_t> CUBE_WIREFRAME_INDICES =
			{
				// Edges of the cube (12 lines)
				0, 1, 1, 2, 2, 3, 3, 0, // Front face
				4, 5, 5, 6, 6, 7, 7, 4, // Back face
				0, 4, 1, 5, 2, 6, 3, 7  // Connecting lines between front and back faces
			};

			BufferLayout layout =
			{
				{ShaderDataType::Float3, "aPos"},

			};

			ResourceManager::GetInstance().LoadMesh("AEON_CUBE_WIREFRAME_MESH", CUBE_WIREFRAME_VERTICES, CUBE_WIREFRAME_INDICES, layout);
			ResourceManager::GetInstance().LoadShader("AEON_CUBE_WIREFRAME_SHADER",
				parent_path.string() + std::string("/resources/shaders/AEON_CUBE_WIREFRAME.vert"), parent_path.string() + std::string("/resources/shaders/AEON_CUBE_WIREFRAME.frag"));
		}

		/*-----------------------------------------------------------------------
		 *	TRIANGLE
		 -----------------------------------------------------------------------*/
		{
			// Shaders for rendering Triangle (filled)
			std::vector<float> TRIANGLE_VERTICES
			{
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f
			};

			BufferLayout layout =
			{
				{ShaderDataType::Float3, "aPos"},
			};

			std::vector<uint32_t> TRIANGLE_INDICES = { 0,1,2 };

			ResourceManager::GetInstance().LoadMesh("AEON_TRIANGLE_MESH", TRIANGLE_VERTICES, TRIANGLE_INDICES, layout);

			ResourceManager::GetInstance().LoadShader("AEON_TRIANGLE_SHADER",
				parent_path.string() + std::string("/resources/shaders/AEON_TRIANGLE.vert"), parent_path.string() + std::string("/resources/shaders/AEON_TRIANGLE.frag"));
		}

		/*-----------------------------------------------------------------------
		 *	LINE
		 -----------------------------------------------------------------------*/
		{
			std::vector<float> LINE_VERTICES
			{
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 1.0f
			};

			BufferLayout LINE_LAYOUT =
			{
				{ShaderDataType::Float3, "aPos"}
			};

			std::vector<uint32_t> LINE_INDICES
			{
				0, 1
			};
			ResourceManager::GetInstance().LoadMesh("AEON_LINE_MESH", LINE_VERTICES, LINE_INDICES, LINE_LAYOUT);
			ResourceManager::GetInstance().LoadShader("AEON_LINE_SHADER",
				parent_path.string() + std::string("/resources/shaders/AEON_LINE.vert"),
				parent_path.string() + std::string("/resources/shaders/AEON_LINE.frag"));

		}

		/*-----------------------------------------------------------------------
		 *	TEXTURE CUBE
		 -----------------------------------------------------------------------*/
		{
			std::vector<float> TEXTURE_VERTICES = {

				-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Vertex 0
				 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Vertex 1
				 0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // Vertex 2
				-0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // Vertex 3

				// Right face
				 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Vertex 4
				 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Vertex 5
				 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Vertex 6
				 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Vertex 7

				 // Back face
				  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // Vertex 8
				 -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, // Vertex 9
				 -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // Vertex 10
				  0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // Vertex 11

				  // Left face
				  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Vertex 12
				  -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Vertex 13
				  -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, // Vertex 14
				  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // Vertex 15

				  // Top face
				  -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Vertex 16
				   0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Vertex 17
				   0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Vertex 18
				  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Vertex 19

				  // Bottom face
				  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // Vertex 20
				   0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // Vertex 21
				   0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, // Vertex 22
				  -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f  // Vertex 23
			};

			std::vector<unsigned int> TEXTURE_INDICES = {

				0, 1, 2, 2, 3, 0, // Front face
				4, 5, 6, 6, 7, 4, // Right face
				8, 9, 10, 10, 11, 8, // Back face
				12, 13, 14, 14, 15, 12, // Left face
				16, 17, 18, 18, 19, 16, // Top face
				20, 21, 22, 22, 23, 20  // Bottom face
			};

			BufferLayout TEXTURE_LAYOUT =
			{
				{ShaderDataType::Float3, "aPos"},
				{ShaderDataType::Float2, "aTexture"},
				{ShaderDataType::Float3, "aNormal"},

			};
			ResourceManager::GetInstance().LoadMesh("AEON_TEXTURE_MESH", TEXTURE_VERTICES, TEXTURE_INDICES, TEXTURE_LAYOUT);
			ResourceManager::GetInstance().LoadShader("AEON_TEXTURE_SHADER",
				parent_path.string() + std::string("/resources/shaders/AEON_TEXTURE.vert"),
				parent_path.string() + std::string("/resources/shaders/AEON_TEXTURE.frag"));
		}

		/*-----------------------------------------------------------------------
		 *	CIRCLE
		 -----------------------------------------------------------------------*/
		{
			std::vector<float> CIRCLE_VERTICES;
			std::vector<uint32_t> CIRCLE_INDICES;

			const int numSegments = 360;
			float radius = 1.0f;
			float angle = 2.0f * PI / numSegments;
			for (int i{}; i < numSegments; ++i)
			{
				float x = radius * cos(i * angle);
				float y = radius * sin(i * angle);
				float z = 0.0f;

				CIRCLE_VERTICES.push_back(x);
				CIRCLE_VERTICES.push_back(y);
				CIRCLE_VERTICES.push_back(z);
			}

			int triangleCount = numSegments - 2;

			// push indexes of each triangle points
			for (int i = 0; i < triangleCount; i++)
			{
				CIRCLE_INDICES.push_back(0);
				CIRCLE_INDICES.push_back(i + 1);
				CIRCLE_INDICES.push_back(i + 2);
			}

			BufferLayout TEXTURE_LAYOUT =
			{
				{ShaderDataType::Float3, "aPos"},
			};

			ResourceManager::GetInstance().LoadMesh("AEON_CIRCLE_MESH", CIRCLE_VERTICES, CIRCLE_INDICES, TEXTURE_LAYOUT);
			ResourceManager::GetInstance().LoadShader("AEON_CIRCLE_SHADER",
				parent_path.string() + std::string("/resources/shaders/AEON_CIRCLE.vert"),
				parent_path.string() + std::string("/resources/shaders/AEON_CIRCLE.frag"));
		}

		/*-----------------------------------------------------------------------
		 *	SPHERE
		 -----------------------------------------------------------------------*/
		{
			//	sectors (longitude) and stacks (latitude)
			std::vector<float> SPHERE_VERTICES;
			std::vector<uint32_t> SPHERE_INDICES;

			const int numStacks = 12;
			const int numSectors = 12;
			float radius = 1.0f;

			float x, y, z, xy;                              // vertex position
			float sectorStep = 2 * PI / numSectors;
			float stackStep = PI / numStacks;
			float sectorAngle, stackAngle;

			for (int i = 0; i <= numSectors; ++i)
			{
				stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
				xy = radius * cosf(stackAngle);             // r * cos(u)
				z = radius * sinf(stackAngle);              // r * sin(u)

				// add (sectorCount+1) vertices per stack
				// first and last vertices have same position and normal, but different tex coords
				for (int j = 0; j <= numStacks; ++j)
				{
					sectorAngle = j * sectorStep;           // starting from 0 to 2pi

					// vertex position (x, y, z)
					x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
					y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
					SPHERE_VERTICES.push_back(x);
					SPHERE_VERTICES.push_back(y);
					SPHERE_VERTICES.push_back(z);

					// Calculate and add the normal (same as the vertex position, normalized)
					glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
					SPHERE_VERTICES.push_back(normal.x);
					SPHERE_VERTICES.push_back(normal.y);
					SPHERE_VERTICES.push_back(normal.z);
				}
			}

			int k1, k2;
			for (int i = 0; i < numSectors; ++i)
			{
				k1 = i * (numSectors + 1);     // beginning of current stack
				k2 = k1 + numSectors + 1;      // beginning of next stack

				for (int j = 0; j < numSectors; ++j, ++k1, ++k2)
				{
					// 2 triangles per sector excluding first and last stacks
					// k1 => k2 => k1+1
					if (i != 0)
					{
						SPHERE_INDICES.push_back(k1);
						SPHERE_INDICES.push_back(k2);
						SPHERE_INDICES.push_back(k1 + 1);
					}

					// k1+1 => k2 => k2+1
					if (i != (numSectors - 1))
					{
						SPHERE_INDICES.push_back(k1 + 1);
						SPHERE_INDICES.push_back(k2);
						SPHERE_INDICES.push_back(k2 + 1);
					}
				}
			}

			BufferLayout TEXTURE_LAYOUT =
			{
				{ShaderDataType::Float3, "aPos"},
				{ShaderDataType::Float3, "aNormal"}
			};

			ResourceManager::GetInstance().LoadMesh("AEON_SPHERE_MESH", SPHERE_VERTICES, SPHERE_INDICES, TEXTURE_LAYOUT);
			ResourceManager::GetInstance().LoadShader("AEON_SPHERE_SHADER",
				parent_path.string() + std::string("/resources/shaders/AEON_SPHERE.vert"),
				parent_path.string() + std::string("/resources/shaders/AEON_SPHERE.frag"));
		}

		/*-----------------------------------------------------------------------
		 *	CYLINDER
		 -----------------------------------------------------------------------*/
		{
			std::vector<float> CYLINDER_VERTICES;
			std::vector<uint32_t> CYLINDER_INDICES;

			int sectorCount = 36;
			float cHeight = 1.0f;
			float radius = 1.0f;

			float sectorStep = 2 * PI / sectorCount;
			float sectorAngle;  // radian

			std::vector<float> unitCircleVertices;
			for (int i = 0; i <= sectorCount; ++i)
			{
				sectorAngle = i * sectorStep;
				unitCircleVertices.push_back(cos(sectorAngle)); // x
				unitCircleVertices.push_back(sin(sectorAngle)); // y
				unitCircleVertices.push_back(0);                // z
			}

			// get unit circle vectors on XY-plane
			std::vector<float> unitVertices = unitCircleVertices;

			// put side vertices to arrays
			for (int i = 0; i < 2; ++i)
			{
				float h = -cHeight / 2.0f + i * cHeight;           // z value; -h/2 to h/2
				float t = 1.0f - i;                              // vertical tex coord; 1 to 0

				for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
				{
					float ux = unitVertices[k];
					float uy = unitVertices[k + 1];
					float uz = unitVertices[k + 2];
					// position vector
					CYLINDER_VERTICES.push_back(ux * radius);             // vx
					CYLINDER_VERTICES.push_back(uy * radius);             // vy
					CYLINDER_VERTICES.push_back(h);                       // vz
					// normal vector
					CYLINDER_VERTICES.push_back(ux);                       // nx
					CYLINDER_VERTICES.push_back(uy);                       // ny
					CYLINDER_VERTICES.push_back(uz);                       // nz
					// texture coordinate
					CYLINDER_VERTICES.push_back((float)j / sectorCount); // s
					CYLINDER_VERTICES.push_back(t);                      // t
				}
			}

			// put base and top vertices to arrays
			for (int i = 0; i < 2; ++i)
			{
				float h = -cHeight / 2.0f + i * cHeight;           // z value; -h/2 to h/2
				float nz = -1.0f + (float)i * 2.0f;                           // z value of normal; -1 to 1

				// center point
				CYLINDER_VERTICES.push_back(0); CYLINDER_VERTICES.push_back(0);     CYLINDER_VERTICES.push_back(h);
				CYLINDER_VERTICES.push_back(0);      CYLINDER_VERTICES.push_back(0);      CYLINDER_VERTICES.push_back(nz);
				CYLINDER_VERTICES.push_back(0.5f); CYLINDER_VERTICES.push_back(0.5f);

				for (int j = 0, k = 0; j < sectorCount; ++j, k += 3)
				{
					float ux = unitVertices[k];
					float uy = unitVertices[k + 1];
					// position vector
					CYLINDER_VERTICES.push_back(ux * radius);             // vx
					CYLINDER_VERTICES.push_back(uy * radius);             // vy
					CYLINDER_VERTICES.push_back(h);                       // vz
					// normal vector
					CYLINDER_VERTICES.push_back(0);                        // nx
					CYLINDER_VERTICES.push_back(0);                        // ny
					CYLINDER_VERTICES.push_back(nz);                       // nz
					// texture coordinate
					CYLINDER_VERTICES.push_back(-ux * 0.5f + 0.5f);      // s
					CYLINDER_VERTICES.push_back(-uy * 0.5f + 0.5f);      // t
				}
			}

			int k1 = 0;                         // 1st vertex index at base
			int k2 = sectorCount + 1;           // 1st vertex index at top

			// indices for the side surface
			for (int i = 0; i < sectorCount; ++i, ++k1, ++k2)
			{
				// 2 triangles per sector
				// k1 => k1+1 => k2
				CYLINDER_INDICES.push_back(k1);
				CYLINDER_INDICES.push_back(k1 + 1);
				CYLINDER_INDICES.push_back(k2);

				// k2 => k1+1 => k2+1
				CYLINDER_INDICES.push_back(k2);
				CYLINDER_INDICES.push_back(k1 + 1);
				CYLINDER_INDICES.push_back(k2 + 1);
			}

			// the starting index for the base/top surface
			int baseCenterIndex = (int)CYLINDER_INDICES.size() / 3;
			int topCenterIndex = (int)CYLINDER_INDICES.size(); // include center vertex

			// indices for the base surface
			for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
			{
				if (i < sectorCount - 1)
				{
					CYLINDER_INDICES.push_back(baseCenterIndex);
					CYLINDER_INDICES.push_back(k + 1);
					CYLINDER_INDICES.push_back(k);
				}
				else // last triangle
				{
					CYLINDER_INDICES.push_back(baseCenterIndex);
					CYLINDER_INDICES.push_back(baseCenterIndex + 1);
					CYLINDER_INDICES.push_back(k);
				}
			}

			// indices for the top surface
			for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
			{
				if (i < sectorCount - 1)
				{
					CYLINDER_INDICES.push_back(topCenterIndex);
					CYLINDER_INDICES.push_back(k);
					CYLINDER_INDICES.push_back(k + 1);
				}
				else // last triangle
				{
					CYLINDER_INDICES.push_back(topCenterIndex);
					CYLINDER_INDICES.push_back(k);
					CYLINDER_INDICES.push_back(topCenterIndex + 1);
				}
			}

			BufferLayout TEXTURE_LAYOUT =
			{
				{ShaderDataType::Float3, "aPos"},
				{ShaderDataType::Float3, "aNormal"},
				{ShaderDataType::Float2, "aTexture"},
			};

			ResourceManager::GetInstance().LoadMesh("AEON_CYLINDER_MESH", CYLINDER_VERTICES, CYLINDER_INDICES, TEXTURE_LAYOUT);
			ResourceManager::GetInstance().LoadShader("AEON_CYLINDER_SHADER",
				parent_path.string() + std::string("/resources/shaders/AEON_CYLINDER.vert"),
				parent_path.string() + std::string("/resources/shaders/AEON_CYLINDER.frag"));
		}

		/*-----------------------------------------------------------------------
		 *	PLANE
		 -----------------------------------------------------------------------*/
		{
			std::vector<float> PLANE_VERTICES;
			std::vector<uint32_t> PLANE_INDICES;

			PLANE_VERTICES =
			{
				// Positions        // Normals        // Texture Coordinates
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
				-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
			};

			PLANE_INDICES =
			{
				0,1,2,
				0,2,3
			};

			BufferLayout TEXTURE_LAYOUT =
			{
				{ShaderDataType::Float3, "aPos"},
				{ShaderDataType::Float3, "aNormal"},
				{ShaderDataType::Float2, "aTexture"},
			};

			ResourceManager::GetInstance().LoadMesh("AEON_PLANE_MESH", PLANE_VERTICES, PLANE_INDICES, TEXTURE_LAYOUT);
			ResourceManager::GetInstance().LoadShader("AEON_PLANE_SHADER",
				parent_path.string() + std::string("/resources/shaders/AEON_PLANE.vert"),
				parent_path.string() + std::string("/resources/shaders/AEON_PLANE.frag"));
		}

		// Model shader init
		{

			Shader* shader = ResourceManager::GetInstance().LoadShader("AEON_MODEL_SHADER",
				parent_path.string() + std::string("/resources/shaders/AEON_MODEL.vert"),
				parent_path.string() + std::string("/resources/shaders/AEON_MODEL.frag"));

			//////////////////////////////////////////////////////////////////////////////////////////
			{
				// Configure uniform buffer obj
				// unsigned int uniformBlockIndex = glGetUniformBlockIndex(shader->ID, "Matrices");
				// if (uniformBlockIndex != GL_INVALID_INDEX)
				// {
				// 	glUniformBlockBinding(shader->ID, uniformBlockIndex, UNIFORMBLOCKBINDING_0);
				// 
				// 	// Create buffer
				// 	glGenBuffers(1, &uboMatrices);
				// 	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
				// 	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
				// 	glBindBuffer(GL_UNIFORM_BUFFER, 0);
				// 
				// 	// define range of buffer
				// 	glBindBufferRange(GL_UNIFORM_BUFFER, UNIFORMBLOCKBINDING_0, uboMatrices, 0, sizeof(glm::mat4));
				// }

				// Creating UBO for Matrices
				UBO_Matrices = std::make_shared<OpenGLUniformBuffer>(2 * sizeof(glm::mat4));
				UBO_Matrices->AttachShaderUniformBlock(*shader, "Matrices", OpenGLUniformBuffer::BLOCK_BINDING_POINT::MATRICES);
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			
			
			
			// Shader for Batching
			{
				/*Shader* shader =*/ ResourceManager::GetInstance().LoadShader("AEON_MODEL_SHADER_BATCHING",
					parent_path.string() + std::string("/resources/shaders/AEON_MODELBATCHING.vert"),
					parent_path.string() + std::string("/resources/shaders/AEON_MODELBATCHING.frag"));


			}
		}

		// Font shader init
		{
			std::vector<float> vertices =
			{
				// positions    // texture coords
				 0.5f,  0.5f, 1.0f, 1.0f, // top right
				 0.5f, -0.5f, 1.0f, 0.0f, // bottom right
				-0.5f, -0.5f, 0.0f, 0.0f, // bottom left
				-0.5f,  0.5f, 0.0f, 1.0f  // top left 
			};

			std::vector<uint32_t> indices =
			{
				 0, 1, 3, // first triangle
				 1, 2, 3  // second triangle
			};

			BufferLayout FONT_LAYOUT
			{
				{ShaderDataType::Float2, "aPos"},
				{ShaderDataType::Float2, "aTexture"}
			};
			ResourceManager::GetInstance().LoadMesh("AEON_FONT_MESH", vertices, indices, FONT_LAYOUT);
			ResourceManager::GetInstance().LoadShader("AEON_FONT_SHADER",
				parent_path.string() + std::string("/resources/shaders/AEON_FONT.vert"),
				parent_path.string() + std::string("/resources/shaders/AEON_FONT.frag"));
		}

		// Skybox 
		{
			std::vector<float> skyboxVertices =
			{
				// positions          
			   -1.0f,  1.0f, -1.0f,
			   -1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
			   -1.0f,  1.0f, -1.0f,

			   -1.0f, -1.0f,  1.0f,
			   -1.0f, -1.0f, -1.0f,
			   -1.0f,  1.0f, -1.0f,
			   -1.0f,  1.0f, -1.0f,
			   -1.0f,  1.0f,  1.0f,
			   -1.0f, -1.0f,  1.0f,

				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,

			   -1.0f, -1.0f,  1.0f,
			   -1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f, -1.0f,  1.0f,
			   -1.0f, -1.0f,  1.0f,

			   -1.0f,  1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
			   -1.0f,  1.0f,  1.0f,
			   -1.0f,  1.0f, -1.0f,

			   -1.0f, -1.0f, -1.0f,
			   -1.0f, -1.0f,  1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
			   -1.0f, -1.0f,  1.0f,
				1.0f, -1.0f,  1.0f
			};

			std::vector<unsigned int> skyboxIndices;
			//{
			//	// Right
			//	1, 2, 6,
			//	6, 5, 1,
			//	// Left
			//	0, 4, 7,
			//	7, 3, 0,
			//	// Top
			//	4, 5, 6,
			//	6, 7, 4,
			//	// Bottom
			//	0, 3, 2,
			//	2, 1, 0,
			//	// Back
			//	0, 1, 5,
			//	5, 4, 0,
			//	// Front
			//	3, 7, 6,
			//	6, 2, 3
			//};


			BufferLayout layout =
			{
				{ShaderDataType::Float3, "aPos"}
			};

			ResourceManager::GetInstance().LoadShader("AEON_SKYBOX_SHADER",
				parent_path.string() + std::string("/resources/shaders/AEON_SKYBOX.vert"),
				parent_path.string() + std::string("/resources/shaders/AEON_SKYBOX.frag"));

			ResourceManager::GetInstance().LoadMesh("AEON_SKYBOX_MESH", skyboxVertices, skyboxIndices, layout);

			std::vector<std::string> texturesSkybox
			{
				parent_path.string() + "/resources/textures/right.dds"	,
				parent_path.string() + "/resources/textures/left.dds"	,
				parent_path.string() + "/resources/textures/top.dds"	,
				parent_path.string() + "/resources/textures/bottom.dds"	,
				parent_path.string() + "/resources/textures/front.dds"	,
				parent_path.string() + "/resources/textures/back.dds"	
			};
			ResourceManager::GetInstance().LoadTexture("SkyBox", texturesSkybox, Texture::Type::CubeMap);

			std::vector<std::string> texturesSkybox_Day
			{
				parent_path.string() + "/resources/textures/Right_Day.dds"	,
				parent_path.string() + "/resources/textures/Left_Day.dds"	,
				parent_path.string() + "/resources/textures/Top_Day.dds"	,
				parent_path.string() + "/resources/textures/Bottom_Day.dds"	,
				parent_path.string() + "/resources/textures/Front_Day.dds"	,
				parent_path.string() + "/resources/textures/Back_Day.dds"
			};

			ResourceManager::GetInstance().LoadTexture("SkyBox_Day", texturesSkybox_Day, Texture::Type::CubeMap);
		}

		// Texture2D
		{
			std::vector<float> vertices = {
				// positions          // colors           // texture coords
				 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
				 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
				-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
				-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
			};
			std::vector<unsigned int> indices = {
				0, 1, 3, // first triangle
				1, 2, 3  // second triangle
			};
			BufferLayout layout =
			{
				{ShaderDataType::Float3, "aPos"},
				{ShaderDataType::Float3, "aColor"},
				{ShaderDataType::Float2, "aUV"},
			};
			ResourceManager::GetInstance().LoadMesh("AEON_MESH_2D_QUAD", vertices, indices, layout);
			ResourceManager::GetInstance().LoadShader("AEON_SHADER_2D_QUAD",
				parent_path.string() + std::string("/resources/shaders/AEON_TEXTURE_2D.vert"),
				parent_path.string() + std::string("/resources/shaders/AEON_TEXTURE_2D.frag"));

			//ResourceManager::GetInstance().LoadTexture("container", parent_path.string() + "/resources/textures/container.dds", Texture::Type::None);

		}

		// Light Source shader init
		{
			ResourceManager::GetInstance().LoadShader("AEON_LIGHT_SRC_SHADER",
				parent_path.string() + std::string("/resources/shaders/AEON_LIGHT_SRC_SHADER.vert"),
				parent_path.string() + std::string("/resources/shaders/AEON_LIGHT_SRC_SHADER.frag"));
		}
	}


	void Rendering::swap_buffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

	void Rendering::framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		(void)window;
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		//glViewport(0, 0, width, height);
		//AeonCore_INFO("{0}, {1}", width, height);

		if(Application::GetApplication().GetRuntimeState() != RuntimeState::Start)
		{
			FramebufferManager::GetInstance().RescaleFramebuffer("f1", width, height);
		}
		else
		{
			glViewport(0, 0, width, height);
		}
	}

	void Rendering::error_callback(int error, const char* description)
	{
		AeonCore_INFO("Error Code {0}: {1}", error, description);
	}

	std::string Rendering::GetGLVersion()
	{
		return m_version;
	}

	std::string Rendering::GetGLVendor()
	{
		return m_vendor;
	}

	std::string Rendering::GetGLRenderer()
	{
		return m_renderer;
	}

	void Rendering::processInput(GLFWwindow* window, float dt)
	{
		(void)dt;

		auto timestep = TimerStep::GetInstance().get_delta_time();
		(void)timestep;

		//if (Input::IsJoyStickConnected())
		//{
		//	if (Input::GetInstance().buttons == nullptr)
		//	{
		//		return;
		//	}
		//	if (Input::GetInstance().buttons[7] == GLFW_PRESS || Input::GetInstance().buttons[7] == GLFW_REPEAT)
		//	{
		//		glfwSetWindowShouldClose(window, true);
		//	}

		//	if (Input::axesInput[3] != 0 && Input::axesInput[3] > 0.08f)
		//	{
		//		//default_camera->ProcessKeyboard(BACKWARD, timestep);
		//	}

		//	if (Input::axesInput[3] != 0 && Input::axesInput[3] < -0.08f)
		//	{
		//		//default_camera->ProcessKeyboard(FORWARD, timestep);
		//	}

		//	if  (Input::axesInput[2] != 0 && Input::axesInput[2] < -0.09f)
		//	{
		//		//default_camera->ProcessKeyboard(LEFT, timestep);
		//	}
		//	if (Input::axesInput[2] != 0 && Input::axesInput[2] > 0.15f)
		//	{
		//		//default_camera->ProcessKeyboard(RIGHT, timestep);
		//	}

		//}
		//else
		//{
		//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		//		glfwSetWindowShouldClose(window, true);

		//	/*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		//		default_camera->ProcessKeyboard(FORWARD, timestep);
		//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		//		default_camera->ProcessKeyboard(BACKWARD, timestep);
		//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		//		default_camera->ProcessKeyboard(LEFT, timestep);
		//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		//		default_camera->ProcessKeyboard(RIGHT, timestep);*/
		//}

		// TODO: Need to be handled by the Input class by zhong en...

		// if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		// 	glfwSetWindowShouldClose(window, true);
		// 
		// if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		// 	ourCamera->ProcessKeyboard(FORWARD, dt);
		// if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		// 	ourCamera->ProcessKeyboard(BACKWARD, dt);
		// if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		// 	ourCamera->ProcessKeyboard(LEFT, dt);
		// if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		// 	ourCamera->ProcessKeyboard(RIGHT, dt);
	}

	void Rendering::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		// default_camera->ProcessMouseScroll(static_cast<float>(yoffset));
		(void)window;
		(void)xoffset;
		(void)yoffset;
	}

	void Rendering::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
	{
		(void)window;
		(void)xposIn;
		(void)yposIn;
		// TODO: Need to be handled by the Input class by zhong en...

		 // float xpos = static_cast<float>(xposIn);
		 // float ypos = static_cast<float>(yposIn);
		 // 
		 // if (firstMouse)
		 // {
		 // 	lastX = xpos;
		 // 	lastY = ypos;
		 // 	firstMouse = false;
		 // }
		 // 
		 // float xoffset = xpos - lastX;
		 // float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		 // 
		 // lastX = xpos;
		 // lastY = ypos;
		 // 
		 // ourCamera->ProcessMouseMovement(xoffset, yoffset);
	}

	/*float Render::GetDT() const
	{
		return m_dt;
	}*/

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//													Draw Functions												      //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*---------------------------------------------------------------------------------------------------------------------
	 *	Set polygon rasterisation mode to GL_LINE for wire frame
	 ---------------------------------------------------------------------------------------------------------------------*/
	void Rendering::DrawWireframe(bool isOn)
	{
		if (isOn)
		{
			glPolygonMode(GL_FRONT, GL_LINE);
			glPolygonMode(GL_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);

		}
	}

	/*---------------------------------------------------------------------------------------------------------------------
	 *	Render a line object using 2 positions
	 ---------------------------------------------------------------------------------------------------------------------*/
	void Rendering::DrawLine(glm::vec3 pos0, glm::vec3 pos1, glm::vec3 color, Camera& camera)
	{
		const glm::mat4 projection = camera.GetCameraProjectionMatrix();
		const glm::mat4 view = camera.GetCameraViewMatrix();

		glm::mat4 mdl(1.0f);

		Shader* shader = ResourceManager::GetInstance().GetShader("AEON_LINE_SHADER");
		AeonCore_ASSERT(shader, "Failed to retrieve Shader from ResourceManager...")

		Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_LINE_MESH");
		AeonCore_ASSERT(mesh, "Failed to retrieve Mesh from ResourceManager...")

		shader->use();
		shader->setMat4("model", mdl);
		shader->setMat4("vp", projection * view);
		shader->setVec3("color", color);

		// Update the vertices with the new positions
		std::vector<float> vertices
		{
			pos0.x, pos0.y, pos0.z,
			pos1.x, pos1.y, pos1.z
		};

		mesh->m_VertexArray->Bind();
		mesh->m_VertexBuffer->Bind();
		mesh->m_VertexBuffer->UpdateBufferStore(vertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(float)));
		//glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data()); // Update the vertex buffer
		glLineWidth(2.0f);
		glDrawElements(GL_LINES, static_cast<GLsizei>(mesh->m_IndexBuffer->GetCount()), GL_UNSIGNED_INT, 0);
	}

	/*---------------------------------------------------------------------------------------------------------------------
	 *	Render a quad object using 4 positions
	 *	TODO: define a easier way for users to use instead of them having to calculate each position (i.e. give a position and scale)
	 ---------------------------------------------------------------------------------------------------------------------*/
	void Rendering::DrawQuad(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color, Camera& camera)
	{
		DrawLine(p0, p1, color, camera);
		DrawLine(p1, p2, color, camera);
		DrawLine(p2, p3, color, camera);
		DrawLine(p3, p0, color, camera);
	}

	/*---------------------------------------------------------------------------------------------------------------------
	 *	Render a cube object (Wireframe)
	 ---------------------------------------------------------------------------------------------------------------------*/
	void Rendering::DrawCubeFrame(Transform* transform, glm::vec3 color, Camera& camera) const
	{
		const glm::mat4 projection = glm::perspective(glm::radians(camera.GetCameraZoom()), static_cast<float>(m_winWidth) / static_cast<float>(m_winHeight), 0.1f, 100.0f);
		const glm::mat4 view = camera.GetCameraViewMatrix();

		glm::mat4 model(1.0f);
		// Apply Euler angle rotations if needed
		model = glm::rotate(model, transform->GetEulerAngle().x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, transform->GetEulerAngle().y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, transform->GetEulerAngle().z, glm::vec3(0.0f, 0.0f, 1.0f));

		// Adjust the model matrix to center the wireframe cube at the specified position
		model = glm::translate(model, transform->GetPosition());
		model = glm::scale(model, transform->GetScale());

		Shader* shader = ResourceManager::GetInstance().GetShader("AEON_CUBE_WIREFRAME_SHADER");
		AeonCore_ASSERT(shader, "Failed to retrieve Shader from ResourceManager...")

		Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_CUBE_WIREFRAME_MESH");
		AeonCore_ASSERT(mesh, "Failed to retrieve Mesh from ResourceManager...")

		shader->use();
		shader->setMat4("model", model);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		shader->setVec3("color", color);

		mesh->m_VertexArray->Bind();
		glDrawElements(GL_LINES, static_cast<GLsizei>(mesh->m_IndexBuffer->GetCount()), GL_UNSIGNED_INT, nullptr);
	}

	/*---------------------------------------------------------------------------------------------------------------------
	 *	Render a triangle object
	 ---------------------------------------------------------------------------------------------------------------------*/
	void Rendering::DrawTriangle(Transform* transform, glm::vec3 color, Camera& camera) const
	{
		const glm::mat4 projection = glm::perspective(glm::radians(camera.GetCameraZoom()), static_cast<float>(m_winWidth) / static_cast<float>(m_winHeight), 0.1f, 100.0f);
		const glm::mat4 view = camera.GetCameraViewMatrix();

		glm::mat4 mdl(1.0f);
		// Apply Euler angle rotations
		mdl = glm::rotate(mdl, transform->GetEulerAngle().x, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
		mdl = glm::rotate(mdl, transform->GetEulerAngle().y, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
		mdl = glm::rotate(mdl, transform->GetEulerAngle().z, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis

		mdl = glm::translate(mdl, transform->GetPosition());
		mdl = glm::scale(mdl, transform->GetScale());

		Shader* shader = ResourceManager::GetInstance().GetShader("AEON_TRIANGLE_SHADER");
		AeonCore_ASSERT(shader, "Failed to retrieve Shader from ResourceManager...")

		Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_TRIANGLE_MESH");
		AeonCore_ASSERT(mesh, "Failed to retrieve Mesh from ResourceManager...")

		shader->use();
		shader->setMat4("model", mdl);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		shader->setVec3("color", color);

		mesh->m_VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->m_IndexBuffer->GetCount()), GL_UNSIGNED_INT, nullptr);
	}

	/*---------------------------------------------------------------------------------------------------------------------
	 *	Render a circle object (filled)
	 ---------------------------------------------------------------------------------------------------------------------*/
	void Rendering::DrawCircle(Transform* transform, glm::vec3 color, Camera& camera, int numSegments) const
	{
		const glm::mat4 projection = glm::perspective(glm::radians(camera.GetCameraZoom()), static_cast<float>(m_winWidth) / static_cast<float>(m_winHeight), 0.1f, 100.0f);
		const glm::mat4 view = camera.GetCameraViewMatrix();

		glm::mat4 mdl(1.0f);
		mdl = glm::rotate(mdl, transform->GetEulerAngle().x, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around the X-axis
		mdl = glm::rotate(mdl, transform->GetEulerAngle().y, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around the Y-axis
		mdl = glm::rotate(mdl, transform->GetEulerAngle().z, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around the Z-axis

		mdl = glm::translate(mdl, transform->GetPosition()); // Translate to the specified position
		//mdl = glm::scale(mdl, glm::vec3(radius, radius, 1.0f)); // Scale to the specified radius
		mdl = glm::scale(mdl, transform->GetScale());

		Shader* shader = ResourceManager::GetInstance().GetShader("AEON_CIRCLE_SHADER");
		AeonCore_ASSERT(shader, "Failed to retrieve Shader from ResourceManager...")

		const Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_CIRCLE_MESH");
		AeonCore_ASSERT(mesh, "Failed to retrieve Mesh from ResourceManager...")

		shader->use();
		shader->setMat4("model", mdl);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		shader->setVec3("color", color);

		std::vector<float> vertices;
		std::vector<uint32_t> indices;
		//	Create new vertices based on user define radius
		{
			const float angle = 360.0f / static_cast<float>(numSegments);

			const int triangleCount = numSegments - 2;

			std::vector<glm::vec3> temp;
			// positions
			for (int i = 0; i < numSegments; i++)
			{
				const float currentAngle = i * angle;
				float x = transform->GetScale().x * cos(glm::radians(currentAngle));
				float y = transform->GetScale().y * sin(glm::radians(currentAngle));
				float z = 0.0f;

				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
			}

			// push indexes of each triangle points
			for (int i = 0; i < triangleCount; i++)
			{
				indices.push_back(0);
				indices.push_back(i + 1);
				indices.push_back(i + 2);
			}
		}

		mesh->m_VertexArray->Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data());
		mesh->m_VertexBuffer->Bind();
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(indices.size() * sizeof(uint32_t)), indices.data());

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
	}

	/*---------------------------------------------------------------------------------------------------------------------
	 *	Render a circle frame object
	 ---------------------------------------------------------------------------------------------------------------------*/
	void Rendering::DrawCircleFrame(Transform* transform, glm::vec3 color, Camera& camera, int numSegments) const
	{
		const glm::mat4 projection = glm::perspective(glm::radians(camera.GetCameraZoom()), static_cast<float>(m_winWidth) / static_cast<float>(m_winHeight), 0.1f, 100.0f);
		const glm::mat4 view = camera.GetCameraViewMatrix();

		glm::mat4 mdl(1.0f);
		mdl = glm::rotate(mdl, transform->GetEulerAngle().x, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around the X-axis
		mdl = glm::rotate(mdl, transform->GetEulerAngle().y, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around the Y-axis
		mdl = glm::rotate(mdl, transform->GetEulerAngle().z, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around the Z-axis

		mdl = glm::translate(mdl, transform->GetPosition());
		//mdl = glm::scale(mdl, glm::vec3(radius, radius, 1.0f));
		mdl = glm::scale(mdl, transform->GetScale());

		Shader* shader = ResourceManager::GetInstance().GetShader("AEON_CIRCLE_SHADER");
		AeonCore_ASSERT(shader, "Failed to retrieve Shader from ResourceManager...")

		const Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_CIRCLE_MESH");
		AeonCore_ASSERT(mesh, "Failed to retrieve Mesh from ResourceManager...")

		shader->use();
		shader->setMat4("model", mdl);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		shader->setVec3("color", color);

		std::vector<float> vertices;

		// Create new vertices based on the user-defined radius
		{
			const float angle = 360.0f / static_cast<float>(numSegments);

			// Positions
			for (int i = 0; i < numSegments; i++)
			{
				const float currentAngle = angle * static_cast<float>(i);
				float x = transform->GetScale().x * cos(glm::radians(currentAngle));
				float y = transform->GetScale().y * sin(glm::radians(currentAngle));
				float z = 0.0f;

				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
			}
		}

		mesh->m_VertexArray->Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data());

		// Draw the circle outline using GL_LINE_LOOP
		glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(vertices.size() / 3)); // Divide by 3 because each vertex has 3 components
	}

	/*---------------------------------------------------------------------------------------------------------------------
	 *	Render a AABB box using physics geometry
	 ---------------------------------------------------------------------------------------------------------------------*/
	void Rendering::DrawColliderBox(Transform& _trans, RigidBody& _rigid, Collider& _coll, ACPhysics::CubeGeometry& _geom, Camera& _cam) const
	{
		const glm::mat4 projection = _cam.GetCameraProjectionMatrix();
		const glm::mat4 view = _cam.GetCameraViewMatrix();

		glm::mat4 mdl = glm::mat4(1.0f);
		mdl = glm::translate(mdl, _trans.GetPosition() + _coll.GetOffset());
		mdl = glm::scale(mdl, _geom.GetHalfExtents() * 2.f);

		Shader* shader = ResourceManager::GetInstance().GetShader("AEON_CUBE_WIREFRAME_SHADER");
		AeonCore_ASSERT(shader, "Failed to retrieve Shader from ResourceManager...")

		Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_CUBE_WIREFRAME_MESH");
		AeonCore_ASSERT(mesh, "Failed to retrieve Mesh from ResourceManager...")

		shader->use();
		shader->setMat4("model", mdl);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		shader->setVec3("color", glm::vec3(0, 1, 0));

		mesh->m_VertexArray->Bind();
		glDrawElements(GL_LINES, static_cast<GLsizei>(mesh->m_IndexBuffer->GetCount()), GL_UNSIGNED_INT, nullptr);
	}

	void Rendering::DrawColliderSphere(Transform& _trans, RigidBody& _rigid, Collider& _coll,
		ACPhysics::SphereGeometry& _geom, Camera& _cam)
	{
		const glm::mat4 projection = _cam.GetCameraProjectionMatrix();
		const glm::mat4 view = _cam.GetCameraViewMatrix();

		glm::mat4 mdl{ 1.0f };

		mdl = glm::rotate(mdl, _trans.GetEulerAngle().x, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around the X-axis
		mdl = glm::rotate(mdl, _trans.GetEulerAngle().y, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around the Y-axis
		mdl = glm::rotate(mdl, _trans.GetEulerAngle().z, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around the Z-axis

		mdl = glm::translate(mdl, _trans.GetPosition() + _coll.GetOffset());
		mdl = glm::scale(mdl, glm::vec3(_geom.GetRadius()));

		Shader* shader = ResourceManager::GetInstance().GetShader("AEON_SPHERE_SHADER");
		AeonCore_ASSERT(shader, "Failed to retrieve Shader from ResourceManager...")

		const Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_SPHERE_MESH");
		AeonCore_ASSERT(mesh, "Failed to retrieve Mesh from ResourceManager...")

		shader->use();
		shader->setMat4("model", mdl);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		shader->setVec3("color", glm::vec3(0, 1, 0));

		mesh->m_VertexArray->Bind();

		// Draw the wireframe sphere
		glDrawElements(GL_LINES, static_cast<GLsizei>(mesh->m_IndexBuffer->GetCount()), GL_UNSIGNED_INT, nullptr);
	}

	/*---------------------------------------------------------------------------------------------------------------------
	 *	Render a sphere frame object
	 ---------------------------------------------------------------------------------------------------------------------*/
	void Rendering::DrawSphereFrame(Transform* transform, glm::vec3 color, Camera& camera) const
	{
		const glm::mat4 projection = glm::perspective(glm::radians(camera.GetCameraZoom()), static_cast<float>(m_winWidth) / static_cast<float>(m_winHeight), 0.1f, 100.0f);
		const glm::mat4 view = camera.GetCameraViewMatrix();

		glm::mat4 mdl(1.0f);
		mdl = glm::translate(mdl, transform->GetPosition());
		/*mdl = glm::scale(mdl, glm::vec3(radius, radius, radius));*/
		mdl = glm::scale(mdl, transform->GetScale());

		Shader* shader = ResourceManager::GetInstance().GetShader("AEON_SPHERE_SHADER");
		AeonCore_ASSERT(shader, "Failed to retrieve Shader from ResourceManager...")

		const Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_SPHERE_MESH");
		AeonCore_ASSERT(mesh, "Failed to retrieve Mesh from ResourceManager...")

		shader->use();
		shader->setMat4("model", mdl);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		shader->setVec3("color", color);

		mesh->m_VertexArray->Bind();
		glDrawElements(GL_LINES, static_cast<GLsizei>(mesh->m_IndexBuffer->GetCount()), GL_UNSIGNED_INT, nullptr);
	}

	void Rendering::DrawFont(std::string text, Font& font, Transform& transform, Camera& camera, glm::vec3 color) const
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		const glm::mat4 projection = camera.GetCameraProjectionMatrix();
		const glm::mat4 view = camera.GetCameraViewMatrix();

		glm::mat4 mdl(1.0f);
		mdl = glm::translate(mdl, transform.GetPosition());
		mdl = glm::scale(mdl, transform.GetScale());
		mdl = glm::rotate(mdl, transform.GetEulerAngle().x, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
		mdl = glm::rotate(mdl, transform.GetEulerAngle().y, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
		mdl = glm::rotate(mdl, transform.GetEulerAngle().z, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis

		{
			float x = transform.GetPosition().x;
			float y = transform.GetPosition().y;

			Shader* shader = ResourceManager::GetInstance().GetShader("AEON_FONT_SHADER");
			Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_FONT_MESH");
			
			shader->use();
			shader->setMat4("mdl", mdl);
			shader->setMat4("vp", projection * view);
			shader->setVec3("color", color);

			shader->setVec3("textColor", color);
			glActiveTexture(GL_TEXTURE0);
			mesh->m_VertexArray->Bind();

			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); ++c)
			{
				FontResource::Character ch = font.GetCharacter(*c);

				float xpos = x + ch.Bearing[0] * transform.GetScale().x;
				float ypos = y - (ch.Size[1] - ch.Bearing[1]) * transform.GetScale().y;

				float w = ch.Size[0] * transform.GetScale().x;
				float h = ch.Size[1] * transform.GetScale().y;

				// update vbo for each char, send to mesh->vbo->updateBufferStore()
				std::vector<float> vertices =
				{
					xpos,     ypos + h,   0.0f, 0.0f,
					xpos,     ypos,       0.0f, 1.0f,
					xpos + w, ypos,       1.0f, 1.0f,
					xpos,     ypos + h,   0.0f, 0.0f,
					xpos + w, ypos,       1.0f, 1.0f,
					xpos + w, ypos + h,   1.0f, 0.0f
				};

				glBindTexture(GL_TEXTURE_2D, ch.TextureID);
				mesh->m_VertexBuffer->UpdateBufferStore(vertices.data(), (uint32_t)(vertices.size() * sizeof(float)));
				
				// Unbind
				mesh->m_VertexBuffer->UnBind();
				glDrawArrays(GL_TRIANGLES, 0, mesh->m_IndexBuffer->GetCount());

				x += (ch.Advance >> 6) * transform.GetScale().x;
				//y += (ch.Advance >> 6) * transform.GetScale().y;

			}
		}

		glEnable(GL_DEPTH_TEST);
	}

	void Rendering::DrawFont(std::string text, UI& ui, Transform& transform, Camera& camera) const
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		const glm::mat4 projection =
			glm::ortho(-camera.GetWindowWidth()/ 2.0f, camera.GetWindowWidth() / 2.0f,
				-camera.GetWindowHeight() / 2.0f, camera.GetWindowHeight() / 2.0f,
			-camera.GetNear(), camera.GetFar());
		

		glm::mat4 mdl(1.0f);
		mdl = glm::translate(mdl, transform.GetPosition());
		mdl = glm::scale(mdl, transform.GetScale());
		mdl = glm::rotate(mdl, transform.GetEulerAngle().z, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Y-axis

		{
			float x = transform.GetPosition().x;
			float y = transform.GetPosition().y;

			Shader* shader = ResourceManager::GetInstance().GetShader("AEON_FONT_SHADER");
			Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_FONT_MESH");

			shader->use();
			shader->setMat4("mdl", mdl);
			shader->setMat4("vp", projection);
			shader->setVec4("textColor", glm::vec4(ui.GetColor(), ui.GetOpacity()));
			// shader->setFloat("opacity", ui.GetOpacity());
			
 			glActiveTexture(GL_TEXTURE0);
			mesh->m_VertexArray->Bind();

			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); ++c)
			{
				FontResource::Character ch = ui.GetFontPtr()->GetCharacter(*c);

				float xpos = x + ch.Bearing[0] * transform.GetScale().x;
				float ypos = y - (ch.Size[1] - ch.Bearing[1]) * transform.GetScale().y;

				float w = ch.Size[0] * transform.GetScale().x;
				float h = ch.Size[1] * transform.GetScale().y;

				// update vbo for each char, send to mesh->vbo->updateBufferStore()
				std::vector<float> vertices =
				{
					xpos,     ypos + h,   0.0f, 0.0f,
					xpos,     ypos,       0.0f, 1.0f,
					xpos + w, ypos,       1.0f, 1.0f,
					xpos,     ypos + h,   0.0f, 0.0f,
					xpos + w, ypos,       1.0f, 1.0f,
					xpos + w, ypos + h,   1.0f, 0.0f
				};

				glBindTexture(GL_TEXTURE_2D, ch.TextureID);
				mesh->m_VertexBuffer->UpdateBufferStore(vertices.data(), (uint32_t)(vertices.size() * sizeof(float)));

				// Unbind
				mesh->m_VertexBuffer->UnBind();
				glDrawArrays(GL_TRIANGLES, 0, mesh->m_IndexBuffer->GetCount());

				x += (ch.Advance >> 6) * transform.GetScale().x;
				//y += (ch.Advance >> 6) * transform.GetScale().y;

			}

		}

		glEnable(GL_DEPTH_TEST);
	}

	void Rendering::DrawTexture2D(Transform& transform, Camera& camera, UI& ui) const
	{

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		const glm::mat4 projection =
			glm::ortho(-camera.GetWindowWidth() / 2.0f, camera.GetWindowWidth() / 2.0f,
				camera.GetWindowHeight() / 2.0f, -camera.GetWindowHeight() / 2.0f);
		
		glm::mat4 mdl(1.0f);
		mdl = glm::translate(mdl, transform.GetPosition());
		mdl = glm::scale(mdl, transform.GetScale());
		mdl = glm::rotate(mdl, transform.GetEulerAngle().z, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Y-axis


		{
			Mesh* m = ResourceManager::GetInstance().GetMesh("AEON_MESH_2D_QUAD");
			Shader* shader = ResourceManager::GetInstance().GetShader("AEON_SHADER_2D_QUAD");

			AeonCore_ASSERT(m, "mesh used is wrong!");
			AeonCore_ASSERT(shader, "Shader used is wrong!");

			shader->use();
			shader->setMat4("vp", projection);
			shader->setMat4("model", mdl);
			shader->setFloat("opacity", ui.GetOpacity());
			shader->setVec3("color", ui.GetColor());

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ui.GetTexturePtr()->GetTextureID());
			shader->setInt("texture1", GL_TEXTURE0);

			m->m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m->m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);

			// unbind
			glBindVertexArray(0);
			glActiveTexture(0);
		}

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//													3D shapes for Lighting											  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*---------------------------------------------------------------------------------------------------------------------
	 *	Draw Textured Model
	 ---------------------------------------------------------------------------------------------------------------------*/
	
	void Rendering::DrawModel(Transform& transform, Camera& camera, Model& model, Material& material) const
	{
		const glm::mat4 projection = camera.GetCameraProjectionMatrix();
		const glm::mat4 view = camera.GetCameraViewMatrix();
		
		glm::mat4 vp = projection * view;
		glm::mat4 mdl(1.0f);
		
		 mdl = glm::translate(mdl, transform.GetPosition());
		 
		 // Apply Euler angle rotations
		 mdl = glm::rotate(mdl, transform.GetEulerAngle().x, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
		 mdl = glm::rotate(mdl, transform.GetEulerAngle().y, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
		 mdl = glm::rotate(mdl, transform.GetEulerAngle().z, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis
		 
		 mdl = glm::scale(mdl, transform.GetScale());
		
		{
			Model* m = &model;
			Shader* shader_LightOnto = ResourceManager::GetInstance().GetShader("AEON_MODEL_SHADER");
			Shader* shader_LightSrc = ResourceManager::GetInstance().GetShader("AEON_LIGHT_SRC_SHADER");
			Shader* shader_depthMap = ResourceManager::GetInstance().GetShader("AEON_SHADOW_MAP");
		
			AeonCore_ASSERT(m, "Model used is wrong!");
			AeonCore_ASSERT(shader_LightOnto, "Shader used is wrong!");
		
			UBO_Matrices->Bind();
			UBO_Matrices->SetData(0, sizeof(glm::mat4), glm::value_ptr(vp));
			UBO_Matrices->SetData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mdl));

			for (Mesh& meshes : m->GetMeshes())
			{

				shader_LightSrc->use();
				material.Use(*shader_LightSrc);

				shader_LightOnto->use();
				material.Use(*shader_LightOnto);

				meshes.Draw(transform, camera);
			}
		
		}
	}

	void Rendering::DrawModel(Transform& transform, glm::mat4 view, glm::mat4 projection, Model& model, Material& material, ShadowMapFBO::MODE mode) const
	{
		// Compute MVP
		{
			glm::mat4 vp = projection * view;

			glm::mat4 mdl(1.0f);

			mdl = glm::translate(mdl, transform.GetPosition());

			// Apply Euler angle rotations
			mdl = glm::rotate(mdl, transform.GetEulerAngle().x, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
			mdl = glm::rotate(mdl, transform.GetEulerAngle().y, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
			mdl = glm::rotate(mdl, transform.GetEulerAngle().z, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis

			mdl = glm::scale(mdl, transform.GetScale());

			// Render
			{
				Model* m = &model;
				Shader* shader_LightOnto = ResourceManager::GetInstance().GetShader("AEON_MODEL_SHADER");
				Shader* shader_LightSrc = ResourceManager::GetInstance().GetShader("AEON_LIGHT_SRC_SHADER");
				// Shader* shader_ShadowMap = ResourceManager::GetInstance().GetShader("AEON_SHADOW_MAP");
				

				UBO_Matrices->Bind();
				UBO_Matrices->SetData(0, sizeof(glm::mat4), glm::value_ptr(vp));
				UBO_Matrices->SetData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mdl));

				for (Mesh& meshes : m->GetMeshes())
				{

					shader_LightOnto->use();
					//if (mode == ShadowMapFBO::MODE::DIRECTIONAL || mode == ShadowMapFBO::MODE::SPOT)
					//{
					//	//glActiveTexture(GL_TEXTURE0 + SHADOWMAP_TEXTURE_SLOT);
					//	//glBindTexture(GL_TEXTURE_2D, shadowMapID);
					//	shader_LightOnto->setInt("shadowMap", SHADOWMAP_TEXTURE_SLOT);

					//}
					//else
					//{
					//	glActiveTexture(GL_TEXTURE0 + CUBEMAP_TEXTURE_SLOT);
					//	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMapID);
					//	shader_LightOnto->setInt("depthMap_Point", CUBEMAP_TEXTURE_SLOT);
					//}
					glActiveTexture(GL_TEXTURE0 + 32);								// Activate texture unit XXX
					glBindTexture(GL_TEXTURE_2D, material.GetDiffuseTexturePtr().GetTextureID());		// Bind texture id to texture unit
					shader_LightOnto->setInt("material.diffuseTexID", 32);			// Set sampler's texture unit to XXX 
					/*shader_LightSrc->use();
					glActiveTexture(GL_TEXTURE0 + DIFFUSE_TEXTURE_SLOT);
					glBindTexture(GL_TEXTURE_2D, material.GetDiffuseTexturePtr().GetTextureID());
					shader_LightSrc->setInt("material.diffuseTexID", DIFFUSE_TEXTURE_SLOT);*/

					

					meshes.Draw();
				}
			}

		}

		
	}

	void Rendering::DrawModel(Transform& transform, glm::mat4 view, glm::mat4 projection, Model& model, Material& material) const
	{
		glm::mat4 vp = projection * view;

		glm::mat4 mdl(1.0f);

		mdl = glm::translate(mdl, transform.GetPosition());

		// Apply Euler angle rotations
		mdl = glm::rotate(mdl, transform.GetEulerAngle().x, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
		mdl = glm::rotate(mdl, transform.GetEulerAngle().y, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
		mdl = glm::rotate(mdl, transform.GetEulerAngle().z, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis

		mdl = glm::scale(mdl, transform.GetScale());

		// Render
		{
			Model* m = &model;
			Shader* shader_LightOnto = ResourceManager::GetInstance().GetShader("AEON_MODEL_SHADER");
			Shader* shader_LightSrc = ResourceManager::GetInstance().GetShader("AEON_LIGHT_SRC_SHADER");


			UBO_Matrices->Bind();
			UBO_Matrices->SetData(0, sizeof(glm::mat4), glm::value_ptr(vp));
			UBO_Matrices->SetData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mdl));

			for (Mesh& meshes : m->GetMeshes())
			{

				shader_LightOnto->use();
				
				glActiveTexture(GL_TEXTURE0 + DIFFUSE_TEXTURE_SLOT);
				glBindTexture(GL_TEXTURE_2D, material.GetDiffuseTexturePtr().GetTextureID());
				shader_LightOnto->setInt("material.diffuseTexID", DIFFUSE_TEXTURE_SLOT);

				meshes.Draw();
			}
		}

	}
	

	/*---------------------------------------------------------------------------------------------------------------------
	 *	Render a object with a texture
	 ---------------------------------------------------------------------------------------------------------------------*/
	// void Rendering::DrawTexture(std::string TextureName, Transform* transform, glm::vec3 color, CameraProps props, glm::vec3 lightPos, glm::vec3 lightColor)
	// {
	// 
	// 	//glm::mat4 projection = glm::perspective(glm::radians(cam->Zoom), (float)m_winWidth / (float)m_winHeight, 0.1f, 100.0f);
	// 	//glm::mat4 view = cam->GetViewMatrix();
	// 	//glm::mat4 vp = projection * view;
	// 
	// 	//glm::mat4 mdl(1.0f);
	// 	//mdl = glm::scale(mdl, scale);
	// 	//mdl = glm::rotate(mdl, rot[0], glm::vec3(rot[1], rot[2], rot[3]));
	// 	//mdl = glm::translate(mdl, pos);
	// 	//// for each vtx, vtx = m*vtx
	// 	//// push vtx into vbo
	// 
	// 	//{
	// 	//	Mesh*		textureMesh		= ResourceManager::GetInstance().GetMesh("AEON_TEXTURE_MESH");
	// 	//	Shader*		textureShader	= ResourceManager::GetInstance().GetShader("AEON_TEXTURE_SHADER");
	// 	//	Texture*	texture			= ResourceManager::GetInstance().GetTexture(TextureName);
	// 
	// 	//	textureShader->use();
	// 	//	textureShader->setMat4("model", mdl);
	// 	//	textureShader->setMat4("vp", vp);
	// 	//	textureShader->setInt("TextureID", GL_TEXTURE0);
	// 
	// 	//	textureMesh->m_VertexArray->Bind();
	// 	//	glActiveTexture(GL_TEXTURE0);
	// 	//	glBindTexture(GL_TEXTURE_2D, texture->id);
	// 	//	glDrawElements(GL_TRIANGLES, textureMesh->m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
	// 	//}
	// 
	// 	const glm::mat4 projection = glm::perspective(glm::radians(props.sZoom), static_cast<float>(m_winWidth) / static_cast<float>(m_winHeight), 0.1f, 100.0f);
	// 	const glm::mat4 view = props.sViewProjection;
	// 	glm::mat4 vp = projection * view;
	// 	glm::mat4 mdl(1.0f);
	// 
	// 	// Apply Euler angle rotations
	// 	mdl = glm::rotate(mdl, transform->GetEulerAngle().x, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
	// 	mdl = glm::rotate(mdl, transform->GetEulerAngle().y, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
	// 	mdl = glm::rotate(mdl, transform->GetEulerAngle().z, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis
	// 
	// 	mdl = glm::translate(mdl, transform->GetPosition());
	// 	mdl = glm::scale(mdl, transform->GetScale());
	// 
	// 	Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_TEXTURE_MESH");
	// 	Shader* shader = ResourceManager::GetInstance().GetShader("AEON_TEXTURE_SHADER");
	// 	Texture* texture = ResourceManager::GetInstance().GetTexture(TextureName);
	// 
	// 	AeonCore_ASSERT(mesh, "Mesh used is wrong!");
	// 	AeonCore_ASSERT(shader, "Shader used is wrong!");
	// 	AeonCore_ASSERT(texture, "TextureName used is wrong!");
	// 
	// 	shader->use();
	// 	shader->setMat4("model", mdl);
	// 	shader->setMat4("vp", vp);
	// 	shader->setVec3("objectColor", color);
	// 
	// 	// Lighting...
	// 	if (lightColor != glm::vec3())	//	lighting enable
	// 	{
	// 		shader->setInt("lightMode", 1);
	// 		shader->setVec3("lightPos", lightPos);
	// 		shader->setVec3("lightColor", lightColor);
	// 	}
	// 	else
	// 		shader->setInt("lightMode", 0);		//	lighting disable
	// 
	// 
	// 	shader->setInt("TextureID", GL_TEXTURE0);
	// 
	// 	mesh->m_VertexArray->Bind();
	// 	glActiveTexture(GL_TEXTURE0);
	// 	//glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
	// 	glDrawElements(GL_TRIANGLES, mesh->m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
	// }

	/*---------------------------------------------------------------------------------------------------------------------
	 *	Render a cube object (filled)
	 ---------------------------------------------------------------------------------------------------------------------*/
	void Rendering::DrawCubeFilled(Transform& transform, Camera& camera, Material& material, Lighting& lighting) const
	{
		const glm::mat4 projection = glm::perspective(glm::radians(camera.GetCameraZoom()), static_cast<float>(m_winWidth) / static_cast<float>(m_winHeight), 0.1f, 100.0f);
		const glm::mat4 view = camera.GetCameraViewMatrix();

		glm::mat4 mdl(1.0f);
		// Apply Euler angle rotations
		mdl = glm::rotate(mdl, transform.GetEulerAngle().x, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
		mdl = glm::rotate(mdl, transform.GetEulerAngle().y, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
		mdl = glm::rotate(mdl, transform.GetEulerAngle().z, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis

		mdl = glm::translate(mdl, transform.GetPosition());
		mdl = glm::scale(mdl, transform.GetScale());

		Shader* shader = ResourceManager::GetInstance().GetShader("AEON_CUBE_SHADER");
		AeonCore_ASSERT(shader, "Failed to retrieve Shader from ResourceManager...")

		Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_CUBE_MESH");
		AeonCore_ASSERT(mesh, "Failed to retrieve Mesh from ResourceManager...")

		shader->use();
		shader->setMat4("model", mdl);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		// shader->setVec3("color", lighting.getLightColour());
		// 
		// // Lighting...
		// if (lighting.getLightColour() != glm::vec3())	//	lighting enable
		// {
		// 	shader->setInt("lightMode", 1);
		// 	shader->setVec3("lightPos", lighting.getLightPos());
		// 	shader->setVec3("lightColor", lighting.getLightColour());
		// }
		// else
		// {
		// 	shader->setInt("lightMode", 0);		//	lighting disable
		// }

		mesh->m_VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->m_IndexBuffer->GetCount()), GL_UNSIGNED_INT, nullptr);
	}

	

	/*---------------------------------------------------------------------------------------------------------------------
	 *	Render a sphere
	 ---------------------------------------------------------------------------------------------------------------------*/
	void Rendering::DrawSphere(Transform& transform, Camera& camera, Material& material, Lighting& lighting) const
	{
		const glm::mat4 projection = glm::perspective(glm::radians(camera.GetCameraZoom()), static_cast<float>(m_winWidth) / static_cast<float>(m_winHeight), 0.1f, 100.0f);
		const glm::mat4 view = camera.GetCameraViewMatrix();

		glm::mat4 mdl(1.0f);
		mdl = glm::translate(mdl, transform.GetPosition());
		/*mdl = glm::scale(mdl, glm::vec3(radius, radius, radius));*/
		mdl = glm::scale(mdl, transform.GetScale());

		Shader* shader = ResourceManager::GetInstance().GetShader("AEON_SPHERE_SHADER");
		AeonCore_ASSERT(shader, "Failed to retrieve Shader from ResourceManager...")

		const Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_SPHERE_MESH");
		AeonCore_ASSERT(mesh, "Failed to retrieve Mesh from ResourceManager...")

		shader->use();
		shader->setMat4("model", mdl);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		//shader->setVec3("color", color);

		// if (lighting.getLightColour() != glm::vec3())	//	lighting enable
		// {
		// 	shader->setInt("lightMode", 1);
		// 	shader->setVec3("lightPos", lighting.getLightPos());
		// 	shader->setVec3("lightColor", lighting.getLightColour());
		// }
		// else
		// 	shader->setInt("lightMode", 0);		//	lighting disable

		mesh->m_VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->m_IndexBuffer->GetCount()), GL_UNSIGNED_INT, nullptr);
	}

	/*---------------------------------------------------------------------------------------------------------------------
	 *	Render a cylinder
	 ---------------------------------------------------------------------------------------------------------------------*/
	void Rendering::DrawCylinder(Transform& transform, Camera& camera, Material& material, Lighting& lighting) const
	{
		const glm::mat4 projection = glm::perspective(glm::radians(camera.GetCameraZoom()), static_cast<float>(m_winWidth) / static_cast<float>(m_winHeight), 0.1f, 100.0f);
		const glm::mat4 view = camera.GetCameraViewMatrix();
		glm::mat4 mdl(1.0f);

		// Apply Euler angle rotations
		mdl = glm::rotate(mdl, transform.GetEulerAngle().x, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
		mdl = glm::rotate(mdl, transform.GetEulerAngle().y, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
		mdl = glm::rotate(mdl, transform.GetEulerAngle().z, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis

		mdl = glm::translate(mdl, transform.GetPosition());
		/*mdl = glm::scale(mdl, glm::vec3(radius, height, radius));*/
		mdl = glm::scale(mdl, transform.GetScale());

		Shader* shader = ResourceManager::GetInstance().GetShader("AEON_CYLINDER_SHADER");
		AeonCore_ASSERT(shader, "Failed to retrieve Shader from ResourceManager...")

		const Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_CYLINDER_MESH");
		AeonCore_ASSERT(mesh, "Failed to retrieve Mesh from ResourceManager...")

		shader->use();
		shader->setMat4("model", mdl);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		//shader->setVec3("color", color);

		// if (lighting.getLightColour() != glm::vec3())	//	lighting enable
		// {
		// 	shader->setInt("lightMode", 1);
		// 	shader->setVec3("lightPos", lighting.getLightPos());
		// 	shader->setVec3("lightColor", lighting.getLightColour());
		// }
		// else
		// 	shader->setInt("lightMode", 0);		//	lighting disable

		mesh->m_VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->m_IndexBuffer->GetCount()), GL_UNSIGNED_INT, nullptr);
	}

	void Rendering::DrawSkyBox(Camera& camera)
	{
		//const glm::mat4 projection = glm::perspective(glm::radians(camera.GetCameraZoom()), static_cast<float>(m_winWidth) / static_cast<float>(m_winHeight), 0.1f, 100.0f);
		const glm::mat4 projection = camera.GetCameraProjectionMatrix();
		const glm::mat4 view = glm::mat4(glm::mat3(camera.GetCameraViewMatrix()));

		Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_SKYBOX_MESH");
		Shader* shader = ResourceManager::GetInstance().GetShader("AEON_SKYBOX_SHADER");
		Texture* texture = ResourceManager::GetInstance().GetTexture("SkyBox");

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);			// doesnt do anything

		shader->use();
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);

		mesh->m_VertexArray->Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture->GetTextureID());

		glDrawArrays(GL_TRIANGLES, 0, 36);
		mesh->m_VertexArray->UnBind();
		glDepthFunc(GL_LESS); // set depth function back to default
	}

	void Rendering::DrawSkyBox(const char* texture_name, glm::mat4 view, glm::mat4 proj)
	{

		Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_SKYBOX_MESH");
		Shader* shader = ResourceManager::GetInstance().GetShader("AEON_SKYBOX_SHADER");
		Texture* texture = ResourceManager::GetInstance().GetTexture(texture_name);

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);			// doesnt do anything

		shader->use();
		shader->setMat4("view", glm::mat4(glm::mat3(view)));
		shader->setMat4("projection", proj);

		//glActiveTexture(GL_TEXTURE0 + CUBEMAP_TEXTURE_SLOT);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, texture->GetTextureID());

		mesh->m_VertexArray->Bind();
		glActiveTexture(GL_TEXTURE0 + WORLD_TEXTURE_SLOT);		// 4
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture->GetTextureID());
		shader->setInt("skybox", WORLD_TEXTURE_SLOT);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS); // set depth function back to default
	}

	void Rendering::DrawShapeObject(Shape3D _shape, Transform* _transform, glm::vec3 _col,
									Camera& cam, glm::vec3 _lightpos, glm::vec3 _lightcol)
	{
		/*switch (_shape)
		{
		case Shape3D::cube:
			//Rendering::GetInstance().DrawCubeFilled(_transform, _col, cam, _lightpos, _lightcol);
			break;

		case Shape3D::sphere:
			Rendering::GetInstance().DrawSphere(_transform, _col, cam, _lightpos, _lightcol);
			break;

		case Shape3D::cylinder:
			Rendering::GetInstance().DrawCylinder(_transform, _col, cam, _lightpos, _lightcol);
			break;

		default:;
		}*/
	}

	void Rendering::DrawParticle(Particles& _particles, Camera& _camera, Model& _model, Material& _material, std::unordered_map<EntityID, Lighting*>& _lights) const
	{
		/*const glm::mat4 projection = _camera.GetCameraProjectionMatrix();
		const glm::mat4 view = _camera.GetCameraViewMatrix();

		for(auto &p : _particles.GetParticleContainer())
		{
			if(p.life > 0.0f)
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, p.position);
				model = glm::scale(model, p.scale);

				Shader* shader = ResourceManager::GetInstance().GetShader("AEON_CUBE_SHADER");
				AeonCore_ASSERT(shader, "Failed to retrieve Shader from ResourceManager...")

				Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_CUBE_MESH");
				AeonCore_ASSERT(mesh, "Failed to retrieve Mesh from ResourceManager...")

				shader->use();
				shader->setMat4("model", model);
				shader->setMat4("view", view);
				shader->setMat4("projection", projection);
				shader->setVec3("color", p.colour);

				mesh->m_VertexArray->Bind();
				glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->m_IndexBuffer->GetCount()), GL_UNSIGNED_INT, nullptr);
			}
		}*/

		const glm::mat4 projection = _camera.GetCameraProjectionMatrix();
		const glm::mat4 view = _camera.GetCameraViewMatrix();

		for (auto& p : _particles.GetParticleContainer())
		{

			if (p.life > 0.0f)
			{
				glm::mat4 vp = projection * view;
				glm::mat4 mdl(1.0f);

				mdl = glm::translate(mdl, p.position);
				mdl = glm::scale(mdl, p.scale);

				{
					Model* m = &_model;
					Shader* shader_LightOnto = ResourceManager::GetInstance().GetShader("AEON_MODEL_SHADER");
					Shader* shader_LightSrc = ResourceManager::GetInstance().GetShader("AEON_LIGHT_SRC_SHADER");

					AeonCore_ASSERT(m, "Model used is wrong!");
					AeonCore_ASSERT(shader_LightOnto, "Shader used is wrong!");

					shader_LightSrc->use();
					shader_LightSrc->setMat4("vp", vp);
					shader_LightSrc->setMat4("model", mdl);

					shader_LightOnto->setVec3("ambience", AeonCore::LightManager::GetInstance().GetAmbientLight());
					int counter = 0;

					for (const auto& val : _lights)
					{

						shader_LightOnto->use();
						shader_LightOnto->setVec3("ambience", AeonCore::LightManager::GetInstance().GetAmbientLight());
						shader_LightOnto->setVec3("counter", glm::vec3(_lights.size(), 0, 0));
						shader_LightOnto->setMat4("vp", vp);
						shader_LightOnto->setMat4("model", mdl);

						shader_LightOnto->setVec3("viewPos", _camera.GetCameraPosition());

						std::string str_idx = std::to_string(counter);

						switch (val.second->GetLightType())
						{
						case Lighting::LightType::SPOTLIGHT:
							//AeonCore_INFO("SPOTLIGHT");

							// AeonCore_INFO("Light Position: {}, {}, {}", val.second->GetPosition().x, val.second->GetPosition().y, val.second->GetPosition().z);
							// AeonCore_INFO("Light Direction: {}, {}, {}", val.second->GetLightDirection().x, val.second->GetLightDirection().y, val.second->GetLightDirection().z);
							// AeonCore_INFO("Light Color: {}, {}, {}", val.second->GetColor().x, val.second->GetColor().y, val.second->GetColor().z);
							// AeonCore_INFO("Light Intensity: {}", val.second->GetLightIntensity());

							shader_LightOnto->use();
							shader_LightOnto->setVec3("spotlight[" + str_idx + "].position", val.second->GetPosition());
							shader_LightOnto->setVec3("spotlight[" + str_idx + "].direction", val.second->GetLightDirection());
							shader_LightOnto->setFloat("spotlight[" + str_idx + "].cutOff", glm::cos(glm::radians((float)val.second->GetLightCutoff())));
							shader_LightOnto->setFloat("spotlight[" + str_idx + "].outerCutOff", glm::cos(glm::radians((float)val.second->GetLightOuterCutoff())));
							shader_LightOnto->setVec3("spotlight[" + str_idx + "].color", val.second->GetColor());
							shader_LightOnto->setFloat("spotlight[" + str_idx + "].constant", 1.0f);
							shader_LightOnto->setFloat("spotlight[" + str_idx + "].linear", 0.09f);
							shader_LightOnto->setFloat("spotlight[" + str_idx + "].quadratic", 0.032f);
							shader_LightOnto->setFloat("spotlight[" + str_idx + "].intensity", val.second->GetLightIntensity());

							break;
						case Lighting::LightType::POINT:
							//AeonCore_INFO("POINT");
							break;
						case Lighting::LightType::DIRECTIONAL:
							//AeonCore_INFO("DIRECTIONAL");
							break;
						}
						++counter;
					}

					for (int i = 0; i < m->GetMeshes().size(); ++i)
					{

						_material.Use(*shader_LightOnto);
						_material.Use(*shader_LightSrc);

						// draw mesh 
						m->GetMeshes()[i].m_VertexArray->Bind();
						glDrawElements(GL_TRIANGLES, m->GetMeshes()[i].m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);

						// unbind
						glBindVertexArray(0);
						glActiveTexture(0);
					}
				}
			}
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Rendering>("Rendering")
		.constructor()();
		// .property("universal light", &Rendering::GetUniversalLightColor, &Rendering::SetUniversalLightColor);
	}
}
