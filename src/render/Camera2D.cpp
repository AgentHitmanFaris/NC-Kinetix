#include "nc/render/Camera2D.h"

namespace nc::render {

    Camera2D::Camera2D(float width, float height) 
        : m_width(width), m_height(height) 
    {
        updateMatrices();
    }

    void Camera2D::setPosition(const glm::vec2& pos) {
        m_position = pos;
        m_dirty = true;
    }

    void Camera2D::setZoom(float zoom) {
        if (zoom > 0.001f) {
            m_zoom = zoom;
            m_dirty = true;
        }
    }

    void Camera2D::setRotation(float rotationDegrees) {
        m_rotation = rotationDegrees;
        m_dirty = true;
    }
    
    void Camera2D::setDimensions(float width, float height) {
        m_width = width;
        m_height = height;
        m_dirty = true;
    }

    void Camera2D::updateMatrices() {
        // Projection: Orthographic centered at (0,0)
        // Adjust for aspect ratio or keep units 1:1 pixel?
        // Let's assume standard pixel coords centered.
        float left = -m_width / 2.0f;
        float right = m_width / 2.0f;
        float bottom = -m_height / 2.0f;
        float top = m_height / 2.0f;

        m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

        // View: Inverse of Camera Transform
        glm::mat4 transform = glm::mat4(1.0f);
        // Translate -> Rotate -> Scale
        // View Matrix is ScaleInv * RotateInv * TranslateInv
        
        // Note on zoom: Camera zoom implies scaling the world up, or the view window down.
        // Applying scale to the view matrix acts as zoom.
        transform = glm::scale(transform, glm::vec3(m_zoom, m_zoom, 1.0f));
        transform = glm::rotate(transform, glm::radians(-m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::translate(transform, glm::vec3(-m_position.x, -m_position.y, 0.0f));
        
        m_viewMatrix = transform;
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        
        m_dirty = false;
    }

    const glm::mat4& Camera2D::getViewMatrix() {
        if (m_dirty) updateMatrices();
        return m_viewMatrix;
    }

    const glm::mat4& Camera2D::getProjectionMatrix() {
        if (m_dirty) updateMatrices();
        return m_projectionMatrix;
    }

    const glm::mat4& Camera2D::getViewProjectionMatrix() {
        if (m_dirty) updateMatrices();
        return m_viewProjectionMatrix;
    }

}
