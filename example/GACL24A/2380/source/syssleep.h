/*
 * =====================================================================================
 *
 *       Filename:  syssleep.h
 *
 *    Description:  wrap sleep function for cmt2380f32
 *
 *        Version:  1.0
 *        Created:  2022/5/10 15:29:18
 *       Revision:  none
 *       Compiler:  keil armcc
 *
 *         Author:  Scott Chen (), czg0002@163.com
 *   Organization:  GOVA
 *
 * =====================================================================================
 */

#ifndef  syssleep_INC
#define  syssleep_INC

#include <stdint.h>

typedef enum {
	sysStateSleep = 0x00,
	sysStateWakeup,
	sysStateWaitForCalled,
	sysStateWaitForDiscovered,
} SysState_e;

extern SysState_e sysState;

void syssleep_init(void);
void syssleep_start(uint32_t ms);
void syssleep_setState(SysState_e new_state);

#endif   /* ----- #ifndef syssleep_INC  ----- */
