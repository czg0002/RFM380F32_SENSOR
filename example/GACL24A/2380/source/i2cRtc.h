/*
 * =====================================================================================
 *
 *       Filename:  i2cRtc.h
 *
 *    Description:  i2c read write rtc SD3078 function.
 *
 *        Version:  1.0
 *        Created:  2019-10-22 15:22:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Scott Chen (), czg0002@163.com
 *   Organization:  GOVA
 *
 * =====================================================================================
 */

#ifndef  i2cRtc_INC
#define  i2cRtc_INC

#include <stdint.h>

//ic type define
//#define SD3078
#define DS3232

typedef struct
{
    uint8_t  second;	//uint8_t, not BCD, when read from or writhe to rtc, covert from or to BCD first
    uint8_t  minute;
    uint8_t  hour;
    uint8_t  weekday;
    uint8_t  date;
    uint8_t  month;
    uint8_t  year;
} RtcTime_s;		

//int8_t i2cRtc_init(void);
int8_t i2cRtc_getTime(RtcTime_s *time);
int8_t i2cRtc_setTime(RtcTime_s time);

#ifdef SD3078
int8_t i2cRtc_setIntFrequency();
int8_t i2cRtc_setDelayPulse();
int8_t i2cRtc_writeEnable(void);
int8_t i2cRtc_writeDisable(void);
#endif

int8_t i2cRtc_setDelayPulse(uint8_t delay_seconds);
int8_t i2cRtc_resetIntn(void);
int8_t i2cRtc_disableIntn(void);

//assistant function
uint32_t i2cRtc_getTimeElapsed(RtcTime_s current_time, RtcTime_s previous_time);

#endif   /* ----- #ifndef i2cRtc_INC  ----- */
