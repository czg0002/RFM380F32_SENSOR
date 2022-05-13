/*
 * =====================================================================================
 *
 *       Filename:  common.h
 *
 *    Description: common globla variable macro and function 
 *
 *        Version:  1.0
 *        Created:  2022/4/8 10:41:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Scott Chen (), czg0002@163.com
 *   Organization:  GOVA
 *
 * =====================================================================================
 */
#ifndef __COMMON_H
#define __COMMON_H

#include <stdint.h>

#define BOOL uint8_t
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

void Delay(uint32_t Count);

#endif
