#pragma once

#include "nc/scene/SceneNode.h"
#include <glm/glm.hpp>
#include <string>

namespace nc::render {
    class TextRenderer;
}

namespace nc::scene {

    class TextNode : public SceneNode {
    public:
        TextNode(const std::string& name = "TextNode");
        
        void setContent(const std::string& content);
        void setColor(const glm::vec4& color);
        void setScale(float scale);
        void setFontID(const std::string& fontID);
        
        const std::string& getContent() const { return m_content; }
        const glm::vec4& getColor() const { return m_color; }
        float getTextScale() const { return m_textScale; }
        const std::string& getFontID() const { return m_fontID; }
        
        void render(const glm::mat4& viewProjection) override;
        
        // Dependency injection for renderer
        static void setTextRenderer(render::TextRenderer* renderer);
        
    private:
        std::string m_content;
        glm::vec4 m_color{1.0f, 1.0f, 1.0f, 1.0f};
        float m_textScale{1.0f};
        std::string m_fontID;
        
        static render::TextRenderer* s_textRenderer;
    };

}
