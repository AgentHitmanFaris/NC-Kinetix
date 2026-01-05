#include "nc/scene/TextNode.h"
#include "nc/render/TextRenderer.h"

namespace nc::scene {

    render::TextRenderer* TextNode::s_textRenderer = nullptr;

    TextNode::TextNode(const std::string& name) : SceneNode(name) {}

    void TextNode::setContent(const std::string& content) {
        m_content = content;
    }

    void TextNode::setColor(const glm::vec4& color) {
        m_color = color;
    }

    void TextNode::setScale(float scale) {
        m_textScale = scale;
    }

    void TextNode::setFontID(const std::string& fontID) {
        m_fontID = fontID;
    }

    void TextNode::setTextRenderer(render::TextRenderer* renderer) {
        s_textRenderer = renderer;
    }

    void TextNode::render(const glm::mat4& viewProjection) {
        if (s_textRenderer && !m_content.empty()) {
            // Get global transform
            const glm::mat4& globalTransform = getGlobalTransform();
            
            // Render text with global transform (position, rotation, scale)
            s_textRenderer->renderText(
                m_content,
                globalTransform, // Pass the full matrix
                m_color,
                viewProjection
            );
        }
        
        // Render children
        SceneNode::render(viewProjection);
    }

}
