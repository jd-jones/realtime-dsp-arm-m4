cmake_minimum_required(VERSION 3.15.0)
enable_language(C ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(cppcheck.cmake)


project(2024-12_sample)
set(EXECUTABLE ${PROJECT_NAME})
add_executable(${EXECUTABLE}
    Src/main.c
    Src/syscalls.c
    Src/sysmem.c
    Src/adc.c
    Src/uart.c
    Inc/adc.h
    Inc/uart.h
    Startup/startup_stm32f411retx.s
    Drivers/CMSIS/DSP/Source/FilteringFunctions/arm_fir_init_f32.c
    Drivers/CMSIS/DSP/Source/FilteringFunctions/arm_fir_f32.c
)
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

target_include_directories(${EXECUTABLE} PRIVATE
    Inc
    Drivers/CMSIS/DSP/Include
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/CMSIS/Include
)
target_compile_definitions(${EXECUTABLE} PRIVATE
    -DSTM32
    -DSTM32F4
    -DSTM32F411RETx
    -DSTM32F411xE
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
)
target_link_options(${EXECUTABLE} PRIVATE
    -T${CMAKE_SOURCE_DIR}/STM32F411RETX_FLASH.ld
    --specs=nosys.specs
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections
    -static
    -L${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Lib/GCC
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
