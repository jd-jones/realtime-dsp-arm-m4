cmake_minimum_required(VERSION 3.21)


function(add_cmsis_core cmsis_root device_compile_definition)
    if(${device_compile_definition} STREQUAL "STM32F407xx")
        set(device_startup_filename     startup_stm32f407xx.s)
    else()
        message(FATAL_ERROR "Device not recognized")
    endif()

    set(cmsis_library stm32f4_cmsis_core)
    add_library(${cmsis_library}
        ${cmsis_root}/Device/ST/STM32F4xx/Source/Templates/gcc/${device_startup_filename}
        ${cmsis_root}/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
    )
    target_compile_definitions(${cmsis_library} PUBLIC
        STM32
        STM32F4
        ${device_compile_definition}
    )
    target_include_directories(${cmsis_library} PUBLIC
        ${cmsis_root}/Core/Include
        ${cmsis_root}/Device/ST/STM32F4xx/Include
    )
endfunction()


function(add_stm32f4_ll stm32f4_ll_root stm32f4_hal_conf_dir device_compile_definition)
    set(STM32F4_LL stm32f4_ll_driver)
    set(ll_src ${stm32f4_ll_root}/Src)

    add_library(${STM32F4_LL}
        ${ll_src}/stm32f4xx_ll_adc.c
        ${ll_src}/stm32f4xx_ll_crc.c
        ${ll_src}/stm32f4xx_ll_dac.c
        ${ll_src}/stm32f4xx_ll_dma.c
        ${ll_src}/stm32f4xx_ll_dma2d.c
        ${ll_src}/stm32f4xx_ll_exti.c
        ${ll_src}/stm32f4xx_ll_fmc.c
        ${ll_src}/stm32f4xx_ll_fmpi2c.c
        ${ll_src}/stm32f4xx_ll_fsmc.c
        ${ll_src}/stm32f4xx_ll_gpio.c
        ${ll_src}/stm32f4xx_ll_i2c.c
        ${ll_src}/stm32f4xx_ll_lptim.c
        ${ll_src}/stm32f4xx_ll_pwr.c
        ${ll_src}/stm32f4xx_ll_rcc.c
        ${ll_src}/stm32f4xx_ll_rng.c
        ${ll_src}/stm32f4xx_ll_rtc.c
        ${ll_src}/stm32f4xx_ll_sdmmc.c
        ${ll_src}/stm32f4xx_ll_spi.c
        ${ll_src}/stm32f4xx_ll_tim.c
        ${ll_src}/stm32f4xx_ll_usart.c
        ${ll_src}/stm32f4xx_ll_usb.c
        ${ll_src}/stm32f4xx_ll_utils.c
    )

    message(stm32f4_hal_conf_dir="${stm32f4_hal_conf_dir}")
    target_include_directories(${STM32F4_LL} PUBLIC
        # ${cmsis_root}/Core/Include
        ${cmsis_root}/Device/ST/STM32F4xx/Include
        ${stm32f4_ll_root}/Inc
        ${stm32f4_hal_conf_dir}
    )
    target_compile_definitions(${STM32F4_LL} PUBLIC
        STM32
        STM32F4
        ${device_compile_definition}
    )
    target_link_libraries(${STM32F4_LL} PUBLIC
        stm32f4_cmsis_core
    )
endfunction()


function(add_cmsis_dsp cmsis_root)
    # TODO
    target_compile_definitions(${EXECUTABLE} PRIVATE
        __FPU_PRESENT=1U
        ARM_MATH_CM4
    )
endfunction()
