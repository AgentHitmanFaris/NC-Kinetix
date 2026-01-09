# Changelog

## [0.3.0] - 2026-01-09

### Added - Visual Fidelity
- **Sprite Support**:
  - `SpriteNode` for rendering 2D images
  - `SpriteRenderer` with batching support
  - `Texture` resource management using `stb_image`
- **Unicode Support**:
  - `StringUtils::decodeUTF8` for UTF-8 string parsing
  - `TextNode` support for multi-byte characters
  - `TextRenderer` updated to handle extended character sets

## [0.2.0] - 2026-01-09

### Added - Core Mechanics
- **Audio Integration**:
  - `AudioEngine` using `miniaudio`
  - Timeline synchronization with audio playback
  - Audio file loading from project JSON
- **Advanced Animation**:
  - **Text Splitting**: Automatically split text into individual character nodes for per-character animation
  - **New Easing Functions**:
    - `BackIn`, `BackOut`, `BackInOut`
    - `BounceIn`, `BounceOut`, `BounceInOut`
    - `ElasticIn`, `ElasticOut`, `ElasticInOut`

## [0.1.0] - 2026-01-05

### Added - Full Implementation

#### Core Infrastructure
- **GLAD OpenGL Loader**: Custom minimal GLAD implementation for OpenGL 3.3+ function loading
- **Shader System**: Complete shader compilation, linking, and uniform management
  - `Shader` class with error checking and validation
  - Embedded shader support for convenience

#### Rendering Pipeline
- **Enhanced TextRenderer**: 
  - FreeType integration with 128 ASCII character atlas
  - Batched quad rendering for optimal performance
  - OpenGL VAO/VBO management
  - Texture atlas generation from FreeType glyphs
  - Blending support for alpha compositing

- **SDF Shaders**:
  - `text_sdf.vert`: Vertex shader with projection matrix support
  - `text_sdf.frag`: Fragment shader with distance field rendering
  - Smooth anti-aliasing at any zoom level

#### Scene Graph
- **TextNode**: Specialized scene node for text rendering
  - Content, color, and scale properties
  - Font ID association
  - Global transform rendering
  - Static TextRenderer dependency injection

#### Animation System
- **Timeline**: Keyframe-based animation manager
  - Support for multiple property types (Position, Rotation, Scale, Color, Zoom)
  - Camera animation capabilities
  - Easing function integration (Linear, QuadIn/Out, QuadInOut, ElasticOut)
  - Automatic keyframe sorting and interpolation
  - Playback controls (play, pause, stop)

#### Data Loading
- **ProjectLoader**: JSON project file parser
  - Scene graph construction from layout definitions
  - Timeline building from animation sequences
  - Font asset management
  - Hierarchical node creation with children support
  - Easing function string-to-enum conversion

#### Engine Integration
- Complete integration of all systems
- GLAD initialization after OpenGL context creation
- Automatic system lifecycle management
- Project loading and timeline playback
- Node map for animation target resolution

### Modified
- `Engine.h/cpp`: Updated to integrate all new systems
- `TextRenderer.h/cpp`: Completely rewritten with modern OpenGL
- `CMakeLists.txt`: Added C language support, GLAD sources, shader copying

### Technical Details
- **Language**: C++20 with concepts and smart pointers
- **OpenGL**: Core profile 3.3+
- **Dependencies**: GLFW 3.3.8, GLM 0.9.9.8, FreeType 2.13.2, nlohmann/json 3.11.2
- **Architecture**: Data-driven with JSON configuration
- **Rendering**: Modern pipeline with shaders, no legacy fixed-function GL

### Documentation
- `BUILD.md`: Comprehensive build instructions for Windows and Linux
- Implementation plan and walkthrough artifacts
