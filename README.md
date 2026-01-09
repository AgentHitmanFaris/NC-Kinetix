# NC-Kinetix

![License](https://img.shields.io/badge/license-MIT-blue.svg) ![Standard](https://img.shields.io/badge/C%2B%2B-20-blue.svg?logo=c%2B%2B) ![OpenGL](https://img.shields.io/badge/OpenGL-4.5%2B-green.svg) ![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)

**NC-Kinetix** is a high-performance **Kinetic Typography Engine** tailored for Windows. It renders complex, resolution-independent text animations by employing a "Static Canvas" architecture where a dynamic Virtual Camera maneuvers through the scene—mirroring professional motion graphics workflows found in tools like After Effects.

Designed to be fully data-driven, NC-Kinetix accepts JSON instructions to choreograph text movements, serving as the dedicated rendering backend for [NC-KTV](https://github.com/AgentHitmanFaris/NC-KTV). Check out `examples/demo_project.json` for a sample configuration.

---

## Features

- **Virtual Camera System**: Cinematic 2D camera control supporting smooth panning, zooming, and rolling across an infinite canvas.
- **Sprite & Image Support**: Import and animate PNG/JPG images alongside text elements seamlessly.
- **Advanced Audio Sync**: Precision timeline synchronization with audio tracks (`.mp3`, `.wav`) for rhythm-perfect motion graphics.
- **Unicode Text**: Full support for UTF-8 languages and symbols.
- **Per-Character Animation**: toggle `split_text: true` to automatically break strings into individual nodes for granular control.
- **Static Canvas Architecture**: A robust Scene Graph hierarchy that maintains relative positioning (e.g., letters -> words -> sentences), essential for kinetic typography.
- **SDF Text Rendering**: Utilizes **Signed Distance Fields** (FreeType) to ensure text remains razor-sharp at any zoom level, from macro shots to wide-angle views.
- **Data-Driven Animation**: Animations are defined via portable JSON sequences, supporting complex timeline manipulation and precise audio synchronization.
- **High Performance**: Built on **C++20** and **Modern OpenGL (4.5+)**, optimized for maximum framerate and rendering efficiency.

## Technology Stack

| Component | Technology | Description |
| :--- | :--- | :--- |
| **Core** | **C++20** | Modern C++ features for performance and safety. |
| **Graphics** | **OpenGL 4.5+** | Hardware-accelerated rendering pipeline. |
| **Windowing** | **GLFW** | Cross-platform window context and input handling. |
| **Math** | **GLM** | OpenGL Mathematics library for vector/matrix operations. |
| **Typography** | **FreeType** | Glyph loading and SDF generation. |
| **Serialization** | **nlohmann/json** | Modern JSON parsing for animation data. |
| **Build System** | **CMake** | Cross-platform build configuration. |

## Getting Started

### Prerequisites

Ensure you have the following installed on your development machine:

*   **CMake** (3.16 or higher)
*   **C++ Compiler** with C++20 support (e.g., MinGW-w64, MSVC)
*   **Git**

### Installation

1.  **Clone the Repository**
    ```bash
    git clone https://github.com/AgentHitmanFaris/NC-Kinetix.git
    cd NC-Kinetix
    ```

2.  **Configure the Project**
    Create a build directory and generate the build files using CMake.
    ```bash
    mkdir build
    cd build
    cmake ..
    ```

3.  **Build the Executable**
    Compile the project. This will automatically fetch and build all dependencies (GLFW, GLM, FreeType, etc.).
    ```bash
    cmake --build . --config Release
    ```

## Contributing

Contributions are welcome. Please follow these steps:
1.  Fork the project.
2.  Create your feature branch (`git checkout -b feature/AmazingFeature`).
3.  Commit your changes (`git commit -m 'Add some AmazingFeature'`).
4.  Push to the branch (`git push origin feature/AmazingFeature`).
5.  Open a Pull Request.

## License

This project is licensed under the MIT License - see the `LICENSE` file for details.
