#pragma once

#include "nc/scene/SceneNode.h"
#include <string>
#include <memory>

namespace nc::render {
    class SpriteRenderer;
    class Texture;
}

namespace nc::scene {

    class SpriteNode : public SceneNode {
    public:
        SpriteNode(const std::string& name = "SpriteNode");
        
        void setTexture(const std::string& path);
        const std::string& getTexturePath() const { return m_texturePath; }

        void render(const glm::mat4& viewProjection) override;
        
        static void setRenderer(render::SpriteRenderer* renderer);

    private:
        std::string m_texturePath;
        std::shared_ptr<render::Texture> m_texture;
        glm::vec4 m_color{1.0f, 1.0f, 1.0f, 1.0f};
        
        static render::SpriteRenderer* s_renderer;
    };

}
