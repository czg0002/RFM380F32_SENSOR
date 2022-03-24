/******************************************************************************/
/** \file ddl.h
 **
 ** DDL common define.
 ** @link SampleGroup Some description @endlink
 **
 **   - 2017-04-29  1.0       First version.
 **
 ******************************************************************************/

#ifndef __DDL_H__
#define __DDL_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "base_types.h"
#include "CMT2380F32.h"
#include "system_CMT2380F32.h"
#include "clk.h"
#include "gpio.h"
#include "bt.h"

/* C binding of definitions if building with C++ compiler                     */
#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/* Macro for initializing local structures to zero                            */
/******************************************************************************/
#define DDL_ZERO_STRUCT(x)          ddl_memclr((uint8_t*)&(x), (uint32_t)(sizeof(x)))

#define DEC2BCD(x)                  ((((x)/10)<<4) + ((x)%10))
#define BCD2DEC(x)                  ((((x)>>4)*10) + ((x)&0x0F))

#define setBit(addr,offset,flag)    {   if( (flag) > 0u){\
                                            *((volatile uint32_t *)(addr)) |= ((1UL)<<(offset));\
                                        }else{\
                                            *((volatile uint32_t *)(addr)) &= (~(1UL<<(offset)));\
                                        }\
                                    }

#define getBit(addr,offset)         ((((*((volatile uint32_t *)(addr))) >> (offset)) & 1u)>0?1u:0)

/**
 ******************************************************************************
 ** Global Device Series List
 ******************************************************************************/
#define DDL_DEVICE_SERIES_CMT2380F32       (0u)

/**
 ******************************************************************************
 ** Global Device Package List
 ******************************************************************************/
// package definitions of HC device.
#define DDL_DEVICE_PACKAGE_HC_C         (0x00u)
#define DDL_DEVICE_PACKAGE_HC_F         (0x10u)
#define DDL_DEVICE_PACKAGE_HC_J         (0x20u)
#define DDL_DEVICE_PACKAGE_HC_K         (0x30u)

/******************************************************************************/
/* User Device Setting Include file                                           */
/******************************************************************************/
#include "ddl_device.h"  // MUST be included here!

/**
 ******************************************************************************
 ** \brief IRQ name definition for all type MCUs
 ******************************************************************************/
    
    #define PORT0_IRQHandler(void)              IRQ000_Handler(void)
    #define PORT1_IRQHandler(void)              IRQ001_Handler(void)
    #define PORT2_IRQHandler(void)              IRQ002_Handler(void)
    #define PORT3_IRQHandler(void)              IRQ003_Handler(void)
//                                              IRQ004_Handler(void)
//                                              IRQ005_Handler(void)
    #define UART0_IRQHandler(void)              IRQ006_Handler(void)
    #define UART1_IRQHandler(void)              IRQ007_Handler(void)
    #define LPUART_IRQHandler(void)             IRQ008_Handler(void)
//                                              IRQ009_Handler(void)
    #define SPI_IRQHandler(void)                IRQ010_Handler(void)
//                                              IRQ011_Handler(void)
    #define I2C_IRQHandler(void)                IRQ012_Handler(void)
//                                              IRQ013_Handler(void)
    #define TIM0_IRQHandler(void)               IRQ014_Handler(void)
    #define TIM1_IRQHandler(void)               IRQ015_Handler(void)
    #define TIM2_IRQHandler(void)               IRQ016_Handler(void)
    #define LPTIM_IRQHandler(void)              IRQ017_Handler(void)
    #define TIM4_IRQHandler(void)               IRQ018_Handler(void)
    #define TIM5_IRQHandler(void)               IRQ019_Handler(void)
    #define TIM6_IRQHandler(void)               IRQ020_Handler(void)
    #define PCA_IRQHandler(void)                IRQ021_Handler(void)
    #define WDT_IRQHandler(void)                IRQ022_Handler(void)
    #define RTC_IRQHandler(void)                IRQ023_Handler(void)
    #define ADC_IRQHandler(void)                IRQ024_Handler(void)
//                                              IRQ025_Handler(void)
    #define VC0_IRQHandler(void)                IRQ026_Handler(void)
    #define VC1_IRQHandler(void)                IRQ027_Handler(void)
    #define LVD_IRQHandler(void)                IRQ028_Handler(void)
//                                              IRQ029_Handler(void)
    #define EF_RAM_IRQHandler(void)             IRQ030_Handler(void)
    #define CLKTRIM_IRQHandler(void)            IRQ031_Handler(void)

/******************************************************************************/
/* Global type definitions ('typedef')                                        */
/******************************************************************************/
/**
 ******************************************************************************
 ** \brief Level
 **
 ** Specifies levels.
 **
 ******************************************************************************/
typedef enum en_level
{
    DdlLow      = 0u,  ///< Low level  '0'
    DdlHigh     = 1u   ///< High level '1'
} en_level_t;

/**
 ******************************************************************************
 ** \brief Generic Flag Code
 **
 ** Specifies flags.
 **
 ******************************************************************************/
typedef enum en_flag
{
    DdlClr = 0u,       ///< Flag clr '0'
    DdlSet = 1u        ///< Flag set '1'
} en_stat_flag_t, en_irq_flag_t;
/******************************************************************************/
/* Global variable declarations ('extern', definition in C source)            */
/******************************************************************************/

/******************************************************************************/
/* Global function prototypes ('extern', definition in C source)              */
/******************************************************************************/

/*******************************************************************************
 * Global function prototypes
 ******************************************************************************/
extern void ddl_memclr(void* pu8Address, uint32_t u32Count);
uint32_t Log2(uint32_t u32Val);
/**
 *******************************************************************************
 ** This hook is part of wait loops.
 ******************************************************************************/
extern void DDL_WAIT_LOOP_HOOK(void);

void Debug_UartInit(void);

void delay1ms(uint32_t u32Cnt);
void delay100us(uint32_t u32Cnt);
#ifdef __cplusplus
}
#endif

#endif /* __DDL_H__ */

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

