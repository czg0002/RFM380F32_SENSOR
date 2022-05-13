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
	rxresult = RF_RxValidPacket(10000);
	if (rxresult == RF_RX_DONE)	//into wakeup state
	{
		rfCmdProc_processCmd();
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
			}
		}
	}
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
