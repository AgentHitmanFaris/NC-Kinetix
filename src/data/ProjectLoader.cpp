#include "nc/data/ProjectLoader.h"
#include "nc/scene/SceneNode.h"
#include "nc/scene/TextNode.h"
#include "nc/scene/SpriteNode.h"
#include "nc/animation/Timeline.h"
#include "nc/render/TextRenderer.h"
#include "nc/render/Camera2D.h"
#include "nc/utils/StringUtils.h"
#include <fstream>
#include <iostream>

namespace nc::data {

    ProjectLoader::ProjectLoader() {
        m_rootNode = std::make_shared<scene::SceneNode>("Root");
        m_timeline = std::make_shared<animation::Timeline>();
    }

    bool ProjectLoader::loadFromFile(const std::string& jsonPath) {
        try {
            std::ifstream file(jsonPath);
            if (!file.is_open()) {
                std::cerr << "ERROR::ProjectLoader: Failed to open file: " << jsonPath << std::endl;
                return false;
            }
            
            m_data = nlohmann::json::parse(file);
            file.close();
            
            if (m_data.contains("audio")) {
                m_audioPath = m_data["audio"];
            } else if (m_data.contains("assets") && m_data["assets"].contains("audio")) {
                m_audioPath = m_data["assets"]["audio"];
            }

            // buildSceneGraph(); // <- Postponed until fonts are loaded
            buildTimeline();
            
            std::cout << "Loaded project: " << jsonPath << std::endl;
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "ERROR::ProjectLoader: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProjectLoader::loadFromJSON(const nlohmann::json& data) {
        try {
            m_data = data;
            
            // Parse audio path
            if (m_data.contains("audio")) {
                m_audioPath = m_data["audio"];
            } else if (m_data.contains("assets") && m_data["assets"].contains("audio")) {
                m_audioPath = m_data["assets"]["audio"];
            }

            // buildSceneGraph();
            buildTimeline();
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "ERROR::ProjectLoader: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProjectLoader::loadFonts(render::TextRenderer* textRenderer) {
        if (!m_data.contains("assets") || !m_data["assets"].contains("fonts")) {
            std::cerr << "WARNING::ProjectLoader: No fonts defined in project" << std::endl;
            return false;
        }
        
        for (const auto& font : m_data["assets"]["fonts"]) {
            std::string fontID = font["id"];
            std::string fontPath = font["path"];
            
            m_fontPaths[fontID] = fontPath;
            
            // Load font (default size 48)
            if (!textRenderer->loadFont(fontPath, 48)) {
                std::cerr << "WARNING::ProjectLoader: Failed to load font: " << fontPath << std::endl;
            }
        }
        
        return true;
    }

    void ProjectLoader::buildSceneGraph(render::TextRenderer* textRenderer) {
        m_rootNode = std::make_shared<scene::SceneNode>("Root");
        m_nodeMap.clear();
        
        if (!m_data.contains("layout")) {
            return;
        }
        
        for (const auto& nodeData : m_data["layout"]) {
            auto node = createNodeFromJSON(nodeData, textRenderer);
            if (node) {
                m_rootNode->addChild(node);
            }
        }
    }

    std::shared_ptr<scene::SceneNode> ProjectLoader::createNodeFromJSON(const nlohmann::json& nodeData, render::TextRenderer* textRenderer) {
        std::string id = nodeData["id"];
        std::string type = nodeData["type"];
        
        std::shared_ptr<scene::SceneNode> node;
        
        if (type == "text") {
            auto textNode = std::make_shared<scene::TextNode>(id);
            std::string content = nodeData.value("content", "");
            textNode->setContent(content);
            
            if (nodeData.contains("font_id")) {
                textNode->setFontID(nodeData["font_id"]);
            }
            
            // Handle Text Splitting
            if (nodeData.contains("split_text") && nodeData["split_text"] == true && textRenderer) {
                // We will clear the parent content so it doesn't render the full string twice
                // But we keep it in memory if needed? No, standard is to not render parent.
                textNode->setContent(""); 
                
                float xOffset = 0.0f;
                // Simple splitting logic (UTF-8 aware)
                const char* ptr = content.c_str();
                const char* end = ptr + content.length();
                int idx = 0;

                while (ptr < end) {
                    nc::utils::Codepoint cp = nc::utils::decodeUTF8(ptr);
                    uint32_t c = cp.value;
                    
                    std::string charStr(ptr, cp.length);
                    
                    auto charNode = std::make_shared<scene::TextNode>(id + "_c" + std::to_string(idx));
                    charNode->setContent(charStr);
                    charNode->setFontID(textNode->getFontID());
                    
                    // Get advance
                    const render::Character* ch = textRenderer->getCharacter(c);
                    float advance = 0.0f;
                    if (ch) {
                        advance = (float)(ch->advance >> 6); // Convert 1/64 pixel to pixel
                    } else {
                         advance = 20.0f; // Fallback
                    }
                    
                    charNode->setPosition(glm::vec2(xOffset, 0.0f));
                    textNode->addChild(charNode);
                    
                    // Register child in map for animation targeting!
                    m_nodeMap[charNode->getName()] = charNode;
                    
                    xOffset += advance;
                    ptr += cp.length;
                    idx++;
                }
            }
            
            node = textNode;
        } else if (type == "sprite") {
            auto spriteNode = std::make_shared<scene::SpriteNode>(id);
            if (nodeData.contains("texture")) {
                spriteNode->setTexture(nodeData["texture"]);
            }
            node = spriteNode;
        } else if (type == "group") {
            node = std::make_shared<scene::SceneNode>(id);
        } else {
            node = std::make_shared<scene::SceneNode>(id);
        }
        
        // Set position
        if (nodeData.contains("position")) {
            float x = nodeData["position"].value("x", 0.0f);
            float y = nodeData["position"].value("y", 0.0f);
            node->setPosition(glm::vec2(x, y));
        }
        
        // Set rotation
        if (nodeData.contains("rotation")) {
            node->setRotation(nodeData["rotation"]);
        }
        
        // Set scale
        if (nodeData.contains("scale")) {
            if (nodeData["scale"].is_object()) {
                float x = nodeData["scale"].value("x", 1.0f);
                float y = nodeData["scale"].value("y", 1.0f);
                node->setScale(glm::vec2(x, y));
            } else {
                float s = nodeData["scale"];
                node->setScale(glm::vec2(s, s));
            }
        }
        
        // Store in map
        m_nodeMap[id] = node;
        
        // Process children
        if (nodeData.contains("children")) {
            for (const auto& childData : nodeData["children"]) {
                auto child = createNodeFromJSON(childData, textRenderer);
                if (child) {
                    node->addChild(child);
                }
            }
        }
        
        return node;
    }

    void ProjectLoader::buildTimeline() {
        m_timeline = std::make_shared<animation::Timeline>();
        
        if (!m_data.contains("timeline")) {
            return;
        }
        
        for (const auto& animData : m_data["timeline"]) {
            float timeStart = animData.value("time_start", 0.0f);
            float timeEnd = animData.value("time_end", 1.0f);
            std::string targetID = animData["target_id"];
            std::string type = animData["type"];
            std::string easingStr = animData.value("easing", "linear");
            
            math::EasingType easing = stringToEasing(easingStr);
            
            // Create start keyframe
            animation::Keyframe startKF;
            startKF.time = timeStart;
            startKF.targetID = targetID;
            startKF.easing = easing;
            
            // Create end keyframe
            animation::Keyframe endKF;
            endKF.time = timeEnd;
            endKF.targetID = targetID;
            endKF.easing = easing;
            
            if (type == "transform") {
                // Position
                if (animData.contains("from") && animData["from"].contains("x")) {
                    startKF.property = (targetID == "camera") ? animation::PropertyType::CameraPosition : animation::PropertyType::Position;
                    endKF.property = startKF.property;
                    
                    startKF.vec2Value = glm::vec2(
                        animData["from"].value("x", 0.0f),
                        animData["from"].value("y", 0.0f)
                    );
                    endKF.vec2Value = glm::vec2(
                        animData["to"].value("x", 0.0f),
                        animData["to"].value("y", 0.0f)
                    );
                    
                    m_timeline->addKeyframe(startKF);
                    m_timeline->addKeyframe(endKF);
                }
                
                // Rotation
                if (animData.contains("from") && animData["from"].contains("rotation")) {
                    animation::Keyframe rotStart = startKF;
                    animation::Keyframe rotEnd = endKF;
                    
                    rotStart.property = (targetID == "camera") ? animation::PropertyType::CameraRotation : animation::PropertyType::Rotation;
                    rotEnd.property = rotStart.property;
                    
                    rotStart.floatValue = animData["from"].value("rotation", 0.0f);
                    rotEnd.floatValue = animData["to"].value("rotation", 0.0f);
                    
                    m_timeline->addKeyframe(rotStart);
                    m_timeline->addKeyframe(rotEnd);
                }
                
                // Scale (Node only)
                if (animData.contains("from") && animData["from"].contains("scale")) {
                    if (targetID != "camera") {  // Camera doesn't have scale, it has zoom
                        animation::Keyframe scaleStart = startKF;
                        animation::Keyframe scaleEnd = endKF;
                        
                        scaleStart.property = animation::PropertyType::Scale;
                        scaleEnd.property = animation::PropertyType::Scale;
                        
                        // Handle uniform scale (number) or vec2 scale (object)
                        if (animData["from"]["scale"].is_object()) {
                            scaleStart.vec2Value = glm::vec2(
                                animData["from"]["scale"].value("x", 1.0f),
                                animData["from"]["scale"].value("y", 1.0f)
                            );
                        } else {
                            float s = animData["from"].value("scale", 1.0f);
                            scaleStart.vec2Value = glm::vec2(s, s);
                        }
                        
                        if (animData["to"]["scale"].is_object()) {
                            scaleEnd.vec2Value = glm::vec2(
                                animData["to"]["scale"].value("x", 1.0f),
                                animData["to"]["scale"].value("y", 1.0f)
                            );
                        } else {
                            float s = animData["to"].value("scale", 1.0f);
                            scaleEnd.vec2Value = glm::vec2(s, s);
                        }
                        
                        m_timeline->addKeyframe(scaleStart);
                        m_timeline->addKeyframe(scaleEnd);
                    }
                }

                // Zoom (camera only)
                if (animData.contains("from") && animData["from"].contains("zoom")) {
                    animation::Keyframe zoomStart = startKF;
                    animation::Keyframe zoomEnd = endKF;
                    
                    zoomStart.property = animation::PropertyType::CameraZoom;
                    zoomEnd.property = animation::PropertyType::CameraZoom;
                    
                    zoomStart.floatValue = animData["from"].value("zoom", 1.0f);
                    zoomEnd.floatValue = animData["to"].value("zoom", 1.0f);
                    
                    m_timeline->addKeyframe(zoomStart);
                    m_timeline->addKeyframe(zoomEnd);
                }
            } else if (type == "color") {
                startKF.property = animation::PropertyType::Color;
                endKF.property = animation::PropertyType::Color;
                
                startKF.vec4Value = glm::vec4(
                    animData["from"].value("r", 1.0f),
                    animData["from"].value("g", 1.0f),
                    animData["from"].value("b", 1.0f),
                    animData["from"].value("a", 1.0f)
                );
                endKF.vec4Value = glm::vec4(
                    animData["to"].value("r", 1.0f),
                    animData["to"].value("g", 1.0f),
                    animData["to"].value("b", 1.0f),
                    animData["to"].value("a", 1.0f)
                );
                
                m_timeline->addKeyframe(startKF);
                m_timeline->addKeyframe(endKF);
            }
        }
    }

    math::EasingType ProjectLoader::stringToEasing(const std::string& easingStr) {
        if (easingStr == "linear") return math::EasingType::Linear;
        if (easingStr == "easeInQuad") return math::EasingType::QuadIn;
        if (easingStr == "easeOutQuad") return math::EasingType::QuadOut;
        if (easingStr == "easeInOutQuad") return math::EasingType::QuadInOut;
        if (easingStr == "easeOutElastic") return math::EasingType::ElasticOut;
        if (easingStr == "easeInBack") return math::EasingType::BackIn;
        if (easingStr == "easeOutBack") return math::EasingType::BackOut;
        if (easingStr == "easeInOutBack") return math::EasingType::BackInOut;
        if (easingStr == "easeInBounce") return math::EasingType::BounceIn;
        if (easingStr == "easeOutBounce") return math::EasingType::BounceOut;
        if (easingStr == "easeInOutBounce") return math::EasingType::BounceInOut;
        return math::EasingType::Linear;
    }

}
