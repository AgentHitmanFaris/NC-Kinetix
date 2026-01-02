#include "nc/scene/SceneNode.h"
#include <iostream>
#include <algorithm>

namespace nc::scene {

    SceneNode::SceneNode(const std::string& name) : m_name(name) {}

    void SceneNode::setPosition(const glm::vec2& pos) {
        m_position = pos;
        markDirty();
    }

    void SceneNode::setRotation(float angleDegrees) {
        m_rotation = angleDegrees;
        markDirty();
    }

    void SceneNode::setScale(const glm::vec2& scale) {
        m_scale = scale;
        markDirty();
    }

    glm::vec2 SceneNode::getPosition() const { return m_position; }
    float SceneNode::getRotation() const { return m_rotation; }
    glm::vec2 SceneNode::getScale() const { return m_scale; }

    void SceneNode::addChild(Ptr child) {
        if (child) {
            child->m_parent = shared_from_this();
            m_children.push_back(child);
            child->markDirty(); // Parent transform affects child
        }
    }

    void SceneNode::removeChild(Ptr child) {
        auto it = std::remove(m_children.begin(), m_children.end(), child);
        if (it != m_children.end()) {
            (*it)->m_parent.reset();
            m_children.erase(it, m_children.end());
        }
    }

    const std::vector<SceneNode::Ptr>& SceneNode::getChildren() const {
        return m_children;
    }

    SceneNode::Ptr SceneNode::getParent() const {
        return m_parent.lock();
    }

    void SceneNode::update(float dt) {
        // Update logic here (animation etc)
        
        // Propagate to children
        for (auto& child : m_children) {
            child->update(dt);
        }
    }

    void SceneNode::render(const glm::mat4& viewProjection) {
        // Base node renders nothing, but logic would be:
        // glm::mat4 mvp = viewProjection * getGlobalTransform();
        // Render Self...
        
        // Render Children
        for (auto& child : m_children) {
            child->render(viewProjection);
        }
    }

    void SceneNode::markDirty() {
        if (!m_dirty) {
            m_dirty = true;
            for (auto& child : m_children) {
                child->markDirty();
            }
        }
    }

    void SceneNode::updateGlobalTransform() {
        if (m_dirty) {
            glm::mat4 local = glm::mat4(1.0f);
            local = glm::translate(local, glm::vec3(m_position, 0.0f));
            local = glm::rotate(local, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            local = glm::scale(local, glm::vec3(m_scale, 1.0f));

            if (auto parent = m_parent.lock()) {
                m_globalTransform = parent->getGlobalTransform() * local;
            } else {
                m_globalTransform = local;
            }
            m_dirty = false;
        }
    }

    const glm::mat4& SceneNode::getGlobalTransform() {
        updateGlobalTransform();
        return m_globalTransform;
    }

}
