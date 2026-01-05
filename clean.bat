@echo off
REM Clean build script - removes build directory
echo Cleaning build directory...
if exist build (
    rmdir /s /q build
    echo Build directory removed.
) else (
    echo No build directory found.
)
echo.
echo Run build.bat to rebuild the project.
pause
