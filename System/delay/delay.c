/* == HISTORY =========================================================
 *
 * Name     Date                Ver     Action
 * --------------------------------------------------------------------
 * Carols   03-sept-2016                add delay api
 *
 **/
#include "delay.h"


/***************************************************************************************
* Data
***************************************************************************************/
static u8  u8_fac_us = 0;
static u16 u16_fac_ms = 0;

/**************************************************************************************
* Function Implementation
**************************************************************************************/
void delay_init(u8 SYSCLK)
{
//	SysTick->CTRL&=0xfffffffb;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	
	u8_fac_us=SYSCLK/8;		    
	u16_fac_ms=(u16)u8_fac_us*1000;
}								    

void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*u16_fac_ms;
	SysTick->VAL =0x00;          
	SysTick->CTRL=0x01 ;          
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL=0x00;       
	SysTick->VAL =0X00;       	  	    
}   
		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*u8_fac_us; 	  		 
	SysTick->VAL=0x00;        
	SysTick->CTRL=0x01 ;       	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL=0x00;       
	SysTick->VAL =0X00;       	 
}
