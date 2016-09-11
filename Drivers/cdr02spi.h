#ifndef __CDR02SPI_H
#define __CDR02SPI_H

/**************************************************************************************
* Includes
**************************************************************************************/
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_spi.h"

/**************************************************************************************
* Define
**************************************************************************************/
#define DR02_SPI_CS_GPIO                   GPIO_Pin_4
#define DR02_SPI_SCK_GPIO                  GPIO_Pin_5
#define DR02_SPI_MISO_GPIO                 GPIO_Pin_6
#define DR02_SPI_MOSI_GPIO                 GPIO_Pin_7

#define DR02_SPI_CS_PORT                   GPIOA
#define DR02_SPI_SCK_PORT                  GPIOA
#define DR02_SPI_MISO_PORT                 GPIOA
#define DR02_SPI_MOSI_PORT                 GPIOA

#define DR02_SET_SPI_CS_HIGH               GPIO_SetBits(DR02_SPI_CS_PORT, DR02_SPI_CS_GPIO)
#define DR02_SET_SPI_CS_LOW                GPIO_ResetBits(DR02_SPI_CS_PORT, DR02_SPI_CS_GPIO)

#define DR02_SPI_READ_WRITE_RETRY_CNT      200

/***************************************************************************************
* Data
***************************************************************************************/

/***************************************************************************************
* Function prototypes
****************************************************************************************/
void p_cdr02_SpiInit(void);
u8   p_cdr02_SpiWriteByte(u8 u8_WriteByte);
u8   p_cdr02_SpiReadByte(u8 u8_ReadByte);
#endif

