/******************************************************************************/
/** \file lpt.c
 **
 ** Common API of Low Power timer.
 ** @link lptGroup Some description @endlink
 **
 **   - 2017-05-16
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "lpt.h"
/**
 *******************************************************************************
 ** \addtogroup LptGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static func_ptr_t pfnLpTimCallback = NULL;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *****************************************************************************
 ** \brief Low Power Timer 中断标志获取
 **
 **
 ** 
 ** 
 ** \retval TRUE or FALSE                                      
 *****************************************************************************/
boolean_t Lpt_GetIntFlag(void)
{
    boolean_t bRetVal = FALSE;
    
    bRetVal = M0P_LPTIMER->IFR_f.TF ? TRUE : FALSE;
    
    return bRetVal;
}

/**
 *****************************************************************************
 ** \brief Low Power Timer 中断标志清除
 **
 **
 ** 
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Lpt_ClearIntFlag(void)
{
    en_result_t enResult = Error;

    M0P_LPTIMER->ICLR_f.TFC = FALSE;
    enResult = Ok;

    return enResult;
}

/**
 *****************************************************************************
 ** \brief Low Power Timer 中断服务函数
 **
 **
 ** \param [in] u8Param         == 0
 **                                      
 *****************************************************************************/
void LpTim_IRQHandler(uint8_t u8Param)
{   
    pfnLpTimCallback();
}

/**
 *****************************************************************************
 ** \brief Low Power Timer 中断使能
 **
 **
 ** 
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Lpt_EnableIrq (void)
{
    en_result_t enResult = Error;
    
    M0P_LPTIMER->CR_f.IE = TRUE;
    enResult = Ok;
   
    return enResult;
}

/**
 *****************************************************************************
 ** \brief Low Power Timer 中断禁止
 **
 **
 ** 
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Lpt_DisableIrq(void)
{
    en_result_t enResult = Error;

    M0P_LPTIMER->CR_f.IE = FALSE;
    enResult = Ok;

    return enResult;
}

/**
 *****************************************************************************
 ** \brief Low Power Timer 初始化配置
 **
 **
 ** \param [in]  pstcConfig       初始化配置结构体指针
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Lpt_Init(stc_lpt_config_t* pstcConfig)
{
    en_result_t enResult = Error;  

    M0P_LPTIMER->CR_f.GATE_P  = pstcConfig->enGateP;
    M0P_LPTIMER->CR_f.GATE    = pstcConfig->enGate;
    M0P_LPTIMER->CR_f.TCK_SEL = pstcConfig->enTckSel;
    M0P_LPTIMER->CR_f.TOG_EN  = pstcConfig->enTog;
    M0P_LPTIMER->CR_f.CT      = pstcConfig->enCT;
    M0P_LPTIMER->CR_f.MD      = pstcConfig->enMD;
    
    pfnLpTimCallback          = pstcConfig->pfnLpTimCb;
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief Low Power Timer 启动运行
 **
 **
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Lpt_Run(void)
{
    en_result_t enResult = Error;
    
    M0P_LPTIMER->CR_f.TR = TRUE;
    enResult = Ok;
    
    return enResult;    
}

/**
 *****************************************************************************
 ** \brief Low Power Timer 停止运行
 **
 **
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Lpt_Stop(void)
{
    en_result_t enResult = Error;
    
    M0P_LPTIMER->CR_f.TR = FALSE;
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief Low Power Timer 重载值设置
 **
 **
 ** \param [in]  u16Data          16bits重载值
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Lpt_ARRSet(uint16_t u16Data)
{
    en_result_t enResult = Error;
    boolean_t bRetVal = FALSE;
    
    bRetVal = M0P_LPTIMER->CR_f.WT_FLAG ? TRUE : FALSE;
    if(TRUE == bRetVal)
    {
        M0P_LPTIMER->ARR_f.ARR = u16Data;
        enResult = Ok;
    }
    else  
    {
        enResult = Error;
    }
    return enResult; 
}

/**
 *****************************************************************************
 ** \brief Low Power Timer 16位计数值初值设置
 **
 **
 ** \param [in]  u16Data          16位初值
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Lpt_Cnt16Set(uint16_t u16Data)
{
    en_result_t enResult = Error;
  
    M0P_LPTIMER->CNT_f.CNT = u16Data;
    enResult = Ok;
    
    return enResult; 
}

/**
 *****************************************************************************
 ** \brief Low Power Timer 16位计数值获取
 **
 **
 ** 
 ** \retval 16bits计数值                                      
 *****************************************************************************/
uint16_t Lpt_Cnt16Get(void)
{
    uint16_t    u16CntData = 0;
  
    u16CntData = M0P_LPTIMER->CNT_f.CNT;
    
    return u16CntData; 
}

//@} // LptGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
