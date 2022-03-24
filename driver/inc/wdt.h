/*****************************************************************************/
/** \file wdt.h
 **
 ** Headerfile for WDT functions
 **  
 **
 ** History:
 **   - 2017-05-10   Cathy     First Version
 **
 *****************************************************************************/

#ifndef __WDT_H__
#define __WDT_H__

#include "ddl.h"
#include "interrupts_CMT2380F32.h"

/**
 ******************************************************************************
 ** \defgroup WdtGroup Watchdog Timer (WDT)
 **
 ******************************************************************************/
//@{

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
/**
 ******************************************************************************
 ** \brief wdt溢出后复位或中断配置
 *****************************************************************************/
typedef enum en_wdt_reset
{
    WRESET_EN    = 0,///<复位使能
    WINT_EN      = 1,///<中断使能
}en_wdt_reset_t;
/**
 ******************************************************************************
 ** \brief wdt当前运行状态
 *****************************************************************************/

typedef enum en_wdt_status
{
    Stop_Mode    = 0,///<停止模块
    Run_Mode     = 1,///<正常运行模式
}en_wdt_status_t;

/**
 ******************************************************************************
 ** \brief wdt总体配置，定时时间 、中断函数等
 *****************************************************************************/

typedef struct stc_wdt_config
{
    uint8_t          u8LoadValue;   ///<溢出时间      
    en_wdt_reset_t  enResetEnable;  ///<中断或复位使能       
    func_ptr_t       pfnWdtIrqCb;   ///<中断服务接口函数
} stc_wdt_config_t;
//wdt初始化
en_result_t Wdt_Init(stc_wdt_config_t* pstcConfig);
void Wdt_DeInit(void);

//wdt开始和停止
en_result_t Wdt_Start(void);
//void Wdt_Stop(void);

//wdt溢出时间设置及当前计数时间、状态读取
void Wdt_WriteWdtLoad(uint8_t u8LoadValue);
uint8_t Wdt_ReadWdtValue(void);
uint8_t Wdt_ReadwdtStatus(void);

//喂狗处理
void Wdt_Feed(void);

//@} // WdtGroup

#ifdef __cplusplus
#endif

#endif /* __WDT_H__ */



