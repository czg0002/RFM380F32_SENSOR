/******************************************************************************/
/** \file adt.h
 **
 ** Headerfile for Advance Timer functions
 ** @link ADT Group Some description @endlink
 **
 **   - 2017-06-16     First Version
 **
 ******************************************************************************/

#ifndef __ADT_H__
#define __ADT_H__

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "interrupts_CMT2380F32.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup AdtGroup Advance Timer (ADT)
  **
 ******************************************************************************/
//@{

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

 /**
 ******************************************************************************
 ** \brief 高性能定时器选择
 *****************************************************************************/
typedef enum en_adt_unit
{
    AdTIM4    = 0u,            ///< TIM4
    AdTIM5    = 1u,            ///< TIM5
    AdTIM6    = 2u,            ///< TIM6
}en_adt_unit_t;

 /**
 ******************************************************************************
 ** \brief ADT CHx端口定义
 *****************************************************************************/
typedef enum en_adt_CHxX_port
{
    AdtCHxA    = 0u,            ///< CHx A端口
    AdtCHxB    = 1u,            ///< CHx B端口
}en_adt_CHxX_port_t;

 /**
 ******************************************************************************
 ** \brief ADT TRIG端口定义
 *****************************************************************************/
typedef enum en_adt_trig_port
{
    AdtTrigA    = 0u,            ///< TIMx 触发A端口
    AdtTrigB    = 1u,            ///< TIMx 触发B端口
    AdtTrigC    = 2u,            ///< TIMx 触发C端口
    AdtTrigD    = 3u,            ///< TIMx 触发D端口
}en_adt_trig_port_t;

/**
 ******************************************************************************
 ** \brief ADT通用控制 - Z相输入屏蔽周期数
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_gconr_zmsk
{
    AdtZMaskDis,                ///< Z相输入屏蔽功能无效
    AdtZMask4Cyl,               ///< 位置计数上溢后或下溢后的4个计数周期内的Z相输入被屏蔽
    AdtZMask8Cyl,               ///< 位置计数上溢后或下溢后的8个计数周期内的Z相输入被屏蔽
    AdtZMask16Cyl,              ///< 位置计数上溢后或下溢后的16个计数周期内的Z相输入被屏蔽
}en_adt_gconr_zmsk_t;

/**
 ******************************************************************************
 ** \brief ADT通用控制 - 计数时钟选择
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_cnt_ckdiv
{
    AdtClkPClk0,                ///< PCLK0
    AdtClkPClk0Div2,            ///< PCLK0/2
    AdtClkPClk0Div4,            ///< PCLK0/4
    AdtClkPClk0Div8,            ///< PCLK0/8
    AdtClkPClk0Div16,           ///< PCLK0/16
    AdtClkPClk0Div64,           ///< PCLK0/64
    AdtClkPClk0Div256,          ///< PCLK0/256
    AdtClkPClk0Div1024,         ///< PCLK0/1024
}en_adt_cnt_ckdiv_t;

/**
 ******************************************************************************
 ** \brief ADT计数模式
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_cnt_mode
{
    AdtSawtoothMode = 0,            ///< 锯齿波模式
    AdtTriangleModeA = 4,           ///< 三角波A模式
    AdtTriangleModeB = 5,           ///< 三角波B模式
}en_adt_cnt_mode_t;

/**
 ******************************************************************************
 ** \brief ADT计数方向
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_cnt_dir
{
    AdtCntDown,         ///< 递减计数
    AdtCntUp,           ///< 递加计数
}en_adt_cnt_dir_t;

/**
 ******************************************************************************
 ** \brief ADT比较基准
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_compare
{
    AdtCompareA,            ///< 通用比较基准A
    AdtCompareB,            ///< 通用比较基准B
    AdtCompareC,            ///< 通用比较基准C
    AdtCompareD,            ///< 通用比较基准D
}en_adt_compare_t;

/**
 ******************************************************************************
 ** \brief ADT端口控制 - TIMx输出状态控制
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_pconr_disval
{
    AdtTIMxDisValNorm,          ///< 强制输出无效条件0~3中被选择的条件成立时，CHx端口正常输出
    AdtTIMxDisValHiZ,           ///< 强制输出无效条件0~3中被选择的条件成立时，CHx端口输出高阻态
    AdtTIMxDisValLow,           ///< 强制输出无效条件0~3中被选择的条件成立时，CHx端口输出低电平
    AdtTIMxDisValHigh,          ///< 强制输出无效条件0~3中被选择的条件成立时，CHx端口输出高电平
}en_adt_pconr_disval_t;

/**
 ******************************************************************************
 ** \brief ADT端口控制 - CHx强制输出无效条件选择
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_pconr_dissel
{
    AdtCHxDisSel0,             ///< 选择强制输出无效条件0
    AdtCHxDisSel1,             ///< 选择强制输出无效条件1
    AdtCHxDisSel2,             ///< 选择强制输出无效条件2
    AdtCHxDisSel3,             ///< 选择强制输出无效条件3
}en_adt_pconr_dissel_t;

/**
 ******************************************************************************
 ** \brief ADT端口控制 - CHx周期值匹配时端口状态设定
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_pconr_perc
{
    AdtCHxPeriodLow,           ///< 计数器计数值与周期值相等时，CHx端口输出保持为低电平
    AdtCHxPeriodHigh,          ///< 计数器计数值与周期值相等时，CHx端口输出设定为高电平
    AdtCHxPeriodKeep,          ///< 计数器计数值与周期值相等时，CHx端口输出设定为先前状态
    AdtCHxPeriodInv,           ///< 计数器计数值与周期值相等时，CHx端口输出设定为反转电平
}en_adt_pconr_perc_t;

/**
 ******************************************************************************
 ** \brief ADT端口控制 - CHx比较值匹配时端口状态设定
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_pconr_cmpc
{
    AdtCHxCompareLow,          ///< 计数器计数值与GCMxR相等时，CHx端口输出保持为低电平
    AdtCHxCompareHigh,         ///< 计数器计数值与GCMxR相等时，CHx端口输出设定为高电平
    AdtCHxCompareKeep,         ///< 计数器计数值与GCMxR相等时，CHx端口输出设定为先前状态
    AdtCHxCompareInv,          ///< 计数器计数值与GCMxR相等时，CHx端口输出设定为反转电平
}en_adt_pconr_cmpc_t;

/**
 ******************************************************************************
 ** \brief ADT端口控制 - CHx端口输出
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_pconr_port_out
{
    AdtCHxPortOutLow,      ///< CHx端口输出设定为低电平
    AdtCHxPortOutHigh,     ///< CHx端口输出设定为高电平
}en_adt_pconr_port_out_t;

/**
 ******************************************************************************
 ** \brief ADT端口控制 - CHx端口功能模式选择
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_pconr_capc
{
    AdtCHxCompareOutput,       ///< CHx端口设定为比较输出功能
    AdtCHxCompareInput,        ///< CHx端口设定为捕获输入功能
}en_adt_pconr_capc_t;

/**
 ******************************************************************************
 ** \brief ADT端口控制 - CHx计数开始停止端口状态选择
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_pconr_stastps
{
    AdtCHxStateSelSS,      ///< 计数开始或停止时，CHx端口输出由STACB、STPCB决定
    AdtCHxStateSelKeep,    ///< 计数开始或停止时，CHx端口输出设定为先前状态
}en_adt_pconr_stastps_t;

/**
 ******************************************************************************
 ** \brief ADT死区控制 - CHx死区分离设定
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_dconr_sepa
{
    AdtCHxDtSeperate,      ///< DTUAR和DTDAR分别设定
    AdtCHxDtEqual,         ///< DTDAR的值和DTUAR的值自动相等
}en_adt_dconr_sepa_t;

/**
 ******************************************************************************
 ** \brief ADT滤波控制 - TRIx/TIMxIx端口滤波采样基准时钟选择
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_fconr_nofick
{
    AdtFltClkPclk0,         ///< PCLK0
    AdtFltClkPclk0Div4,     ///< PCLK0/4
    AdtFltClkPclk0Div16,    ///< PCLK0/16
    AdtFltClkPclk0Div64,    ///< PCLK0/64
}en_adt_fconr_nofick_t;

/**
 ******************************************************************************
 ** \brief ADT有效周期 - TIMx有效周期选择
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_vperr_pcnts
{
    AdtPeriodCnts0,         ///< 有效周期选择功能无效
    AdtPeriodCnts1,         ///< 每隔1个周期有效一次
    AdtPeriodCnts2,         ///< 每隔2个周期有效一次
    AdtPeriodCnts3,         ///< 每隔3个周期有效一次
    AdtPeriodCnts4,         ///< 每隔4个周期有效一次
    AdtPeriodCnts5,         ///< 每隔5个周期有效一次
    AdtPeriodCnts6,         ///< 每隔6个周期有效一次
    AdtPeriodCnts7,         ///< 每隔7个周期有效一次
}en_adt_vperr_pcnts_t;

/**
 ******************************************************************************
 ** \brief ADT有效周期 - 计数条件选择
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_vperr_pcnte
{
    AdtPeriodCnteDisable,       ///< 有效周期选择功能无效
    AdtPeriodCnteMin,           ///< 锯齿波计数上、下溢点或三角波波谷做为计数条件
    AdtPeriodCnteMax,           ///< 锯齿波计数上、下溢点或三角波波峰做为计数条件
    AdtPeriodCnteBoth,          ///< 锯齿波计数上、下溢点或三角波波峰，波谷做为计数条件
}en_adt_vperr_pcnte_t;

/**
 ******************************************************************************
 ** \brief ADT端口触发控制 - 触发源选择
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_ttrig_trigxs
{
    AdtTrigxSelP01,         ///< P01
    AdtTrigxSelP02,         ///< P02
    AdtTrigxSelP03,         ///< P03
    AdtTrigxSelP15,         ///< P15
    AdtTrigxSelP14,         ///< P14
    AdtTrigxSelP23,         ///< P23
    AdtTrigxSelP24,         ///< P24
    AdtTrigxSelP25,         ///< P25
    AdtTrigxSelP26,         ///< P26
    AdtTrigxSelP27,         ///< P27
    AdtTrigxSelP31,         ///< P31
    AdtTrigxSelP32,         ///< P32
    AdtTrigxSelP33,         ///< P33
    AdtTrigxSelP34,         ///< P34
    AdtTrigxSelP35,         ///< P35
    AdtTrigxSelP36,         ///< P36
}en_adt_ttrig_trigxs_t;

/**
 ******************************************************************************
 ** \brief ADT AOS触发控制 - AOSx触发源选择
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_itrig_iaosxs
{
    AdtAosxTrigSelTim0Int,          ///< TIM0_INT
    AdtAosxTrigSelTim1Int,          ///< TIM1_INT
    AdtAosxTrigSelTim2Int,          ///< TIM2_INT
    AdtAosxTrigSelLpTimInt,         ///< LPTIMER_INT
    AdtAosxTrigSelTim4Int,          ///< TIM4_INT
    AdtAosxTrigSelTim5Int,          ///< TIM5_INT
    AdtAosxTrigSelTim6Int,          ///< TIM6_INT
    AdtAosxTrigSelUart0Int,         ///< UART0_INT
    AdtAosxTrigSelUart1Int,         ///< UART1_INT
    AdtAosxTrigSelUart2Int,         ///< UART2_INT
    AdtAosxTrigSelVc1Int,           ///< VC1_INT
    AdtAosxTrigSelVc2Int,           ///< VC2_INT
    AdtAosxTrigSelRtcInt,           ///< RTC_INT
    AdtAosxTrigSelPcaInt,           ///< PCA_INT
    AdtAosxTrigSelSpiInt,           ///< SPI_INT
    AdtAosxTrigSelAdcInt,           ///< ADC_INT
}en_adt_itrig_iaosxs_t;

/**
 ******************************************************************************
 ** \brief ADT硬件(启动/停止/清零/捕获)事件触发选择
 **
 ** \note
 ******************************************************************************/

typedef enum en_adt_hw_trig
{
    AdtHwTrigAos0,                  ///< 从AOS来的事件触发0有效
    AdtHwTrigAos1,                  ///< 从AOS来的事件触发1有效
    AdtHwTrigAos2,                  ///< 从AOS来的事件触发2有效
    AdtHwTrigAos3,                  ///< 从AOS来的事件触发3有效
    AdtHwTrigTimxARise,             ///< CHxA端口上采样到上升沿
    AdtHwTrigTimxAFall,             ///< CHxA端口上采样到下降沿
    AdtHwTrigTimxBRise,             ///< CHxB端口上采样到上升沿
    AdtHwTrigTimxBFall,             ///< CHxB端口上采样到下降沿
    AdtHwTrigTimTriARise,           ///< TIMTRIA端口上采样到上升沿
    AdtHwTrigTimTriAFall,           ///< TIMTRIA端口上采样到下降沿
    AdtHwTrigTimTriBRise,           ///< TIMTRIB端口上采样到上升沿
    AdtHwTrigTimTriBFall,           ///< TIMTRIB端口上采样到下降沿
    AdtHwTrigTimTriCRise,           ///< TIMTRIC端口上采样到上升沿
    AdtHwTrigTimTriCFall,           ///< TIMTRIC端口上采样到下降沿
    AdtHwTrigTimTriDRise,           ///< TIMTRID端口上采样到上升沿
    AdtHwTrigTimTriDFall,           ///< TIMTRID端口上采样到下降沿
    AdtHwTrigEnd,
}en_adt_hw_trig_t;

/**
 ******************************************************************************
 ** \brief ADT硬件(递加/递减)事件触发选择
 **
 ** \note
 ******************************************************************************/

typedef enum en_adt_hw_cnt
{
    AdtHwCntTimxALowTimxBRise   = 0,    ///< CHxA端口为低电平时，CHxB端口上采样到上升沿
    AdtHwCntTimxALowTimxBFall   = 1,    ///< CHxA端口为低电平时，CHxB端口上采样到下降沿
    AdtHwCntTimxAHighTimxBRise  = 2,    ///< CHxA端口为高电平时，CHxB端口上采样到上升沿
    AdtHwCntTimxAHighTimxBFall  = 3,    ///< CHxA端口为高电平时，CHxB端口上采样到下降沿
    AdtHwCntTimxBLowTimxARise   = 4,    ///< CHxB端口为低电平时，CHxA端口上采样到上升沿
    AdtHwCntTimxBLowTimxAFall   = 5,    ///< CHxB端口为低电平时，CHxA端口上采样到下降沿
    AdtHwCntTimxBHighTimxARise  = 6,    ///< CHxB端口为高电平时，CHxA端口上采样到上升沿
    AdtHwCntTimxBHighTimxAFall  = 7,    ///< CHxB端口为高电平时，CHxA端口上采样到下降沿
    AdtHwCntTimTriARise         = 8,    ///< TIMTRIA端口上采样到上升沿
    AdtHwCntTimTriAFall         = 9,    ///< TIMTRIA端口上采样到下降沿
    AdtHwCntTimTriBRise         = 10,   ///< TIMTRIB端口上采样到上升沿
    AdtHwCntTimTriBFall         = 11,   ///< TIMTRIB端口上采样到下降沿
    AdtHwCntTimTriCRise         = 12,   ///< TIMTRIC端口上采样到上升沿
    AdtHwCntTimTriCFall         = 13,   ///< TIMTRIC端口上采样到下降沿
    AdtHwCntTimTriDRise         = 14,   ///< TIMTRID端口上采样到上升沿
    AdtHwCntTimTriDFall         = 15,   ///< TIMTRID端口上采样到下降沿
    AdtHwCntAos0                = 16,   ///< 从AOS来的事件触发0有效
    AdtHwCntAos1                = 17,   ///< 从AOS来的事件触发1有效
    AdtHwCntAos2                = 18,   ///< 从AOS来的事件触发2有效
    AdtHwCntAos3                = 19,   ///< 从AOS来的事件触发3有效
    AdtHwCntMax                 = 20,
}en_adt_hw_cnt_t;

/**
 ******************************************************************************
 ** \brief ADT端口刹车极性控制
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_ptbrk_polarity
{
    AdtPtBrkHigh,       ///< 端口刹车极性高电平有效
    AdtPtBrkLow,        ///< 端口刹车极性低电平有效
}en_adt_ptbrk_polarity_t;

/**
 ******************************************************************************
 ** \brief ADT PWM展频计数选择
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_pwm_dither_type
{
    AdtPwmDitherUnderFlow,      ///< PWM展频计数下溢出
    AdtPwmDitherOverFlow,       ///< PWM展频计数上溢出
}en_adt_pwm_dither_type_t;

/**
 ******************************************************************************
 ** \brief ADT中断类型
 **
 ** \note
 ******************************************************************************/

typedef enum en_adt_irq_type
{
    AdtCMAIrq  = 0,     ///< 计数匹配A（或捕获输入）中断
    AdtCMBIrq  = 1,     ///< 计数匹配B（或捕获输入）中断
    AdtCMCIrq  = 2,     ///< 计数匹配C中断
    AdtCMDIrq  = 3,     ///< 计数匹配D中断
    AdtOVFIrq  = 6,     ///< 上溢匹配中断
    AdtUDFIrq  = 7,     ///< 下溢匹配中断
    AdtDTEIrq  = 8,     ///< 死区时间错误中断
}en_adt_irq_type_t;

/**
 ******************************************************************************
 ** \brief ADT软件同步配置
 ** \note
 ******************************************************************************/
typedef struct stc_adt_sw_sync
{
    boolean_t               bAdTim4;      ///< Timer 4
    boolean_t               bAdTim5;      ///< Timer 5
    boolean_t               bAdTim6;      ///< Timer 6

}stc_adt_sw_sync_t;

/**
 ******************************************************************************
 ** \brief ADT AOS触发配置
 ** \note
 ******************************************************************************/
typedef struct stc_adt_aos_trig_cfg
{
    en_adt_itrig_iaosxs_t   enAos0TrigSrc;      ///< AOS0触发源选择
    en_adt_itrig_iaosxs_t   enAos1TrigSrc;      ///< AOS1触发源选择
    en_adt_itrig_iaosxs_t   enAos2TrigSrc;      ///< AOS2触发源选择
    en_adt_itrig_iaosxs_t   enAos3TrigSrc;      ///< AOS3触发源选择
}stc_adt_aos_trig_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT 中断触发配置
 ** \note
 ******************************************************************************/
typedef struct stc_adt_irq_trig_cfg
{
    boolean_t   bAdtUnderFlowTrigEn;      ///< 下溢匹配使能触发ADC
    boolean_t   bAdtOverFlowTrigEn;       ///< 上溢匹配使能触发ADC
    boolean_t   bAdtCntMatchDTrigEn;      ///< 计数匹配D使能触发ADC
    boolean_t   bAdtCntMatchCTrigEn;      ///< 计数匹配C使能触发ADC
    boolean_t   bAdtCntMatchBTrigEn;      ///< 计数匹配B使能触发ADC
    boolean_t   bAdtCntMatchATrigEn;      ///< 计数匹配A使能触发ADC
}stc_adt_irq_trig_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT Trig端口配置
 ** \note
 ******************************************************************************/
typedef struct stc_adt_port_trig_cfg
{
    en_adt_ttrig_trigxs_t   enTrigSrc;      ///< 触发源选择
    boolean_t               bFltEn;         ///< 触发源捕获输入滤波使能
    en_adt_fconr_nofick_t   enFltClk;       ///< 滤波采样基准时钟
}stc_adt_port_trig_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT Z相输入屏蔽功能配置
 ** \note
 ******************************************************************************/
typedef struct stc_adt_zmask_cfg
{
    en_adt_gconr_zmsk_t     enZMaskCycle;              ///< Z相输入屏蔽计数周期选择
    boolean_t               bFltPosCntMaksEn;          ///< Z相输入时的屏蔽周期内，位置计数器的清零功能不屏蔽（FALSE）或屏蔽(TRUE)
    boolean_t               bFltRevCntMaksEn;          ///< Z相输入时的屏蔽周期内，公转计数器的计数功能不屏蔽（FALSE）或屏蔽(TRUE)
}stc_adt_zmask_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT TIMxX端口配置
 ** \note
 ******************************************************************************/
typedef struct stc_adt_TIMxX_port_cfg
{
    en_adt_pconr_capc_t     enCap;      ///< 端口功能模式
    boolean_t               bOutEn;     ///< 输出使能
    en_adt_pconr_perc_t     enPerc;     ///< 周期值匹配时端口状态
    en_adt_pconr_cmpc_t     enCmpc;     ///< 比较值匹配时端口状态
    en_adt_pconr_stastps_t  enStaStp;   ///< 计数开始停止端口状态选择
    en_adt_pconr_port_out_t enStaOut;   ///< 计数开始端口输出状态
    en_adt_pconr_port_out_t enStpOut;   ///< 计数停止端口输出状态
    en_adt_pconr_disval_t   enDisVal;   ///< 强制输出无效时输出状态控制
    en_adt_pconr_dissel_t   enDisSel;   ///< 强制输出无效条件选择
    boolean_t               bFltEn;     ///< 端口捕获输入滤波使能
    en_adt_fconr_nofick_t   enFltClk;   ///< 端口滤波采样基准时钟
}stc_adt_CHxX_port_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT刹车端口配置
 ** \note
 ******************************************************************************/
typedef struct stc_adt_break_port_cfg
{
    boolean_t               bPortEn;    ///< 端口使能
    en_adt_ptbrk_polarity_t enPol;      ///< 极性选择
}stc_adt_break_port_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT无效条件3配置
 ** \note
 ******************************************************************************/
typedef struct stc_adt_disable_3_cfg
{
    stc_adt_break_port_cfg_t    stcBrkPtCfg[16];    ///< 刹车端口配置
    boolean_t                   bFltEn;             ///< 刹车端口滤波使能
    en_adt_fconr_nofick_t       enFltClk;           ///< 滤波采样基准时钟
    boolean_t                   bSwBrk;             ///< 软件刹车
}stc_adt_disable_3_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT无效条件1配置
 ** \note
 ******************************************************************************/
typedef struct stc_adt_disable_1_cfg
{
    boolean_t   bTim6OutSH;     ///< TIM6输出同高
    boolean_t   bTim5OutSH;     ///< TIM5输出同高
    boolean_t   bTim4OutSH;     ///< TIM4输出同高
    boolean_t   bTim6OutSL;     ///< TIM6输出同低
    boolean_t   bTim5OutSL;     ///< TIM5输出同低
    boolean_t   bTim4OutSL;     ///< TIM4输出同低
}stc_adt_disable_1_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT PWM展频计数配置
 ** \note
 ******************************************************************************/
typedef struct stc_adt_pwm_dither_cfg
{
    en_adt_pwm_dither_type_t    enAdtPDType;    ///< PWM展频计数选择
    boolean_t   bTimxBPDEn;                     ///< PWM通道B展频使能
    boolean_t   bTimxAPDEn;                     ///< PWM通道A展频使能
}stc_adt_pwm_dither_cfg_t;


/**
 ******************************************************************************
 ** \brief ADT基本计数配置
 ** \note
 ******************************************************************************/
typedef struct stc_adt_basecnt_cfg
{
    en_adt_cnt_mode_t       enCntMode;      ///< 计数模式
    en_adt_cnt_dir_t        enCntDir;       ///< 计数方向
    en_adt_cnt_ckdiv_t      enCntClkDiv;    ///< 计数时钟选择
}stc_adt_basecnt_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT计数状态
 ** \note
 ******************************************************************************/
typedef struct stc_adt_cntstate_cfg
{
    uint16_t         u16Counter;      ///< 当前计数器的计数值
    boolean_t        enCntDir;        ///< 计数方向
    uint8_t          u8ValidPeriod;   ///< 有效周期计数
    boolean_t        bDTEF;           ///< 死区时间错误标志
    boolean_t        bUDFF;           ///< 下溢匹配标志
    boolean_t        bOVFF;           ///< 上溢匹配标志
    boolean_t        bCMDF;           ///< 计数匹配D标志
    boolean_t        bCMCF;           ///< 计数匹配C标志
    boolean_t        bCMBF;           ///< 计数匹配B标志
    boolean_t        bCMAF;           ///< 计数匹配A标志
}stc_adt_cntstate_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT有效计数周期
 ** \note
 ******************************************************************************/
typedef struct stc_adt_validper_cfg
{
    en_adt_vperr_pcnts_t    enValidCnt;     ///< 有效周期选择
    en_adt_vperr_pcnte_t    enValidCdt;     ///< 有效周期计数条件
    boolean_t               bPeriodD;       ///< 通用信号有效周期选择D
    boolean_t               bPeriodC;       ///< 通用信号有效周期选择C
    boolean_t               bPeriodB;       ///< 通用信号有效周期选择B
    boolean_t               bPeriodA;       ///< 通用信号有效周期选择A
}stc_adt_validper_cfg_t;

/******************************************************************************
 * Global definitions
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
en_result_t Adt_ConfigHwCntUp(en_adt_unit_t enAdtUnit, en_adt_hw_cnt_t enAdtHwCntUp);
en_result_t Adt_ClearHwCntUp(en_adt_unit_t enAdtUnit);
en_result_t Adt_ConfigHwCntDwn(en_adt_unit_t enAdtUnit, en_adt_hw_cnt_t enAdtHwCntDwn);
en_result_t Adt_ClearHwCntDwn(en_adt_unit_t enAdtUnit);
en_result_t Adt_ConfigHwStart(en_adt_unit_t enAdtUnit, en_adt_hw_trig_t enAdtHwStart);
en_result_t Adt_ClearHwStart(en_adt_unit_t enAdtUnit);
en_result_t Adt_EnableHwStart(en_adt_unit_t enAdtUnit);
en_result_t Adt_DisableHwStart(en_adt_unit_t enAdtUnit);
en_result_t Adt_ConfigHwStop(en_adt_unit_t enAdtUnit, en_adt_hw_trig_t enAdtHwStop);
en_result_t Adt_ClearHwStop(en_adt_unit_t enAdtUnit);
en_result_t Adt_EnableHwStop(en_adt_unit_t enAdtUnit);
en_result_t Adt_DisableHwStop(en_adt_unit_t enAdtUnit);
en_result_t Adt_ConfigHwClear(en_adt_unit_t enAdtUnit, en_adt_hw_trig_t enAdtHwClear);
en_result_t Adt_ClearHwClear(en_adt_unit_t enAdtUnit);
en_result_t Adt_EnableHwClear(en_adt_unit_t enAdtUnit);
en_result_t Adt_DisableHwClear(en_adt_unit_t enAdtUnit);
en_result_t Adt_ConfigHwCaptureA(en_adt_unit_t enAdtUnit, en_adt_hw_trig_t enAdtHwCaptureA);
en_result_t Adt_ClearHwCaptureA(en_adt_unit_t enAdtUnit);
en_result_t Adt_ConfigHwCaptureB(en_adt_unit_t enAdtUnit, en_adt_hw_trig_t enAdtHwCaptureB);
en_result_t Adt_ClearHwCaptureB(en_adt_unit_t enAdtUnit);

en_result_t Adt_SwSyncStart(stc_adt_sw_sync_t* pstcAdtSwSyncStart);
en_result_t Adt_SwSyncStop(stc_adt_sw_sync_t* pstcAdtSwSyncStop);
en_result_t Adt_SwSyncClear(stc_adt_sw_sync_t* pstcAdtSwSyncClear);
en_result_t Adt_GetSwSyncState(stc_adt_sw_sync_t* pstcAdtSwSyncState);

en_result_t Adt_AosTrigConfig(stc_adt_aos_trig_cfg_t* pstcAdtAosTrigCfg);

en_result_t Adt_IrqTrigConfig(en_adt_unit_t enAdtUnit,
                              stc_adt_irq_trig_cfg_t* pstcAdtIrqTrigCfg);

en_result_t Adt_PortTrigConfig(en_adt_trig_port_t enAdtTrigPort,
                               stc_adt_port_trig_cfg_t* pstcAdtPortTrigCfg);

en_result_t Adt_CHxXPortConfig(en_adt_unit_t enAdtUnit,
                                en_adt_CHxX_port_t enAdtCHxXPort,
                                stc_adt_CHxX_port_cfg_t* pstcAdtCHxXCfg);

en_result_t Adt_EnableBreakPort(uint8_t port, stc_adt_break_port_cfg_t* pstcAdtBrkPtCfg);
void Adt_ClearBrakePort(void);
en_result_t Adt_Disable3Cfg(stc_adt_disable_3_cfg_t* pstcAdtDisable3);
boolean_t Adt_GetPortBrakeFlag(void);
void Adt_ClearPortBrakeFlag(void);
en_result_t Adt_Disable1Cfg(stc_adt_disable_1_cfg_t* pstcAdtDisable1);
boolean_t Adt_GetSameBrakeFlag(void);
void Adt_ClearSameBrakeFlag(void);

en_result_t Adt_PwmDitherConfig(stc_adt_pwm_dither_cfg_t* pstcAdtPwmDitherCfg);

en_result_t Adt_Init(en_adt_unit_t enAdtUnit, stc_adt_basecnt_cfg_t* pstcAdtBaseCntCfg);
en_result_t Adt_DeInit(en_adt_unit_t enAdtUnit);

en_result_t Adt_StartCount(en_adt_unit_t enAdtUnit);
en_result_t Adt_StopCount(en_adt_unit_t enAdtUnit);
en_result_t Adt_SetCount(en_adt_unit_t enAdtUnit, uint16_t u16Value);
en_result_t Adt_ClearCount(en_adt_unit_t enAdtUnit);
en_result_t Adt_GetCntState(en_adt_unit_t enAdtUnit, stc_adt_cntstate_cfg_t* pstcAdtCntState);
en_result_t Adt_SetPeriod(en_adt_unit_t enAdtUnit, uint16_t u16Period);
en_result_t Adt_SetPeriodBuf(en_adt_unit_t enAdtUnit, uint16_t u16PeriodBuf);
en_result_t Adt_ClearPeriodBuf(en_adt_unit_t enAdtUnit);
en_result_t Adt_SetValidPeriod(en_adt_unit_t enAdtUnit,
                               stc_adt_validper_cfg_t* pstcAdtValidPerCfg);
en_result_t Adt_SetCompareValue(en_adt_unit_t enAdtUnit,
                                en_adt_compare_t enAdtCompare,
                                uint16_t u16Compare);
en_result_t Adt_EnableValueBuf(en_adt_unit_t enAdtUnit,
                                 en_adt_CHxX_port_t enAdtCHxXPort,
                                 boolean_t bCompareBufEn);
en_result_t Adt_ClearValueBuf(en_adt_unit_t enAdtUnit,
                                     en_adt_CHxX_port_t enAdtCHxXPort);
en_result_t Adt_GetCaptureValue(en_adt_unit_t enAdtUnit,
                                en_adt_CHxX_port_t enAdtCHxXPort,
                                uint16_t* pu16Capture);
en_result_t Adt_GetCaptureBuf(en_adt_unit_t enAdtUnit,
                                en_adt_CHxX_port_t enAdtCHxXPort,
                                uint16_t* pu16CaptureBuf);

en_result_t Adt_SetDTUA(en_adt_unit_t enAdtUnit,
                        uint16_t u16Value);
en_result_t Adt_SetDTDA(en_adt_unit_t enAdtUnit,
                        uint16_t u16Value);
en_result_t Adt_ConfigDT(en_adt_unit_t enAdtUnit,
                         boolean_t bDTEn,
                         boolean_t bEqual);

en_result_t Adt_ConfigIrq(en_adt_unit_t enAdtUnit,
                          en_adt_irq_type_t enAdtIrq,
                          boolean_t bEn,
                          func_ptr_t pfnIrqCalbak);
en_result_t Adt_GetIrqFlag(en_adt_unit_t enAdtUnit,
                           en_adt_irq_type_t enAdtIrq,
                           boolean_t* pbFlag);
en_result_t Adt_ClearIrqFlag(en_adt_unit_t enAdtUnit,
                             en_adt_irq_type_t enAdtIrq);
en_result_t Adt_ConfigZMask(en_adt_unit_t enAdtUnit, 
                            stc_adt_zmask_cfg_t* pstcAdtZMaskCfg);

//@} // ADT Group

#ifdef __cplusplus
}
#endif

#endif /* __ADT_H__ */
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
