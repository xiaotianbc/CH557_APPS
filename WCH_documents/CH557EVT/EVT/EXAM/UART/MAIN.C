/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : CH557����0~1 ��ѯ���ж��շ���ʾ��ʵ�����ݻػ�                     
*******************************************************************************/
#include "DEBUG.H"
#include "UART.H"

/* Global Variable */ 
UINT8 dat;

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Return         : None
*******************************************************************************/
void main(void)
{
	CfgFsys();
	mDelaymS(20);
	mInitSTDIO();                                                             /* Ϊ���ü����ͨ�����ڼ����ʾ���� */
	CH557UART0Alter();                                                         //����0����ӳ��
	printf("ID=%02X\n", (UINT16)CHIP_ID);
	printf("UART demo start ...\n");

	CH557UART1Init();                                                          //����1��ʼ��
	CH557UART1Alter();                                                         //����1����ӳ��   

	dat = 0;
	
	while(1){
#ifndef UART1_INTERRUPT
		dat = CH557UART1RcvByte();
		CH557UART1SendByte(dat);
		
#endif
	}
}