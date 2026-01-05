# Building NC-Kinetix (Fully Portable)

## 🎯 Portable Build Overview

**All dependencies are downloaded and built within the project folder!**
- Dependencies: `build/_deps/` directory
- No system-wide installation needed
- Fully self-contained executable

## Prerequisites

1. **CMake** (3.16 or higher) - [Download](https://cmake.org/download/)
2. **C++ Compiler** with C++20 support:
   - **Windows**: MinGW-w64 or MSVC
   - **Linux**: GCC 10+ or Clang 11+
3. **Git** - For dependency fetching
4. **Internet connection** - First build only (to download dependencies)

## Build Instructions

### Windows (MinGW)

```powershell
# 1. Navigate to project directory
cd D:\Document\NC-Kinetix

# 2. Configure (downloads all deps to build/_deps/)
cmake -S . -B build -G "MinGW Makefiles"

# 3. Build (everything is in the project folder)
cmake --build build --config Release

# 4. Run
.\build\nc-kinetix.exe
```

### Windows (Visual Studio)

```powershell
# 1. Navigate to project
cd D:\Document\NC-Kinetix

# 2. Configure
cmake -S . -B build

# 3. Build
cmake --build build --config Release

# 4. Run
.\build\Release\nc-kinetix.exe
```

### Linux

```bash
# 1. Navigate to project
cd ~/NC-Kinetix

# 2. Configure
cmake -S . -B build

# 3. Build
cmake --build build --config Release

# 4. Run
./build/nc-kinetix
```

## 📁 Portable Directory Structure

After building, everything is self-contained:

```
NC-Kinetix/                    # Your project folder
├── build/                     # Build output (ALL dependencies here!)
│   ├── _deps/                # Downloaded dependencies
│   │   ├── glfw-src/        # GLFW source
│   │   ├── glm-src/         # GLM source
│   │   ├── json-src/        # nlohmann/json source
│   │   └── freetype-src/    # FreeType source
│   ├── nc-kinetix.exe       # Your executable
│   ├── shaders/             # Auto-copied shaders
│   ├── assets/              # Auto-created assets folder
│   └── demo_project.json    # Auto-copied project file
├── install/                  # Local install directory (if needed)
├── src/                      # Source code
├── include/                  # Headers
└── shaders/                  # Original shaders
```

## 🚀 Portable Deployment

To deploy your build to another machine:

1. **Copy the entire `build/` directory** (or just the necessary files):
   ```
   build/
   ├── nc-kinetix.exe
   ├── shaders/
   ├── assets/
   └── demo_project.json
   ```

2. **Add fonts** to the `assets/` folder:
   - Download Roboto from [Google Fonts](https://fonts.google.com/specimen/Roboto)
   - Place `Roboto-Bold.ttf` and `Roboto-Regular.ttf` in `build/assets/`

3. **Run** the executable - no installation required!

## 📦 What Gets Downloaded Locally?

On first build, CMake downloads to `build/_deps/`:
- **GLFW** 3.3.8 (~2 MB)
- **GLM** 0.9.9.8 (~4 MB)
- **FreeType** 2.13.2 (~12 MB)
- **nlohmann/json** 3.11.2 (~1 MB)

**Total**: ~20 MB, all in your project folder!

## 🔧 Clean Build

To rebuild from scratch (removes all downloaded dependencies):

```bash
# Delete build directory
rm -rf build

# Rebuild
cmake -S . -B build
cmake --build build --config Release
```

## ✅ Portability Features

- ✅ All dependencies in `build/_deps/`
- ✅ Static linking (no DLL dependencies on Windows)
- ✅ Self-contained executable
- ✅ No registry entries
- ✅ No system-wide installation
- ✅ Copy build folder = working application
- ✅ Offline execution after first build

## Troubleshooting

### "CMake not found"
- Download from https://cmake.org/download/
- Add to PATH or use full path: `"C:\Program Files\CMake\bin\cmake.exe"`

### "Git not found during dependency download"
- Install Git from https://git-scm.com/
- CMake needs Git to download dependencies

### Build errors
1. Delete `build/` directory completely
2. Run cmake configuration again
3. All dependencies will be re-downloaded to `build/_deps/`

## 💡 Tips

- **First build takes longer** (~5-10 min) due to dependency downloads
- **Subsequent builds are fast** (~30 sec) as deps are cached
- **No internet needed** after first successful build
- **Safe to delete build/** - just rebuild and deps re-download
- **Move entire NC-Kinetix folder** to any location - still works!
