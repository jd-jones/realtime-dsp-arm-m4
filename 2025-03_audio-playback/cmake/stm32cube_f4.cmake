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

    target_include_directories(${STM32F4_LL} PUBLIC
        ${cmsis_root}/Core/Include
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


function(add_stm32f4_hal stm32f4_hal_root cmsis_root stm32f4_hal_conf_dir device_compile_definition)
    set(STM32F4_HAL stm32f4_hal_driver)
    set(hal_src ${stm32f4_hal_root}/Src)

    add_library(${STM32F4_HAL}
		${hal_src}/stm32f4xx_hal.c
		${hal_src}/stm32f4xx_hal_i2c.c
		${hal_src}/stm32f4xx_hal_i2c_ex.c
		${hal_src}/stm32f4xx_hal_i2s.c
		${hal_src}/stm32f4xx_hal_i2s_ex.c
		${hal_src}/stm32f4xx_hal_sram.c
		${hal_src}/stm32f4xx_hal_tim.c
		${hal_src}/stm32f4xx_hal_tim_ex.c
		${hal_src}/stm32f4xx_hal_adc.c
		${hal_src}/stm32f4xx_hal_adc_ex.c
		${hal_src}/stm32f4xx_hal_can.c
		${hal_src}/stm32f4xx_hal_irda.c
		${hal_src}/stm32f4xx_hal_cec.c
		${hal_src}/stm32f4xx_hal_iwdg.c
		# ${hal_src}/stm32f4xx_hal_timebase_rtc_alarm_template.c
		# ${hal_src}/stm32f4xx_hal_timebase_rtc_wakeup_template.c
		# ${hal_src}/stm32f4xx_hal_timebase_tim_template.c
		${hal_src}/stm32f4xx_hal_cortex.c
		${hal_src}/stm32f4xx_hal_lptim.c
		${hal_src}/stm32f4xx_hal_uart.c
		${hal_src}/stm32f4xx_hal_crc.c
		${hal_src}/stm32f4xx_hal_ltdc.c
		${hal_src}/stm32f4xx_hal_ltdc_ex.c
		${hal_src}/stm32f4xx_hal_usart.c
		${hal_src}/stm32f4xx_hal_cryp.c
		${hal_src}/stm32f4xx_hal_cryp_ex.c
		${hal_src}/stm32f4xx_hal_wwdg.c
		${hal_src}/stm32f4xx_hal_mmc.c
		${hal_src}/stm32f4xx_hal_dac.c
		# ${hal_src}/stm32f4xx_hal_msp_template.c
		${hal_src}/stm32f4xx_hal_dac_ex.c
		${hal_src}/stm32f4xx_hal_nand.c
		${hal_src}/stm32f4xx_hal_dcmi.c
		${hal_src}/stm32f4xx_hal_nor.c
		${hal_src}/stm32f4xx_hal_dcmi_ex.c
		${hal_src}/stm32f4xx_hal_pccard.c
		${hal_src}/stm32f4xx_hal_dfsdm.c
		${hal_src}/stm32f4xx_hal_pcd.c
		${hal_src}/stm32f4xx_hal_dma.c
		${hal_src}/stm32f4xx_hal_pcd_ex.c
		${hal_src}/stm32f4xx_hal_dma2d.c
		${hal_src}/stm32f4xx_hal_pwr.c
		${hal_src}/stm32f4xx_hal_dma_ex.c
		${hal_src}/stm32f4xx_hal_pwr_ex.c
		${hal_src}/stm32f4xx_hal_dsi.c
		${hal_src}/stm32f4xx_hal_qspi.c
		${hal_src}/stm32f4xx_hal_eth.c
		${hal_src}/stm32f4xx_hal_rcc.c
		${hal_src}/stm32f4xx_hal_exti.c
		${hal_src}/stm32f4xx_hal_rcc_ex.c
		${hal_src}/stm32f4xx_hal_flash.c
		${hal_src}/stm32f4xx_hal_rng.c
		${hal_src}/stm32f4xx_hal_flash_ex.c
		${hal_src}/stm32f4xx_hal_rtc.c
		${hal_src}/stm32f4xx_hal_flash_ramfunc.c
		${hal_src}/stm32f4xx_hal_rtc_ex.c
		${hal_src}/stm32f4xx_hal_fmpi2c.c
		${hal_src}/stm32f4xx_hal_sai.c
		${hal_src}/stm32f4xx_hal_fmpi2c_ex.c
		${hal_src}/stm32f4xx_hal_sai_ex.c
		${hal_src}/stm32f4xx_hal_fmpsmbus.c
		${hal_src}/stm32f4xx_hal_sd.c
		${hal_src}/stm32f4xx_hal_fmpsmbus_ex.c
		${hal_src}/stm32f4xx_hal_sdram.c
		${hal_src}/stm32f4xx_hal_gpio.c
		${hal_src}/stm32f4xx_hal_smartcard.c
		${hal_src}/stm32f4xx_hal_hash.c
		${hal_src}/stm32f4xx_hal_smbus.c
		${hal_src}/stm32f4xx_hal_hash_ex.c
		${hal_src}/stm32f4xx_hal_spdifrx.c
		${hal_src}/stm32f4xx_hal_hcd.c
		${hal_src}/stm32f4xx_hal_spi.c
		${hal_src}/stm32f4xx_ll_adc.c
		${hal_src}/stm32f4xx_ll_crc.c
		${hal_src}/stm32f4xx_ll_dac.c
		${hal_src}/stm32f4xx_ll_dma.c
		${hal_src}/stm32f4xx_ll_dma2d.c
		${hal_src}/stm32f4xx_ll_exti.c
		${hal_src}/stm32f4xx_ll_fmc.c
		${hal_src}/stm32f4xx_ll_fmpi2c.c
		${hal_src}/stm32f4xx_ll_fsmc.c
		${hal_src}/stm32f4xx_ll_gpio.c
		${hal_src}/stm32f4xx_ll_i2c.c
		${hal_src}/stm32f4xx_ll_lptim.c
		${hal_src}/stm32f4xx_ll_pwr.c
		${hal_src}/stm32f4xx_ll_rcc.c
		${hal_src}/stm32f4xx_ll_rng.c
		${hal_src}/stm32f4xx_ll_rtc.c
		${hal_src}/stm32f4xx_ll_sdmmc.c
		${hal_src}/stm32f4xx_ll_spi.c
		${hal_src}/stm32f4xx_ll_tim.c
		${hal_src}/stm32f4xx_ll_usart.c
		${hal_src}/stm32f4xx_ll_usb.c
		${hal_src}/stm32f4xx_ll_utils.c
    )

    target_include_directories(${STM32F4_HAL} PUBLIC
        ${cmsis_root}/Core/Include
        ${cmsis_root}/Device/ST/STM32F4xx/Include
        ${stm32f4_hal_root}/Inc
        ${stm32f4_hal_conf_dir}
    )
    target_compile_definitions(${STM32F4_HAL} PUBLIC
        STM32
        STM32F4
        USE_HAL_DRIVER
        ${device_compile_definition}
    )
    target_link_libraries(${STM32F4_HAL} PUBLIC
        stm32f4_cmsis_core
    )
endfunction()


function(add_fat_fs fat_fs_root fat_fs_conf_dir device_compile_definition)
    set(fat_fs_library fat_fs)

    add_library(${fat_fs_library}
        ${fat_fs_root}/option/ccsbcs.c
        ${fat_fs_root}/option/syscall.c
        ${fat_fs_root}/option/unicode.c
		${fat_fs_root}/diskio.c
		${fat_fs_root}/ff.c
		${fat_fs_root}/ff_gen_drv.c
    )
    target_include_directories(${fat_fs_library} PUBLIC
        ${fat_fs_root}
        ${fat_fs_conf_dir}
    )
    target_compile_definitions(${fat_fs_library} PUBLIC
        STM32
        STM32F4
        ${device_compile_definition}
    )
endfunction()


function(add_usb_host_core usb_host_core_root cmsis_root stm32f4_hal_root usb_host_conf_dir device_compile_definition)
    set(usb_host_core_library usb_host_core)

    add_library(${usb_host_core_library}
        ${usb_host_conf_dir}/usbh_conf.c
        ${usb_host_core_root}/Src/usbh_core.c
        ${usb_host_core_root}/Src/usbh_ctlreq.c
        ${usb_host_core_root}/Src/usbh_ioreq.c
        ${usb_host_core_root}/Src/usbh_pipes.c
    )
    target_include_directories(${usb_host_core_library} PUBLIC
        ${cmsis_root}/Core/Include
        ${cmsis_root}/Device/ST/STM32F4xx/Include
        ${stm32f4_hal_root}/Inc
        ${usb_host_conf_dir}
        ${usb_host_core_root}/Inc
    )
    target_compile_definitions(${usb_host_core_library} PUBLIC
        STM32
        STM32F4
        USE_HAL_DRIVER
        ${device_compile_definition}
    )
    target_link_libraries(${usb_host_core_library} PUBLIC
        stm32f4_hal_driver
    )
endfunction()


function(add_usb_host_msc usb_host_msc_root usb_host_core_root device_compile_definition)
    set(usb_host_msc_library usb_host_msc)

    add_library(${usb_host_msc_library}
		${usb_host_msc_root}/Src/usbh_msc.c
		${usb_host_msc_root}/Src/usbh_msc_bot.c
		${usb_host_msc_root}/Src/usbh_msc_scsi.c
    )
    target_include_directories(${usb_host_msc_library} PUBLIC
        ${usb_host_core_root}/Inc
        ${usb_host_msc_root}/Inc
    )
    target_compile_definitions(${usb_host_msc_library} PUBLIC
        STM32
        STM32F4
        ${device_compile_definition}
    )
    target_link_libraries(${usb_host_msc_library} PUBLIC
        usb_host_core
    )
endfunction()


function(add_cmsis_dsp cmsis_root)
    # TODO
    target_compile_definitions(${EXECUTABLE} PRIVATE
        __FPU_PRESENT=1U
        ARM_MATH_CM4
    )
endfunction()
