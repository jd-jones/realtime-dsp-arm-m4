# == PROJECT SETUP ============================================================
set(device_compile_definition       STM32F407xx)
set(cmsis_root                      ../libraries/CMSIS)
set(stm32f4_hal_root                ../libraries/device_support/stm32f4xx_hal_driver)
set(fat_fs_root                     ../libraries/software_components/fat_fs/src)
set(usb_host_core_root				../libraries/software_components/stm32-mw-usb-host/Core)
set(usb_host_msc_root				../libraries/software_components/stm32-mw-usb-host/Class/MSC)
set(cmake_root                      ../cmake)
# =============================================================================


# == COMPILE AND LINK EXECUTABLE ==============================================
set(EXECUTABLE ${PROJECT_NAME})

include(${cmake_root}/stm32cube_f4.cmake)
add_cmsis_core(
    ${cmsis_root}
    ${device_compile_definition}
)
add_stm32f4_hal(
    ${stm32f4_hal_root}
    ${cmsis_root}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${device_compile_definition}
)
add_fat_fs(${fat_fs_root} ${CMAKE_CURRENT_SOURCE_DIR} ${device_compile_definition})
add_usb_host_core(${usb_host_core_root} ${cmsis_root} ${stm32f4_hal_root} ${CMAKE_CURRENT_SOURCE_DIR} ${device_compile_definition})
add_usb_host_msc(${usb_host_msc_root} ${usb_host_core_root} ${device_compile_definition})

add_executable(${EXECUTABLE} main.c sysmem.c)
target_include_directories(${EXECUTABLE} PRIVATE
    ${cmsis_root}/Core/Include
    ${cmsis_root}/Device/ST/STM32F4xx/Include
    ${stm32f4_hal_root}/Inc
    ${fat_fs_root}
)
target_link_libraries(${EXECUTABLE} PRIVATE
    -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F407VGTX_FLASH.ld
    stm32f4_cmsis_core
    stm32f4_hal_driver
    usb_host_core
    usb_host_msc
    fat_fs
)
# =============================================================================
