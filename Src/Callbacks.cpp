#include "Callbacks.hpp"

#ifdef STM32H7A3xx

#include "u8g2.h"
#include "spi.h"
#include "gpio.h"
#include "cmsis_os2.h"

extern osSemaphoreId_t spiSemHandle;

uint8_t u8x8_gpio_and_delay_stm32(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_DELAY_MILLI:
        break;

    case U8X8_MSG_DELAY_10MICRO:
        break;

    case U8X8_MSG_DELAY_100NANO:
        break;

    case U8X8_MSG_GPIO_DC:
        HAL_GPIO_WritePin(LCD_CD_GPIO_Port, LCD_CD_Pin, arg_int ? GPIO_PIN_SET : GPIO_PIN_RESET);
        break;

    case U8X8_MSG_GPIO_RESET:
        HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, arg_int ? GPIO_PIN_SET : GPIO_PIN_RESET);
        break;

    case U8X8_MSG_GPIO_CS:
        
        break;

    default:
        return 0;
    }

    return 1;
}

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr)
{
    switch (msg)
        {
        case U8X8_MSG_BYTE_INIT:
            break;

        case U8X8_MSG_BYTE_SEND:
            if (HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)arg_ptr, arg_int) == HAL_OK) 
            {
                osSemaphoreAcquire(spiSemHandle, osWaitForever);
            }

            if (arg_ptr != NULL)
            {

            }

            break;

        case U8X8_MSG_BYTE_SET_DC:
            HAL_GPIO_WritePin(LCD_CD_GPIO_Port, LCD_CD_Pin, arg_int ? GPIO_PIN_SET : GPIO_PIN_RESET);
            break;

        case U8X8_MSG_BYTE_START_TRANSFER:
            break;

        case U8X8_MSG_BYTE_END_TRANSFER:
            break;

        default:
            return 0;
        }

        return 1;
}

#endif