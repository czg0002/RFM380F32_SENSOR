/******************************************************************************/
/** \file trim.c
 **
 ** Common API of trim.
 ** @link trimGroup Some description @endlink
 **
 **   - 2017-05-16
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "trim.h"
/**
 *******************************************************************************
 ** \addtogroup TrimGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
#define IS_VALID_TRIMINT(x)             (RefStop      == (x) ||\
                                         CalCntOf     == (x) ||\
                                         XTAL32KFault == (x) ||\
                                         XTAL32MFault == (x))


/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static func_ptr_t pfnTrimCallback = NULL;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *****************************************************************************
 ** \brief Trim中断标志获取
 **
 **
 ** \param [in]  enIntType           中断类型（RefStop、CalCntOf、XTAL32KFault、XTAL32MFault）
 ** 
 ** \retval TRUE or FALSE                                      
 *****************************************************************************/
boolean_t Trim_GetIntFlag(en_trim_inttype_t enIntType)
{
    boolean_t bRetVal = FALSE;
  
    ASSERT(IS_VALID_TRIMINT(enIntType));
    
    switch (enIntType)
    {
        case RefStop:
            bRetVal = M0P_CLK_TRIM->IFR_f.STOP ? TRUE : FALSE;
            break;    
        case CalCntOf: 
            bRetVal = M0P_CLK_TRIM->IFR_f.CALCNT_OF ? TRUE : FALSE;
            break;    
        case XTAL32KFault: 
            bRetVal = M0P_CLK_TRIM->IFR_f.XTL_FAULT ? TRUE : FALSE;
            break;    
        case XTAL32MFault: 
            bRetVal = M0P_CLK_TRIM->IFR_f.XTH_FAULT ? TRUE : FALSE;
            break;    
        default:
            bRetVal = FALSE;
            break;
    }
    
    return bRetVal;     
}

/**
 *****************************************************************************
 ** \brief Trim中断标志清除
 **
 **
 ** \param [in]  enIntType           中断类型（RefStop、CalCntOf、XTAL32KFault、XTAL32MFault）
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_ClearIntFlag(en_trim_inttype_t enIntType)
{
    en_result_t enResult = Error;
  
    ASSERT(IS_VALID_TRIMINT(enIntType));
    
    switch (enIntType)
    {
        case RefStop:
            M0P_CLK_TRIM->CR_f.TRIM_START = FALSE;
            enResult = Ok;
            break;    
        case CalCntOf:
            M0P_CLK_TRIM->CR_f.TRIM_START = FALSE;
            enResult = Ok;
            break;    
        case XTAL32KFault:
            M0P_CLK_TRIM->ICLR_f.XTL_FAULT_CLR = FALSE;
            enResult = Ok;
            break;    
        case XTAL32MFault: 
            M0P_CLK_TRIM->ICLR_f.XTH_FAULT_CLR = FALSE;
            enResult = Ok;
            break;    
        default:
            enResult = Error;
            break;
    }
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief Trim中断服务函数
 **
 **
 ** \param [in]  u8Param           == 0
 ** 
 ** \retval NULL                                     
 *****************************************************************************/
void ClkTrim_IRQHandler(uint8_t u8Param)
{
    pfnTrimCallback();
}

/**
 *****************************************************************************
 ** \brief Trim中断使能
 **
 **
 **
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_EnableIrq (void)
{
    en_result_t enResult = Error;

    M0P_CLK_TRIM->CR_f.IE = TRUE;
    enResult = Ok;
    
    return enResult; 
}

/**
 *****************************************************************************
 ** \brief Trim中断禁止
 **
 **
 **
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_DisableIrq(void)
{
    en_result_t enResult = Error;

    M0P_CLK_TRIM->CR_f.IE = FALSE;
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief Trim初始化配置
 **
 **
 ** \param [in]  pstcConfig           Trim配置结构体指针
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_Init(stc_trim_config_t* pstcConfig)
{
    en_result_t enResult = Error;

    M0P_CLK_TRIM->CR_f.MON_EN     = pstcConfig->enMON;
    M0P_CLK_TRIM->CR_f.CALCLK_SEL = pstcConfig->enCALCLK;
    M0P_CLK_TRIM->CR_f.REFCLK_SEL = pstcConfig->enREFCLK;
    
    pfnTrimCallback               = pstcConfig->pfnTrimCb;
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief Trim校准/监测启动运行
 **
 **
 **
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_Run(void)
{
    en_result_t enResult = Error;

    M0P_CLK_TRIM->CR_f.TRIM_START = TRUE;
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief Trim校准/监测停止
 **
 **
 **
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_Stop(void)
{
    en_result_t enResult = Error;

    M0P_CLK_TRIM->CR_f.TRIM_START = FALSE;
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief Trim参考计数器初值配置
 **
 **
 ** \param [in]  u32Data           参考计数器初值
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_RefConSet(uint32_t u32Data)
{
    en_result_t enResult = Error;

    M0P_CLK_TRIM->REFCON_f.RCNTVAL = u32Data;
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief Trim校准计数器初值配置
 **
 **
 ** \param [in]  u32Data           校准计数器初值
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_CalConSet(uint32_t u32Data)
{
    en_result_t enResult = Error;

    M0P_CLK_TRIM->CALCON_f.CCNTVAL= u32Data;
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief Trim参考计数器计数值获取
 **
 **
 ** 
 ** 
 ** \retval u32Data                参考计数器计数值                      
 *****************************************************************************/
uint32_t Trim_RefCntGet(void)
{
    uint32_t u32Data = 0;

    u32Data = M0P_CLK_TRIM->REFCNT_f.REFCNT;
    
    return u32Data;
}

/**
 *****************************************************************************
 ** \brief Trim校准计数器计数值获取
 **
 **
 ** 
 ** 
 ** \retval u32Data                校准计数器计数值                      
 *****************************************************************************/
uint32_t Trim_CalCntGet(void)
{
    uint32_t u32Data = 0;

    u32Data = M0P_CLK_TRIM->CALCNT_f.CALCNT;
    
    return u32Data;
}

//@} // TrimGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
