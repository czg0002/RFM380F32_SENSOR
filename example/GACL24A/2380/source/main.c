/******************************************************************************/
/** \file main.c
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "gpio.h"
#include "ddl.h"
#include "uart.h"
#include "lpt.h"
#include "lpm.h"
#include "CMT2380F32_Demo.h"
#include "cmt2300a.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/
//#define DEBUG_PRINT

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint32_t u32LptTestFlag = 0x00;
/******************************************************************************
 * Local type definitions ('typedef')                                         
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void LptInt(void)
{
    if (TRUE == Lpt_GetIntFlag())
    {
        Lpt_ClearIntFlag();
        u32LptTestFlag = 0x01;
    }
}
/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
 uint8_t CheckFlg=0,CoutNum=0,Uartlen=10;
 
 #define MAX_UART 100 //���ڽ��ջ���
 uint8_t u8RxData[MAX_UART]={0xAA,0x55,0x11,0x22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
 uint8_t u8RxFlg=0;

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
 
/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample
 **
 ******************************************************************************/
int32_t main(void)
{
	int i;	
	MCU_Init(); 
	SystemCoreClockUpdate();
	CMT2300A_Init();
	CMT2300A_GoSleep();
	for (i = 0; i < 500; i++)
	{
		delay1ms(10);
		Gpio_SetIO(3,3,1);
		delay1ms(10);
		Gpio_SetIO(3,3,0);
	}
//	Clk_Enable(ClkXTL, TRUE);
//	Clk_SwitchTo(ClkXTL);
//	for (i = 0; i < 1000; i++)
//	{
//		delay1ms(10);
//		Gpio_SetIO(3,3,0);
//		delay1ms(10);
//		Gpio_SetIO(3,3,1);
//	}
	
	//test low power
	//使能RCL
    Clk_Enable(ClkRCL, TRUE);
    //使能Lpt、GPIO外设时钟
    Clk_SetPeripheralGate(ClkPeripheralLpTim, TRUE);
    Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE);
		{
		stc_lpt_config_t stcConfig;
    en_result_t      enResult = Error;
    uint16_t         u16ArrData = 0;
		stc_lpm_config_t stcLpmCfg;
    
    
    stcConfig.enGateP  = LptPositive; 
    stcConfig.enGate   = LptGateDisable;
    stcConfig.enTckSel = LptIRC32K;
    stcConfig.enTog    = LptTogDisable;
    stcConfig.enCT     = LptTimer;
    stcConfig.enMD     = LptMode2;
    
    stcConfig.pfnLpTimCb = LptInt;
    
    if (Ok != Lpt_Init(&stcConfig))
    {
        enResult = Error;
    }
    
    //Lpm Cfg
    stcLpmCfg.enSEVONPEND   = SevPndDisable;
    stcLpmCfg.enSLEEPDEEP   = SlpDpEnable;
    stcLpmCfg.enSLEEPONEXIT = SlpExtDisable;
    Lpm_Config(&stcLpmCfg);
    
    //Lpt 中断使能
    Lpt_ClearIntFlag();
    Lpt_EnableIrq();
    EnableNvic(LPTIM_IRQn, 3, TRUE);
    
    
    //设置重载值，计数初值，启动计数
    Lpt_ARRSet(u16ArrData);
    Lpt_Run();
    
    
    //进入低功耗模式……
    Lpm_GotoLpmMode();
 
    //低功耗模式下，继续计数，直到溢出产生中断，退出低功耗模式。
    while(1)
    {
        if (0x01 == u32LptTestFlag)
        {
            u32LptTestFlag = 0;
            Lpt_Stop();
            enResult = Ok;
            break;
        }
    }
		}
	//
	#if RF_STATUS  
	GO_STBY();	  
	CMT2300A_EnableReadFifo();
	Clr_INT(); //����ж�
	Clr_FIFO(); //���FIFO	
	GO_RX();
	#else 
	
	
	
	while(1)
		Send_Pack(u8RxData,8,0);
	#endif	
	while(1)
	 {		 
		 	#if RF_STATUS  	
			KeyScan_Rx();  
			#else 	
			 #endif	
	  }
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


