/******************************************************************************/
/** \file system_CMT2380F32.c
 **
 ** System clock initialization.
 ** @link SampleGroup Some description @endlink
 **
 **   - 2016-02-16  1.0   First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "base_types.h"
#include "CMT2380F32.h"
#include "system_CMT2380F32.h"
#include "ddl.h"

/**
 ******************************************************************************
 ** System Clock Frequency (Core Clock) Variable according CMSIS
 ******************************************************************************/
uint32_t SystemCoreClock = 4000000;
//add clock source.
void SystemCoreClockUpdate (void) // Update SystemCoreClock variable
{
    SystemCoreClock = Clk_GetHClkFreq();
}

/**
 ******************************************************************************
 ** \brief  Setup the microcontroller system. Initialize the System and update
 ** the SystemCoreClock variable.
 **
 ** \param  none
 ** \return none
 ******************************************************************************/
void SystemInit(void)
{
    stc_clk_systickcfg_t stcCfg;

    // TODO load trim from flash and enable RCH
    //hcr 4MHz manual trim.
    Clk_Enable(ClkRCH, TRUE);
		//Clk_Enable(ClkXTH, TRUE);	
    //Clk_SetRCHFreq(ClkFreq16Mhz);                                            
    SystemCoreClockUpdate();
	
    DDL_ZERO_STRUCT(stcCfg);
    stcCfg.bNoRef = TRUE;
    stcCfg.u32LoadVal = 0xFFFFFF;
    Clk_SysTickConfig(&stcCfg);
		
}


