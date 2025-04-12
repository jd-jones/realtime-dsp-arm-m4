#!/bin/bash
set -xue


BUILD_DIR="./build"
project_name="2025-03_audio-playback"


STM32_Programmer_CLI \
    --connect port=SWD \
    --write "$BUILD_DIR"/"$project_name".elf \
    --verify \
    -rst


# To debug:
#   1. Start up st-util in another pane
#   2. Run arm-none-eabi-gdb $BUILD_DIR/$project_name.elf
#   3. Inside gbd: target extended-remote :4242 (or whatever port is found by st-util)
