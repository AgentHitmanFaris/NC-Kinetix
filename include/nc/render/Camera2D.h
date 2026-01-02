#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace nc::render {

    class Camera2D {
    public:
        Camera2D(float width, float height);

        void setPosition(const glm::vec2& pos);
        void setZoom(float zoom);
        void setRotation(float rotationDegrees);
        void setDimensions(float width, float height);

        const glm::mat4& getViewMatrix();
        const glm::mat4& getProjectionMatrix();
        const glm::mat4& getViewProjectionMatrix();

        glm::vec2 getPosition() const { return m_position; }
        float getZoom() const { return m_zoom; }
        float getRotation() const { return m_rotation; }

    private:
        void updateMatrices();

        glm::vec2 m_position{0.0f, 0.0f};
        float m_rotation{0.0f};
        float m_zoom{1.0f};
        
        float m_width;
        float m_height;

        glm::mat4 m_viewMatrix{1.0f};
        glm::mat4 m_projectionMatrix{1.0f};
        glm::mat4 m_viewProjectionMatrix{1.0f};
        
        bool m_dirty{true};
    };

}
