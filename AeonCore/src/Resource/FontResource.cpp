#include "acpch.h"
/*!***************************************************************************
	\file			FontResource.cpp
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					manage the Font Resource
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Resource/FontResource.h"

namespace AeonCore
{
	FontResource::Character FontResource::GetCharacter(char c)
	{
		return m_Characters[c];
	}

	std::string FontResource::GetPath()
	{
		return m_path;
	}

	FontResource::FontResource(const char* path) : m_path(path)
	{
		//binaryFontResource.buffer = new unsigned char();
		Load(path);
	}

	FontResource::~FontResource()
	{
		// GOt problem dleteing... src of mem leak is this 1 byte
		//if (buffer)
		//	delete buffer;
	}

	void FontResource::Load(const char* path)
	{
		// FT_Library ft;
		// if (FT_Init_FreeType(&ft)) {
		// 	AeonCore_INFO("Failed to init ft lib");
		// }
		// 
		// // Decompress from .font file first
		// std::ifstream ifs(path, std::ios::binary);
		// 
		// if (ifs.is_open())
		// {
		// 	size_t TotalSize;
		// 	ifs.read(reinterpret_cast<char*>(&TotalSize), sizeof(size_t));
		// 
		// 	for (int i = 0; i < TotalSize; ++i)
		// 	{
		// 		char ch;
		// 		Character character;
		// 
		// 		ifs.read(reinterpret_cast<char*>(&ch), sizeof(char));
		// 		ifs.read(reinterpret_cast<char*>(&character), sizeof(Character));
		// 
		// 		binaryFont.Characters.emplace(ch, character);
		// 
		// 	}
		// 
		// 	size_t buffer_size;
		// 	ifs.read(reinterpret_cast<char*>(&buffer_size), sizeof(size_t));
		// 
		// 	FT_Face* face{};
		// 	ifs.read(reinterpret_cast<char*>(&binaryFont.buffer), sizeof(unsigned char));
		// 	// FT_New_Memory_Face(ft, reinterpret_cast<const FT_Byte*>(&binaryFont.buffer), buffer_size, 0, face);
		// 
		// 	ifs.close();
		// }
		// else
		// {
		// 	AeonCore_INFO("Failed to load: ", path);
		// 	ifs.close();
		// }
		// 
		// 
		// 
		// for (auto& c : this->binaryFont.Characters)
		// {
		// 	// Then load as normal using glGenTexture...
		// 	glGenTextures(1, &c.second.TextureID);
		// 	glBindTexture(GL_TEXTURE_2D, c.second.TextureID);
		// 	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 
		// 	 	c.second.Size[0], c.second.Size[1], 0, GL_RED, GL_UNSIGNED_BYTE, binaryFont.buffer);
		// 
		// 	// set texture options
		// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// }

		// Use naive method to load Font
		// FreeType
		// --------
		FT_Library ft;
		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&ft))
		{
			AeonCore_INFO("ERROR::FREETYPE: Could not init FreeType Library");
		}

		// load font as face
		FT_Face face;
		if (FT_New_Face(ft, path, 0, &face)) {
			AeonCore_INFO("ERROR::FREETYPE: Failed to load font");
		}
		else
		{
			// set size to load glyphs as
			FT_Set_Pixel_Sizes(face, 0, 48);

			// disable byte-alignment restriction
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			// load first 128 characters of ASCII set
			for (unsigned char c = 0; c < 128; c++)
			{
				// Load character glyph 
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					AeonCore_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
					continue;
				}
				// generate texture
				unsigned int texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);
				// set texture options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				// now store character for later use
				Character character = {
					texture,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					static_cast<unsigned int>(face->glyph->advance.x)
				};
				m_Characters.insert(std::pair<char, Character>(c, character));
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		// destroy FreeType once we're finished
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

	}

}
