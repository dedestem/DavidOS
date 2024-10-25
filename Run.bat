@echo off
title OS Runner - Building docker
cls

echo Building docker environment...
docker pull randomdude/gcc-cross-x86_64-elf
docker build -t myos-buildenv ./buildenv

if %errorlevel% neq 0 (
    echo.
    echo Docker build failed!
    echo.
    echo Press any key to exit!
    pause >nul
    exit /b %errorlevel%
)

cls
echo.
echo Building!
echo.

docker run --rm -it -v "%cd%":/root/env myos-buildenv make build-x86_64

if %errorlevel% neq 0 (
    echo.
    echo Build failed!
    echo.
    echo Press any key to exit!
    pause >nul
    exit /b %errorlevel%
)

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
