#pragma once
/*!***************************************************************************
    \file			FontResource.h
    \author			Rommel Sim
    \par			zhenpengrommel.sim@digipen.edu
    \date			Oct 1, 2023
    \brief			This header file consists of the function declarations to
                    manage the Font Resource
    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/Log.h"
#include "Core/AeonAPI.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <map>
#include "Resource/Mesh.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace AeonCore
{
    AEON_API class FontResource
    {
    public:
        struct Character {
            unsigned int TextureID; // ID handle of the glyph texture
            glm::ivec2   Size;     // Size of glyph
            glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
            unsigned int Advance;   // Horizontal offset to advance to next glyph
        };

        FontResource() = default;
        FontResource(const char* path);
        ~FontResource();

        Character GetCharacter(char c);
        std::string GetPath();

    private:
        std::string                 m_path;
        std::map<char, Character>   m_Characters;
        void Load(const char* path);

    };
}
