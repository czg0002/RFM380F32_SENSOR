/******************************************************************************/
/** \file lvd.c
 **
 ** Low Voltage Detect driver API.
 ** @link Lvd Group Some description @endlink
 **
 **   - 2017-06-28     First Version
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "lvd.h"

/**
 ******************************************************************************
 ** \addtogroup LvdGroup
 ******************************************************************************/
//@{

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

#define IS_VALID_INPUT(x)       ( (x) <= LvdInputP25 )

#define IS_VALID_THRESHOLD(x)   ( (x) <= LvdTH3p3V )

#define IS_VALID_FILTER(x)      ( (x) <= LvdFilter29ms )

#define IS_VALID_IRQTYPE(x)     ( (x) <= LvdIrqFall )


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')        *
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
// static void LvdEnableNvic(void);
// static void LvdDisableNvic(void);
// static en_result_t LvdEnable(en_lvd_type_t enType, boolean_t bFlag);

/******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static func_ptr_t    pfnLvdIrqCbk;

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 *****************************************************************************/

 /**
 * \brief   
 *          LVD中断服务程序
 *
 * \param   [in]  u8Param  未使用
 *
 * \retval  无
 */
void Lvd_IRQHandler(uint8_t u8Param)
{
    M0P_LVD->IFR_f.INTF = 0u;
    if (NULL != pfnLvdIrqCbk)
    {
        pfnLvdIrqCbk();
    }
}

/**
 * \brief   
 *          使能NVIC中LVD中断
 *
 * \param   无
 *
 * \retval  无
 */
static void LvdEnableNvic(void)
{
    NVIC_ClearPendingIRQ(LVD_IRQn);
    NVIC_SetPriority(LVD_IRQn, DDL_IRQ_LEVEL_DEFAULT);
    NVIC_EnableIRQ(LVD_IRQn);
}

/**
 * \brief   
 *          除能NVIC中LVD中断
 *
 * \param   无
 *
 * \retval  无
 */
static void LvdDisableNvic(void)
{
    NVIC_ClearPendingIRQ(LVD_IRQn);
    NVIC_DisableIRQ(LVD_IRQn);
    NVIC_SetPriority(LVD_IRQn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 * \brief   
 *          使能LVD中断
 *
 * \param   [in]  enType  LVD中断类型
 *
 * \retval  en_result_t  Ok:  设置成功
 * \retval  en_result_t  ErrorInvalidParameter:  无效类型
 */
en_result_t Lvd_EnableIrq(en_lvd_irq_type_t enType)
{
    en_result_t enRet = Ok;

    if (enType > LvdIrqFall)
    {
        return ErrorInvalidParameter;
    }
    else
    {   switch (enType)
        {
            case LvdIrqHigh:
                M0P_LVD->CR_f.HTEN = 1u;
                M0P_LVD->CR_f.RTEN = 0u;
                M0P_LVD->CR_f.FTEN = 0u;
                break;
            case LvdIrqRise:
                M0P_LVD->CR_f.HTEN = 0u;
                M0P_LVD->CR_f.RTEN = 1u;
                M0P_LVD->CR_f.FTEN = 0u;
                break;
            case LvdIrqFall:
                M0P_LVD->CR_f.HTEN = 0u;
                M0P_LVD->CR_f.RTEN = 0u;
                M0P_LVD->CR_f.FTEN = 1u;
                break;
            default:
                break;
        }

        M0P_LVD->CR_f.IE = 1u;
        LvdEnableNvic();
    }
    return enRet;
}

/**
 * \brief   
 *          除能LVD中断
 *
 * \param   无
 *
 * \retval  无
 */
void Lvd_DisableIrq(void)
{
    LvdDisableNvic();
    M0P_LVD->CR_f.IE = 0u;
    M0P_LVD->CR_f.HTEN = 0u;
    M0P_LVD->CR_f.RTEN = 0u;
    M0P_LVD->CR_f.FTEN = 0u;
}

/**
 * \brief   
 *          LVD初始化
 *
 * \param   [in]  pstcConfig  LVD配置指针
 *
 * \retval  无
 */
void Lvd_Init(stc_lvd_config_t *pstcConfig)
{
    ASSERT(pstcConfig);
    ASSERT(IS_VALID_INPUT(pstcConfig->enInput));
    ASSERT(IS_VALID_THRESHOLD(pstcConfig->enThreshold));
    ASSERT(IS_VALID_FILTER(pstcConfig->enFilterTime));
    ASSERT(IS_VALID_IRQTYPE(pstcConfig->enIrqType));

    //NEED to DISABLE first.
    Lvd_Disable();
    Lvd_DisableIrq();
    LvdDisableNvic();

    M0P_LVD->CR_f.DEBOUNCE_TIME = pstcConfig->enFilterTime;
    M0P_LVD->CR_f.FLTEN = pstcConfig->bFilter;
    M0P_LVD->CR_f.VTDS = pstcConfig->enThreshold;
    M0P_LVD->CR_f.SOURCE_SEL = pstcConfig->enInput;
    M0P_LVD->CR_f.ACT = pstcConfig->bLvdReset;

    pfnLvdIrqCbk = pstcConfig->pfnIrqCbk;
}

/**
 * \brief   
 *          LVD deinit
 *
 * \param   无
 *
 * \retval  无
 */
void Lvd_DeInit(void)
{
    Lvd_DisableIrq();
    LvdDisableNvic();

    pfnLvdIrqCbk = NULL;
    Lvd_Disable();
}

/**
 * \brief   
 *          使能LVD
 *
 * \param   无
 *
 * \retval  无
 *
 */
void Lvd_Enable(void)
{
    M0P_LVD->CR_f.LVDEN = 1u;
}

/**
 * \brief   
 *          除能LVD
 *
 * \param   无
 *
 * \retval  无
 */
void Lvd_Disable(void)
{
    M0P_LVD->CR_f.LVDEN = 0u;
}

/**
 * \brief   
 *          获取LVD中断标志
 *
 * \param   无
 *
 * \retval  boolean_t  中断标志
 */
boolean_t Lvd_GetIntrStat(void)
{
    return M0P_LVD->IFR_f.INTF;

}

/**
 * \brief   
 *          清除LVD中断标志
 *
 * \param   无
 *
 * \retval  无
 */
void Lvd_ClearIrq(void)
{
    M0P_LVD->IFR_f.INTF = 0u;
}


//@} // LvdGroup

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

