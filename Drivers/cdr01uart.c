/* == HISTORY =========================================================
 *
 * Name     Date                Ver     Action
 * --------------------------------------------------------------------
 * Carols   03-sept-2016                add  uart driver
 *
 **/
#include <stdarg.h>
#include <stdio.h>
#include "cdr01uart.h"

/***************************************************************************************
* Data
***************************************************************************************/

/**************************************************************************************
* Function Implementation
**************************************************************************************/
/*************************************************************************************
    Function Name: p_cdr01_UartInit
    Input        : NULL
    Output       : NULL
    Return       : NULL
    Description  : Usart Init
**************************************************************************************/
void p_cdr01_UartInit(void)
{
    GPIO_InitTypeDef  st_GpioInit;
    USART_InitTypeDef st_UsartInit;
            
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);                     // Enable the clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    st_GpioInit.GPIO_Pin = DR01_UART_TX_GPIO | DR01_UART_RX_GPIO;                 
    st_GpioInit.GPIO_Mode = GPIO_Mode_AF; 
    st_GpioInit.GPIO_OType = GPIO_OType_PP;
    st_GpioInit.GPIO_PuPd = GPIO_PuPd_UP;
    st_GpioInit.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &st_GpioInit);        

    GPIO_PinAFConfig(DR01_UART_TX_PORT, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(DR01_UART_RX_PORT, GPIO_PinSource10, GPIO_AF_1);        
    
    st_UsartInit.USART_BaudRate = 115200;                                    // Set the baud rate
    st_UsartInit.USART_WordLength = USART_WordLength_8b;                     // Set word length
    st_UsartInit.USART_StopBits = USART_StopBits_1;                          // Set stop bits
    st_UsartInit.USART_Parity = USART_Parity_No;                             // Set parity
    st_UsartInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // Set flow control
    st_UsartInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 // Set work mode
    USART_Init(USART1, &st_UsartInit);                                       // Init uart
    
    USART_Cmd(USART1, ENABLE);                                               // Enable the uart1
}

/*************************************************************************************
    Function Name: p_dr01_UartSendByte
    Input        : u8_DataByte -> want to send data
    Output       : NULL
    Return       : NULL
    Description  : Usart send a byte data
**************************************************************************************/
void p_dr01_UartSendByte(u8 u8_DataByte)
{
    while(!((USART1->ISR)&(1<<7)))       // Wait the send success
        USART1->TDR = u8_DataByte;
}		

/*************************************************************************************
    Function Name: p_dr01_UartSendData
    Input        : pu8_Data-> want to send data buffer, u32_DataLen-> Buffer data len 
    Output       : NULL
    Return       : NULL
    Description  : Usart send some data
**************************************************************************************/
void p_dr01_UartSendData(u8 *pu8_Data, u32 u32_DataLen)
{
	while(u32_DataLen != 0)
	{
		while(!((USART1->ISR)&(1<<7)))  // Wait the send success
		USART1->TDR = *pu8_Data;
		pu8_Data++;
		u32_DataLen--;
	}
}

/*************************************************************************************
    Function Name: p_dr01_UartReciveData
    Input        : NULL
    Output       : NULL
    Return       : Receive data
    Description  : Usart receive one byte data
**************************************************************************************/
u8 p_dr01_UartReciveData(void)
{	
	while(!(USART1->ISR & (1<<5)))     // Wait the data receive success
	{};
	return (USART1->RDR);
}
		
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (u8)ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

    return ch;
}

int fgetc(FILE *f)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    return (int)USART_ReceiveData(USART1);
}

