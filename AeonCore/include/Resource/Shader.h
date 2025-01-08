
#ifndef SHADER_H
#define SHADER_H
/*****************************************************************//**
 * \file   Shader.h
 * \brief  Shader class
 *
 *
 * \author Rommel
 * \date   October 2023
 *
 * Copyright (C) 2023 Digipen Insitute Of Technology
 * Reproduction or disclosure of this file or its contents without
 * the prior consent of Digipen Insitute Of Technology is prohibited.
 *********************************************************************/
#include "Core/AeonAPI.h"
#include "Core/Log.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace AeonCore
{
    class AEON_API Shader
    {
    public:
        unsigned int ID;
        // constructor generates the shader on the fly
        // ------------------------------------------------------------------------
        Shader() {}
        Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

        // activate the shader
        // ------------------------------------------------------------------------
        void use();
        // utility uniform functions
        // ------------------------------------------------------------------------
        void setBool(const std::string& name, bool value) const;
        // ------------------------------------------------------------------------
        void setInt(const std::string& name, int value) const;
        // ------------------------------------------------------------------------
        void setFloat(const std::string& name, float value) const;
        // ------------------------------------------------------------------------
        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec2(const std::string& name, float x, float y) const;
        // ------------------------------------------------------------------------
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec3(const std::string& name, float x, float y, float z) const;
        // ------------------------------------------------------------------------
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setVec4(const std::string& name, float x, float y, float z, float w);
        // ------------------------------------------------------------------------
        void setMat2(const std::string& name, const glm::mat2& mat) const;
        // ------------------------------------------------------------------------
        void setMat3(const std::string& name, const glm::mat3& mat);
        // ------------------------------------------------------------------------
        void setMat4(const std::string& name, const glm::mat4& mat) const;

        // Uniform blocks
        void BindUniformBlockToBindingPoint(const std::string& name, const GLuint bindingPt) const;
    private:
        // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------
        GLuint GetUniformBlockIndex(const std::string& name) const;
        bool m_CreationFlag;
        bool checkCompileErrors(GLuint shader, std::string type);
    };

}
#endif