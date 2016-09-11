/* == HISTORY =========================================================
 *
 * Name     Date                Ver     Action
 * --------------------------------------------------------------------
 * Carols   03-sept-2016                add spi driver
 *
 **/
#include "cdr02spi.h"


/***************************************************************************************
* Data
***************************************************************************************/

/**************************************************************************************
* Function Implementation
**************************************************************************************/
/*************************************************************************************
    Function Name: p_cdr02_SpiInit
    Input        : NULL
    Output       : NULL
    Return       : NULL
    Description  : Spi Init
**************************************************************************************/
void p_cdr02_SpiInit(void)
{
    GPIO_InitTypeDef st_GpioInit;
    SPI_InitTypeDef  st_SpiInit;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);                // Configuration gpio clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);               // Configuration spi clock
    st_GpioInit.GPIO_Pin = DR02_SPI_SCK_GPIO | DR02_SPI_MISO_GPIO | DR02_SPI_MOSI_GPIO;
    st_GpioInit.GPIO_Mode = GPIO_Mode_AF;
    st_GpioInit.GPIO_Speed = GPIO_Speed_50MHz;                         // Set gpio speed
    st_GpioInit.GPIO_OType = GPIO_OType_PP;
    st_GpioInit.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &st_GpioInit);                                    // Init spi gpio
    
 	GPIO_SetBits(GPIOA, DR02_SPI_SCK_GPIO | DR02_SPI_MISO_GPIO | DR02_SPI_MOSI_GPIO); // Default output high

    // Set gpio port alternate function 
    GPIO_PinAFConfig(DR02_SPI_SCK_PORT, GPIO_PinSource5, GPIO_AF_0);   // Connect PXx to SPI_SCK
    GPIO_PinAFConfig(DR02_SPI_MISO_PORT, GPIO_PinSource6, GPIO_AF_0);  // Connect PXx to SPI_MISO
    GPIO_PinAFConfig(DR02_SPI_MOSI_PORT, GPIO_PinSource7, GPIO_AF_0);  // Connect PXx to SPI_MOSI

    st_GpioInit.GPIO_Pin = DR02_SPI_CS_GPIO;
    st_GpioInit.GPIO_Mode = GPIO_Mode_OUT;
    st_GpioInit.GPIO_OType = GPIO_OType_PP;
    st_GpioInit.GPIO_PuPd = GPIO_PuPd_UP;
    st_GpioInit.GPIO_Speed = GPIO_Speed_Level_3;
    GPIO_Init(DR02_SPI_CS_PORT, &st_GpioInit);

    DR02_SET_SPI_CS_HIGH;                                              // Spi cs default output high

    st_SpiInit.SPI_Direction = SPI_Direction_2Lines_FullDuplex;        // Use spi full duplex mode
    st_SpiInit.SPI_Mode = SPI_Mode_Master;                             // Set spi mode, use master mode
    st_SpiInit.SPI_DataSize = SPI_DataSize_8b;                         // Set data format
    st_SpiInit.SPI_CPOL = SPI_CPOL_High;                               // Set CPOL
    st_SpiInit.SPI_CPHA = SPI_CPHA_2Edge;                              // Set CPHA
    st_SpiInit.SPI_NSS = SPI_NSS_Soft;                                 // Set spi cs mode, use software control
    st_SpiInit.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;        // Set spi baud rate prescaler
    st_SpiInit.SPI_FirstBit = SPI_FirstBit_MSB;                        // use msb
    st_SpiInit.SPI_CRCPolynomial = 7;                                  // crc set
    SPI_Init(SPI1, &st_SpiInit);                                       // Init spi
    SPI_RxFIFOThresholdConfig(SPI2, SPI_RxFIFOThreshold_QF);           // Set the spi1 data receive fifo threshold
    SPI_Cmd(SPI1, ENABLE);
}

/*************************************************************************************
    Function Name: p_cdr02_SpiWriteByte
    Input        : u8_WriteByte -> Write data
    Output       : NULL
    Return       : 1 -> Spi write failed, 0 -> Spi write success
    Description  : Write spi data
**************************************************************************************/
u8 p_cdr02_SpiWriteByte(u8 u8_WriteByte)
{    
	u8 u8_RetryCnt = 0;				 	
    
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)     // Wait the send success
    {
        u8_RetryCnt++;
        if(u8_RetryCnt > DR02_SPI_READ_WRITE_RETRY_CNT)
            return 1;
    }
    SPI_SendData8(SPI1, u8_WriteByte);                                 // Send data byte
    
    return 0;
}

/*************************************************************************************
    Function Name: p_cdr02_SpiReadByte
    Input        : NULL
    Output       : NULL
    Return       : -1 -> Spi read failed, 0 -> Spi read success
    Description  : Read spi data
**************************************************************************************/
u8 p_cdr02_SpiReadByte(u8 u8_ReadByte)
{    
	u8 u8_RetryCnt = 0;				 	
    
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)     // Wait the send success
    {
        u8_RetryCnt++;
        if(u8_RetryCnt > DR02_SPI_READ_WRITE_RETRY_CNT)
            return 1;
    }
    SPI_SendData8(SPI1, 0xFF);

    u8_RetryCnt = 0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)   // Wait the read success
    {
        u8_RetryCnt++;
        if(u8_RetryCnt > DR02_SPI_READ_WRITE_RETRY_CNT)
            return 1;
    }
    
    return SPI_ReceiveData8(SPI1);                                    // Return the byte read from the spi bus
}

