#pragma once

#include "nc/math/Tween.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include <map>

namespace nc::scene {
    class SceneNode;
}

namespace nc::render {
    class Camera2D;
}

namespace nc::animation {

    enum class PropertyType {
        Position,
        Rotation,
        Scale,
        Color,
        Zoom,
        CameraPosition,
        CameraRotation,
        CameraZoom
    };

    struct Keyframe {
        float time;
        PropertyType property;
        std::string targetID;
        
        // Property values
        glm::vec2 vec2Value;
        glm::vec4 vec4Value;
        float floatValue;
        
        math::EasingType easing;
    };

    class Timeline {
    public:
        Timeline();
        
        void addKeyframe(const Keyframe& keyframe);
        void setCurrentTime(float time);
        float getCurrentTime() const { return m_currentTime; }
        
        void update(float dt);
        void apply(std::map<std::string, std::shared_ptr<scene::SceneNode>>& nodes, render::Camera2D* camera);
        
        void play();
        void pause();
        void stop();
        
        bool isPlaying() const { return m_playing; }
        float getDuration() const { return m_duration; }
        
    private:
        std::vector<Keyframe> m_keyframes;
        float m_currentTime;
        float m_duration;
        bool m_playing;
        
        // Find keyframes surrounding current time for interpolation
        void findKeyframes(const std::string& targetID, PropertyType property, 
                          Keyframe& start, Keyframe& end, float& alpha);
    };

}
