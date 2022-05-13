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
#include "mcu_init.h"
#include "gpio.h"
#include "gpio_setting.h"

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
	M0P_GPIO->P1DIR = P1DIR_Data;
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
   *         Name:  MCU_Init
   *  Description:
   * =====================================================================================
   */
void MCU_Init(void)
{
	CLK_Init();
	IO_Init();
	return;
} /* -----  end of function MCU_Init  ----- */
