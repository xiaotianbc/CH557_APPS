/********************************** (C) COPYRIGHT *******************************
* File Name          : MAIN.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : ADC��ʼ����ADC�жϺͲ�ѯ��ʽ�ɼ���ʾʾ��
*******************************************************************************/                                                  
#include "Debug.H"
#include "ADC.H"

#pragma  NOAREGS

/* Global Variable */ 
UINT8 i;

#define ADC_INTERRUPT   0           //0:��ѯ��ʽ,1:�жϷ�ʽ

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Return         : None
*******************************************************************************/
void main(void) 
{
	mDelaymS(50);
	CfgFsys();                                                                //CH557ʱ��ѡ������       
	mInitSTDIO();                                                             //����0��ʼ��
	printf("CHIP_ID:%02x\n",(UINT16)CHIP_ID); 
	printf("EXAM ADC...\n");

	i = 1;
	ADCInit(2);

#if ADC_INTERRUPT    
	ADC_ChannelSelect(i);
	EA = 1;
	ADC_START();
	
#endif  
  
	while(1){
#if ADC_INTERRUPT == 0
		for(i=0; i<16;i++){
			ADC_ChannelSelect(i); 
			ADC_START(); 
			while(ADC_CTRL & bADC_START);
			printf("Channel:%02d %04d\n",(UINT16)i,ADC_DAT);
			mDelaymS(200);
		}
		
#endif		
	}
}

#if ADC_INTERRUPT
/*******************************************************************************
* Function Name  : ADCInterrupt
* Description    : ADC �жϷ������
* Input          : None
* Return         : None
*******************************************************************************/
void ADCInterrupt(void) interrupt INT_NO_ADC using 1                       //ADC�жϷ������,ʹ�üĴ�����1
{  
	ADCIF_CLEAR();                                                               //���ADC�жϱ�־

	printf("Channel:%02d %04d\n",(UINT16)i++,ADC_DAT);
	ADC_ChannelSelect(i); 
	ADC_START();
	if(i==16) i=0;
}

#endif