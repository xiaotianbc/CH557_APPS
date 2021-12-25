/********************************** (C) COPYRIGHT *******************************
* File Name          : Debug.H
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/03
* Description        : This file contains all the functions prototypes for UART
*                      Printf , Delay functions.									 
*******************************************************************************/
#ifndef	__DEBUG_H__
#define __DEBUG_H__

#include <absacc.h>
#include <stdio.h>
#include <string.h>
#include <intrins.h>
#include "CH557X.H"

//���庯������ֵ
#ifndef  SUCCESS
#define  SUCCESS    0
#endif
#ifndef  FAIL
#define  FAIL       0xFF
#endif


#define	 FREQ_SYS	                       24000000	          //ϵͳ��Ƶ24MHz
#define  OSC_EN_XT                       0                    //�ⲿ����ʹ�ܣ�Ĭ�Ͽ����ڲ�����
#ifndef  UART0_BUAD
#define  UART0_BUAD                      115200
#define  UART1_BUAD                      57600
#endif
#ifndef  DE_PRINTF                                            //����Pirntf�������
#define  DE_PRINTF                      1
#endif

#ifndef MIN
#define MIN(a,b)                (((a) <= (b)) ? (a) : (b))
#endif

void CfgFsys(void);                                           //CH557ʱ��ѡ�������
void mDelayuS(UINT16 n);                                 //��uSΪ��λ��ʱ
void mDelaymS(UINT16 n);                                 //��mSΪ��λ��ʱ
void mInitSTDIO(void);                                        //T1��Ϊ�����ʷ�����
void CH557UART0Alter(void);                                    //CH557����0����ӳ�䵽P0.2/P0.3
void CH557UART0SendByte(UINT8 SendDat);                    //CH557UART0����һ���ֽ�
UINT8 CH557UART0RcvByte(void);                                //CH557UART0��ѯ��ʽ����һ���ֽ�

void CH557WDTModeSelect(UINT8 mode);                       //CH557���Ź�ģʽ���� 
void CH557WDTFeed(UINT8 tim);                              //CH557���Ź�ι��
void CH557SoftReset(void);                                    //CH557����λ
  
#endif