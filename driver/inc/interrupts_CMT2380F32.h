/******************************************************************************/
/** \file interrupts_CMT2380F32.h
 **
 ** Interrupt common define.
 ** @link IRQGroup Some description @endlink
 **
 **   - 2017-04-29  1.0       First version.
 **
 ******************************************************************************/
#ifndef __INTERRUPTS_CMT2380F32_H__
#define __INTERRUPTS_CMT2380F32_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/

#define DDL_IRQ_LEVEL_DEFAULT                   3u

///< 系统中断使能开关
void EnableNvic(uint32_t u32Irq,uint8_t u8Level,boolean_t bEn);


#ifdef __cplusplus
}
#endif


#endif /* __INTERRUPTS_CMT2380F32_H__ */

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
