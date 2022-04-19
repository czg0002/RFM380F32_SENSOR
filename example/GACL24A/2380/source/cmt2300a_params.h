/*
 * =====================================================================================
 *
 *       Filename:  cmt2300a_params.h
 *
 *    Description: header file for cmt2300a configuration parameters 
 *
 *        Version:  1.0
 *        Created:  2022/3/31 15:46:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Scott Chen (), czg0002@163.com
 *   Organization:  GOVA
 *
 * =====================================================================================
 */

#ifndef  __CMT2300A_PARAMS
#define  __CMT2300A_PARAMS

#include "CMT2380F32_Demo.h"	//TODO: to be removed
extern uint8_t g_cmt2300aCmtBank[12];
extern uint8_t g_cmt2300aSystemBank[12];
extern uint8_t g_cmt2300aFrequencyBank[8];
extern uint8_t g_cmt2300aDataRateBank[24];
extern uint8_t g_cmt2300aBasebandBank[29];
extern uint8_t g_cmt2300aTxBank[11];

#endif
