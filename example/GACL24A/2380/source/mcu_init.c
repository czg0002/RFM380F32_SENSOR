/*
 * =====================================================================================
 *
 *       Filename:  mcu_init.c
 *
 *    Description:  initialize cmt2380f32 peripheral, timer, interrupt settings
 *
 *        Version:  1.0
 *        Created:  2022/4/21 9:59:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Scott Chen (), czg0002@163.com
 *   Organization:  GOVA
 *
 * =====================================================================================
 */
#include <stdint.h>
#include "common.h"
#include "gpio.h"
#include "clk.h"
#include "i2c.h"
#include "rtc.h"
#include "gpio_setting.h"
#include "mcu_init.h"

uint32_t rtcCycled = 0;
static void RtcCycCb(void)
{
	rtcCycled += 1;
}
static void RtcAlarmCb(void)
{
	;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  CLK_Init
 *  Description:
 * =====================================================================================
 */
static void
CLK_Init(void)
{
	stc_clk_systickcfg_t stcCfg;
	int i;
	Clk_SetRCHFreq(ClkFreq24Mhz);

	Clk_SetHClkDiv(ClkDiv1);
	Clk_SetPClkDiv(ClkDiv1);

	// use RCL as systick clock. 
	Clk_Enable(ClkRCL, TRUE);
	// wait for RCL stable
	for (i = 0; i < 100000; i++)
	{
		if (TRUE == Clk_GetClkRdy(ClkRCL))
			break;
	}

	Clk_Enable(ClkXTL, TRUE);
	// wait for XTL stable
	for (i = 0; i < 100000; i++)
	{
		if (TRUE == Clk_GetClkRdy(ClkXTL))
			break;
	}

	Clk_SetPeripheralGate(ClkPeripheralLpTim, TRUE);
	Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE);
	// systick switch to RCL
	DDL_ZERO_STRUCT(stcCfg);
	stcCfg.bNoRef = FALSE;
	stcCfg.enClk = ClkRCL;
	stcCfg.u32LoadVal = 0x0;
	Clk_SysTickConfig(&stcCfg);

	//TODO: XTL as rtc clock
	return;
} /* -----  end of function CLK_Init  ----- */
  /*
   * ===  FUNCTION  ======================================================================
   *         Name:  IO_Init
   *  Description:  initialize io setting according to rfm380f32 internal connection
   *				  and external circuit connection
   * =====================================================================================
   */
static void
IO_Init(void)
{
	//Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE);

	M0P_GPIO->P0DIR = P0DIR_Data;
	M0P_GPIO->P1DIR = P1DIR_Data;		//NOTE: must set P14 to input for XTL to function
	M0P_GPIO->P2DIR = P2DIR_Data;
	M0P_GPIO->P3DIR = P3DIR_Data;

	M0P_GPIO->P0OUT = P0OUT_Data;
	M0P_GPIO->P1OUT = P1OUT_Data;
	M0P_GPIO->P2OUT = P2OUT_Data;
	M0P_GPIO->P3OUT = P3OUT_Data;

	M0P_GPIO->P0PU = P0PU_Data;
	M0P_GPIO->P1PU = P1PU_Data;
	M0P_GPIO->P2PU = P2PU_Data;
	M0P_GPIO->P3PU = P3PU_Data;
	return;
} /* -----  end of static function IO_Init  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  SystemRtc_Init
 *  Description:  ??????RTC???24??????????????????????????????????????????
 *				 TODO:  ??????????????????????????????????????????????????????
 * =====================================================================================
 */
static void SystemRtc_Init ( void )
{
#if 1
    stc_rtc_config_t 	stcRtcConfig; 
    stc_rtc_irq_cb_t 	stcIrqCb;
    stc_rtc_time_t  	stcTime;
    stc_rtc_alarmset_t 	stcAlarm;
    stc_rtc_cyc_sel_t   stcCycSel;
//    stc_lpm_config_t 	stcLpmCfg;
    
    DDL_ZERO_STRUCT(stcRtcConfig);
    DDL_ZERO_STRUCT(stcIrqCb);
    DDL_ZERO_STRUCT(stcAlarm);
    DDL_ZERO_STRUCT(stcTime);
    DDL_ZERO_STRUCT(stcCycSel);

    Clk_SetPeripheralGate(ClkPeripheralRtc,TRUE);		//????????rtc????????
    
    stcRtcConfig.enClkSel = RtcClk32768;				//RtcClkHxt1024;//RtcClk32;//
    stcRtcConfig.enAmpmSel = Rtc24h;					//Rtc12h;//
   

    stcCycSel.enCyc_sel = RtcPradx;
    stcCycSel.u8Prdx = 9u;  //set period to 5s.

    stcRtcConfig.pstcCycSel = &stcCycSel;


    Rtc_DisableFunc(RtcCount);
//    stcAlarm.u8Minute 	= 0x59;
//    stcAlarm.u8Hour 	= 0x12;
//    stcAlarm.u8Week 	= 0x08;
		stcAlarm.u8Minute 	= 0x99;
    stcAlarm.u8Hour 	= 0x99;
    stcAlarm.u8Week 	= 0x99;
    Rtc_DisableFunc(RtcAlarmEn);
    Rtc_EnAlarmIrq(Rtc_AlarmInt_Enable);
    Rtc_SetAlarmTime(&stcAlarm);
    Rtc_EnableFunc(RtcAlarmEn);

    stcTime.u8Year 	= 0x17;
    stcTime.u8Month = 0x06;
    stcTime.u8Day 	= 0x07;
    stcTime.u8Hour 	= 0x01;
    stcTime.u8Minute = 0x05;
    stcTime.u8Second = 0x05;
    stcTime.u8DayOfWeek = Rtc_CalWeek(&stcTime.u8Day);
    stcRtcConfig.pstcTimeDate = &stcTime;
    
    stcIrqCb.pfnAlarmIrqCb = RtcAlarmCb;
    stcIrqCb.pfnTimerIrqCb = RtcCycCb;
    stcRtcConfig.pstcIrqCb = &stcIrqCb;
    stcRtcConfig.bTouchNvic = TRUE;
    
    Rtc_DisableFunc(RtcCount);
    Rtc_Init(&stcRtcConfig); 
    
    Rtc_EnableFunc(RtcCount);
		//delay
//		Delay(800);
//		Clk_SetFunc(ClkFuncRtcLPWEn, TRUE);
#endif
	return ;
}		/* -----  end of static function RTC_Init  ----- */
  /*
   * ===  FUNCTION  ======================================================================
   *         Name:  MCU_Init
   *  Description:
   * =====================================================================================
   */
void MCU_Init(void)
{
	CLK_Init();	
	IO_Init();
	SystemRtc_Init();
	return;
} /* -----  end of function MCU_Init  ----- */
