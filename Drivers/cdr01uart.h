#ifndef __CDR01UART_H
#define __CDR01UART_H

/**************************************************************************************
* Includes
**************************************************************************************/
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_usart.h"

/**************************************************************************************
* Define
**************************************************************************************/
#define DR01_UART_TX_GPIO                             GPIO_Pin_9
#define DR01_UART_RX_GPIO                             GPIO_Pin_10
#define DR01_UART_TX_PORT                             GPIOA
#define DR01_UART_RX_PORT                             GPIOA

/***************************************************************************************
* Data
***************************************************************************************/

/***************************************************************************************
* Function prototypes
****************************************************************************************/
void p_cdr01_UartInit(void);
void p_dr01_UartSendByte(u8 u8_DataByte);
void p_dr01_UartSendData(u8 *pu8_Data, u32 u32_DataLen);
u8   p_dr01_UartReciveData(void);
#endif

