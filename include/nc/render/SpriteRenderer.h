#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace nc::render {

    class Shader;
    class Texture;

    class SpriteRenderer {
    public:
        SpriteRenderer();
        ~SpriteRenderer();

        void init();
        
        // Render a texture
        void drawSprite(const Texture* texture, const glm::mat4& model, const glm::vec4& color, const glm::mat4& projection);
        
        Shader* getShader() const { return m_shader.get(); }

    private:
        std::unique_ptr<Shader> m_shader;
        unsigned int m_VAO;
    };

}
