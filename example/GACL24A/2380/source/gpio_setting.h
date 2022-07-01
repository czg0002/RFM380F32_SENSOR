/*
 * =====================================================================================
 *
 *       Filename:  gpio_setting.h
 *
 *    Description:  gpio settings and defines 
 *
 *        Version:  1.0
 *        Created:  2022/4/21 14:06:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Scott Chen (), czg0002@163.com
 *   Organization:  GOVA
 *
 * =====================================================================================
 */

#ifndef  gpio_setting_INC
#define  gpio_setting_INC

#define GPIO1	0x04	//used in TEST_GPIO1 macro  not conenct in 
#define	GPIO2	0x10	//used in TEST_GPIO2 macro,
#define	SDIO	0x08
	
//set1:input, set0:output;
//P01:SDA2
//P02:SCL2
//P03:CMT_CSB

//P23:CMT_SDIO
//P24:CMT_CLK
//P25:EE_SDA
//P26:EE_SCL
//P36:SCL1
//P35:SDA1
//P33:PCON, front power control
//P32: CMT_FCSB
//P34: CMT_GPIO2
//reset state
#define	P0DIR_Data	0x00000006	//P03z
#define	P1DIR_Data	0x00000030
#define	P2DIR_Data	0x00000000
#define	P3DIR_Data	0x00000010

#define	P0OUT_Data	0x00000008
#define	P1OUT_Data	0x00000000
#define	P2OUT_Data	0x00000000
#define	P3OUT_Data	0x00000004

#define	P0PU_Data		0x00000000
#define	P1PU_Data		0x00000000
#define	P2PU_Data		0x00000000
#define	P3PU_Data		0x00000000

#define GPIO_EXTPOWER_ON() Gpio_SetIO(3, 3, 1)
#define GPIO_EXTPOWER_OFF() Gpio_SetIO(3, 3, 0)

#endif   /* ----- #ifndef gpio_setting_INC  ----- */
