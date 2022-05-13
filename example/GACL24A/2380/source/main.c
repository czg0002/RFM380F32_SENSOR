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
#include "gpio_setting.h"
#include "radio.h"
#include "syssleep.h"
#include "rfCmdProc.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
//#define DEBUG_PRINT

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
//uint8_t CheckFlg = 0, CoutNum = 0, Uartlen = 10;

SysState_e sysState = sysStateSleep;
//uint8_t u8RxFlg = 0;

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
	uint32_t i;
	EnumRFResult rxresult;
	uint8_t rxlen;
	MCU_Init();
	syssleep_init();
	SystemCoreClockUpdate();
	Gpio_SetIO(3, 3, 0);	//power control
	CMT2300A_Init();
	CMT2300A_GoSleep();
//	for (i = 0; i < 100; i++)
//	{
//		delay1ms(10);
//		Gpio_SetIO(3, 3, 0);
//		delay1ms(10);
//		Gpio_SetIO(3, 3, 1);
//	}
//	Gpio_SetIO(3, 3, 0);
	rxresult = RF_RxValidPacket(10000);
	if (rxresult == RF_RX_DONE)	//into wakeup state
	{
		rfCmdProc_processCmd();
//		RF_TxPacket(gTxPacket, 12, 10);	//just test call cmd
		sysState = sysStateWakeup;
	}
	else //sleep state
	{
		sysState = sysStateSleep;
	}
	while (1)
	{

		if (sysState == sysStateSleep)
		{
			syssleep_start(5);
			rxresult = RF_RxWakeupPacket(10);
			if (rxresult == RF_RX_DONE)
			{
				sysState = sysStateWakeup;
			}
		}
		else	//wakup state
		{
			rxresult = RF_RxValidPacket(20000);
			if (rxresult == RF_RX_DONE)	//into wakeup state
			{
				rfCmdProc_processCmd();
//				RF_TxPacket(gTxPacket, 12, 10);	//just test call cmd
			}
		}
	}
#if 0	
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
#endif
#if RF_STATUS
#if 0
	GO_STBY();
	CMT2300A_EnableReadFifo();
	Clr_INT();	//����ж�
	Clr_FIFO(); //���FIFO
	GO_RX();
	while (1)
	{
		unsigned char i = 0;
		uint32_t u32Data = 0, length = 0;
		uint8_t len;
		if (Test_GPIO2())
		{
			GO_STBY();
			gRxPacket[0] = Read_FIFO();
			length = gRxPacket[0];
			if (length >= 100 || length <= 0)
			{
				Gpio_SetIO(3, 3, 0);
			}
			else
			{
				for (i = 0; i < length; i++)
				{
					gRxPacket[1 + i] = Read_FIFO();
				}
				Gpio_SetIO(3, 3, 1);
			}
			GO_SLEEP();
			GO_STBY();
			Clr_INT();
			Clr_FIFO();
			GO_RX();
		}
	}
#endif
#if 0
	CMT2300A_GoStby();
	CMT2300A_EnableReadFifo();
	CMT2300A_EnableReadFifo();
	CMT2300A_ClearRxFifo();
	CMT2300A_GoRx();
	while (1)
	{
		unsigned char i = 0;
		uint32_t u32Data = 0, length = 0;
		uint8_t len;
		if (Test_GPIO2())
		{
			CMT2300A_GoStby();
			gRxPacket[0] = Read_FIFO();
			length = gRxPacket[0];
			if (length >= 100 || length <= 0)
			{
			}
			else
			{
				for (i = 0; i < length; i++)
				{
					gRxPacket[1 + i] = Read_FIFO();
				}
			}
			GO_SLEEP();
			GO_STBY();
			Clr_INT();
			Clr_FIFO();
			GO_RX();
		}
	}
#endif
//#if 0
//	while(1)
//	{
//		rxresult = RF_RxPacket(gRxPacket, &rxlen, 5000);
//		
//		if (rxresult == RF_RX_DONE)
//		{
//			Gpio_SetIO(3, 3, 1);
//		}
//		else
//		{
//			Gpio_SetIO(3, 3, 0);
//		}
//	}
//#endif
#else

	while (1)
	{
		// Send_Pack(gTxPacket, 8, 0);
		RF_TxPacket(gTxPacket, 8, 10);
		gTxPacket[2] += 1;
		delay1ms(1000);
	}
#endif
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
