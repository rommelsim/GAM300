#include "acpch.h"
/*!***************************************************************************
	\file			AMesh.cpp
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\brief			Implementation of the AMesh class. This class is designed
					for setting up and rendering 3D meshes with a focus on
					animation support. It allows the definition of 3D
					vertices with attributes like position, normal, texture
					coordinates, tangent, bitangent, bone IDs, and weights.
					The AMesh class provides a Draw method for rendering
					meshes within an animated scene, considering camera views.
					This code is intended for use with OpenGL and includes
					private functions for configuring mesh data.
					
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Resource/AMesh.h"

namespace AeonCore
{
	AMesh::AMesh(std::vector<AVertex> _vertices, std::vector<unsigned> _indices, std::vector<ATexture> _textures)
	{
		this->mVertices = _vertices;
		this->mIndices = _indices;
		this->mTextures = _textures;

		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		setupMesh();
	}

	void AMesh::Draw(Camera& _camera, Animator& _anima) const
	{
		Application& app = Application::GetApplication();
		const int width = static_cast<int>(app.GetWindow().GetWidth());
		const int height = static_cast<int>(app.GetWindow().GetHeight());

		const glm::mat4 projection = glm::perspective(glm::radians(_camera.GetCameraZoom()),
			static_cast<float>(width) / static_cast<float>(height), 0.1f,
			10000.0f);
		const glm::mat4 view = _camera.GetCameraViewMatrix();

		Shader* shader = ResourceManager::GetInstance().GetShader("AEON_ANIMATION_SHADER");
		shader->use();

		shader->setMat4("projection", projection);
		shader->setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.f, 0.f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
		//model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));	// it's a bit too big for our scene, so scale it down
		shader->setMat4("model", model);

		auto transforms = _anima.GetFinalBoneMatrices();
		for (unsigned int i = 0; i < transforms.size(); ++i)
			shader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

		// bind appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		//Mesh* mesh = ResourceManager::GetInstance().GetMesh("AEON_ANIMATION_MESH");

		for (unsigned int i = 0; i < mTextures.size(); i++)
		{
		    glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding

		    std::string number;
		    std::string name = mTextures[i].type;

		    if (name == "texture_diffuse")
		        number = std::to_string(diffuseNr++);
		    else if (name == "texture_specular")
		        number = std::to_string(specularNr++); // transfer unsigned int to string
		    else if (name == "texture_normal")
		        number = std::to_string(normalNr++); // transfer unsigned int to string
		    else if (name == "texture_height")
		        number = std::to_string(heightNr++); // transfer unsigned int to string

		    // now set the sampler to the correct texture unit
		    glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), static_cast<GLint>(i));
			//glUniform1i(glGetUniformLocation(shader->ID, std::string("texture_diffuse1").c_str()), 0);
		    // and finally bind the texture
		    glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
		}

		// draw mesh
		glBindVertexArray(mVAO);
		//mesh->m_VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->m_IndexBuffer->GetCount()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// always good practice to set everything back to defaults once configured.
		glActiveTexture(GL_TEXTURE0);
	}

	//  PRIVATE FUNCTIONS

	void AMesh::setupMesh()
	{
		// create buffers/arrays
		glGenVertexArrays(1, &mVAO);
		glGenBuffers(1, &mVBO);
		glGenBuffers(1, &mEBO);

		glBindVertexArray(mVAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(AVertex), &mVertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AVertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AVertex), (void*)offsetof(AVertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AVertex), (void*)offsetof(AVertex, TexCoords));
		//// vertex tangent
		//glEnableVertexAttribArray(3);
		//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(AVertex), (void*)offsetof(AVertex, Tangent));
		//// vertex bitangent
		//glEnableVertexAttribArray(4);
		//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(AVertex), (void*)offsetof(AVertex, Bitangent));
		// ids
		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 4, GL_INT, sizeof(AVertex), (void*)offsetof(AVertex, BoneIDs));

		// weights
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(AVertex), (void*)offsetof(AVertex, Weights));
		glBindVertexArray(0);

		std::vector<float> VERTICES;

		for (const AVertex& vertex : mVertices)
		 {
			 // Extract and add the components to the VERTICES vector
			 VERTICES.push_back(vertex.Position.x);
			 VERTICES.push_back(vertex.Position.y);
			 VERTICES.push_back(vertex.Position.z);

			 VERTICES.push_back(vertex.Normal.x);
			 VERTICES.push_back(vertex.Normal.y);
			 VERTICES.push_back(vertex.Normal.z);

			 VERTICES.push_back(vertex.TexCoords.x);
			 VERTICES.push_back(vertex.TexCoords.y);

			/* VERTICES.push_back(vertex.Tangent.x);
			 VERTICES.push_back(vertex.Tangent.y);
			 VERTICES.push_back(vertex.Tangent.z);

			 VERTICES.push_back(vertex.Bitangent.x);
			 VERTICES.push_back(vertex.Bitangent.y);
			 VERTICES.push_back(vertex.Bitangent.z);*/

			 for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
			 {
				 VERTICES.push_back(static_cast<float>(vertex.BoneIDs[i]));
			 }

			 for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
			 {
				 VERTICES.push_back(vertex.Weights[i]);
			 }
		 }

		// Define the layout of the vertex data
		 BufferLayout LAYOUT =
		 {
			 {ShaderDataType::Float3, "pos"},
			 {ShaderDataType::Float3, "norm"},
			 {ShaderDataType::Float2, "tex"},
			 {ShaderDataType::Int4, "boneIds"},
			 {ShaderDataType::Float4, "weights"}
		 };

		 const std::filesystem::path working_dir(std::filesystem::current_path());
		 const auto parent_path = working_dir;

		 ResourceManager::GetInstance().LoadMesh("AEON_ANIMATION_MESH", VERTICES, mIndices, LAYOUT);
		 ResourceManager::GetInstance().LoadShader("AEON_ANIMATION_SHADER",
		     parent_path.string() + std::string("/resources/shaders/AEON_ANIMATION.vert"),
		     parent_path.string() + std::string("/resources/shaders/AEON_ANIMATION.frag"));
	}
}
