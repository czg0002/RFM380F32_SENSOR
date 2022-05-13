/*
 * =====================================================================================
 *
 *       Filename:  radio.c
 *
 *    Description: wrapper function for rf tx rx
 *
 *        Version:  1.0
 *        Created:  2022/4/28 13:13:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Scott Chen (), czg0002@163.com
 *   Organization:  GOVA
 *
 * =====================================================================================
 */
#include <string.h>
#include "radio.h"
#include "ddl.h"
#include "gpio_setting.h"
#include "cmt2300a_hal.h"
#include "cmt2300a.h"
#include "CMT2380F32_Demo.h"
#include "radio.h"
#include "rfCmdProc.h"

CmdRxPacket_s gRxPacket = {0};
uint8_t gTxPayload[PACKET_MAX_LEN] = { 0xAA, 0x55, 22, 0x7f, 7, 0, 89, 0xc2, 1, 0, 0, 0};

static uint8_t globalWakeupCmd[10] = {0xAA, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA9, 0x01, 0x00};
static uint8_t singleWakeupCmd[10] = {0xAA, 0x55, 0x16, 0x7f, 0x07, 0x00, 0x59, 0xA1, 0x01, 0x00};
static uint8_t broadcastAddr[7] = {0xAA, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00};
static uint8_t sensorAddr[7] = {0xAA, 0x55, 0x16, 0x7f, 0x07, 0x00, 0x59};
static uint8_t shortSensorAddr[7] = {0xAA, 0x55, 0x00, 0x00, 0x00, 0x00, 0x59};
/*
 * ===  FUNCTION  ======================================================================
 *         Name:  RF_RxPacket
 *  Description:  Timeout in Ms, must not exceed 512000ms !!!
 * 				  rx fifo limit to 32 byte(not use fifo merge)
 * =====================================================================================
 */
EnumRFResult RF_RxPacket(uint8_t rxBuf[], uint8_t *rxlen, uint32_t rxTimeoutMs) // TODO: add time elapsed
{
	volatile uint32_t tick_val;
	uint32_t end;
	uint8_t length = 0;
	int i;
	EnumRFResult result;
	CMT2300A_GoStby();
	CMT2300A_ConfigGpio(CMT2300A_GPIO2_SEL_INT2);	//for rx
	CMT2300A_EnableReadFifo();
	CMT2300A_ClearInterruptFlags();
	CMT2300A_ClearRxFifo();
	CMT2300A_GoRx();

	SysTick->VAL = 0;
	end = 0x1000000 - rxTimeoutMs * CLK_RCL_VAL / 1053;
	while ((tick_val = SysTick->VAL) > end || tick_val == 0)
	//while (1)
	{
		if (Test_GPIO2())
		{
			CMT2300A_GoStby();
			CMT2300A_ReadFifo(rxBuf, 1);
			//length = Read_FIFO();
			//rxBuf[0] = length;
			// *rxlen = length;
			length = rxBuf[0];
			*rxlen = rxBuf[0];
			if (length > 32 || length <= 0) // error, how to solve?
			{
				result =  RF_ERROR;
			}
			else
			{
				CMT2300A_ReadFifo(&rxBuf[1], length);
				result =  RF_RX_DONE;
			}
			CMT2300A_GoSleep();
			return result;
		}
	}
	CMT2300A_GoSleep();
	result = RF_RX_TIMEOUT;
	return result;
}

EnumRFResult RF_TxPacket(uint8_t txBuf[], uint8_t txlen, uint32_t txTimeoutMs)
// txTimeoutMs: 0 means wait for ever
// 
{
	int i = 0;
	EnumRFResult result;
	CMT2300A_GoStby();
	CMT2300A_ConfigGpio(CMT2300A_GPIO2_SEL_INT1);	//for tx
	CMT2300A_EnableWriteFifo(); 
	CMT2300A_ClearTxFifo();
	CMT2300A_ClearInterruptFlags();
	CMT2300A_SetPayloadLength(txlen);
	CMT2300A_WriteFifo(txBuf, txlen);
	CMT2300A_SetFifoAutoResEn(1);
	CMT2300A_GoTx();
	while (1) //for (i = 0; i < txTimeout*4000; i++)	//use rough timing
	{
		if (Test_GPIO2())
		{
			result = RF_TX_DONE;
			break;
		}
		if (txTimeoutMs > 0)
		{
			i += 1;
			if (i > txTimeoutMs * 4000)
			{
				result = RF_TX_TIMEOUT;
				break;
			}
		}
	}

	CMT2300A_GoSleep();
	return result;
}

EnumRFResult RF_RxWakeupPacket(uint32_t rxTimeoutMs)
	//filter valid command, TODO: better use hardware node id fiter function
{
	volatile uint32_t tick_val;
	uint32_t end;
	uint8_t length = 0;
	int i;
	EnumRFResult result;
	CMT2300A_GoStby();
	CMT2300A_ConfigGpio(CMT2300A_GPIO2_SEL_INT2);	//for rx
	CMT2300A_EnableReadFifo();
	CMT2300A_ClearInterruptFlags();
	CMT2300A_ClearRxFifo();
	CMT2300A_GoRx();

	SysTick->VAL = 0;
	end = 0x1000000 - rxTimeoutMs * CLK_RCL_VAL / 1053;
	while ((tick_val = SysTick->VAL) > end || tick_val == 0)
	{
		if (Test_GPIO2())
		{
			CMT2300A_GoStby();
			CMT2300A_ReadFifo(&gRxPacket.len, 1);
			//length = Read_FIFO();
			//rxBuf[0] = length;
			// *rxlen = length;
//			length = rxBuf[0];
//			*rxlen = rxBuf[0];
			if (gRxPacket.len> 32 || gRxPacket.len  <= 0) // error, how to solve?
			{
				CMT2300A_ClearInterruptFlags();
				CMT2300A_ClearRxFifo();
				CMT2300A_GoRx();
				continue;
			}
			else
			{
				CMT2300A_ReadFifo(gRxPacket.payload, gRxPacket.len);
				if (memcmp(gRxPacket.payload, globalWakeupCmd, 10) == 0
						|| memcmp(gRxPacket.payload, singleWakeupCmd, 10) == 0)
				{
					result =  RF_RX_DONE;
				}
				else	//not valid wakeup command
				{
					CMT2300A_ClearInterruptFlags();
					CMT2300A_ClearRxFifo();
					CMT2300A_GoRx();
					continue;
				}
			}
			CMT2300A_GoSleep();
			return result;
		}
	}
	CMT2300A_GoSleep();
	result = RF_RX_TIMEOUT;
	return result;
}

EnumRFResult RF_RxValidPacket(uint32_t rxTimeoutMs)
{
	volatile uint32_t tick_val;
	uint32_t end;
	uint8_t length = 0;
	int i;
	EnumRFResult result;
	CMT2300A_GoStby();
	CMT2300A_ConfigGpio(CMT2300A_GPIO2_SEL_INT2);	//for rx
	CMT2300A_EnableReadFifo();
	CMT2300A_ClearInterruptFlags();
	CMT2300A_ClearRxFifo();
	CMT2300A_GoRx();

	SysTick->VAL = 0;
	end = 0x1000000 - rxTimeoutMs * CLK_RCL_VAL / 1053;
	while ((tick_val = SysTick->VAL) > end || tick_val == 0)
	{
		if (Test_GPIO2())
		{
			CMT2300A_GoStby();
			CMT2300A_ReadFifo(&gRxPacket.len, 1);
			//length = Read_FIFO();
			//rxBuf[0] = length;
			// *rxlen = length;
//			length = rxBuf[0];
//			*rxlen = rxBuf[0];
			if (gRxPacket.len> 32 || gRxPacket.len  <= 0) // error, how to solve?
			{
				CMT2300A_ClearInterruptFlags();
				CMT2300A_ClearRxFifo();
				CMT2300A_GoRx();
				continue;
			}
			else
			{
				CMT2300A_ReadFifo(gRxPacket.payload, gRxPacket.len);
				if (memcmp(gRxPacket.payload, broadcastAddr, 7) == 0 )
				{
					gRxPacket.cmdType = CMDTYPE_BROADCAST;
					result = RF_RX_DONE;
				}
				else if ( memcmp(gRxPacket.payload, sensorAddr, 7) == 0
						|| memcmp(gRxPacket.payload, shortSensorAddr, 7) == 0) 
				{
					gRxPacket.cmdType = CMDTYPE_SINGLE;
					result = RF_RX_DONE;
				}
				else	//not valid wakeup command
				{
					gRxPacket.cmdType = CMDTYPE_NONE;
					CMT2300A_ClearInterruptFlags();
					CMT2300A_ClearRxFifo();
					CMT2300A_GoRx();
					continue;
				}
			}
			CMT2300A_GoSleep();
			return result;
		}
	}
	CMT2300A_GoSleep();
	result = RF_RX_TIMEOUT;
	return result;
}
