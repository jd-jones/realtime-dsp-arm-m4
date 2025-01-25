#!/bin/bash
set -xue


BUILD_DIR="./cmake-build-debug"


STM32_Programmer_CLI \
    --connect port=SWD \
    --write "$BUILD_DIR"/2024-12_sample.elf \
    --verify \
    -rst
