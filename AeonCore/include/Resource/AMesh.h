#pragma once
/*!***************************************************************************
    \file			AModel.h
    \author			Leong Wai Kit
    \par			l.waikit\@digipen.edu
    \brief			Definition of the AModel class. This class represents a 3D
					model and provides functionality for loading and rendering
					3D models using the ASSIMP library. It includes features
					such as gamma correction, texture loading, and bone weight
					extraction for skeletal animation. The class defines the
					model's data structures, including vertices, indices, and
					textures, and provides methods for rendering and processing
					the model's meshes. The AModel class is designed for use in
					a game engine or graphics framework, particularly in an
					OpenGL rendering context.

    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Core/AeonAPI.h"
#include "Core/Application.h"

#include "Animation/Animator.h"
#include "Resource/ResourceManager.h"
#include "Camera/CameraManager.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#define MAX_BONE_INFLUENCE 4

namespace AeonCore
{
    struct AVertex
    {
        glm::vec3 Position;                 //  position
        glm::vec3 Normal;                   //  normal
        glm::vec2 TexCoords;                //  texCoords
        glm::vec3 Tangent;                  //  tangent
        glm::vec3 Bitangent;                //  bitangent
        int BoneIDs[MAX_BONE_INFLUENCE];    //  bone indexes which will influence this vertex
        float Weights[MAX_BONE_INFLUENCE];  //  weights from each bone
    };

    struct ATexture
    {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class AEON_API AMesh
    {
    public:
        AMesh(std::vector<AVertex> _vertices, std::vector<unsigned int> _indices, std::vector<ATexture> _textures);
        void Draw(Camera& _camera, Animator& _anima) const;

    private:
        //  mesh Data
        std::vector<AVertex>        mVertices;
        std::vector<unsigned int>   mIndices;
        std::vector<ATexture>       mTextures;
        unsigned int mVAO{};
        unsigned int mVBO{}, mEBO{};

        //  render data 
        void setupMesh();
    };
}
