#pragma once

#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace nc::scene {

    class SceneNode : public std::enable_shared_from_this<SceneNode> {
    public:
        using Ptr = std::shared_ptr<SceneNode>;
        using WeakPtr = std::weak_ptr<SceneNode>;

        SceneNode(const std::string& name = "Node");
        virtual ~SceneNode() = default;

        // Transform manipulation
        void setPosition(const glm::vec2& pos);
        void setRotation(float angleDegrees);
        void setScale(const glm::vec2& scale);

        glm::vec2 getPosition() const;
        float getRotation() const;
        glm::vec2 getScale() const;

        // Hierarchy
        void addChild(Ptr child);
        void removeChild(Ptr child);
        const std::vector<Ptr>& getChildren() const;
        Ptr getParent() const;

        // Core Loop
        void update(float dt);
        // In a real engine, render would take a Renderer& or CommandBuffer&
        virtual void render(const glm::mat4& viewProjection);

        // Transform Calculation
        const glm::mat4& getGlobalTransform();

    protected:
        std::string m_name;
        
        // Local Transform Components
        glm::vec2 m_position{0.0f, 0.0f};
        float m_rotation{0.0f}; // Degrees
        glm::vec2 m_scale{1.0f, 1.0f};

        // Hierarchy
        WeakPtr m_parent; // Weak pointer to avoid circular references
        std::vector<Ptr> m_children;

        // Cached Global Transform
        glm::mat4 m_globalTransform{1.0f};
        bool m_dirty{true}; // If true, recompute global transform

        void markDirty();
        void updateGlobalTransform();
    };

}
