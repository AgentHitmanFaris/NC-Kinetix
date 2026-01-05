#include "nc/render/Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace nc::render {

    Shader::Shader() : m_program(0) {}

    Shader::~Shader() {
        if (m_program != 0) {
            glDeleteProgram(m_program);
        }
    }

    bool Shader::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
        std::string vertexCode, fragmentCode;
        
        try {
            std::ifstream vShaderFile(vertexPath);
            std::ifstream fShaderFile(fragmentPath);
            
            if (!vShaderFile.is_open()) {
                std::cerr << "ERROR::SHADER::VERTEX::FILE_NOT_FOUND: " << vertexPath << std::endl;
                return false;
            }
            if (!fShaderFile.is_open()) {
                std::cerr << "ERROR::SHADER::FRAGMENT::FILE_NOT_FOUND: " << fragmentPath << std::endl;
                return false;
            }
            
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            
            vShaderFile.close();
            fShaderFile.close();
            
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::exception& e) {
            std::cerr << "ERROR::SHADER::FILE_READ_FAILED: " << e.what() << std::endl;
            return false;
        }
        
        return loadFromSource(vertexCode, fragmentCode);
    }

    bool Shader::loadFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
        unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexSource);
        if (vertex == 0) return false;
        
        unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
        if (fragment == 0) {
            glDeleteShader(vertex);
            return false;
        }
        
        // Link program
        m_program = glCreateProgram();
        glAttachShader(m_program, vertex);
        glAttachShader(m_program, fragment);
        glLinkProgram(m_program);
        
        bool success = checkLinkErrors();
        
        // Clean up shaders (they're linked into the program now)
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        
        if (!success) {
            glDeleteProgram(m_program);
            m_program = 0;
            return false;
        }
        
        return true;
    }

    unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
        unsigned int shader = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
        
        std::string typeName = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        if (!checkCompileErrors(shader, typeName)) {
            glDeleteShader(shader);
            return 0;
        }
        
        return shader;
    }

    bool Shader::checkCompileErrors(unsigned int shader, const std::string& type) {
        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        
        if (!success) {
            char infoLog[1024];
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
            return false;
        }
        return true;
    }

    bool Shader::checkLinkErrors() {
        int success;
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);
        
        if (!success) {
            char infoLog[1024];
            glGetProgramInfoLog(m_program, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            return false;
        }
        return true;
    }

    void Shader::use() const {
        if (m_program != 0) {
            glUseProgram(m_program);
        }
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }

    void Shader::setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(m_program, name.c_str()), value);
    }

    void Shader::setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(m_program, name.c_str()), value);
    }

    void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
        glUniform3f(glGetUniformLocation(m_program, name.c_str()), value.x, value.y, value.z);
    }

    void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
        glUniform4f(glGetUniformLocation(m_program, name.c_str()), value.x, value.y, value.z, value.w);
    }

    void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
        glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

}
