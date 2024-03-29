/********************************** (C) COPYRIGHT *******************************
* File Name          : RGB.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : CH557 RGB初始化相关
*******************************************************************************/
#include "RGB.H"

/* Global Variable */ 
BOOL  data	 IsCode;
UINT16D	 BufferAddr;

/* 4字节对齐 */
UINT8X	RGBdataBuffer[128][4]  _at_ 0x0000; 
UINT8X	RGBdataBuffer1[128][4] _at_ 0x0200; 

/*******************************************************************************
* Function Name  : InitRGBLED
* Description    : RGBLED 初始化
* Input          : None
* Return         : None
*******************************************************************************/
void InitRGBLED(void)
{ 
	P4 = P2 = P1 = 0xFF;                                   // R/G/B PWM pins invert output, low action for sink current
	P0 = P3 = 0xFF;                                        // COMMON pin invert output, low action for drive external PMOS
	
	P4_MOD_OC = P2_MOD_OC = P1_MOD_OC = 0;                 // R/G/B PWM pins push-pull output
	P0_MOD_OC = P3_MOD_OC = 0;                             // COMMON pins push-pull output
	
	LED_CTRL = bLED_EN;
	LED_CYCLE = bLED_INTEN_CYC0;                           // PWM_128@color,PWM_128@intenisy,1x bLED_COLOR_CYC|  MASK_LED_INT_CYC;//
	LED_FRAME =  bLED_INH_TMR2|bLED_INH_TMR0;              // 101->8 intenisy PWM cycles, same PWM data repeat 1 time
	LED_DMA = BufferAddr = (UINT16)(&RGBdataBuffer[0]);    // buffer start address 
	IsCode = 0;                                            // indicate data@xRAM (1=data@flash ROM)
	LED_INT_ADJ = 0x05;                                    // adjust: increase intenisy
	LED_PWM_OE = 0xFE;
	LED_COMMON = 0xFF;                                     // preset COMMON selection, next is 0, start @interrupt
	LED_CTRL = bLED_IE_INHIB | bLED_BLUE_EN | bLED_GREEN_EN | bLED_RED_EN | bLED_COM_AHEAD |bLED_PWM_INHIB | bLED_EN;
	LED_STATUS |= bLED_IF_SET;                             // force bLED_IF to enter interrupt service 
	IE_PWM_I2C = 1;
}

