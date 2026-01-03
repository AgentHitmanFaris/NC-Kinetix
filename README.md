# NC-Kinetix

NC-Kinetix is a high-performance **Kinetic Typography Engine** tailored for Windows. It renders complex, resolution-independent text animations by employing a "Static Canvas" architecture where a dynamic Virtual Camera moves through the scene, similar to professional motion graphics workflows.

It is designed to be data-driven, accepting JSON instructions to choreograph text movements, making it an ideal backend for tools like [NC-KTV](https://github.com/AgentHitmanFaris/NC-KTV).

## Features

*   **Scene Graph Architecture**: Organized hierarchy for robust parent-child relationships, essential for kinetic typography (e.g., letters relative to words).
*   **Virtual Camera System**: A powerful 2D camera supporting smooth pan, zoom, and roll, allowing for cinematic transitions across the infinite canvas.
*   **SDF Text Rendering**: Uses Signed Distance Fields to ensure text remains crisp and sharp at any zoom level.
*   **Data-Driven Animation**: Fully controllable via JSON sequences, supporting complex timelines and audio synchronization.
*   **Core C++ Performance**: Built with C++20 and Modern OpenGL (4.5+) for maximum rendering speed and efficiency.

## Technology Stack

*   **Language**: C++20
*   **Graphics API**: OpenGL 4.5+
*   **Windowing/Input**: GLFW
*   **Math**: GLM
*   **Font Engine**: FreeType
*   **Serialization**: nlohmann/json
*   **Build System**: CMake

## Build Instructions

### Prerequisites
*   **CMake** (3.16 or higher)
*   **C++ Compiler** with C++20 support (e.g., MinGW-w64, MSVC)
*   **Git**

### Building from Source

1.  Clone the repository:
    ```bash
    git clone https://github.com/AgentHitmanFaris/NC-Kinetix.git
    cd NC-Kinetix
    ```

2.  Configure the project with CMake:
    ```bash
    mkdir build
    cd build
    cmake ..
    ```

3.  Build the executable:
    ```bash
    cmake --build .
    ```

    *Note: Dependencies (GLFW, GLM, FreeType, json) are automatically fetched and built via CMake's `FetchContent`.*
