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

#define BYTE0_OF(x)	(x & 0xff)
#define BYTE1_OF(x)	((x >> 8) & 0xff)
#define BYTE2_OF(x)	((x >> 16) & 0xff)
#define BYTE3_OF(x)	((x >> 24) & 0xff)

#define JOINT_TO_UINT16(array, start) (array[start] + (array[start + 1] << 8))
#define JOINT_TO_UINT32(array, start) (array[start] + (array[start + 1] << 8) + (array[start + 2] << 16) + (array[start + 3] << 24))

void Delay(uint32_t Count);
uint8_t calc_checksum(uint8_t *ptr, uint16_t len);
#endif
