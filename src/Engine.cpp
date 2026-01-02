#include "nc/Engine.h"
#include "nc/scene/SceneNode.h"
#include "nc/render/Camera2D.h"
#include "nc/render/TextRenderer.h"
#include "nc/math/Tween.h"

#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

namespace nc {

    Engine::Engine(int width, int height, const std::string& title)
        : m_width(width), m_height(height), m_title(title), m_window(nullptr), m_running(false)
    {
    }

    Engine::~Engine() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Engine::init() {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
        if (!m_window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(m_window);
        
        // Initialize Systems
        m_camera = std::make_unique<render::Camera2D>(static_cast<float>(m_width), static_cast<float>(m_height));
        m_textRenderer = std::make_unique<render::TextRenderer>();
        m_rootNode = std::make_shared<scene::SceneNode>("Root");
        
        // Setup a simple scene for demo
        auto textNode = std::make_shared<scene::SceneNode>("DemoText");
        textNode->setPosition({100.0f, 0.0f});
        m_rootNode->addChild(textNode);
        
        std::cout << "NC-Kinetix Engine Initialized." << std::endl;
    }

    void Engine::loadProject(const std::string& jsonPath) {
        // Architecture Stub: Load JSON using nlohmann/json
        std::cout << "Loading project: " << jsonPath << std::endl;
        
        // Example parsing (fake)
        /*
        std::ifstream f(jsonPath);
        nlohmann::json data = nlohmann::json::parse(f);
        */
       // Validate that we can use nlohmann::json
       nlohmann::json j;
       j["test"] = "ok";
    }

    void Engine::run() {
        m_running = true;
        
        float lastTime = 0.0f;
        
        while (!glfwWindowShouldClose(m_window) && m_running) {
            float time = static_cast<float>(glfwGetTime());
            float dt = time - lastTime;
            lastTime = time;

            processInput();
            update(dt);
            render();

            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
    }

    void Engine::processInput() {
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_window, true);
    }

    void Engine::update(float dt) {
        static float timeElapsed = 0.0f;
        timeElapsed += dt;
        
        // Continuous Heartbeat Animation (Sine Wave)
        // Zoom oscillates between 1.0x and 2.0x
        float zoom = 1.5f + 0.5f * std::sin(timeElapsed * 2.0f);
        m_camera->setZoom(zoom);
        
        // Gentle swaying rotation (-5 to 5 degrees)
        m_camera->setRotation(std::sin(timeElapsed) * 5.0f);
        
        m_rootNode->update(dt);
    }

    void Engine::render() {
        // Clear screen to a dark background
        glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Get View-Projection matrix from the camera
        auto viewProj = m_camera->getViewProjectionMatrix();
        
        // --- Temporary Verification Rendering ---
        // This uses legacy GL to verify the engine loop before the Modern Pipeline is implemented.
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(glm::value_ptr(viewProj));
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Draw a test square in the center
        glBegin(GL_QUADS);
            glColor3f(0.0f, 0.6f, 1.0f); // NC-Kinetix Cyan-Blue
            glVertex2f(-50.0f, -50.0f);
            glVertex2f( 50.0f, -50.0f);
            glVertex2f( 50.0f,  50.0f);
            glVertex2f(-50.0f,  50.0f);
        glEnd();
        // ----------------------------------------

        // Render Scene (Scene Graph propagation)
        m_rootNode->render(viewProj);
    }

}
