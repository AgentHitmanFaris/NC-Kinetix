#include "nc/Engine.h"
#include "nc/scene/SceneNode.h"
#include "nc/scene/TextNode.h"
#include "nc/render/Camera2D.h"
#include "nc/render/TextRenderer.h"
#include "nc/render/SpriteRenderer.h"
#include "nc/scene/SpriteNode.h"
#include "nc/animation/Timeline.h"
#include "nc/data/ProjectLoader.h"
#include "nc/audio/AudioEngine.h"

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

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
        
        // Initialize GLAD
        if (!gladLoadGL()) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return;
        }
        
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        
        // Initialize Systems
        m_camera = std::make_unique<render::Camera2D>(static_cast<float>(m_width), static_cast<float>(m_height));
        m_textRenderer = std::make_unique<render::TextRenderer>();
        m_textRenderer->init();

        m_spriteRenderer = std::make_unique<render::SpriteRenderer>();
        m_spriteRenderer->init();
        scene::SpriteNode::setRenderer(m_spriteRenderer.get());
        
        m_rootNode = std::make_shared<scene::SceneNode>("Root");
        m_timeline = std::make_unique<animation::Timeline>();
        m_audioEngine = std::make_unique<audio::AudioEngine>();
        if (!m_audioEngine->init()) {
            std::cerr << "Warning: AudioEngine failed to initialize" << std::endl;
        }

        m_projectLoader = std::make_unique<data::ProjectLoader>();
        
        // Set up TextNode renderer
        scene::TextNode::setTextRenderer(m_textRenderer.get());
        
        std::cout << "NC-Kinetix Engine Initialized." << std::endl;
    }

    void Engine::loadProject(const std::string& jsonPath) {
        std::cout << "Loading project: " << jsonPath << std::endl;
        
        if (m_projectLoader->loadFromFile(jsonPath)) {
            // Load fonts
            m_projectLoader->loadFonts(m_textRenderer.get());
            
            // Build Scene Graph (Now that fonts are loaded & renderer is ready)
            m_projectLoader->buildSceneGraph(m_textRenderer.get());

            // Get scene graph and timeline
            m_rootNode = m_projectLoader->getRootNode();
            
            // Copy timeline data (ProjectLoader returns shared_ptr, we store unique_ptr)
            auto timelineShared = m_projectLoader->getTimeline();
            if (timelineShared) {
                m_timeline = std::make_unique<animation::Timeline>(*timelineShared);
            }
            
            m_nodeMap = m_projectLoader->getNodeMap();
            
            // Start timeline and audio
            if (m_timeline) {
                 // Check if project has audio
                 std::string audioPath = m_projectLoader->getAudioPath();
                 if (!audioPath.empty()) {
                     std::cout << "Loading audio: " << audioPath << std::endl;
                     m_audioEngine->playSound(audioPath);
                     m_timeline->play();
                 } else {
                     m_timeline->play();
                 }
            }
            
            std::cout << "Project loaded successfully" << std::endl;
        } else {
            std::cerr << "Failed to load project" << std::endl;
        }
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
        // Update timeline
        if (m_timeline) {
            float currentTime = 0.0f;
            
            // If we have audio playing, let it drive the clock
            if (m_audioEngine && m_audioEngine->isPlaying()) {
                currentTime = m_audioEngine->getTime();
                m_timeline->setCurrentTime(currentTime);
            } else {
                // otherwise accumulate dt
                m_timeline->update(dt);
            }

            m_timeline->apply(m_nodeMap, m_camera.get());
        }
        
        // Update scene graph
        if (m_rootNode) {
            m_rootNode->update(dt);
        }
    }

    void Engine::render() {
        // Clear screen to a dark background
        glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Get View-Projection matrix from the camera
        auto viewProj = m_camera->getViewProjectionMatrix();
        
        // Render Scene
        if (m_rootNode) {
            m_rootNode->render(viewProj);
        }
    }

}
