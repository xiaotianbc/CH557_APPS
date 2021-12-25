/********************************** (C) COPYRIGHT *******************************
* File Name          : Debug.C
* Author             : WCH
* Version            : V1.4
* Date               : 2021/12/15
* Description        : This file contains all the functions prototypes for UART
*                      Printf , Delay functions.									 
*******************************************************************************/
#include "Debug.H"

/*******************************************************************************
* Function Name  : CfgFsys
* Description    : CH557ʱ��ѡ������ú���,��λĬ��ʹ���ڲ�����Fsys=12MHz��
*                   FREQ_SYS����ͨ��CLOCK_CFG���õõ�����ʽ���£�
*                   Fsys = (Fosc * 4/(CLOCK_CFG & MASK_SYS_CK_SEL);����ʱ����Ҫ�Լ����� 
* Input          : None
* Return         : None
*******************************************************************************/ 
void CfgFsys(void)  
{
#if OSC_EN_XT	
	P7 = P7 & 0xF0 | 0x06;							  //�����ⲿ����ǰ,P7.0���ڵ͵�ƽ,P7.1��������״̬
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	CLOCK_CFG |= bOSC_EN_XT;                          //ʹ���ⲿ����
	mDelaymS(10);
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	CLOCK_CFG &= ~bOSC_EN_INT;                        //�ر��ڲ����� 
	SAFE_MOD = 0x00;
#endif	
	
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
#if FREQ_SYS == 48000000	
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x07;  // 48MHz	
	
#endif	
	
#if FREQ_SYS == 32000000	
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x06;  // 32MHz	
	
#endif
	
#if FREQ_SYS == 24000000	
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x05;  // 24MHz	
	
#endif	

#if FREQ_SYS == 16000000		
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x04;  // 16MHz
	
#endif

#if FREQ_SYS == 12000000		
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x03;  // 12MHz
	
#endif	

#if FREQ_SYS == 3000000	
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x02;  // 3MHz
	
#endif

#if FREQ_SYS == 750000	
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x01;  // 750KHz
	
#endif

#if FREQ_SYS == 187500	
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x00;  // 187.5KHz	
	
#endif

	SAFE_MOD = 0x00;
}

/*******************************************************************************
* Function Name  : mDelayus
* Description    : us��ʱ��������uSΪ��λ��ʱ
* Input          : n
* Return         : None
*******************************************************************************/ 
void mDelayuS(UINT16 n)  
{
#ifdef	FREQ_SYS
#if		FREQ_SYS <= 6000000
		n >>= 2;
#endif
#if		FREQ_SYS <= 3000000
		n >>= 2;
#endif
#if		FREQ_SYS <= 750000
		n >>= 4;
#endif
#endif
	while(n){  // total = 12~13 Fsys cycles, 1uS @Fsys=12MHz
		++ SAFE_MOD;  // 2 Fsys cycles, for higher Fsys, add operation here
#ifdef	FREQ_SYS
#if		FREQ_SYS >= 14000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 16000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 18000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 20000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 22000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 24000000
		++ SAFE_MOD;
#endif
#endif
		-- n;
	}
}

/*******************************************************************************
* Function Name  : mDelayms
* Description    : ms��ʱ��������mSΪ��λ��ʱ
* Input          : n
* Return         : None
*******************************************************************************/
void mDelaymS(UINT16 n)                                                    
{
	while(n){
		mDelayuS(1000);
		-- n;
	}
}                                         

/*******************************************************************************
* Function Name  : CH557UART0Alter
* Description    : CH557����0����ӳ��,����ӳ�䵽P0.2��P0.3
* Input          : None
* Return         : None
*******************************************************************************/
void CH557UART0Alter(void)
{
	PIN_FUNC |= bUART0_PIN_X;                                                 
}

/*******************************************************************************
* Function Name  : mInitSTDIO
* Description    : CH557����0��ʼ��,Ĭ��ʹ��T1�������ʷ�������Ҳ��ʹ��T2
*                   -T1��UART0�Ĳ����ʷ�������RCLK=0��TCLK=0
*                   -T2��UART0�Ĳ����ʷ�������RCLK=1��TCLK=1               
* Input          : None
* Return         : None
*******************************************************************************/
void mInitSTDIO(void)
{
	UINT32 x;
	UINT8 x2; 

	/* ����0ʹ��ģʽ1 */
	SM0 = 0;
	SM1 = 1;
	SM2 = 0;                                                                 
	
	/* ʹ��Timer1��Ϊ�����ʷ����� */
	RCLK = 0;                                                                  //UART0����ʱ��
	TCLK = 0;                                                                  //UART0����ʱ��
	
	PCON |= SMOD;
	x = 10 * FREQ_SYS / UART0_BUAD / 16;                                       //���������Ƶ��ע��x��ֵ��Ҫ���                            
	x2 = x % 10;
	x /= 10;
	if(x2 >= 5) x++;                                                       //��������

	TMOD = TMOD & ~ bT1_GATE & ~ bT1_CT & ~ MASK_T1_MOD | bT1_M1;              //0X20��Timer1��Ϊ8λ�Զ����ض�ʱ��
	T2MOD = T2MOD | bTMR_CLK | bT1_CLK;                                        //Timer1ʱ��ѡ��
	TH1 = 0-x;                                                                 //12MHz����,buad/12Ϊʵ�������ò�����
	TR1 = 1;                                                                   //������ʱ��1
	TI = 1;
	REN = 1;                                                                   //����0����ʹ��
}

/*******************************************************************************
* Function Name  : CH557UART0RcvByte
* Description    : CH557UART0����һ���ֽ�
* Input          : None
* Return         : SBUF
*******************************************************************************/
UINT8 CH557UART0RcvByte(void)
{
	while(RI == 0);                                                            //��ѯ���գ��жϷ�ʽ�ɲ���
	RI = 0;
	return SBUF;
}

/*******************************************************************************
* Function Name  : CH557UART0SendByte
* Description    : CH557UART0����һ���ֽ�
* Input          : SendDat��Ҫ���͵�����
* Return         : None
*******************************************************************************/
void CH557UART0SendByte(UINT8 SendDat)
{
	SBUF = SendDat;                                                              //��ѯ���ͣ��жϷ�ʽ�ɲ�������2�����,������ǰ��TI=0
	while(TI == 0);
	TI = 0;
}

/*******************************************************************************
* Function Name  : CH557WDTModeSelect
* Description    : CH557���Ź�ģʽѡ��
* Input          : mode��
*                   0��timer
*                   1��watchDog
* Return         : None
*******************************************************************************/
void CH557WDTModeSelect(UINT8 mode)
{
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xaa;                                                             //���밲ȫģʽ
	if(mode){
	 GLOBAL_CFG |= bWDOG_EN;                                                    //�������Ź���λ
	}
	else GLOBAL_CFG &= ~bWDOG_EN;	                                              //�������Ź�������Ϊ��ʱ��
	SAFE_MOD = 0x00;                                                             //�˳���ȫģʽ
	WDOG_COUNT = 0;                                                              //���Ź�����ֵ
}

/*******************************************************************************
* Function Name  : CH557WDTFeed
* Description    : CH557���Ź�ι��
*                  ���Ź���λʱ��(s) = (256-tim)/(Fsys/131072)
*                  00H(Fsys=12MHz)=2.8s
*                  80H(Fsys=12MHz)=1.4s
* Input          : tim�����Ź�������ֵ
* Return         : None
*******************************************************************************/
void CH557WDTFeed(UINT8 tim)
{
	WDOG_COUNT = tim;                                                             //���Ź���������ֵ	
}

/*******************************************************************************
* Function Name  : CH557SoftReset
* Description    : CH557����λ
* Input          : None
* Return         : None
*******************************************************************************/
void CH557SoftReset(void)
{
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	GLOBAL_CFG |= bSW_RESET;
}
