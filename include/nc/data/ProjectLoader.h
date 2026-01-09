#pragma once

#include <string>
#include <memory>
#include <map>
#include <nlohmann/json.hpp>

namespace nc::scene {
    class SceneNode;
    class TextNode;
}

namespace nc::animation {
    class Timeline;
}

namespace nc::render {
    class TextRenderer;
    class Camera2D;
}

namespace nc::math {
    enum class EasingType;
}

namespace nc::data {

    class ProjectLoader {
    public:
        ProjectLoader();
        
        bool loadFromFile(const std::string& jsonPath);
        bool loadFromJSON(const nlohmann::json& data);
        
        std::shared_ptr<scene::SceneNode> getRootNode() const { return m_rootNode; }
        std::shared_ptr<animation::Timeline> getTimeline() const { return m_timeline; }
        
        const std::map<std::string, std::shared_ptr<scene::SceneNode>>& getNodeMap() const { return m_nodeMap; }
        
        // Font management
        bool loadFonts(render::TextRenderer* textRenderer);
        
        std::string getAudioPath() const { return m_audioPath; }

        void buildSceneGraph(render::TextRenderer* textRenderer = nullptr);

    private:
        nlohmann::json m_data;
        std::string m_audioPath;
        std::shared_ptr<scene::SceneNode> m_rootNode;
        std::shared_ptr<animation::Timeline> m_timeline;
        std::map<std::string, std::shared_ptr<scene::SceneNode>> m_nodeMap;
        std::map<std::string, std::string> m_fontPaths;
        
        void buildTimeline();
        
        std::shared_ptr<scene::SceneNode> createNodeFromJSON(const nlohmann::json& nodeData, render::TextRenderer* textRenderer);
        math::EasingType stringToEasing(const std::string& easingStr);
    };

}
