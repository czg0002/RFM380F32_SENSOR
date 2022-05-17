/*
 * =====================================================================================
 *
 *       Filename:  flashData.h
 *
 *    Description:  flash data, basic configurations of sensor
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  ccs
 *
 *         Author:  Scott Chen (), czg0002@163.com
 *   Organization:  GOVA
 *
 * =====================================================================================
 */

#ifndef  flashData_INC
#define  flashData_INC

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t  year; 
    uint8_t  flag[2];
    uint8_t  batch;
    uint8_t  number;
}ProductSerial_s;

typedef struct {
	uint32_t tempCali[4];
	uint32_t humiCali[4];
	uint32_t presCali[4];
}Calibration_s;

typedef struct {
	uint8_t FactorySerial[8];
	uint8_t CustomerSerial[8];
	uint8_t debugMode;
	uint8_t sleepIntervalSeconds;	
	uint8_t sniffingTimeMs;
	uint8_t idleTimeoutMin;
	Calibration_s caliPara;
	uint8_t checksum;
}FactoryCfg_s;

typedef struct {
	uint16_t pressure;
	uint16_t temperature;
	uint16_t humidity;
	uint16_t battery;
}SensorData_s;	//converted result: display result * 100

typedef struct {
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t weekday;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}Time_s;		//inverted with RtcTime_s, just for backward compatibility

/*last operation time saving struct and data index
 *saving in internal flash user section 2
 */
typedef struct {
	Time_s lastSaveTime;
	Time_s lastSendTime;
	uint32_t saveIndex;	
	uint32_t sendIndex;
	uint8_t firstTimeFull;	//0: flash first time used; 1: flash has been full at least once.
	uint8_t flashFull;
	uint16_t sensorLocationDataNum;
}DataOperationParameter_s;

typedef struct
{
    SensorData_s sensorData;
    Time_s rtcTime;
//	uint16_t frameCount;	//may be change to CRC
	uint8_t checksum;
}Record_s;

/* global variable to  load from and save to flash*/
extern FactoryCfg_s gFactoryCfg;
extern FactoryCfg_s defaultFactoryCfg;

int8_t flashData_Init(void);

/* ------------internal flash function:--------------------------- */
//int8_t flashData_saveManufacturerSer(ProductSerial_s manuafacturerSer);
//int8_t flashData_readManufacturerSer(ProductSerial_s *manuafacturerSer);
int8_t flashData_restoregFactoryCfg(void);
int8_t flashData_loadgFactoryCfg(void);
int8_t flashData_savegFactoryCfg(void);


#endif   /* ----- #ifndef flashData_INC  ----- */
