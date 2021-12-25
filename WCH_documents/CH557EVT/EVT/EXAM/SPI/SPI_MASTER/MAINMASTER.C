/********************************** (C) COPYRIGHT *******************************
* File Name          : MAINSLAVE.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : CH557 SPI����������ʾ������SPI�ӻ����������շ�
*******************************************************************************/
#include "CH557X.H"
#include "Debug.H"
#include "SPI.H"

/*Ӳ���ӿڶ���*/
/******************************************************************************
ʹ��CH557 Ӳ��SPI�ӿ�
         CH557
         P1.4����SCS
         P1.5����MOSI
         P1.6����MISO
         P1.7����SCK
*******************************************************************************/

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Return         : None
*******************************************************************************/
void main(void)
{
	UINT8 ret,i=0;
	CfgFsys();
	mDelaymS(5);                                                               //�޸�ϵͳ��Ƶ�������Լ���ʱ�ȴ���Ƶ�ȶ�
	mInitSTDIO();                                                             //����0��ʼ��
	printf("EXAM SPI Master\n");
	SPIMasterModeSet(3);                                                       //SPI����ģʽ���ã�ģʽ3
	SPI_CK_SET(24);                                                            //24��Ƶ

	while(1)
	{
		if(i>0xff) i = 0;
			
		SCS = 0;                                                               //SPI������������
		CH557SPIMasterWrite(i);
		ret = CH557SPIMasterRead();                                            //����SPI�ӻ����ص�����	
		SCS = 1;
		
		if(ret != (i^0xff))
		{
			printf("Err: %02X  %02X  \n",(UINT16)i,(UINT16)ret);               //��������ڷ��͵����ݣ���ӡ������Ϣ
		}
		else
		{
			printf("success %02x\n",(UINT16)i);                                //ÿ�ɹ�һ�δ�ӡһ��
		}
		i = i+1;

		mDelaymS(80);
	}
}

