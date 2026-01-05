@echo off
REM Run NC-Kinetix executable
if exist build\nc-kinetix.exe (
    echo Starting NC-Kinetix...
    cd build
    nc-kinetix.exe
) else (
    echo ERROR: Executable not found!
    echo Please run build.bat first.
    pause
)
