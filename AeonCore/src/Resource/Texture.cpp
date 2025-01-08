#include "acpch.h"
/*****************************************************************//**
 * \file   Texture.cpp
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

#include "Resource/Texture.h"
#include <GL/glext.h>
#include <stb_image.h>

namespace AeonCore
{

	Texture::Texture(const char* path, Texture::Type _type) : m_path(path), m_type(_type)
	{
		unsigned char* buffer = LoadDDS(path);

		// unused
		//unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
		unsigned int format;
		unsigned int blockSize;
		switch (fourCC)
		{
		case FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			blockSize = 8;
			break;
		case FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			blockSize = 16;
			break;
		case FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			blockSize = 16;
			break;
		default:
			free(buffer);
			return;
		}

		// unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
		unsigned int offset = 0;

		GenerateTextureID();
		AeonCore_ASSERT(textureID, "Failed to generate TextureID");
		Bind();

		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // not needed, redundant, in most cases slower

		// incomplete texture alert!
		AeonCore_ASSERT(textureID, "Incomplete texture alert");

		// they really just did a check to see if there was mipmaps, width, and height
		// which are required by implementation of the exportor to give you
		// just check out the DDS header.dwFlags if you care that much
		
		LoadCompressedTextureToGPU(blockSize, format, buffer, offset);

		free(buffer);

		Unbind();

		AeonCore_INFO("Texture Loaded: {}", path);
	}

	void Texture::GenerateTextureID()
	{
		glGenTextures(1, &textureID);
	}

	void Texture::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	void Texture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::LoadCompressedTextureToGPU(int blockSize, unsigned int format, unsigned char* buffer, unsigned int offset)
	{
		for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
		{
			unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

			glCompressedTexImage2D(
				GL_TEXTURE_2D,
				level,
				format,
				width,
				height,
				0,
				size,
				buffer + offset

			);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


			offset += size;
			width /= 2;
			height /= 2;
		}
	}

	void Texture::LoadCompressedTextureToGPU(int blockSize, unsigned int format, std::vector<unsigned char*> buffers, unsigned int offset)
	{

		assert(buffers.size() == 6);

		for (int iFace = 0; iFace < 6; ++iFace)
		{
			auto w = width;
			auto h = height;
			auto o = offset;

			for (unsigned int level = 0; level < mipMapCount && (w || h); ++level)
			{
				unsigned int size = ((w + 3) / 4) * ((h + 3) / 4) * blockSize;

				glCompressedTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + iFace,
					level,
					format,
					w,
					h,
					0,
					size,
					buffers[iFace] + o		// maybe can test without sizeof(unsigned char) * i
					);

				o += size;
				w /= 2;
				h /= 2;
			}
	    }


		/*
		for (unsigned int level = 0; level < mipMapCount && (w || h); ++level)
		{
			unsigned int size = ((w + 3) / 4) * ((h + 3) / 4) * blockSize;
			
			// shouldnt't the glTexParameteri be done 6 times only? Why doing >6?
			for (int i = 0; i < 6; ++i)
			{
				glCompressedTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					level,
					format,
					w,
					h,
					0,
					size,
					buffers[i] + offset		// maybe can test without sizeof(unsigned char) * i
				);
			}

			offset += size;
			w /= 2;
			h /= 2;
		}
		*/

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	unsigned char* Texture::LoadDDS(const char* path)
	{
		unsigned char header[124]; // dynamic allocation to conserve space

		FILE* fp;
		fp = fopen(path, "rb"); // too long
		if (fp == NULL)
		{
			return 0; // other blocks repeat too much
		}

		char filecode[4]; // can be included into header[] too freakin easy
		fread(filecode, 1, 4, fp);

		// DON'T USE strncmp WHEN COMPARING buffer TO string!
		// faster to use memcpy
		// won't undefined error if filecode has \0 in it
		if (strncmp(filecode, "DDS ", 4) != 0)
		{
			fclose(fp); // this repeats too much
			return 0;
		}

		// umm... they read a 124 byte data type, but 1 of them, lol
		fread(&header, 124, 1, fp);

		// the compiler should complain about this unsafe thing
		height = *(unsigned int*)&(header[8]);
		width = *(unsigned int*)&(header[12]);
		linearSize = *(unsigned int*)&(header[16]);
		mipMapCount = *(unsigned int*)&(header[24]);
		fourCC = *(unsigned int*)&(header[80]);

		// wait why did they make two separate declarations
		unsigned char* buffer;
		unsigned int bufsize;

		// they use linearSize*2 because the sum of all pixels is less than
		// two times the biggest mipmap's resolution
		// so if you have no mipmaps and that ternary check wasn't there
		// you'd of allocated 2x what is needed, its about a 45%-65% bigger
		// buffer than you need
		bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
		buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
		fread(buffer, 1, bufsize, fp);

		fclose(fp);
		return buffer;
	}

	Texture::Texture(std::vector<std::string> faces, Texture::Type _type) : m_type(_type)
	{
		std::vector<unsigned char*> buffers;

		for (int i = 0; i < faces.size(); ++i)
		{
			buffers.push_back(LoadDDS(faces[i].c_str()));
		}

		GenerateTextureID();
		//Bind();

		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		unsigned int format;
		unsigned int blockSize;
		unsigned int offset = 0;

		switch (fourCC)
		{
		case FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			blockSize = 8;
			break;
		case FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			blockSize = 16;
			break;
		case FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			blockSize = 16;
			break;
		default:
			for(int i = 0; i < buffers.size(); ++i)
				free(buffers[i]);
			return;
		}

		LoadCompressedTextureToGPU(blockSize, format, buffers, offset);

		for (int i = 0; i < buffers.size(); ++i)
		{
			free(buffers[i]);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		//Unbind();

		for (int i = 0; i < faces.size(); ++i)
			AeonCore_INFO("Skybox loaded: {0}", faces[i]);
	}

	/*unsigned int Texture::GetTextureID()
	{
		return textureID;
	}*/


}