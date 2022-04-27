#include "CMT2380F32_Demo.h"
#include "Flash_ID.h"


typedef struct {
    uint8_t u8Port;
    uint8_t u8Pin;
}stc_gpio_list_t;

const stc_gpio_list_t gInputList[] = 
{
 	{ 2, 3 }, { 0, 1 },
};

const stc_gpio_list_t gIrqList[] = 
{
	{ 2, 3 }, { 0, 1 },
};


/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * \brief   Gpio_IRQHandler 
 *          
 *  
 * \param   [in]  u8Param            port index
 * \retval  void             
 */


void Gpio_IRQHandler(uint8_t u8Param)
{
    *((uint32_t *)((uint32_t)&M0P_GPIO->P0ICLR + u8Param * 0x40)) = 0;
}

/************************************
**���ƣ�BaseTime0_Init  1MS
**���ܣ���ʼ��������ʱ��0
**��Σ���
**���Σ���
************************************/
void BaseTime0_Init(void)
{
	stc_bt_config_t BaseTime0Buf;
  //Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);	//����ʱ��	
	BaseTime0Buf.enGateP=BtPositive;		
	BaseTime0Buf.enGate=BtGateDisable;	//��ֹ�ſ�
	BaseTime0Buf.enPRS=BtPCLKDiv1;		//����Ƶ
	BaseTime0Buf.enTog=BtTogDisable;	  //
	BaseTime0Buf.enCT=BtTimer;			//��ʱ������
	BaseTime0Buf.enMD=BtMode2;			//16λ�Զ����ض�ʱ
	BaseTime0Buf.pfnTim0Cb=Time0_Irq;		
	Bt_Init(TIM0,&BaseTime0Buf);

	Bt_Cnt16Set(TIM0,(65536-15957));	//��ֵ
	Bt_ARRSet(TIM0,(65536-15957));		//����ֵ
	Bt_ClearIntFlag(TIM0);				//���־
	Bt_EnableIrq(TIM0);					//�����ж�
	EnableNvic(TIM0_IRQn,3,TRUE);
	Bt_Run(TIM0);						//����
		
}


//void MCU_Init(void)
//{
//    MCU_Config();               //16M
//    IO_Init();                  //IO
//}


void BaseTime1_Init(void)
{
	stc_bt_config_t BaseTime1Buf;
  //Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);	//����ʱ��	
	BaseTime1Buf.enGateP=BtPositive;		
	BaseTime1Buf.enGate=BtGateDisable;	//��ֹ�ſ�
	BaseTime1Buf.enPRS=BtPCLKDiv1;		//����Ƶ
	BaseTime1Buf.enTog=BtTogDisable;	  //
	BaseTime1Buf.enCT=BtTimer;			//��ʱ������
	BaseTime1Buf.enMD=BtMode2;			//16λ�Զ����ض�ʱ
	BaseTime1Buf.pfnTim1Cb=Time1_Irq;		
	Bt_Init(TIM1,&BaseTime1Buf);

	Bt_Cnt16Set(TIM1,(65536-15957));	//��ֵ
	Bt_ARRSet(TIM1,(65536-15957));		//����ֵ
	Bt_ClearIntFlag(TIM1);				//���־
	Bt_EnableIrq(TIM1);					//�����ж�
	EnableNvic(TIM1_IRQn,4,TRUE);
	Bt_Run(TIM1);						//����
		
}



//void IO_Init(void)
//{
//uint8_t i=0;	
//	
//		Clk_SetPeripheralGate(ClkPeripheralGpio,TRUE);
//	  Clk_SetPeripheralGate(ClkPeripheralUart0,TRUE);
//    Clk_SetPeripheralGate(ClkPeripheralBt, TRUE);


//		M0P_GPIO->P0DIR=P0DIR_Data;
//		M0P_GPIO->P1DIR=P1DIR_Data;
//		M0P_GPIO->P2DIR=P2DIR_Data;
//		M0P_GPIO->P3DIR=P3DIR_Data;
//	
//		M0P_GPIO->P0OUT=P0OUT_Data;
//		M0P_GPIO->P1OUT=P1OUT_Data;
//		M0P_GPIO->P2OUT=P2OUT_Data;
//		M0P_GPIO->P3OUT=P3OUT_Data;
//	
//		M0P_GPIO->P0PU=P0PU_Data;
//		M0P_GPIO->P1PU=P1PU_Data;
//		M0P_GPIO->P2PU=P2PU_Data;
//		M0P_GPIO->P3PU=P3PU_Data;	
//}	


//void MCU_Config(void)
//{
////	Clk_SetRCHFreq(ClkFreq16Mhz);//(ClkFreq16Mhz);	//16M
//	Clk_SetHClkDiv(ClkDiv1);			//CPUʱ��
//	Clk_SetPClkDiv(ClkDiv1);			//����ʱ��
//}


//������ʱ
void Delay(word Cont)
{
	for(;Cont!=0;Cont--);
}

