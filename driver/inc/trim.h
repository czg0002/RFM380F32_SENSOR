/*****************************************************************************/
/** \file trim.h
 **
 ** TRIM 数据结构及API声明
 **  
 **
 ** History:
 **   - 2017-05-10        V1.0
 **
 *****************************************************************************/

#ifndef __TRIM_H__
#define __TRIM_H__

/*****************************************************************************
 * Include files
 *****************************************************************************/
#include "ddl.h"
#include "interrupts_CMT2380F32.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup TrimGroup Clock Trimming (TRIM)
 **
 ******************************************************************************/
//@{

/******************************************************************************
 ** Global pre-processor symbols/macros ('#define')                            
 ******************************************************************************/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

 /**
 ******************************************************************************
 ** \brief 监测模式使能枚举重定义    (MON_EN)
 *****************************************************************************/
typedef enum en_trim_monitor
{
    MonDisable = 0u,            ///< 禁止
    MonEnable  = 1u,            ///< 使能
}en_trim_monitor_t; 
 
 /**
 ******************************************************************************
 ** \brief 待校准/监测时钟选择枚举重定义 (CALCLK_SEL)
 *****************************************************************************/
typedef enum en_trim_calclksel
{
    CALIRC16M  = 0u,            ///< 内部16M
    CALXTAL32M = 1u,            ///< 外部32M
    CALIRC32K  = 2u,            ///< 内部32K
    CALXTAL32K = 3u,            ///< 外部32K
}en_trim_calclksel_t;

/**
 ******************************************************************************
 ** \brief 参考时钟选择枚举重定义 (REFCLK_SEL)
 *****************************************************************************/
typedef enum en_trim_refclksel
{
    RefIRC16M  = 0u,            ///< 内部16M
    RefXTAL32M = 1u,            ///< 外部32M
    RefIRC32K  = 2u,            ///< 内部32K
    RefXTAL32K = 3u,            ///< 外部32K
    RefIRC10K  = 4u,            ///< 内部10K
    ExtClkIn   = 5u,            ///< 外部输入时钟
}en_trim_refclksel_t;

/**
 ******************************************************************************
 ** \brief 中断标志类型枚举重定义
 *****************************************************************************/
typedef enum en_trim_inttype
{
    RefStop      = 0u,            ///< 参考计数器停止标志
    CalCntOf     = 1u,            ///< 校准计数器停止标志
    XTAL32KFault = 2u,            ///< XTAL32K 失效标志
    XTAL32MFault = 3u,            ///< XTAL32M 失效标志
}en_trim_inttype_t;

/**
 ******************************************************************************
 ** \brief TRIM 配置结构体定义 
 *****************************************************************************/
typedef struct stc_trim_config
{
    en_trim_monitor_t    enMON;         ///< 监测模式
    en_trim_calclksel_t  enCALCLK;      ///< 校准时钟选择
    en_trim_refclksel_t  enREFCLK;      ///< 参考时钟选择
    
    func_ptr_t           pfnTrimCb;     ///< TRIM 中断服务回调函数[void function(void)]
}stc_trim_config_t;

/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 *****************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 *****************************************************************************/
//中断操作相关函数
//中断标志获取
boolean_t Trim_GetIntFlag(en_trim_inttype_t enIntType);
//中断标志清除
en_result_t Trim_ClearIntFlag(en_trim_inttype_t enIntType);
//中断使能/禁止
en_result_t Trim_EnableIrq (void);
en_result_t Trim_DisableIrq(void);

//功能配置及操作函数
//Trim 配置及初始化
en_result_t Trim_Init(stc_trim_config_t* pstcConfig);
//校准/监测启动/停止
en_result_t Trim_Run(void);
en_result_t Trim_Stop(void);
//参考计数器初值配置
en_result_t Trim_RefConSet(uint32_t u32Data);
//校准计数器初值配置
en_result_t Trim_CalConSet(uint32_t u32Data);
//参考计数器计数值获取
uint32_t Trim_RefCntGet(void);
//校准计数器计数值获取
uint32_t Trim_CalCntGet(void);

//@} // TrimGroup

#ifdef __cplusplus
#endif


#endif /* __TRIM_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/


