cmake_minimum_required(VERSION 3.15.0)


project(nucleo-2024-12_sample)


enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


set(STM32CUBEMX_GENERATED_FILES
    Src/main.c
    Startup/startup_stm32f411retx.s
)


set(EXECUTABLE ${PROJECT_NAME}.out)


add_executable(${EXECUTABLE} ${STM32CUBEMX_GENERATED_FILES})


target_compile_definitions(${EXECUTABLE} PRIVATE
    -DSTM32F413xx
)


target_include_directories(${EXECUTABLE} PRIVATE
        Inc
        Drivers/CMSIS/DSP/Include
        Drivers/CMSIS/Device/ST/STM32F4xx/Include
        Drivers/CMSIS/Include
)


target_compile_options(${EXECUTABLE} PRIVATE
	-mcpu=cortex-m4
	-mthumb
	-mfpu=fpv4-sp-d16
	-mfloat-abi=hard

	-fdata-sections
	-ffunction-sections

	-Wall

	$<$<CONFIG:Debug>:-Og>
)


target_link_options(${EXECUTABLE} PRIVATE
    -T${CMAKE_SOURCE_DIR}/STM32F411RETX_FLASH.ld
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -specs=nano.specs
    -lc
    -lm
    -lnosys
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections
    -static
    -L${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Lib/GCC
    -u _printf_float -u _scanf_float
    -Wl,--start-group
    -Wl,--end-group
    -larm_cortexM4lf_math
)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-size ${EXECUTABLE})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
    COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)

