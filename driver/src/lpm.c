/******************************************************************************/
/** \file lpm.c
 **
 ** Common API of lpm.
 ** @link LpmGroup Some description @endlink
 **
 **   - 2017-06-06
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "lpm.h"
/**
 *******************************************************************************
 ** \addtogroup LpmGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
#define IS_VALID_SEVONPEND(x)           (SevPndDisable == (x) ||\
                                         SevPndEnable  == (x))
#define IS_VALID_SLEEPDEEP(x)           (SlpDpDisable  == (x) ||\
                                         SlpDpEnable   == (x))
#define IS_VALID_SLEEPONEXIT(x)         (SlpExtDisable == (x) ||\
                                         SlpExtEnable  == (x))                                         

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *****************************************************************************
 ** \brief 低功耗模式配置
 **
 **
 ** \param [in]  pstcConfig           低功耗模式配置结构体指针
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Lpm_Config(stc_lpm_config_t* pstcConfig)
{
    en_result_t enResult = Error;
  
    ASSERT(IS_VALID_SEVONPEND(pstcConfig->enSEVONPEND));
    ASSERT(IS_VALID_SLEEPDEEP(pstcConfig->enSLEEPDEEP));
    ASSERT(IS_VALID_SLEEPONEXIT(pstcConfig->enSLEEPONEXIT));
    
    SCB->SCR = pstcConfig->enSEVONPEND   ? (SCB->SCR | SCB_SCR_SEVONPEND_Msk)   : (SCB->SCR & ~SCB_SCR_SEVONPEND_Msk);
    SCB->SCR = pstcConfig->enSLEEPDEEP   ? (SCB->SCR | SCB_SCR_SLEEPDEEP_Msk)   : (SCB->SCR & ~SCB_SCR_SLEEPDEEP_Msk);
    SCB->SCR = pstcConfig->enSLEEPONEXIT ? (SCB->SCR | SCB_SCR_SLEEPONEXIT_Msk) : (SCB->SCR & ~SCB_SCR_SLEEPONEXIT_Msk);
       
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief 进入睡眠模式
 **
 **
 ** 
 ** \retval NULL                                     
 *****************************************************************************/
void Lpm_GotoLpmMode(void)
{
    __WFI();
}
                        
//@} // LpmGroup                                                                           

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
