/*
 * =====================================================================================
 *
 *       Filename:  radio.h
 *
 *    Description:  header of wrapper function  for rf tx rx 
 *
 *        Version:  1.0
 *        Created:  2022/4/28 10:25:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Scott Chen (), czg0002@163.com
 *   Organization:  GOVA
 *
 * =====================================================================================
 */

#ifndef  radio_INC
#define  radio_INC
#include <stdint.h>

#define PACKET_MAX_LEN 32

/* RF process function results */
typedef enum {
    RF_IDLE = 0,
    RF_BUSY,
    RF_RX_DONE,
    RF_RX_TIMEOUT,
    RF_TX_DONE,
    RF_TX_TIMEOUT,
    RF_ERROR,
} EnumRFResult;

typedef enum {
	CMDTYPE_BROADCAST,
	CMDTYPE_SINGLE,
	CMDTYPE_NONE = 0xff
} CmdType_e;

typedef struct {
	uint8_t payload[PACKET_MAX_LEN];
	uint8_t len;
	uint8_t rssi;
	CmdType_e cmdType;
} CmdRxPacket_s;

extern uint8_t gTxPayload[PACKET_MAX_LEN];
extern CmdRxPacket_s gRxPacket;

#define Test_GPIO2() (M0P_GPIO->P3IN&GPIO2)

EnumRFResult RF_RxPacket(uint8_t rxBuf[], uint8_t *rxlen, uint32_t rxTimeout);
EnumRFResult RF_TxPacket(uint8_t txBuf[], uint8_t txlen, uint32_t txTimeout);

EnumRFResult RF_RxWakeupPacket(uint32_t rxTimeoutMs);
EnumRFResult RF_RxValidPacket(uint32_t rxTimeoutMs);
#endif   /* ----- #ifndef radio_INC  ----- */
