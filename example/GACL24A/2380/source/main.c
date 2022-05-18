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
#include "i2c.h"
//#include "CMT2380F32_Demo.h"
#include "flash.h"
#include "cmt2300a.h"
#include "gpio_setting.h"
#include "mcu_init.h"
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
extern boolean_t rtcCycled;
SysState_e sysState = sysStateSleep;

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
uint8_t u8Senddata[10] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0x00};
uint8_t u8Recdata[10]={0x00};
uint8_t u8DevAddr = 0x50;	//7 bit addr, left shit 1 in function para

int32_t main(void)
{
	uint32_t i;
	uint32_t wlen;
	uint32_t rlen;
	EnumRFResult rxresult;
	uint8_t rxlen;
	MCU_Init();
	syssleep_init();
	SystemCoreClockUpdate();
	Gpio_SetIO(3, 3, 0);	//power control
	CMT2300A_Init();
	CMT2300A_GoSleep();

#if 0
	//test i2c function
	{
		stc_i2c_config_t stcI2cCfg;
		DDL_ZERO_STRUCT(stcI2cCfg);
		Gpio_InitIOExt(2,5,GpioDirOut,FALSE,FALSE,TRUE,FALSE);   
    Gpio_InitIOExt(2,6,GpioDirOut,FALSE,FALSE,TRUE,FALSE);
		Gpio_SetFunc_I2CDAT_P25(); 
    Gpio_SetFunc_I2CCLK_P26();
    Clk_SetPeripheralGate(ClkPeripheralI2c,TRUE);
		
		stcI2cCfg.enFunc = I2cBaud_En;
    stcI2cCfg.u8Tm = 19;//0x04;//400k
    stcI2cCfg.pfnI2cCb = NULL;
    stcI2cCfg.bTouchNvic = FALSE;
    
		I2C_DeInit();  
    I2C_Init(&stcI2cCfg);
    I2C_SetFunc(I2cHlm_En);
    I2C_SetFunc(I2cMode_En);
		wlen = 3;
		rlen = 3;
		I2C_MasterWriteEepromData((u8DevAddr<<1),0x00,&u8Senddata[0],wlen);
    delay1ms(100);
    I2C_MasterReadEepromData((u8DevAddr<<1),0x00,&u8Recdata[0],rlen);
	}
#endif
	rxresult = RF_RxValidPacket(15000);
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
			RF_TxPacket(gTxPayload, 12, 20);
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
			//test rtc function.
			/*
			while (1)
			{
				if (rtcCycled == TRUE)
				{
					RF_TxPacket(gTxPayload, 12, 20);
					rtcCycled = FALSE;
				}
			}
			*/
		}
	}
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
