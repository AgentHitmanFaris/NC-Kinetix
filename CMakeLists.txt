cmake_minimum_required(VERSION 3.16)
project(NC-Kinetix VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# --- Dependencies ---

# 1. GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw)

# 2. GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glm)

# 3. nlohmann/json
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.2
)
FetchContent_MakeAvailable(json)

# 4. FreeType (Local build for portability)
set(FT_DISABLE_ZLIB ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://github.com/freetype/freetype.git
    GIT_TAG        VER-2-13-2
)
FetchContent_MakeAvailable(freetype)

# --- Source Files ---

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/nc/*.h")

# --- Executable ---

add_executable(nc-kinetix ${SOURCES} ${HEADERS})

# Static linking for portability (MinGW)
if(WIN32 AND NOT MSVC)
    target_link_options(nc-kinetix PRIVATE -static-libgcc -static-libstdc++ -static)
endif()

target_include_directories(nc-kinetix PRIVATE 
    include
)

target_link_libraries(nc-kinetix PRIVATE
    glfw
    glm::glm
    nlohmann_json::nlohmann_json
    freetype
    opengl32
    ${CMAKE_DL_LIBS} # For dynamic linking (dlopen on linux)
)

# Standard math for GLM
if(UNIX AND NOT APPLE)
    target_link_libraries(nc-kinetix PRIVATE m)
endif()
