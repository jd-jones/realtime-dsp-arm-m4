/**
  ******************************************************************************
  * @file    main.c 
  * @author  jdjones
  * @brief   Main program body.
  ******************************************************************************
  * TODO
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"          // HAL_Init
#include "stm32f4_discovery.h"      // BSP_LED_INIT, BSP_LED_On
#include "ff.h"
#include "ff_gen_drv.h"
#include "usbh_core.h"
#include "usbh_msc.h"


// ============================================================================
#define test_filename "STM32.TXT"

typedef enum {
  APPLICATION_IDLE = 0,  
  APPLICATION_START,    
  APPLICATION_RUNNING,
} MSC_ApplicationTypeDef;

typedef enum {
    USB_MSC_TEST_SUCCESS = 0,
    USB_MSC_TEST_ERROR_FAILED_MOUNT,
    USB_MSC_TEST_ERROR_FAILED_OPEN_WRITE_MODE,
    USB_MSC_TEST_ERROR_FAILED_OPEN_READ_MODE,
    USB_MSC_TEST_ERROR_FAILED_WRITE,
    USB_MSC_TEST_ERROR_FAILED_READ,
    USB_MSC_TEST_ERROR_READ_DIFFERS_FROM_WRITTEN,
} USB_MSC_TEST_STATUS;
// ============================================================================


// ============================================================================
static void SystemClock_Config(void);
static void Error_Handler(void);
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);
static USB_MSC_TEST_STATUS MSC_Application(void);
static void display_msc_application_status(USB_MSC_TEST_STATUS);
// ============================================================================


// ============================================================================
const Diskio_drvTypeDef  USBH_Driver;
FATFS USBDISKFatFs;           /* File system object for USB disk logical drive */
FIL MyFile;                   /* File object */
char USBDISKPath[4];          /* USB Host logical drive path */
USBH_HandleTypeDef hUSB_Host; /* USB Host handle */

MSC_ApplicationTypeDef application_state = APPLICATION_IDLE;
// ============================================================================


int main(int argc, char const* argv[]) {
    (void)argc;
    (void)argv;

    // == STM32F4xx HAL library initialization: ===============================
    //  - Configure the Flash prefetch, instruction and Data caches
    //  - Configure the Systick to generate an interrupt each 1 msec
    //  - Set NVIC Group Priority to 4
    //  - Global MSP (MCU Support Package) initialization
    HAL_Init();
    SystemClock_Config();   // Configure system clock to 168 MHz
    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4);
    BSP_LED_Init(LED5);
    BSP_LED_Init(LED6);
    // =========================================================================

    BSP_LED_On(LED3);
    BSP_LED_On(LED3);

    if (FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == 0) { 
        USBH_Init(&hUSB_Host, USBH_UserProcess, 0);
        USBH_RegisterClass(&hUSB_Host, USBH_MSC_CLASS);
        USBH_Start(&hUSB_Host);

        USB_MSC_TEST_STATUS msc_application_status;
        while (1) {
            switch(application_state) {
                case APPLICATION_START:
                    msc_application_status = MSC_Application();
                    display_msc_application_status(msc_application_status);
                    break;      
                case APPLICATION_IDLE:
                    default:
                    break;      
            }

            USBH_Process(&hUSB_Host);
        }
    }

    return 0;
}


// USB CODE
// ============================================================================
/**
  * @brief  Main routine for Mass Storage Class
  * @param  None
  * @retval None
  */
static USB_MSC_TEST_STATUS MSC_Application(void) {
    // Register the file system object to the FatFs module
    if(f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 0) != FR_OK) {
        return USB_MSC_TEST_ERROR_FAILED_MOUNT;
    }

    // == WRITE ===============================================================
    // Create and open a new text file object with write access,
    // then write data to the text file
    uint8_t wtext[] = "This is STM32 working with FatFs";
    uint32_t byteswritten;

    if(f_open(&MyFile, test_filename, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) {
        return USB_MSC_TEST_ERROR_FAILED_OPEN_WRITE_MODE;
    }
            
    FRESULT write_status = f_write(
        &MyFile,
        wtext,
        sizeof(wtext),
        (void *)&byteswritten
    );
    if((byteswritten == 0) || (write_status != FR_OK)) {
        return USB_MSC_TEST_ERROR_FAILED_WRITE;
    }
    f_close(&MyFile);
    // ========================================================================

    // == READ ================================================================
    // Open the same file in read mode and read the data.
    uint8_t rtext[100];         // File read buffer
    uint32_t bytesread;

    if(f_open(&MyFile, test_filename, FA_READ) != FR_OK) {
        return USB_MSC_TEST_ERROR_FAILED_OPEN_READ_MODE;
    }

    FRESULT read_status = f_read(
        &MyFile,
        rtext,
        sizeof(rtext),
        (void *)&bytesread
    );
    if((bytesread == 0) || (read_status != FR_OK)) {
        return USB_MSC_TEST_ERROR_FAILED_READ;
    }
    f_close(&MyFile);
    // ========================================================================

    // Compare read data with the expected data
    if(bytesread != byteswritten) {                
        return USB_MSC_TEST_ERROR_READ_DIFFERS_FROM_WRITTEN;
    }
    
    // Success of the demo: no error occurrence
    FATFS_UnLinkDriver(USBDISKPath);
    return USB_MSC_TEST_SUCCESS;
}

static void display_msc_application_status(USB_MSC_TEST_STATUS test_status) {
    switch(test_status) {
        case USB_MSC_TEST_SUCCESS:
            BSP_LED_On(LED4);
            break;
        default:
            Error_Handler();
            break;
    }
}

/**
  * @brief  User Process
  * @param  phost: Host handle
  * @param  id: Host Library user message ID
  * @retval None
  */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id) {  
    switch(id) { 
        case HOST_USER_SELECT_CONFIGURATION:
            break;
        case HOST_USER_DISCONNECTION:
            application_state = APPLICATION_IDLE;
            BSP_LED_Off(LED4);
            BSP_LED_Off(LED5);
            f_mount(NULL, (TCHAR const*)"", 0);          
            break;
        case HOST_USER_CLASS_ACTIVE:
            application_state = APPLICATION_START;
            break;
        default:
            break; 
    }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void) {
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig (&RCC_OscInitStruct);

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

    /* STM32F405x/407x/415x/417x Revision Z and upper devices: prefetch is supported  */
    if (HAL_GetREVID() >= 0x1001) {
        /* Enable the Flash prefetch */
        __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void) {
    /* Turn LED5 on */
    BSP_LED_On(LED5);
    while(1) {}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1) {}
}
#endif

