/*
 * =====================================================================================
 *
 *       Filename:  syssleep.c
 *
 *    Description:  wrap sleep function for cmt2380f32 
 *
 *        Version:  1.0
 *        Created:  2022/5/10 15:36:34
 *       Revision:  none
 *       Compiler:  keil armcc
 *
 *         Author:  Scott Chen (), czg0002@163.com
 *   Organization:  GOVA
 *
 * =====================================================================================
 */
#include "lpt.h"
#include "lpm.h"
#include "rtc.h"
#include "syssleep.h"

SysState_e sysState = sysStateSleep;

//static void LptInt(void)
//{
//	if (TRUE == Lpt_GetIntFlag())
//	{
//		Lpt_ClearIntFlag();
//	}
//}

void syssleep_init(void)
{
#if 0
		stc_lpt_config_t stcConfig;
    en_result_t      enResult = Error;

    
    
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
#endif    
		stc_lpm_config_t stcLpmCfg;
    //Lpm Cfg
    stcLpmCfg.enSEVONPEND   = SevPndDisable;
    stcLpmCfg.enSLEEPDEEP   = SlpDpEnable;
    stcLpmCfg.enSLEEPONEXIT = SlpExtDisable;
    Lpm_Config(&stcLpmCfg);
}

void syssleep_start(void)
{
//		int i;
//	//Lpt 中断使能
//    Lpt_ClearIntFlag();
//    Lpt_EnableIrq();
//    EnableNvic(LPTIM_IRQn, 3, TRUE);
//    
//    
//    //设置重载值，计数初值，启动计数
//    Lpt_ARRSet(0x10000 - 36571);
//    Lpt_Run();
//    
//    for (i = 0; i < sec; i++)
//		{
//			//进入低功耗模式……
			Lpm_GotoLpmMode();
//			delay1ms(1);
//		}
//		Lpt_Stop();
	//TODO: use rtc interrupt to wakeup
}

void syssleep_setState(SysState_e new_state)
{
	sysState = new_state;
	if (new_state == sysStateSleep)
	{
//		Rtc_DisableFunc(RtcAlarmEn);
//    Rtc_EnAlarmIrq(Rtc_AlarmInt_Enable);
//    Rtc_EnableFunc(RtcAlarmEn);
		M0P_RTC->CR0_f.PRDSEL = 1;
	}
	else	//disable rtc period when wakeup
	{
//		Rtc_DisableFunc(RtcAlarmEn);
//    Rtc_EnAlarmIrq(Rtc_AlarmInt_Disable);
//		Rtc_EnableFunc(RtcAlarmEn);
		M0P_RTC->CR0_f.PRDSEL = 0;	// PRDS default to 0;
	}
}
