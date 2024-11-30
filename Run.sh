#!/bin/bash

# Clear the terminal screen
clear

echo "Building docker environment..."
docker pull randomdude/gcc-cross-x86_64-elf
docker build -t myos-buildenv ./buildenv

if [ $? -ne 0 ]; then
    echo
    echo "Docker build failed!"
    echo
    read -n 1 -s -r -p "Press any key to exit..."
    exit 1
fi

clear
echo
echo "Building!"
echo

docker run --rm -it -v "$(pwd)":/root/env myos-buildenv make build-x86_64

if [ $? -ne 0 ]; then
    echo
    echo "Build failed!"
    echo
    read -n 1 -s -r -p "Press any key to exit..."
    exit 1
fi

echo
echo "Build Succeeded!"
echo
read -n 1 -s -r -p "Press any key to continue..."

clear
echo "Running..."
echo
echo "Make sure to close qemu before building again!"
echo

qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso
