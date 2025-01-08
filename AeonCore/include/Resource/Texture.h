#ifndef TEXTURE_H
#define TEXTURE_H
/*****************************************************************//**
 * \file   Texture.h
 * \brief  Contains class to handle Texture creation.
 *
 *
 * \author Rommel
 * \date   October 2023
 *
 * Copyright (C) 2023 Digipen Insitute Of Technology
 * Reproduction or disclosure of this file or its contents without
 * the prior consent of Digipen Insitute Of Technology is prohibited.
 *********************************************************************/


#include "Core/Log.h"
#include "Core/AeonAPI.h"
#include <glad/glad.h>

const unsigned long FOURCC_DXT1 = 0x31545844; //(MAKEFOURCC('D','X','T','1'))
const unsigned long FOURCC_DXT2 = 0x32545844; //(MAKEFOURCC('D','X','T','1'))
const unsigned long FOURCC_DXT3 = 0x33545844; //(MAKEFOURCC('D','X','T','3'))
const unsigned long FOURCC_DXT5 = 0x35545844; //(MAKEFOURCC('D','X','T','5'))

namespace AeonCore
{
	
	AEON_API class Texture
	{
    public:

		#define DIFFUSE_TEXTURE_SLOT	1
		#define SPECULAR_TEXTURE_SLOT	2
		#define SHADOWMAP_TEXTURE_SLOT	3
		#define CUBEMAP_TEXTURE_SLOT	4
		#define WORLD_TEXTURE_SLOT		5	// used for world cube map		

		enum class Type
		{
			None = 0,
			Ambient,
			Diffuse, 
			Specular,
			CubeMap
		};
		Texture() = default;
		Texture(std::vector<std::string> faces, Texture::Type _type);
        Texture(const char* path, Texture::Type _type);
		inline unsigned int  GetTextureID() { return textureID; }

		std::string GetPath() { return m_path; }
	    
    private:
		unsigned int    textureID;
		unsigned int    height;
		unsigned int    width;
		unsigned int    linearSize;
		unsigned int    mipMapCount;
		unsigned int    fourCC;
		std::string     m_path;
		Type		    m_type;

		// Helper functions for internal Texture Loading to OpenGL
		unsigned char*	LoadDDS(const char* path);
		void			LoadCompressedTextureToGPU(int blockSize, unsigned int format, unsigned char* buffer, unsigned int offset);
		void			LoadCompressedTextureToGPU(int blockSize, unsigned int format, std::vector<unsigned char*> buffers, unsigned int offset);
		void			GenerateTextureID();
		void			Bind();
		void			Unbind();
	};

}

#endif // !TEXTURE_H

