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
#include "flashData.h"
#include "cmt2300a.h"
#include "gpio_setting.h"
#include "mcu_init.h"
#include "radio.h"
#include "syssleep.h"
#include "rfCmdProc.h"
#include "rtc.h"
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
stc_rtc_time_t stcReadTime;
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
uint8_t u8Senddata[10] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00};
uint8_t u8Recdata[10] = {0x00};
// uint8_t u8DevAddr = 0x50;	//7 bit addr, left shit 1 in function para

int32_t main(void)
{
	uint32_t i;
	uint32_t wlen;
	uint32_t rlen;
	uint16_t count = 0;
	EnumRFResult rxresult;
	uint8_t rxlen;
	//--------MCU_Init very important, may damage mcu, offline download cannot rescure??-----
	MCU_Init();
	//---------------------
//	while (1)
//	{
//		Rtc_ReadDateTime(&stcReadTime);
//	}
	flashData_Init();
	syssleep_init();
	SystemCoreClockUpdate();
	GPIO_EXTPOWER_OFF();
	CMT2300A_Init();
	CMT2300A_GoSleep();
	//	for (i = 0; i < 50; i++)
	RF_TxPacket(gTxPayload, 12, 20);
#if 1
	// test i2c function
	{
		stc_i2c_config_t stcI2cCfg;
		DDL_ZERO_STRUCT(stcI2cCfg);
		Gpio_InitIOExt(3, 5, GpioDirOut, FALSE, FALSE, TRUE, FALSE); // P2.5, P2.6 for eeprom, P3.5, P3.6 for GHXT1 and 2862, P0.1 P0.2 for GHXT2 and GHXT3
		Gpio_InitIOExt(3, 6, GpioDirOut, FALSE, FALSE, TRUE, FALSE);
		//		Gpio_InitIOExt(3,5,GpioDirOut,FALSE,FALSE,TRUE,FALSE);  //P2.5, P2.6 for eeprom, P3.5, P3.6 for GHXT1 and 2862, P0.1 P0.2 for GHXT2 and GHXT3
		//    Gpio_InitIOExt(3,6,GpioDirOut,FALSE,FALSE,TRUE,FALSE);
		Gpio_InitIOExt(0, 1, GpioDirOut, FALSE, FALSE, TRUE, FALSE); // P2.5, P2.6 for eeprom, P3.5, P3.6 for GHXT1 and 2862, P0.1 P0.2 for GHXT2 and GHXT3
		Gpio_InitIOExt(0, 2, GpioDirOut, FALSE, FALSE, TRUE, FALSE);
		//		Gpio_SetFunc_I2CDAT_P35();
		//    Gpio_SetFunc_I2CCLK_P36();
		Clk_SetPeripheralGate(ClkPeripheralI2c, TRUE);

		stcI2cCfg.enFunc = I2cBaud_En;
		stcI2cCfg.u8Tm = 19; // 0x04;//400k
		stcI2cCfg.pfnI2cCb = NULL;
		stcI2cCfg.bTouchNvic = FALSE;

		I2C_DeInit();
		I2C_Init(&stcI2cCfg);
		//    I2C_SetFunc(I2cHlm_En);
		//    I2C_SetFunc(I2cMode_En);

		//		wlen = 3;
		//		rlen = 3;
		//		I2C_MasterWriteEepromData((u8DevAddr<<1),0x00,&u8Senddata[0],wlen);
		//    delay1ms(100);
		//    I2C_MasterReadEepromData((u8DevAddr<<1),0x00,&u8Recdata[0],rlen);
	}
#endif
	rxresult = RF_RxValidPacket(10000);
	Rtc_ReadDateTime(&stcReadTime);
	
	if (rxresult == RF_RX_DONE) // into wakeup state, except sleep cmd
	{
		sysState = sysStateWakeup;
		rfCmdProc_processCmd();
	}
	else // sleep state
	{
		sysState = sysStateSleep;
	}
	//	sysState = sysStateWakeup;	//TO BE REMOVED, just for test
	while (1)
	{
		if (sysState == sysStateSleep)
		{
			syssleep_start(gFactoryCfg.sleepIntervalSeconds);
#if 1
			Rtc_ReadDateTime(&stcReadTime);
			gTxPayload[7] = 0x03;	//tmp just for mark
			gTxPayload[8] = stcReadTime.u8Second;
			gTxPayload[9] = stcReadTime.u8Minute;
			gTxPayload[10] = stcReadTime.u8Hour;
			gTxPayload[11] = 0;
			gTxPayload[12] = 0;
			RF_TxPacket(gTxPayload, 13, 20);
			rxresult = RF_RxWakeupPacket(10);
			if (rxresult == RF_RX_DONE)
			{
				sysState = sysStateWakeup;
			}
#endif
		}
		else // wakup state
		{
			rxresult = RF_RxValidPacket((uint32_t)gFactoryCfg.idleTimeoutMin * 1000);
			if (rxresult == RF_RX_DONE) // into wakeup state
			{
				rfCmdProc_processCmd();
			}
		}
	}
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
