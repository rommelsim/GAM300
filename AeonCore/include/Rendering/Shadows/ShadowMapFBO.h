#pragma once
#include <glad/glad.h>
#include "Core/Log.h"

#include "Core/AeonAPI.h"
#include <Resource/Shader.h>

namespace AeonCore
{
    

    // Use Non-DSA
    class ShadowMapFBO
    {
    public:

        enum class MODE
        {
            EMPTY = 0,
            SPOT,
            POINT,
            DIRECTIONAL,
        };

        ShadowMapFBO();
        ~ShadowMapFBO();
        bool Init(unsigned int Width, unsigned int Height, bool ForPCF = false, MODE mode = MODE::SPOT);
        void BindForWriting();
        void BindForReading(GLenum TextureUnit);
        GLuint& GetID() { return m_shadowMap; }
    private:
        bool InitDSA(unsigned int Width, unsigned int Height, bool ForPCF = false, MODE mode = MODE::SPOT);
        void BindForReadingDSA(GLenum TextureUnit);
        void BindForReadingDSA(Shader& shader);

        GLuint m_width = 0;
        GLuint m_height = 0;
        GLuint m_fbo;
        GLuint m_shadowMap;
    };
}
