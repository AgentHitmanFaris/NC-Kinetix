#pragma once

#include <string>
#include <glm/glm.hpp>

namespace nc::render {

    class Shader {
    public:
        Shader();
        ~Shader();

        bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
        bool loadFromSource(const std::string& vertexSource, const std::string& fragmentSource);
        
        void use() const;
        void unbind() const;
        
        // Uniform setters
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setMat4(const std::string& name, const glm::mat4& value) const;
        
        unsigned int getProgram() const { return m_program; }
        bool isValid() const { return m_program != 0; }

    private:
        unsigned int compileShader(unsigned int type, const std::string& source);
        bool checkCompileErrors(unsigned int shader, const std::string& type);
        bool checkLinkErrors();
        
        unsigned int m_program;
    };

}
