/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, CMOSTEK SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (C) CMOSTEK SZ.
 */

/*!
 * @file    cmt2300a_hal.h
 * @brief   CMT2300A hardware abstraction layer
 *
 * @version 1.2
 * @date    Jul 17 2017
 * @author  CMOSTEK R@D
 */
 
#ifndef __CMT2300A_HAL_H
#define __CMT2300A_HAL_H

#include <stdint.h>
//#include "typedefs.h"
//#include "gpio_defs.h"
//#include "common.h"
//#include "system.h"

#ifdef __cplusplus 
extern "C" { 
#endif

/* ************************************************************************
*  The following need to be modified by user
*  ************************************************************************ */
#define CMT2300A_SetGpio1In()           SET_GPIO_IN(CMT_GPIO1_GPIO)
#define CMT2300A_SetGpio2In()           SET_GPIO_IN(CMT_GPIO2_GPIO)
#define CMT2300A_SetGpio3In()           SET_GPIO_IN(CMT_GPIO3_GPIO)
#define CMT2300A_ReadGpio1()            READ_GPIO_PIN(CMT_GPIO1_GPIO)
#define CMT2300A_ReadGpio2()            READ_GPIO_PIN(CMT_GPIO2_GPIO)
#define CMT2300A_ReadGpio3()            READ_GPIO_PIN(CMT_GPIO3_GPIO)
#define CMT2300A_DelayMs(ms)            Delay(1000 * ms)//system_delay_ms(ms)	//TODO: temperorily 
#define CMT2300A_DelayUs(us)            system_delay_us(us)
#define CMT2300A_GetTickCount()         g_nSysTickCount
/* ************************************************************************ */

void CMT2300A_InitGpio(void);

uint8_t CMT2300A_ReadReg(uint8_t addr);
void CMT2300A_WriteReg(uint8_t addr, uint8_t dat);

void CMT2300A_ReadFifo(uint8_t buf[], uint16_t len);
void CMT2300A_WriteFifo(const uint8_t buf[], uint16_t len);

#ifdef __cplusplus 
} 
#endif

#endif
