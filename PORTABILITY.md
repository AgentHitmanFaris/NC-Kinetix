# NC-Kinetix Portability Guide

## 100% Portable Build System

NC-Kinetix is designed to be **fully portable** with zero system-wide installations.

## How It Works

### 1. Local Dependency Management

All dependencies are downloaded and built within the project:

```
NC-Kinetix/
└── build/
    └── _deps/           ← All dependencies here!
        ├── glfw-src/
        ├── glm-src/
        ├── json-src/
        └── freetype-src/
```

### 2. No External Libraries

- ❌ No system library dependencies
- ❌ No installation to `C:\Program Files\`
- ❌ No registry entries
- ❌ No environment variables
- ✅ Everything in one folder

### 3. Self-Contained Executable

The built executable is **statically linked**:
- All code compiled into one `.exe` file
- No separate DLL files needed (except system OpenGL)
- OpenGL is provided by Windows/GPU drivers

## Deployment Checklist

To deploy NC-Kinetix to another computer:

### Package These Files:

```
📦 deployment-package/
├── nc-kinetix.exe           ← Main executable
├── shaders/                 ← Shader files
│   ├── text_sdf.vert
│   └── text_sdf.frag
├── assets/                  ← Fonts
│   ├── Roboto-Bold.ttf
│   └── Roboto-Regular.ttf
└── demo_project.json        ← Project file
```

### Requirements on Target Machine:

- **Windows 7+** with graphics drivers (for OpenGL)
- **Linux** with OpenGL support
- **No other software needed!**

## Moving the Project

### Scenario 1: Move to Different Folder

```powershell
# Just copy the entire folder!
xcopy /E /I D:\Document\NC-Kinetix E:\Projects\NC-Kinetix

# Or drag-and-drop the folder
# Everything still works!
```

### Scenario 2: Move to Different Computer

```powershell
# Copy entire project folder
# OR just copy build output:

# Minimum files needed:
NC-Kinetix/
└── build/
    ├── nc-kinetix.exe
    ├── shaders/
    ├── assets/
    └── *.json
```

### Scenario 3: USB Stick Deployment

1. Copy `build/` contents to USB stick
2. Plug into any Windows PC
3. Double-click `nc-kinetix.exe`
4. Works immediately!

## Behind the Scenes

### CMake Configuration

```cmake
# All deps downloaded to build/_deps/
FetchContent_Declare(glfw ...)
FetchContent_MakeAvailable(glfw)

# Static linking on Windows
target_link_options(nc-kinetix PRIVATE -static)

# Local install prefix
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")
```

### Dependency Isolation

Each dependency is built in its own folder:
- `build/_deps/glfw-src/` - Source code
- `build/_deps/glfw-build/` - Build artifacts
- No cross-contamination with other projects!

## Offline Capability

### First Build (Requires Internet)
```bash
cmake -S . -B build    # Downloads dependencies
cmake --build build    # Builds everything
```

### After First Build (No Internet Needed)
```bash
# Delete build folder
rm -rf build

# Rebuild uses cached sources in .git if available
cmake -S . -B build
cmake --build build
```

**Note**: CMake caches downloaded Git repos, so even after deleting `build/`, dependencies re-download quickly from local Git cache.

## Portability Verification

Run this checklist to verify portability:

```powershell
# 1. Build the project
cmake --build build --config Release

# 2. Test in original location
.\build\nc-kinetix.exe

# 3. Copy build to desktop
xcopy /E /I .\build $env:USERPROFILE\Desktop\nc-test

# 4. Run from new location
cd $env:USERPROFILE\Desktop\nc-test
.\nc-kinetix.exe

# 5. Success if it runs without errors!
```

## Common Questions

**Q: Can I move NC-Kinetix to another drive?**  
A: Yes! Just copy the entire folder. All dependencies come with it.

**Q: Do I need to install CMake on the target machine?**  
A: No. CMake is only needed to **build**. The executable runs standalone.

**Q: Will it work on a computer without Visual Studio?**  
A: Yes. The executable is self-contained and doesn't need VS runtime (static linking).

**Q: Can I delete the `build/_deps/` folder after building?**  
A: Yes, but keep the executable and assets. The source files in `_deps/` are only needed for rebuilding.

**Q: What about updates to dependencies?**  
A: Delete `build/`, run cmake again. It re-downloads from GitHub.

## Size Comparison

- **Source code**: ~100 KB
- **Dependencies (downloaded)**: ~20 MB in `build/_deps/`
- **Built executable**: ~3-5 MB (static)
- **Deployment package**: ~5-6 MB (exe + shaders + fonts)

## Summary

✅ **100% Portable**
- No installation wizard
- No admin rights needed  
- Works from any folder
- Copy = Deploy
- Plug-and-play on USB drive

🚀 **One Folder, Everything Included**
