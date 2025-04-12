# == PROJECT SETUP ============================================================
set(device_compile_definition       STM32F407xx)
set(cmsis_root                      ../libraries/CMSIS)
set(stm32f4_ll_root                 ../libraries/device_support/stm32f4xx_ll_driver)
set(cmake_root                      ../cmake)
# =============================================================================


# == COMPILE AND LINK EXECUTABLE ==============================================
set(EXECUTABLE ${PROJECT_NAME})

include(${cmake_root}/stm32cube_f4.cmake)
add_cmsis_core(${cmsis_root} ${device_compile_definition})
add_stm32f4_ll(${stm32f4_ll_root} ${CMAKE_CURRENT_SOURCE_DIR} ${device_compile_definition})

add_executable(${EXECUTABLE} main.c)
target_include_directories(${EXECUTABLE} PRIVATE
    ${cmsis_root}/Core/Include
    ${cmsis_root}/Device/ST/STM32F4xx/Include
    ${stm32f4_ll_root}/Inc
)
target_link_libraries(${EXECUTABLE} PRIVATE
    -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F407VGTX_FLASH.ld
    stm32f4_cmsis_core
    stm32f4_ll_driver
)
# =============================================================================
