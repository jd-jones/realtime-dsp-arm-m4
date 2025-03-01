cmake_minimum_required(VERSION 3.15.0)
enable_language(C ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(cppcheck.cmake)


project(2025-02_audio-playback)
set(EXECUTABLE ${PROJECT_NAME})
add_executable(${EXECUTABLE}
    Core/Src/main.c
    Core/Src/syscalls.c
    Core/Src/sysmem.c
    Core/Src/system_stm32f4xx.c
    Core/Src/gpio.c
    Core/Src/gpio.h
    Core/Startup/startup_stm32f407vgtx.s
)
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

target_include_directories(${EXECUTABLE} PRIVATE
    Core/Inc
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/CMSIS/Include
)
target_compile_definitions(${EXECUTABLE} PRIVATE
    -DSTM32
    -DSTM32F4
    -DSTM32F407xx
    -D__FPU_PRESENT=1U
    -DARM_MATH_CM4
)
set(DEVICE_OPTIONS
	-mcpu=cortex-m4
	-mthumb
	-mfpu=fpv4-sp-d16
	-mfloat-abi=hard
)
target_compile_options(${EXECUTABLE} PRIVATE
    -g      # debug info
    -O0     # O1 O2
	-Wall
    -Wpedantic
    -Wno-unused-parameter
    ${DEVICE_OPTIONS}
	-fdata-sections
	-ffunction-sections
    -fstack-usage
    # -fcyclomatic-complexity
)
target_link_options(${EXECUTABLE} PRIVATE
    -T${CMAKE_SOURCE_DIR}/STM32F407VGTX_FLASH.ld
    --specs=nosys.specs
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections
    -static
    --specs=nano.specs
    -u _printf_float
    -Wl,--start-group
    -lc
    -lm
    -Wl,--end-group
    -Wl,--print-memory-usage
    ${DEVICE_OPTIONS}
)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND arm-none-eabi-size ${EXECUTABLE}${CMAKE_EXECUTABLE_SUFFIX})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE}${CMAKE_EXECUTABLE_SUFFIX} ${PROJECT_NAME}.hex
    COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE}${CMAKE_EXECUTABLE_SUFFIX} ${PROJECT_NAME}.bin)

# Programming target
find_program(STM32_PROGRAMMER_CLI STM32_Programmer_CLI)
if(STM32_PROGRAMMER_CLI)
    message(STATUS "STM32_Programmer_CLI was found, 'program' target can be used")
    add_custom_target(program DEPENDS ${PROJECT_NAME} COMMAND ${STM32_PROGRAMMER_CLI} --connect port=SWD --write ${EXECUTABLE}${CMAKE_EXECUTABLE_SUFFIX} --verify -rst)
else()
    message(STATUS "STM32_Programmer_CLI was not found, program target is inaccessible")
endif()
