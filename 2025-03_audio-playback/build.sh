#!/bin/bash
set -xue


BUILD_DIR="./build"


# == BUILD ====================================================================
mkdir -p "$BUILD_DIR"

cmake --preset debug
cmake --build "${BUILD_DIR}"
# == BUILD ====================================================================

# == MAKE FILES FOR IDES ======================================================
# Link compile commands so vim can run gcc as an analyzer
ln -sf "$BUILD_DIR/compile_commands.json" "./compile_commands.json"

# Create a tag file so vim can navigate the source
ctags -R --exclude=".git" --exclude="docs" --exclude="test" --exclude="build" .

# Link the docs index.html so it's easier to find
ln -sf $(realpath "$BUILD_DIR/docs/doc_doxygen/html/index.html") "./docs"
# == MAKE FILES FOR IDES ======================================================
