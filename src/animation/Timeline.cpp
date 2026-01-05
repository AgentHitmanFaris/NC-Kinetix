#include "nc/animation/Timeline.h"
#include "nc/scene/SceneNode.h"
#include "nc/scene/TextNode.h"
#include "nc/render/Camera2D.h"
#include <algorithm>

namespace nc::animation {

    Timeline::Timeline() : m_currentTime(0.0f), m_duration(0.0f), m_playing(false) {}

    void Timeline::addKeyframe(const Keyframe& keyframe) {
        m_keyframes.push_back(keyframe);
        
        // Update duration
        if (keyframe.time > m_duration) {
            m_duration = keyframe.time;
        }
        
        // Sort keyframes by time
        std::sort(m_keyframes.begin(), m_keyframes.end(), 
                 [](const Keyframe& a, const Keyframe& b) {
                     return a.time < b.time;
                 });
    }

    void Timeline::setCurrentTime(float time) {
        m_currentTime = time;
        if (m_currentTime > m_duration) {
            m_currentTime = m_duration;
        }
        if (m_currentTime < 0.0f) {
            m_currentTime = 0.0f;
        }
    }

    void Timeline::update(float dt) {
        if (m_playing) {
            m_currentTime += dt;
            if (m_currentTime >= m_duration) {
                m_currentTime = m_duration;
                m_playing = false;
            }
        }
    }

    void Timeline::apply(std::map<std::string, std::shared_ptr<scene::SceneNode>>& nodes, render::Camera2D* camera) {
        // Apply animations based on current time
        for (const auto& kf : m_keyframes) {
            if (kf.time > m_currentTime) {
                break; // Haven't reached this keyframe yet
            }
            
            // Find next keyframe for interpolation
            Keyframe start = kf;
            Keyframe end = kf;
            float alpha = 0.0f;
            
            findKeyframes(kf.targetID, kf.property, start, end, alpha);
            
            // Apply to target
            if (kf.targetID == "camera" && camera) {
                switch (kf.property) {
                    case PropertyType::CameraPosition:
                    case PropertyType::Position: {
                        glm::vec2 pos = glm::vec2(
                            math::Tween<float>::Interpolate(start.vec2Value.x, end.vec2Value.x, alpha, start.easing),
                            math::Tween<float>::Interpolate(start.vec2Value.y, end.vec2Value.y, alpha, start.easing)
                        );
                        camera->setPosition(pos);
                        break;
                    }
                    case PropertyType::CameraZoom:
                    case PropertyType::Zoom: {
                        float zoom = math::Tween<float>::Interpolate(start.floatValue, end.floatValue, alpha, start.easing);
                        camera->setZoom(zoom);
                        break;
                    }
                    case PropertyType::CameraRotation:
                    case PropertyType::Rotation: {
                        float rotation = math::Tween<float>::Interpolate(start.floatValue, end.floatValue, alpha, start.easing);
                        camera->setRotation(rotation);
                        break;
                    }
                    default:
                        break;
                }
            } else if (nodes.find(kf.targetID) != nodes.end()) {
                auto node = nodes[kf.targetID];
                
                switch (kf.property) {
                    case PropertyType::Position: {
                        glm::vec2 pos = glm::vec2(
                            math::Tween<float>::Interpolate(start.vec2Value.x, end.vec2Value.x, alpha, start.easing),
                            math::Tween<float>::Interpolate(start.vec2Value.y, end.vec2Value.y, alpha, start.easing)
                        );
                        node->setPosition(pos);
                        break;
                    }
                    case PropertyType::Rotation: {
                        float rotation = math::Tween<float>::Interpolate(start.floatValue, end.floatValue, alpha, start.easing);
                        node->setRotation(rotation);
                        break;
                    }
                    case PropertyType::Scale: {
                        glm::vec2 scale = glm::vec2(
                            math::Tween<float>::Interpolate(start.vec2Value.x, end.vec2Value.x, alpha, start.easing),
                            math::Tween<float>::Interpolate(start.vec2Value.y, end.vec2Value.y, alpha, start.easing)
                        );
                        node->setScale(scale);
                        break;
                    }
                    case PropertyType::Color: {
                        // Check if it's a TextNode
                        auto textNode = std::dynamic_pointer_cast<scene::TextNode>(node);
                        if (textNode) {
                            glm::vec4 color = glm::vec4(
                                math::Tween<float>::Interpolate(start.vec4Value.r, end.vec4Value.r, alpha, start.easing),
                                math::Tween<float>::Interpolate(start.vec4Value.g, end.vec4Value.g, alpha, start.easing),
                                math::Tween<float>::Interpolate(start.vec4Value.b, end.vec4Value.b, alpha, start.easing),
                                math::Tween<float>::Interpolate(start.vec4Value.a, end.vec4Value.a, alpha, start.easing)
                            );
                            textNode->setColor(color);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }

    void Timeline::findKeyframes(const std::string& targetID, PropertyType property,
                                Keyframe& start, Keyframe& end, float& alpha) {
        // Find the two keyframes surrounding current time for this property/target
        Keyframe* prevKF = nullptr;
        Keyframe* nextKF = nullptr;
        
        for (auto& kf : m_keyframes) {
            if (kf.targetID == targetID && kf.property == property) {
                if (kf.time <= m_currentTime) {
                    prevKF = &kf;
                } else if (kf.time > m_currentTime && nextKF == nullptr) {
                    nextKF = &kf;
                    break;
                }
            }
        }
        
        if (prevKF && nextKF) {
            start = *prevKF;
            end = *nextKF;
            float duration = end.time - start.time;
            if (duration > 0.0f) {
                alpha = (m_currentTime - start.time) / duration;
            }
        } else if (prevKF) {
            start = end = *prevKF;
            alpha = 1.0f;
        }
    }

    void Timeline::play() {
        m_playing = true;
    }

    void Timeline::pause() {
        m_playing = false;
    }

    void Timeline::stop() {
        m_playing = false;
        m_currentTime = 0.0f;
    }

}
