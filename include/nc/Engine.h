#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

struct GLFWwindow;

namespace nc {
    
    namespace scene {
        class SceneNode;
    }
    namespace render {
        class Camera2D;
        class TextRenderer;
    }
    namespace animation {
        class Timeline;
    }
    namespace data {
        class ProjectLoader;
    }

    class Engine {
    public:
        Engine(int width, int height, const std::string& title);
        ~Engine();

        void init();
        void run();
        
        // Simulates loading JSON data
        void loadProject(const std::string& jsonPath);

    private:
        void update(float dt);
        void render();
        void processInput();

        int m_width;
        int m_height;
        std::string m_title;
        GLFWwindow* m_window;

        std::shared_ptr<scene::SceneNode> m_rootNode;
        std::unique_ptr<render::Camera2D> m_camera;
        std::unique_ptr<render::TextRenderer> m_textRenderer;
        std::unique_ptr<animation::Timeline> m_timeline;
        std::unique_ptr<data::ProjectLoader> m_projectLoader;
        
        std::map<std::string, std::shared_ptr<scene::SceneNode>> m_nodeMap;
        
        bool m_running;
    };

}
