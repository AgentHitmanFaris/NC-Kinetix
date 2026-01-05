#include "nc/render/TextRenderer.h"
#include "nc/render/Shader.h"
#include <glad/glad.h>
#include <iostream>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

namespace nc::render {

    TextRenderer::TextRenderer() : m_face(nullptr), m_VAO(0), m_VBO(0), m_initialized(false) {
        if (FT_Init_FreeType(&m_ft)) {
            std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }
    }

    TextRenderer::~TextRenderer() {
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO) glDeleteBuffers(1, &m_VBO);
        
        // Clean up textures
        for (auto& pair : m_characters) {
            glDeleteTextures(1, &pair.second.textureID);
        }
        
        if (m_face) FT_Done_Face(m_face);
        FT_Done_FreeType(m_ft);
    }

    void TextRenderer::init() {
        // Create shader from embedded source
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

uniform sampler2D text;
uniform vec4 textColor;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = textColor * sampled;
}
        )";

        m_shader = std::make_unique<Shader>();
        if (!m_shader->loadFromSource(vertexShader, fragmentShader)) {
            std::cerr << "ERROR::TextRenderer: Failed to create shader" << std::endl;
            return;
        }

        // Configure VAO/VBO
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        // Set defaults
        m_shader->use();
        m_shader->setMat4("projection", glm::mat4(1.0f));
        m_shader->setMat4("model", glm::mat4(1.0f));
        
        m_initialized = true;
    }

    bool TextRenderer::loadFont(const std::string& fontPath, int fontSize) {
        // Clear previous characters
        m_characters.clear();

        if (FT_New_Face(m_ft, fontPath.c_str(), 0, &m_face)) {
            std::cerr << "ERROR::FREETYPE: Failed to load font: " << fontPath << std::endl;
            return false;
        }

        FT_Set_Pixel_Sizes(m_face, 0, fontSize);

        // Disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++) {
            // Load character glyph 
            if (FT_Load_Char(m_face, c, FT_LOAD_RENDER)) {
                std::cerr << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            
            // Generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                m_face->glyph->bitmap.width,
                m_face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                m_face->glyph->bitmap.buffer
            );
            
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F); // GL_CLAMP_TO_EDGE
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F); // GL_CLAMP_TO_EDGE
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            // Store character for later use
            Character character = {
                texture,
                glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
                glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
                static_cast<unsigned int>(m_face->glyph->advance.x)
            };
            m_characters.insert(std::pair<char, Character>(c, character));
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);
        
        // Destroy FreeType face once done
        FT_Done_Face(m_face);
        m_face = nullptr;
        
        return true;
    }

    void TextRenderer::renderText(const std::string& text, float x, float y, float scale, const glm::vec4& color, const glm::mat4& projection) {
        // Construct a simple model matrix for compatibility
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x, y, 0.0f));
        model = glm::scale(model, glm::vec3(scale, scale, 1.0f));
        
        renderText(text, model, color, projection);
    }

    void TextRenderer::renderText(const std::string& text, const glm::mat4& model, const glm::vec4& color, const glm::mat4& projection) {
        if (!m_initialized) return;

        // Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Activate corresponding render state	
        m_shader->use();
        m_shader->setVec4("textColor", color);
        m_shader->setMat4("projection", projection);
        m_shader->setMat4("model", model);
        
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(m_VAO);

        // Iterate through all characters
        // Note: For matrix rendering, we start at (0,0) and let the matrix handle position
        float x = 0.0f; 
        float y = 0.0f;

        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) {
            Character ch = m_characters[*c];

            float xpos = x + ch.bearing.x;
            float ypos = y - (ch.size.y - ch.bearing.y);

            float w = (float)ch.size.x;
            float h = (float)ch.size.y;
            
            // Update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },            
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }           
            };
            
            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
        }
        
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_BLEND);
    }

}
