# NC-Kinetix Architecture Document

## Overview
NC-Kinetix is a high-performance Kinetic Typography Engine for Windows, designed to render complex text animations on a "Static Canvas" where the Virtual Camera moves dynamically. It is data-driven, receiving instructions via JSON (e.g., from NC-KTV).

## Core Systems

### 1. Scene Graph (The "Static Canvas")
Instead of a flat ECS, we utilize a **Scene Graph** hierarchy. This is crucial for kinetic typography where letters often move relative to words, and words relative to sentences or "anchor" objects (like the user's "soda can" example).

- **`SceneNode`**: The base class for all entities.
    - Properties: `LocalTransform` (Position, Rotation, Scale), `GlobalTransform` (Cached), `Parent`, `Children`.
    - Methods: `update(dt)`, `render(renderer)`.
    - **Dirty Flag Optimization**: Transforms are only re-calculated when changed.

### 2. Virtual Camera (`Camera2D`)
Handles the view of the global canvas.
- **Functionality**: Manages Position, Zoom (Scale), and Rotation (Roll).
- **Matrices**: Provides `GetViewMatrix()` and `GetProjectionMatrix()` for the shader pipeline.
- **Smooth Follow**: Can targeted to track a specific `SceneNode`.

### 3. Tweening Engine
A decoupled, header-only system for interpolation.
- **Design**: Templated functions avoiding heavy class overhead.
- **Easing**: Supports Linear, Quadratic, Elastic, Back, Bounce.
- **Usage**: Used by the Timeline Manager to interpolate properties (Camera Pos, Text Alpha, etc.) over time.

### 4. Rendering Pipeline
- **Modern OpenGL (4.5+)**.
- **`TextRenderer`**:
    - Uses **FreeType** to load glyphs.
    - Generates **Signed Distance Fields (SDF)** for resolution-independent text (crisp zoom).
    - Batches character quads to minimize draw calls.
- **Shaders**: Custom shaders for SDF rendering and global effects (Bloom, Motion Blur).

### 5. Timeline Manager
Manages the playback state.
- **Keyframes**: Interpolates values based on the current timestamp.
- **Synchronization**: Syncs visual playback with audio timestamps provided in the JSON.

## Data Flow (JSON)
1. **Import**: JSON is loaded -> Parsed into a `Sequence` object.
2. **Build**: The Scene Graph is constructed. Static text nodes are placed at their `global_layout` positions.
3. **Update Loop**:
    - `TimelineManager` updates current time.
    - `AnimationSystem` applies tweens to `SceneNode` transforms based on active effects.
    - `Camera` updates its transform (potentially tweened).
    - `SceneNode::update()` propagates transforms down the tree.
4. **Render Loop**:
    - `Camera` sets View/Proj uniforms.
    - `SceneNode::render()` submits draw commands to the `BatchRenderer`.

## Technology Stack
- **Language**: C++20 (Concepts, Smart Pointers, Modules if supported).
- **Build**: CMake.
- **Windowing**: GLFW.
- **Math**: GLM.
- **Font**: FreeType.
