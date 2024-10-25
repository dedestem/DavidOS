@echo off
title OS Runner - Building docker
cls
echo Building docker
docker pull randomdude/gcc-cross-x86_64-elf
docker rmi myos-buildenv -f
cls
echo Building!
echo.
echo.
docker run --rm -it -v "%cd%":/root/env myos-buildenv make build-x86_64

if %errorlevel% neq 0 (
    echo.
    echo.
    echo Build failed!
    echo.
    echo Press any key to exit!
    pause >nul
    exit /b %errorlevel%
)

echo.
echo.
echo Build Succeeded!
echo.
echo Press any key to run!
pause >nul
title OS Runner - Running
cls
echo Make sure to close qemu before building again!
echo.
qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso