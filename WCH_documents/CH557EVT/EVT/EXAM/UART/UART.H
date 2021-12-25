/********************************** (C) COPYRIGHT *******************************
* File Name          : UART.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : CH557 UART
*******************************************************************************/
#ifndef	__UART_H__
#define __UART_H__

#include "DEBUG.H"

/* UART1��� */
#define   UART1_INTERRUPT                           //�����Ƿ�������1�ж�

/* �ⲿ�����Ӻ��� */
void CH557UART1Init();                      //UART1��ʼ������
void CH557UART1Alter();                     //����1����ӳ��
UINT8 CH557UART1RcvByte();                  //UART1����һ���ֽ�
void CH557UART1SendByte(UINT8 SendDat);     //UART1����һ���ֽ�

#endif