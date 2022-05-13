/*
 * =====================================================================================
 *
 *       Filename:  flashData.h
 *
 *    Description:  flash data structure and corresponding operating function using TI's NVS api.
 *
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
#include "i2cRtc.h"
#define FLASH_DATA_ERROR_NO_NEW_RECORD -1

#define INTERNAL_FLASH_FOR_RECORD	//comment out for using external flash

#define FACTORYCFG_START_ADDR	0		//relative to NVS_REGIONS_BASE, see CC1310_LAUNCHXL.c
#define CUSTOMERCFG_START_ADDR	0x1000
#define RECORD_OPERATION_PARAMETER_START_ADDR	0x2000
//#define FACTORYCFG_BACKUP_START_ADDR	0x3000		//relative to 0x16000, see config file
//#define CUSTOMERCFG_BACKUP_START_ADDR	0x4000
#define SENSOR_LOCATION_INFO_START_ADDR 0x3000
#define MAX_SENSOR_LOCATION_NUM 128	// 0x1000 / 32

#define RECORD_SIZE sizeof(Record_s)
#define RECORD_NUM_PER_SECTOR 256	//	0x1000 / 16

#ifdef  INTERNAL_FLASH_FOR_RECORD
#define RECORD_START_ADDR 0x4000
#define TOTAL_SECTORS_OF_RECORD 5	//the last sector is reserved for ccfg.
#else      /* -----  not INTERNAL_FLASH_FOR_RECORD  ----- */
#define RECORD_START_ADDR 0	
#define TOTAL_SECTORS_OF_RECORD	40		//when using external flash, set this number according to flash size. when using internal flash, set to 3
#endif     /* -----  not INTERNAL_FLASH_FOR_RECORD  ----- */

#define TOTAL_NUMBER_OF_RECORD (RECORD_NUM_PER_SECTOR * TOTAL_SECTORS_OF_RECORD)

typedef struct {
    uint8_t  year; 
    uint8_t  flag[2];
    uint8_t  batch;
    uint8_t  number;
}ProductSerial_s;

typedef struct {
	float temperature_k;
	float temperature_y;		//out_temperature = k*x - y, so y is positive
	float pressure_k;
	float pressure_y;
	float humidity_a_shift;	//TODO: shift value for real_rh = sensor_rh / ((1.0546+a) - (0.00216+b) * temperature_f);
	float humidity_b_shift;

	uint8_t	 temperatureConvertingMethod;	//0 for whole linear , 1 for segment linear

	//TODO: save original calibration data
	uint16_t adcTemperatureM40;
	int16_t temperature1;	//NOTE: format: real_temperature * 100
	uint16_t adcTemperatureM10;
	int16_t temperature2;
	uint16_t adcTemperatureP25;
	int16_t temperature3;
	uint16_t adcTemperatureP60;
	int16_t temperature4;

	//reserved, pressure linearity is good
	/*
	uint16_t adcPressure10;
	uint16_t adcPressure80;
	uint16_t adcPressure150;
	*/
	uint16_t pressConvertingMethod;
	uint16_t adc5VT25;	//divided by 3/5
	uint16_t adc5VT50;

	uint16_t adcHumidity20T25;
	uint16_t adcHumidity40T25;
	uint16_t adcHumidity60T25;
	uint16_t adcHumidity20T50;
	uint16_t adcHumidity40T50;
	uint16_t adcHumidity60T50;

	//pressure segment k y;
	float pressure_k_M10;
	float pressure_y_M10;
	float pressure_k_P25;
	float pressure_y_P25;
	float pressure_k_P50;
	float pressure_y_P50;

	uint16_t h20t25_real;
	uint16_t h40t25_real;
	uint16_t h60t25_real;
	uint16_t h20t50_real;
	uint16_t h40t50_real;
	uint16_t h60t50_real;

} Calibration_s;


typedef struct {
	ProductSerial_s manufacturerSer;
	Calibration_s calibration;
	uint8_t debugMode;	//when 0, normal sleep, when 0xff, always awake, used for calibration and debug
	uint8_t idleTimeout;	//limited to 1~30.
	uint8_t dataChecksum;		//TODO: use as data validation.
}FactoryCfg_s;

//TODO: verify and protect important data

#define PERIOD_SAVE_MODE_BIT 1
#define PERIOD_SEND_MODE_BIT 2

typedef struct {
	ProductSerial_s customerSer;
	uint8_t workingMode;
	uint32_t savingPeriod;
	uint32_t sendingPeriod;
	uint8_t sniffingPeriod;	//limited to 5~60.
	uint8_t dataChecksum;
}CustomerCfg_s;


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

typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint16_t placeUnicode1;
	uint16_t placeUnicode2;
	uint16_t padNum;
	uint32_t bucketNum;
	uint32_t missileNum;
	uint16_t shipNum;
	uint32_t launcherShelfNum;
	uint32_t launcherPosNum;
	uint8_t reserved[3];
}SensorLocationInfo_s;

/* global variable to  load from and save to flash*/
extern FactoryCfg_s gFactoryCfg;
extern CustomerCfg_s gCustomerCfg;
extern DataOperationParameter_s gDataOperationParameter;
extern FactoryCfg_s defaultFactoryCfg;
extern CustomerCfg_s defaultCustomerCfg;

int8_t flashData_Init(void);
int8_t flashData_reset(void);

/* ------------internal flash function:--------------------------- */
//int8_t flashData_saveManufacturerSer(ProductSerial_s manuafacturerSer);
//int8_t flashData_readManufacturerSer(ProductSerial_s *manuafacturerSer);
int8_t flashData_loadgFactoryCfg(void);
int8_t flashData_savegFactoryCfg(void);
int8_t flashData_loadgCustomerCfg(void);
int8_t flashData_savegCustomerCfg(void);
int8_t flashData_loadgDataOperationParameter(void);
int8_t flashData_savegDataOperationParameter(void);

int8_t flashData_saveCalibration(Calibration_s calibration);
int8_t flashData_readCalibration(Calibration_s *calibration);

int8_t flashData_saveCustomerSer(ProductSerial_s customerSer);
int8_t flashData_readCustomerSer(ProductSerial_s *customerSer);

int8_t flashData_saveDataOperationParameter(void);

int8_t flashData_saveSavingPeriod(uint32_t savingPeriod);
int8_t flashData_readSavingPeriod(uint32_t *savingPeriod);

int8_t flashData_saveSendingPeriod(uint32_t sendingPeriod);
int8_t flashData_readSendingPeriod(uint32_t *sendingPeriod);
/* --------------------------------------- */
int8_t flashData_appendOneRecord(Record_s record);		//saveIndex += 1, cycle
int8_t flashData_extractOneUnsentRecord(Record_s *record);	//sendIndex += 1, cycle
uint32_t flashData_getUnsendRecordQuantity(void);	
bool flashData_UnsendRecordExist(void);

int8_t flashData_getLastSendTime(RtcTime_s *lastTime);
int8_t flashData_setLastSendTime(RtcTime_s lastTime);
int8_t flashData_getLastSaveTime(RtcTime_s *lastTime);
int8_t flashData_setLastSaveTime(RtcTime_s lastTime);

//TODO: data validate before poweroff.
int8_t flashData_shutDown(void);
int8_t flashData_reset(void);

//auxiliary function
int8_t flashData_saveRecord(Record_s record, uint32_t index);
int8_t flashData_readRecord(Record_s record, uint32_t index);

//location data operation function
int8_t flashData_appendLocationData(SensorLocationInfo_s *sensor_location);
int8_t flashData_extractOneLocationData(uint16_t index, SensorLocationInfo_s *sensor_location);
int8_t flashData_extractAllLocationData(SensorLocationInfo_s *sensor_location_array);	//note: pointer must point to place with enough storage
int8_t flashData_modifyOneLocationData(uint16_t index, SensorLocationInfo_s * sensor_location);
int8_t flashData_deleteAllLocationData(void);


#endif   /* ----- #ifndef flashData_INC  ----- */
