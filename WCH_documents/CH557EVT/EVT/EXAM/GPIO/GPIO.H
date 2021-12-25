/********************************** (C) COPYRIGHT *******************************
* File Name          : GPIO.H
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : CH557 IO  
*******************************************************************************/
#ifndef	__GPIO_H__
#define __GPIO_H__

#include "Debug.H"

/****************** GPIO_Init �������� ******************/
/* �˿ڶ��� */
#define  PORT0      0x00
#define  PORT1      0x01
#define  PORT2      0x02
#define  PORT3      0x03
#define  PORT4      0x04

/* ���Ŷ��壨������ϣ� */
#define  PIN0       0x01
#define  PIN1       0x02
#define  PIN2       0x04
#define  PIN3       0x08
#define  PIN4       0x10
#define  PIN5       0x20
#define  PIN6       0x40
#define  PIN7       0x80
/* ģʽ���� */
#define  MODE0      0x00                             //��������ģʽ������û����������
#define  MODE1      0x01                             //�������ģʽ�����жԳ�������������������������սϴ����
#define  MODE2      0x02                             //��©�����֧�ָ������룬����û����������
#define  MODE3      0x03                             //׼˫��ģʽ(��׼ 8051)����©�����֧�����룬��������������(Ĭ��ģʽ)
/****************** GPIO_INT_Init �������� ********************/
/* IntSrc �ⲿ�ж�(�����) */
#define  INT_RXD0_L  0x0001
#define  INT_P03_L   0x0008
#define  INT_P14_L   0x0010
#define  INT_P15_L   0x0020

#define  INT_P0x_L   0x1000
#define  INT_P1x_L   0x2000
#define  INT_P2x_L   0x4000
#define  INT_P4x_L   0x8000

#define  INT_RXD1_L  0x0040
#define  INT_INT0_L  0x0080
#define  INT_INT1_L  0x0100
/* �жϴ���ģʽ */
#define  INT_LEVEL   0x00                             //��ƽģʽ
#define  INT_EDGE    0x01                             //����ģʽ
/* �ж�״̬ */
#define  Disable     0x00                             //�ж���Ч
#define  Enable      0x01                             //�ж�ʹ��
/****************** �ⲿ�����Ӻ��� ****************************/
extern void GPIO_Init(UINT8 PORTx, UINT8 PINx, UINT8 MODEx);                   //GPIO��ʼ��
extern void GPIO_INT_Init(UINT16 IntSrc, UINT8 Mode, UINT8 NewState );        //�ⲿ�жϳ�ʼ��
extern void GPIO_INT_P4x(void);                                              //P4�˿��жϳ�ʼ��

#endif