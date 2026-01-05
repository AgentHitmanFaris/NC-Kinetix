#include "nc/Engine.h"

int main() {
    nc::Engine engine(1280, 720, "NC-Kinetix Engine");
    engine.init();
    
    // Simulate loading data
    // Simulate loading data
    engine.loadProject("demo_project.json");
    
    engine.run();
    
    return 0;
}
