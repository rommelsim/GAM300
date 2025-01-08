/*!***************************************************************************
    \file			main.cpp
    \author			Rommel sim
    \par			zhenpengrommel.sim\@digipen.edu
    \brief			Command-line interface for the Aeon resource compiler. This
					program serves as a command-line interface (CLI) for the
					Aeon resource compiler, enabling compression of geometry
					and font resources.

					Command Line Usage:
					- For geometry compression:
                     ./Compiler.exe -geom input.obj -o "output/location/"
					- For font compression:
                     ./Compiler.exe -font input.ttf -o "output/location/"

    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <string>
#include <Geom/geom_compiler.h>
#include <Font/font_compiler.h>

//  [0]             [1]     [2]          [3]    [4]
// ./Compiler.exe -compress backpack.obj -o "/Resources/objects/backpack/"
// will output at where backpack.obj was...
int main(int argc, char* argv[]) {
   
    // Write to...
    std::string file = argv[2];
    std::string outputLocation = argv[4];

    if (std::string(argv[1]) == "-geom")
    {
         if (Geometry::ReadOBJ(file.c_str()))
         {
             if (std::string(argv[3]) == "-o")
             {
                 size_t pos = file.find(".");
                 std::string name(file.substr(0, pos));
                 if (Geometry::Compress(name.c_str(), Geometry::vertices, Geometry::indices))
                 {
                     std::cout << "Compression success. Written to " << outputLocation << std::endl;
                 }
                 else
                 {
                     std::cout << "Compression failed\n";
                 }
             }
             else
             {
                 std::cout << "Wrong output command\n";
             }
         }
    }
    else if (std::string(argv[1]) == "-font")
    {
        std::string Font = Font::GetFontName(file);
        if (Font::LoadFont(file.c_str()))
        {
            Font::Compress(outputLocation.c_str(),Font.c_str());
        }

    }
    else
    {
        std::cout << "Invalid cmd\n";
    }

    return 0;
}
