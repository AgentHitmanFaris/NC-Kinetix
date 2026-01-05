@echo off
REM NC-Kinetix Build Script (Using Local CMake and MinGW)
REM This script uses CMake from cmake_folder and MinGW from tools

echo ==========================================
echo NC-Kinetix Portable Build
echo ==========================================
echo.

REM Set paths
set CMAKE_PATH=%~dp0cmake_folder\bin\cmake.exe
set MINGW_PATH=%~dp0tools\mingw64\bin

REM Check if CMake exists
if not exist "%CMAKE_PATH%" (
    echo ERROR: CMake not found at %CMAKE_PATH%
    echo Please ensure CMake is extracted to cmake_folder/
    pause
    exit /b 1
)

REM Check if MinGW exists
if not exist "%MINGW_PATH%\gcc.exe" (
    echo ERROR: MinGW not found at %MINGW_PATH%
    echo Please ensure MinGW is extracted to tools/mingw64/
    pause
    exit /b 1
)

echo Using CMake: %CMAKE_PATH%
echo Using MinGW: %MINGW_PATH%
echo.

REM Add MinGW to PATH
set PATH=%MINGW_PATH%;%PATH%

REM Configure
echo [Step 1/3] Configuring project...
"%CMAKE_PATH%" -S . -B build -G "MinGW Makefiles"
if %errorlevel% neq 0 (
    echo ERROR: Configuration failed
    pause
    exit /b 1
)

echo.
echo [Step 2/3] Building project...
"%CMAKE_PATH%" --build build --config Release
if %errorlevel% neq 0 (
    echo ERROR: Build failed
    pause
    exit /b 1
)

echo.
echo ==========================================
echo BUILD SUCCESSFUL!
echo ==========================================
echo Executable: build\nc-kinetix.exe
echo.
echo [Step 3/3] Next steps:
echo 1. Add fonts to build\assets\
echo    - Roboto-Bold.ttf
echo    - Roboto-Regular.ttf
echo 2. Run: build\nc-kinetix.exe
echo ==========================================
pause
