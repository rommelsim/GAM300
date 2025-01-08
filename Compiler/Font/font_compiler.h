#pragma once

#include <map>
#include <iostream>
#include <glm/glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Font
{


    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        int          Size[2];      // Size of glyph
        int          Bearing[2];   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

    struct BinaryFont
    {
        std::map<char, Character> Characters;
        FT_Face face;
    };
    BinaryFont binaryFont;
    FT_Library ft;

    /// <summary>
    /// Load font 
    /// </summary>
    /// <param name="path"></param>
    /// <returns></returns>
    bool LoadFont(const char* path)
    {
        

        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return 0;
        }

        if (FT_New_Face(ft, path, 0, &binaryFont.face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            return 0;
        }

        else
        {
            
            FT_Set_Pixel_Sizes(binaryFont.face, 0, 48);

            // load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++)
            {
                static unsigned int texture = 0;
                // Load character glyph 
                if (FT_Load_Char(binaryFont.face, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                    continue;
                }

                // Used as reference when writing in bytes out
                //struct Character {
                //    unsigned int TextureID; // ID handle of the glyph texture
                //    glm::ivec2   Size;      // Size of glyph
                //    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
                //    unsigned int Advance;   // Horizontal offset to advance to next glyph
                //};

                // now store character for later use

                Character character = {
                    texture,
                    {static_cast<int>(binaryFont.face->glyph->bitmap.width), static_cast<int>(binaryFont.face->glyph->bitmap.rows)},
                    {binaryFont.face->glyph->bitmap_left, binaryFont.face->glyph->bitmap_top},
                    static_cast<unsigned int>(binaryFont.face->glyph->advance.x)

                };


                binaryFont.Characters.insert(std::pair<char, Character>(c, character));
            }
        }

        // destroy FreeType once we're finished
      /*  FT_Done_Face(face);
        FT_Done_FreeType(ft);*/

        std::cout << "Glyphs created\n";
        return 1;
    }

    bool Compress(const char* out, const char* fileout_name)
    {
        std::string output = out + std::string("/") + std::string(fileout_name) + ".font";
        std::ofstream ofs(output, std::ios::binary);
        if (ofs.is_open())
        {
            // Order of writing:
            // 1. total size of character count
            // 2. face size
            // 3. buffer 
            // 4. ch + character data
            //


            size_t totalSize = binaryFont.Characters.size();
            ofs.write(reinterpret_cast<char*>(&totalSize), sizeof(size_t));


            // Write char data first
            for (auto& c : binaryFont.Characters)
            {
                char ch = c.first;
                Character character = c.second;
            
                // If correct then write out as binary
                ofs.write(reinterpret_cast<char*>(&ch), sizeof(char));
                ofs.write(reinterpret_cast<char*>(&character), sizeof(Character));
            
            }

            // Write the buffer size 
            size_t buffer_size = sizeof(*binaryFont.face->glyph->bitmap.buffer);
            ofs.write(reinterpret_cast<char*>(&buffer_size), sizeof(size_t));

            // Write buffer data
            ofs.write(reinterpret_cast<char*>(binaryFont.face->glyph->bitmap.buffer), sizeof(unsigned char));
            
            std::cout << "Total Size: " << totalSize << "\n";
            std::cout << "Successfully compressed to: " << output << std::endl;
            ofs.close();
            return 1;
        }

        ofs.close();
        std::cout << "Failed to open " << output << "\n";
        return 0;
    }

    std::string GetFontName(std::string filePath)
    {
        
        size_t lastSlash = filePath.find_last_of('\\');
        size_t extensionDot = filePath.find_last_of('.');

        if (lastSlash != std::string::npos && extensionDot != std::string::npos) {
            std::string fontName = filePath.substr(lastSlash + 1, extensionDot - lastSlash - 1);
            std::cout << "Font Name: " << fontName << std::endl;
            return fontName;
        }

        return "";
    }
}