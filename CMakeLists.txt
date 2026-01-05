cmake_minimum_required(VERSION 3.16)
project(NC-Kinetix VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================
# PORTABLE BUILD CONFIGURATION
# All dependencies will be downloaded and built
# in the build/_deps directory within this project
# ============================================

# Force all dependencies to be built locally (no system libs)
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)
set(FETCHCONTENT_QUIET FALSE)

# Install everything to a local 'install' directory
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Install path" FORCE)

include(FetchContent)

# --- Dependencies (All fetched locally) ---

# 1. GLFW - Downloaded to build/_deps/glfw-src
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(glfw)

# 2. GLM - Downloaded to build/_deps/glm-src
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(glm)

# 3. nlohmann/json - Downloaded to build/_deps/json-src
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)
set(JSON_Install OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(json)

# 4. FreeType - Downloaded to build/_deps/freetype-src
# Disable all optional dependencies for full portability
set(FT_DISABLE_ZLIB ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)
set(FT_REQUIRE_ZLIB OFF CACHE BOOL "" FORCE)
set(FT_REQUIRE_PNG OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://github.com/freetype/freetype.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(freetype)

# --- Source Files ---

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS "include/nc/*.h" "include/glad/*.h")

# --- Executable ---

add_executable(nc-kinetix ${SOURCES} ${HEADERS})

# Full static linking for maximum portability
if(WIN32)
    # Windows: Static link C/C++ runtime and other libraries
    if(MSVC)
        # MSVC: Use static runtime
        set_property(TARGET nc-kinetix PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    else()
        # MinGW: Static link everything
        target_link_options(nc-kinetix PRIVATE 
            -static-libgcc 
            -static-libstdc++ 
            -static
            -Wl,--no-undefined
        )
    endif()
endif()

target_include_directories(nc-kinetix PRIVATE 
    include
)

target_link_libraries(nc-kinetix PRIVATE
    glfw
    glm::glm
    nlohmann_json::nlohmann_json
    freetype
)

# Platform-specific system libraries
if(WIN32)
    target_link_libraries(nc-kinetix PRIVATE opengl32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(nc-kinetix PRIVATE GL m dl pthread)
elseif(APPLE)
    target_link_libraries(nc-kinetix PRIVATE "-framework OpenGL" "-framework Cocoa" "-framework IOKit")
endif()

# Copy runtime assets to build directory
add_custom_command(TARGET nc-kinetix POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:nc-kinetix>/shaders
    COMMENT "Copying shaders to build directory"
)

# Create assets directory in build folder
add_custom_command(TARGET nc-kinetix POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    $<TARGET_FILE_DIR:nc-kinetix>/assets
    COMMENT "Creating assets directory"
)

# Copy schema to build directory
# Copy schema to build directory
add_custom_command(TARGET nc-kinetix POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/examples/demo_project.json
    $<TARGET_FILE_DIR:nc-kinetix>/demo_project.json
    COMMENT "Copying project schema"
)

# ============================================
# PORTABLE BUILD SUMMARY
# ============================================
# All dependencies are in: ${CMAKE_BINARY_DIR}/_deps/
# Executable will be in: ${CMAKE_BINARY_DIR}/
# Install target will use: ${CMAKE_SOURCE_DIR}/install/
# No system-wide installation required!
# ============================================

message(STATUS "===========================================")
message(STATUS "NC-Kinetix - Portable Build Configuration")
message(STATUS "===========================================")
message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")
message(STATUS "Dependencies:    ${CMAKE_BINARY_DIR}/_deps/")
message(STATUS "Install prefix:  ${CMAKE_INSTALL_PREFIX}")
message(STATUS "===========================================")
