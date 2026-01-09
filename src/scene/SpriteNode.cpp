#include "nc/scene/SpriteNode.h"
#include "nc/render/SpriteRenderer.h"
#include "nc/render/Texture.h"
#include <iostream>

namespace nc::scene {

    render::SpriteRenderer* SpriteNode::s_renderer = nullptr;

    SpriteNode::SpriteNode(const std::string& name) : SceneNode(name) {
    }

    void SpriteNode::setTexture(const std::string& path) {
        m_texturePath = path;
        m_texture = std::make_shared<render::Texture>();
        if (!m_texture->loadFromFile(path)) {
            std::cerr << "SpriteNode: Failed to load texture: " << path << std::endl;
        }
    }

    void SpriteNode::setRenderer(render::SpriteRenderer* renderer) {
        s_renderer = renderer;
    }

    void SpriteNode::render(const glm::mat4& viewProjection) {
        if (!s_renderer || !m_texture) return;
        
        s_renderer->drawSprite(m_texture.get(), getGlobalTransform(), m_color, viewProjection);
        
        // Render children
        SceneNode::render(viewProjection);
    }

}
