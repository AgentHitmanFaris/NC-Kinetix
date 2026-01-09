#include "nc/render/SpriteRenderer.h"
#include "nc/render/Shader.h"
#include "nc/render/Texture.h"
#include <glad/glad.h>
#include <iostream>

namespace nc::render {

    SpriteRenderer::SpriteRenderer() : m_VAO(0) {}

    SpriteRenderer::~SpriteRenderer() {
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    }

    void SpriteRenderer::init() {
        // Sprite shader
        const std::string vertexShader = R"(
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 model;

void main() {
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
        )";

        const std::string fragmentShader = R"(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;

void main() {
    color = spriteColor * texture(image, TexCoords);
}
        )";

        m_shader = std::make_unique<Shader>();
        if (!m_shader->loadFromSource(vertexShader, fragmentShader)) {
            std::cerr << "ERROR::SpriteRenderer: Failed to create shader" << std::endl;
        }

        // Configure VAO (Quad)
        unsigned int VBO;
        float vertices[] = { 
            // pos      // tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 
        
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glDeleteBuffers(1, &VBO); // VBO is stored in VAO
        
        m_shader->use();
        m_shader->setInt("image", 0);
        m_shader->setMat4("projection", glm::mat4(1.0f));
    }

    void SpriteRenderer::drawSprite(const Texture* texture, const glm::mat4& model, const glm::vec4& color, const glm::mat4& projection) {
        if (!m_shader) return;
        
        m_shader->use();
        m_shader->setMat4("projection", projection);
        m_shader->setMat4("model", model);
        m_shader->setVec4("spriteColor", color);

        if (texture) {
            glActiveTexture(GL_TEXTURE0);
            texture->bind(0);
        }

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

}
