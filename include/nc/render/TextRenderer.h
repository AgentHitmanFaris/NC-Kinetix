#pragma once

#include <string>
#include <map>
#include <memory>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace nc::render {

    class Shader;

    // Glyph character structure
    struct Character {
        unsigned int textureID;  // Texture atlas ID
        glm::ivec2 size;         // Size of glyph
        glm::ivec2 bearing;      // Offset from baseline
        unsigned int advance;    // Horizontal advance
    };

    class TextRenderer {
    public:
        TextRenderer();
        ~TextRenderer();

        void init();
        bool loadFont(const std::string& fontPath, int fontSize);
        
        // Render text with projection matrix
        void renderText(const std::string& text, float x, float y, float scale, const glm::vec4& color, const glm::mat4& projection);
        
        // Render text with full model matrix (for scene graph)
        void renderText(const std::string& text, const glm::mat4& model, const glm::vec4& color, const glm::mat4& projection);
        
        // Get shader for external use
        Shader* getShader() const { return m_shader.get(); }
        
        const Character* getCharacter(uint32_t c) const;

    private:
        FT_Library m_ft;
        FT_Face m_face;
        
        // OpenGL resources
        unsigned int m_VAO, m_VBO;
        std::map<uint32_t, Character> m_characters;
        std::unique_ptr<Shader> m_shader;
        
        bool m_initialized;
    };

}
