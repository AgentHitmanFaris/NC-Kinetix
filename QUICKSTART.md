# Quick Start Guide - NC-Kinetix

## Using Local CMake (cmake_folder)

You have CMake installed locally in the `cmake_folder` directory. Here's how to build NC-Kinetix:

### 🚀 One-Click Build

Simply double-click:
```
build.bat
```

This will:
1. Use CMake from `cmake_folder/bin/cmake.exe`
2. Download all dependencies to `build/_deps/`
3. Compile NC-Kinetix
4. Create `build/nc-kinetix.exe`

### 📁 Scripts Available

| Script | Purpose |
|--------|---------|
| **build.bat** | Build the entire project |
| **clean.bat** | Delete build directory |
| **run.bat** | Run the compiled executable |

### 📝 Workflow

```powershell
# First time setup
1. Ensure cmake_folder/ contains CMake binaries
2. Double-click build.bat
3. Wait for dependencies to download (~5-10 min)
4. Add fonts to build/assets/

# Running
Double-click run.bat

# Rebuilding
Double-click clean.bat (removes build/)
Double-click build.bat (rebuilds everything)
```

### 📦 Directory Structure

```
NC-Kinetix/
├── cmake_folder/          ← Your local CMake (gitignored)
│   └── bin/
│       └── cmake.exe
├── build.bat              ← Click to build
├── clean.bat              ← Click to clean
├── run.bat                ← Click to run
├── build/                 ← Build output (gitignored)
│   ├── _deps/            ← All dependencies here
│   ├── nc-kinetix.exe    ← Compiled executable
│   ├── shaders/          ← Auto-copied
│   └── assets/           ← Add fonts here!
├── src/                   ← Source code
└── include/               ← Headers
```

### ✅ Before First Run

Add fonts to `build/assets/`:
- Download **Roboto** from [Google Fonts](https://fonts.google.com/specimen/Roboto)
- Place these files:
  - `build/assets/Roboto-Bold.ttf`
  - `build/assets/Roboto-Regular.ttf`

### 🐛 Troubleshooting

**"CMake not found at cmake_folder\bin\cmake.exe"**
- Ensure CMake is extracted to `cmake_folder/`
- Folder structure should be: `cmake_folder/bin/cmake.exe`
- Download CMake from: https://cmake.org/download/

**"MinGW not found" or "Generator not found"**
- You need MinGW compiler installed
- Download from: https://www.mingw-w64.org/
- Or use Visual Studio generator instead

**Build fails**
1. Run `clean.bat`
2. Run `build.bat` again
3. Check that you have internet for first build

### 🎯 That's It!

Everything is self-contained in the `NC-Kinetix` folder. No system-wide installations needed!
