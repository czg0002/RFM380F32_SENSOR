/*
 * =====================================================================================
 *
 *       Filename:  uartProtocol.h
 *
 *    Description:  uart protocol type defines
 *
 *        Version:  1.0
 *        Created:  2019-10-10 15:02:55
 *       Revision:  none
 *       Compiler:  ccs
 *
 *         Author:  Scott Chen (), czg0002@163.com
 *   Organization:  GOVA
 *
 * =====================================================================================
 */

#ifndef  rfCmdProc_INC
#define  rfCmdProc_INC

#include <stdint.h>
#include <stdbool.h>
#include "flashData.h"

#define RF_REC_MAX_DATA_LEN 20
#define RF_CMD_INDEX	5	//SysCmd_e;

typedef enum {
	RF_WRITE_CMD_SUCCESS,
	RF_WRITE_CMD_FAIL,
	RF_READ_DB_COMPLETE
}RFCmdStatus_e;


typedef enum{
   sysNoCmd			= 0,

   sysLocationDataSet = 0x91,
   sysLocationDataReadAll = 0x92,
   sysLocationDataDeleteAll = 0x9f,

   sysWKUPCmd       = 0xa1,
   sysOnceSampleCmd = 0xa3,
   sysReadADCCmd	= 0xa4,
   sysReadDBCmd     = 0xa5,
   sysSleepCmd      = 0xa7,
   sysGlobalWKUPCmd     = 0xa9,
   sysGlobalSleepCmd    = 0xab,
   sysGlobalReadDBCmd   = 0xad,
   sysInConfigCmd   = 0xb1,
   sysConfigIDCmd   = 0xb3,
   sysConfigTimeCmd    = 0xb4,
   sysConfigSendPeriodCmd  = 0xb5,		//sending period
   sysReadSendPeriodCmd  = 0xb6,		//sending period
   sysConfigSavePeriodCmd= 0xb7,		//saving period
   sysReadSavePeriodCmd= 0xb8,		//saving period
   sysConfigStateCmd= 0xb9,
   sysReadStateCmd  = 0xbA,
   sysOutConfigCmd  = 0xbb,

   sysSetCustomerSer	= 0xbc,
   sysGetCustomerSer	= 0xbd,

   sysCallStatusCmd       = 0xc0,
   sysCallCmd       = 0xc2,

   sysCaliTempCmd   = 0xd0,		//0xd1 is used for pad air pressure 
   sysGetCaliTempParaCmd   = 0xd2,		//0xd1 is used for padair pressure 
   sysSetCaliTemp4TCmd	= 0xd3,
   sysGetCaliTemp4TCmd	= 0xd4,
   sysSetTempCaliMethodCmd = 0xd5,
   sysGetTempCaliMethodCmd = 0xd6,
   sysCaliHumiCmd	= 0xd7,
   sysGetCaliHumiParaCmd	= 0xd8,
   sysCaliPresCmd	= 0xd9,
   sysGetCaliPressureParaCmd	= 0xda,
   sysSetCaliPressureMethodCmd	= 0xdb,
   sysGetCaliPressureMethodCmd	= 0xdc,

   sysSetDebugModeCmd = 0xdd,
   sysGetDebugModeCmd = 0xde,

   sysSetPressureCaliParaWithTemperature = 0xdf,	//for compatibility
   sysGetPressureCaliParaWithTemperature = 0xe0,

   sysSetSniffingInterval = 0xe1,
   sysGetSniffingInterval = 0xe2,
   sysSetFlashIndex			= 0xe3,
   sysGetFlashIndex			= 0xe4,
   sysResetFlashIndex			= 0xe5,
   sysSetIdleTimeout		= 0xe7,
   sysGetIdleTimeout		= 0xe8,
   sysRestoreCalibrationDefault = 0xe9,

//   sysGetAccelerometer		= 0xea,

   sysVersionCmd	= 0xf0,
   sysTestRfCmd		= 0xf1,
   sysKeepRfSendingCmd = 0xf2,	
}SysCmd_e;

typedef struct {
    uint8_t  header[2];
    uint8_t  year; 
    uint8_t  flag[2];
    uint8_t  batch;
    uint8_t  number;
    uint8_t  datalen;
//	SensorData_s sensorData;
//  Time_s timeRTC;
    uint8_t  frameCount[2];
    uint8_t  crc[2];		//TODO: crc is redudant here because cc1310 automatically add crc when transfer. consider other usage.
}ReplyMsg_s;

typedef struct {
    uint8_t  header[2];
    uint8_t  year; 
    uint8_t  flag[2];
    uint8_t  batch;
    uint8_t  number;
	uint8_t	 cmd;
	uint8_t  datalen;
	uint8_t  dataWithCRC[RF_REC_MAX_DATA_LEN];
}RecMsg_s;

typedef struct {
	uint8_t cmd;
	bool (*pFunc)(void);
} CmdDict_s;

//extern EasyLink_TxPacket gTxPacket;	
//extern EasyLink_RxPacket gRxPacket;

void rfCmdProc_processCmd(void);
void rfCmdProc_sendOneRecord(Record_s record);


//active send command, send semaphore  to EasyLink  task.
void rfCmdProc_activeSendBatteryVoltage(void);
void rfCmdProc_activeSendRecord(Record_s record);

#endif   /* ----- #ifndef rfCmdProc_INC  ----- */
