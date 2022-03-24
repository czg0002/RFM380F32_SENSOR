/******************************************************************************/
/** \file wdt.c
 **
 ** WDT function driver API.
 ** @link WdtGroup Some description @endlink
 **
 **   - 2017-05-17  1.0   First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "wdt.h"

/**
 ******************************************************************************
 ** \defgroup WdtGroup
 **
 ******************************************************************************/
//@{

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
static func_ptr_t pfnWdtCallback;
/**
 ******************************************************************************
 ** \brief  WDT溢出时间设置函数
 **
 ** \param [in] u8LoadValue 溢出时间
 **
 ** \retval 无
 **
 ******************************************************************************/
void Wdt_WriteWdtLoad(uint8_t u8LoadValue)
{
    M0P_WDT->CON_f.WOV = u8LoadValue;
}
/**
 ******************************************************************************
 ** \brief  WDT初始化函数
 **
 ** \param [in] stcConfig 初始化结构
 **
 ** \retval Ok
 **
 ******************************************************************************/
en_result_t Wdt_Init(stc_wdt_config_t* pstcConfig)
{
    en_result_t enRet = Error;
    ASSERT(NULL != pstcConfig);
    Wdt_WriteWdtLoad(pstcConfig->u8LoadValue);
    pfnWdtCallback = pstcConfig->pfnWdtIrqCb;
    M0P_WDT->CON_f.WINT_EN = pstcConfig->enResetEnable;
    if(pstcConfig->enResetEnable)
    {
        EnableNvic(WDT_IRQn,DDL_IRQ_LEVEL_DEFAULT,TRUE);
    }
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  WDT复位及启动函数
 **
 ** \param [in] 无
 **
 ** \retval Ok
 **
 ******************************************************************************/
en_result_t Wdt_Start(void)
{
    en_result_t enRet = Error;
    M0P_WDT->RST = 0x1E;
    M0P_WDT->RST = 0xE1;
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  WDT复位及启动函数
 **
 ** \param [in] 无
 **
 ** \retval Ok
 **
 ******************************************************************************/
void Wdt_Feed(void) 
{
    M0P_WDT->RST = 0x1E;
    M0P_WDT->RST = 0xE1;  
}
/**
 ******************************************************************************
 ** \brief  WDT读取当前计数值函数
 **
 ** \param [in] 无
 **
 ** \retval 计数值
 **
 ******************************************************************************/
uint8_t Wdt_ReadWdtValue(void)
{
    uint8_t u8Count;
    u8Count = M0P_WDT->CON_f.WCNTL;
    return u8Count;
}
/**
 ******************************************************************************
 ** \brief  WDT读取当前运行状态
 **
 ** \param [in] 无
 **
 ** \retval 状态值
 **
 ******************************************************************************/
uint8_t Wdt_ReadwdtStatus(void)
{
    return M0P_WDT->CON_f.WDTR;
}
/**
 ******************************************************************************
 ** \brief  WDT中断处理函数
 **
 ** \param [in] 无
 **
 ** \retval 无
 **
 ******************************************************************************/
void Wdt_IRQHandler(void)
{
    if(M0P_WDT->CON_f.WDINT)
    {
        Wdt_Start();//clr wdt 标记
        if(NULL != pfnWdtCallback)
        {
            pfnWdtCallback();
        }
    }
}

//@} // WdtGroup
