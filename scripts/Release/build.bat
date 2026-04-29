@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%..\..") do set "ROOT_DIR=%%~fI"
set "BUILD_DIR=%ROOT_DIR%\build"
set "MESA_ROOT=%ROOT_DIR%\..\mesa3d"

echo [1/3] Configuring Release build...
cmake -S "%ROOT_DIR%" -B "%BUILD_DIR%" -DEUI_MESA_ROOT="%MESA_ROOT%"
if errorlevel 1 goto :fail

echo [2/3] Building Release apps...
cmake --build "%BUILD_DIR%" --config Release
if errorlevel 1 goto :fail

if exist "%MESA_ROOT%\x64\opengl32.dll" (
    echo [3/3] Packaging VM builds with Mesa software OpenGL...
    cmake --build "%BUILD_DIR%" --config Release --target package_vm_all
    if errorlevel 1 goto :fail
) else (
    echo [3/3] Skipping VM package: Mesa not found at "%MESA_ROOT%"
    echo       Put mesa3d next to EUI-NEO or pass -DEUI_MESA_ROOT when configuring.
)

echo.
echo Build complete.
echo Release binaries: "%BUILD_DIR%\Release"
echo VM packages:      "%ROOT_DIR%\dist\vm"
exit /b 0

:fail
echo.
echo Build failed.
exit /b 1
