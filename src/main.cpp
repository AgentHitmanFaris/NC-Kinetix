#include "nc/Engine.h"

int main(int argc, char* argv[]) {
    nc::Engine engine(1280, 720, "NC-Kinetix Engine");
    engine.init();
    
    std::string projectPath = "demo_project.json";
    if (argc > 1) {
        projectPath = argv[1];
    }
    
    engine.loadProject(projectPath);
    
    engine.run();
    
    return 0;
}
