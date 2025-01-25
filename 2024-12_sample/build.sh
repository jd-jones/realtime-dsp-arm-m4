#!/bin/bash
set -xue


BUILD_DIR="./cmake-build-debug"


# Clean
mkdir -p "$BUILD_DIR"
rm -rf "$BUILD_DIR/"*
rm -f "./compile_commands.json"

# Build and also run cppcheck
cmake \
    -G "Unix Makefiles" \
    -B "$BUILD_DIR" \
    -DCMAKE_TOOLCHAIN_FILE="arm-none-eabi-gcc.cmake" \
    .
cmake --build "${BUILD_DIR}" -- -j 4
cmake --build "${BUILD_DIR}" --target=cppcheck-check
# cmake --build "${BUILD_DIR}" --target=program

ln -s "$BUILD_DIR/compile_commands.json" "./compile_commands.json"
