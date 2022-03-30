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
**名称：BaseTime0_Init  1MS
**功能：初始化基础定时器0
**入参：无
**出参：无
************************************/
void BaseTime0_Init(void)
{
	stc_bt_config_t BaseTime0Buf;
  //Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);	//开启时钟	
	BaseTime0Buf.enGateP=BtPositive;		
	BaseTime0Buf.enGate=BtGateDisable;	//禁止门控
	BaseTime0Buf.enPRS=BtPCLKDiv1;		//不分频
	BaseTime0Buf.enTog=BtTogDisable;	  //
	BaseTime0Buf.enCT=BtTimer;			//定时器功能
	BaseTime0Buf.enMD=BtMode2;			//16位自动重载定时
	BaseTime0Buf.pfnTim0Cb=Time0_Irq;		
	Bt_Init(TIM0,&BaseTime0Buf);

	Bt_Cnt16Set(TIM0,(65536-15957));	//初值
	Bt_ARRSet(TIM0,(65536-15957));		//重载值
	Bt_ClearIntFlag(TIM0);				//清标志
	Bt_EnableIrq(TIM0);					//开启中断
	EnableNvic(TIM0_IRQn,3,TRUE);
	Bt_Run(TIM0);						//启动
		
}
/******************************
**名字:MCU_Init
**功能:初始化,MCU配置
**入参:无
**出参:无
********************************/
void MCU_Init(void)
{
    MCU_Config();               //16M
    IO_Init();                  //IO
   BaseTime0_Init();           //T0   
   //  BaseTime1_Init();           //T0   
}





void BaseTime1_Init(void)
{
	stc_bt_config_t BaseTime1Buf;
  //Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);	//开启时钟	
	BaseTime1Buf.enGateP=BtPositive;		
	BaseTime1Buf.enGate=BtGateDisable;	//禁止门控
	BaseTime1Buf.enPRS=BtPCLKDiv1;		//不分频
	BaseTime1Buf.enTog=BtTogDisable;	  //
	BaseTime1Buf.enCT=BtTimer;			//定时器功能
	BaseTime1Buf.enMD=BtMode2;			//16位自动重载定时
	BaseTime1Buf.pfnTim1Cb=Time1_Irq;		
	Bt_Init(TIM1,&BaseTime1Buf);

	Bt_Cnt16Set(TIM1,(65536-15957));	//初值
	Bt_ARRSet(TIM1,(65536-15957));		//重载值
	Bt_ClearIntFlag(TIM1);				//清标志
	Bt_EnableIrq(TIM1);					//开启中断
	EnableNvic(TIM1_IRQn,4,TRUE);
	Bt_Run(TIM1);						//启动
		
}


/******************************
**名字:IO_Init
**功能:初始化MCU IO
**入参:无
**出参:无
********************************/
void IO_Init(void)
{
uint8_t i=0;	
	
		Clk_SetPeripheralGate(ClkPeripheralGpio,TRUE);	//开启GPIO时钟	
	  Clk_SetPeripheralGate(ClkPeripheralUart0,TRUE);//开启串口0时钟
    Clk_SetPeripheralGate(ClkPeripheralBt, TRUE);//开启基础时钟


		M0P_GPIO->P0DIR=P0DIR_Data;//端口P0 P1 P2 P3配置成输入输出配置寄存器
		M0P_GPIO->P1DIR=P1DIR_Data;
		M0P_GPIO->P2DIR=P2DIR_Data;
		M0P_GPIO->P3DIR=P3DIR_Data;
	
		M0P_GPIO->P0OUT=P0OUT_Data;//端口P0 P1 P2 P3配置成输出值配置寄存器
		M0P_GPIO->P1OUT=P1OUT_Data;
		M0P_GPIO->P2OUT=P2OUT_Data;
		M0P_GPIO->P3OUT=P3OUT_Data;
	
		M0P_GPIO->P0PU=P0PU_Data;//端口p0 p1 p2 p3 配置成上拉使能配置寄存器
		M0P_GPIO->P1PU=P1PU_Data;
		M0P_GPIO->P2PU=P2PU_Data;
		M0P_GPIO->P3PU=P3PU_Data;	
		//GPIO输入初始化
	//	Gpio_InitIOExt(2,3,GpioDirIn, TRUE,  FALSE,  FALSE, FALSE);  //输入上拉  学习按键
		//将中断触发功能
	//	Gpio_EnableIrq(2,3, GpioIrqFalling); //下降沿
	//	EnableNvic(PORT2_IRQn, DDL_IRQ_LEVEL_DEFAULT, TRUE);
}	

/******************************
**名字:MCU_Config
**功能:初始化配置MCU 16M
**入参:无
**出参:无
********************************/
void MCU_Config(void)
{
//	Clk_SetRCHFreq(ClkFreq16Mhz);//(ClkFreq16Mhz);	//16M
	Clk_SetHClkDiv(ClkDiv1);			//CPU时钟
	Clk_SetPClkDiv(ClkDiv1);			//外设时钟
}


//粗略延时
void Delay(word Cont)
{
	for(;Cont!=0;Cont--);
}

