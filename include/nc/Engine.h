#pragma once

#include <memory>
#include <string>
#include <vector>

struct GLFWwindow;

namespace nc {
    
    namespace scene {
        class SceneNode;
    }
    namespace render {
        class Camera2D;
        class TextRenderer;
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
        
        bool m_running;
    };

}
