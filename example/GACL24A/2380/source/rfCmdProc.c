/*
 * =====================================================================================
 *
 *       Filename:  rfCmdPro.c
 *
 *    Description:  rf command process
 *
 *        Version:  1.0
 *        Created:  2019-12-13 13:33:34
 *       Revision:  none
 *       Compiler:  css
 *
 *         Author:  Scott Chen (), czg0002@163.com
 *   Organization:  GOVA
 *
 * =====================================================================================
 */

/* rx packet payload[6] is cmd len, payload[7] is the first valid data*/
#include <stdbool.h>
#include "radio.h"
#include "rfCmdProc.h"
#define crc_mul 0x1021 

static uint16_t frame_count = 1;

static bool isOwnAddr(void);
static bool isBroadcastAddr(void);
static uint16_t cal_crc(unsigned char *ptr, unsigned char len);
static void replyRfWriteCmdStatus(RFCmdStatus_e cmd_status);
static void replyRfReadCmd(int tail_index);

/*------------------------cmd process function----------------------------*/

static bool rfCmd_locationDataSet(void)
{
#if 0
	uint8_t index;
	SensorLocationInfo_s sensor_location;
	index = gRxPacket.payload[7];
	memcpy(&sensor_location, &(gRxPacket.payload[8]), sizeof(SensorLocationInfo_s) - 3);
#ifdef UART_DEBUG_DISPLAY
	Display_printf(displayHandle, 0, 0, "index: %d", index);
#endif
return true;
	if (index == 0)
	{
		if (true == flashData_appendLocationData(&sensor_location))
			replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
		else
			replyRfWriteCmdStatus(RF_WRITE_CMD_FAIL);
	}
	else
	{
		if (true == flashData_modifyOneLocationData(index - 1, &sensor_location))
			replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
		else
			replyRfWriteCmdStatus(RF_WRITE_CMD_FAIL);
	}
	return true;
#endif
return true;
}

static bool rfCmd_locationDataReadAll(void)
{
#if 0
	int i;
	SensorLocationInfo_s sensor_location_history[gDataOperationParameter.sensorLocationDataNum];
	flashData_extractAllLocationData(sensor_location_history);
	gTxPacket.payload[6] = 1;
	gTxPacket.payload[7] = gDataOperationParameter.sensorLocationDataNum;
	gTxPacket.payload[8] = gRxPacket.rssi;
	gTxPacket.payload[9] = 0;
	gTxPacket.len = 10;
	EasyLink_transmit(&gTxPacket);

	gTxPacket.payload[6] = 0x1e;
	for (i = 0; i < gDataOperationParameter.sensorLocationDataNum; i++)
	{
		usleep(50000);
		gTxPacket.payload[7] = i + 1;
		memcpy(&(gTxPacket.payload[8]), &sensor_location_history[i], sizeof(SensorLocationInfo_s) - 3);
		gTxPacket.payload[37] = gRxPacket.rssi;
		gTxPacket.payload[38] = 0;
		gTxPacket.len = 39;
		EasyLink_transmit(&gTxPacket);
	}
	return true;
#endif
return true;
}

static bool rfCmd_locationDataDeleteAll(void)
{
#if 0
	flashData_deleteAllLocationData();
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	return true;
#endif
return true;
}

static bool rfCmd_wakeup(void)
{
#if 0
	i2cRtc_disableIntn();	//disable i2c intn A1E output. TODO: to be tested.
	if (gSensorState == sysStateSleep)
	{
		gSensorState = sysStateWakeup;
//		gSensorState = sysStateWaitForCall;
	}
	//TODO: reply with tdma
	return true;
#endif
return true;
}

static bool rfCmd_onceSample(void)
{
#if 0
	RtcTime_s sample_time;
	Record_s tmpRecord;
//	uint16_t crc;

	gTxPacket.payload[6] = 1;
	gTxPacket.payload[7] = 0;
	gTxPacket.len = 10;
	EasyLink_transmit(&gTxPacket);
//	adcFunc_getTHPConverted(&temperature, &humidity, &pressure);
//	battery = adcFunc_getBatteryConverted();
//	
	i2cRtc_getTime(&sample_time);
	adcFunc_getSensorData(&(tmpRecord.sensorData));
	tmpRecord.rtcTime.year		= sample_time.year;
	tmpRecord.rtcTime.month 	= sample_time.month;
	tmpRecord.rtcTime.date		= sample_time.date;
	tmpRecord.rtcTime.weekday	= sample_time.weekday;
	tmpRecord.rtcTime.hour		= sample_time.hour;
	tmpRecord.rtcTime.minute	= sample_time.minute;
	tmpRecord.rtcTime.second	= sample_time.second;

	frame_count = 0;	//reset frame_count to 0
	rfCmdProc_sendOneRecord(tmpRecord);
	frame_count = 1;	//set for periodic saving.

    return true;
#endif
return true;
}

static bool rfCmd_readADC(void)
{
#if 0
	uint16_t temperature, humidity, pressure, dc_scan, battery;

	PIN_setOutputValue(pinHandle, PIN_5V_EN, 1);	//insure 5v enable
	if (gFactoryCfg.debugMode != 0xff)	//when debug mode is 0xff, 5V is always on, no need to delay
	{
	//	CPUdelay(12000 * 60);   //60ms for voltage stable(TODO: use oscillarator to check for best time. 3000 for 250us, TODO: 3000~250us is 
		usleep(250000);
	}
	temperature = adcFunc_getTemperatureRaw();
	humidity = adcFunc_getHumidityRaw();
	pressure = adcFunc_getPressureRaw();
	dc_scan = adcFunc_getDCScanRaw();
	if (gFactoryCfg.debugMode != 0xff)
	{
		PIN_setOutputValue(pinHandle, PIN_5V_EN, 0);
	}
	battery = AONBatMonBatteryVoltageGet();

#ifdef UART_DEBUG_DISPLAY
	Display_printf(displayHandle, 0, 0, "get adc value:.\r\ntemp: %d\r\nhumi: %d\r\npress: %d.\r\ndc_scan: %d.\r\nbattery: %d.\r\n", temperature, humidity, pressure, dc_scan, battery);
#endif
return true;

	gTxPacket.len = 17;
	gTxPacket.payload[6] = 10;
	gTxPacket.payload[7] = temperature & 0xff;
	gTxPacket.payload[8] = (temperature >> 8) & 0xff;
	gTxPacket.payload[9] = humidity & 0xff;
	gTxPacket.payload[10] = (humidity >> 8) & 0xff;
	gTxPacket.payload[11] = pressure & 0xff;
	gTxPacket.payload[12] = (pressure >> 8) & 0xff;
	gTxPacket.payload[13] = dc_scan & 0xff;
	gTxPacket.payload[14] = (dc_scan  >> 8) & 0xff;
	gTxPacket.payload[15] = battery & 0xff;
	gTxPacket.payload[16] = (battery  >> 8) & 0xff;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
}

static bool rfCmd_readDB(void)
{
#if 0
	Record_s tmpRecord;
	uint32_t num;	//quantity of records to be sendt
	uint8_t start_position;
	uint32_t next_sector_start_index;
	start_position = gRxPacket.payload[7];
	if (start_position == 1) //read all saved data
	{
		if (gDataOperationParameter.firstTimeFull == 0)
		{
			gDataOperationParameter.sendIndex = 0;
		}
		else	//from last unerased sector
		{
//			gDataOperationParameter.sendIndex = gDataOperationParameter.saveIndex + (RECORD_NUM_PER_SECTOR - 1 - () ;
			if (gDataOperationParameter.saveIndex % RECORD_NUM_PER_SECTOR == 0)
			{
				gDataOperationParameter.sendIndex = gDataOperationParameter.saveIndex;
			}
			else
			{
				gDataOperationParameter.sendIndex = (gDataOperationParameter.sendIndex + RECORD_NUM_PER_SECTOR) % TOTAL_NUMBER_OF_RECORD - (gDataOperationParameter.sendIndex + RECORD_NUM_PER_SECTOR) % RECORD_NUM_PER_SECTOR;
			}
		}

	}

	if (gDataOperationParameter.sendIndex < gDataOperationParameter.saveIndex)
	{
		num = gDataOperationParameter.saveIndex - gDataOperationParameter.sendIndex;
	}
	else if (gDataOperationParameter.sendIndex > gDataOperationParameter.saveIndex)
	{
		num = gDataOperationParameter.saveIndex + TOTAL_NUMBER_OF_RECORD - gDataOperationParameter.sendIndex;
	}
	else // if equal, may be full, or maybe empty.
	{
		if (gDataOperationParameter.flashFull == 0)
			num = 0;
		else
			num = TOTAL_NUMBER_OF_RECORD;
	}
	usleep(40000);
	if (num != 0) //
	{
		//reply msg with number of record
		gTxPacket.payload[6] = 1;
		gTxPacket.payload[7] = num & 0xff;
		gTxPacket.payload[8] = (num >> 8) & 0xff;
		gTxPacket.payload[9] = gRxPacket.rssi;
		gTxPacket.len = 10;
		EasyLink_transmit(&gTxPacket);
		usleep(40000);
		while(FLASH_DATA_ERROR_NO_NEW_RECORD != flashData_extractOneUnsentRecord(&tmpRecord))
		{
			usleep(10000);	//delay 10ms for pad processing
			rfCmdProc_sendOneRecord(tmpRecord);
		}
		usleep(10000);	//delay 10ms for pad processing
	}

	//reply end of msg
	gTxPacket.payload[5] = 0xA5;
	gTxPacket.payload[6] = 2;
	gTxPacket.payload[7] = 0;
	gTxPacket.payload[8] = 0;
	gTxPacket.payload[9] = gRxPacket.rssi;
	gTxPacket.len = 10;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
}

static bool rfCmd_sleep(void)
{
#if 0
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	gSensorState = sysStateSleep;   //TODO: may move i2cRtc command to sysShutdown, if failed to set rtc, must send rf msg to pad
	//sysShutdown();
	return true;
#endif
return true;
}

static bool rfCmd_globalSleep(void)
{
#if 0
	replyRfWriteCmdStatus(RF_READ_DB_COMPLETE);
	//sysShutdown();
	gSensorState = sysStateSleep;
	return true;
#endif
return true;
}

static bool rfCmd_globalReadDB(void)
{
#if 0
	return true;
#endif
return true;
}

static bool rfCmd_inConfig(void)
{
#if 0
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	return true;
#endif
return true;
}

static bool rfCmd_configID(void)
{
#if 0
	//TODO: only change id when in config mode. to insure safety write
#ifdef UART_DEBUG_DISPLAY
	Display_printf(displayHandle, 0, 0, "new id: %d, %d, %d, %d, %d.\r\n", gRxPacket.payload[7], gRxPacket.payload[8], gRxPacket.payload[9], gRxPacket.payload[10], gRxPacket.payload[11]);
#endif
return true;
	memcpy(&(gFactoryCfg.manufacturerSer), &(gRxPacket.payload[7]), sizeof(ProductSerial_s));
	flashData_savegFactoryCfg();
	//updata default tx id
    memcpy(&(gTxPacket.payload[0]), &(gFactoryCfg.manufacturerSer), sizeof(gFactoryCfg.manufacturerSer));
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	return true;
#endif
return true;
}

static bool rfCmd_configTime(void)
{
#if 0
	RtcTime_s current_time;
	current_time.year = gRxPacket.payload[7];
	current_time.month = gRxPacket.payload[8];
	current_time.date = gRxPacket.payload[9];
	current_time.weekday = gRxPacket.payload[10];
	current_time.hour = gRxPacket.payload[11];
	current_time.minute = gRxPacket.payload[12];
	current_time.second = gRxPacket.payload[13];

	flashData_setLastSendTime(current_time);	//reset last save and send time
	flashData_setLastSaveTime(current_time);

	if (true == i2cRtc_setTime(current_time))
	{
		replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	}
	else
	{
		replyRfWriteCmdStatus(RF_WRITE_CMD_FAIL);
	}
	return true;
#endif
return true;
}

static bool rfCmd_configSendPeriod(void)
{
#if 0
	if (gRxPacket.payload[6] == 4)
	{
		gCustomerCfg.sendingPeriod = gRxPacket.payload[7] + ((uint32_t) gRxPacket.payload[8] << 8) + ((uint32_t) gRxPacket.payload[9] << 16) + ((uint32_t) gRxPacket.payload[10] << 24);
	}
	else if (gRxPacket.payload[6] == 2)	// for backward compatibility
	{
		gCustomerCfg.sendingPeriod = gRxPacket.payload[7] + ((uint32_t) gRxPacket.payload[8] << 8);
	}
	flashData_savegCustomerCfg();
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	return true;
#endif
return true;
}

static bool rfCmd_readSendPeriod(void)
{
#if 0
	gTxPacket.payload[6] = 4;
	gTxPacket.payload[7] = gCustomerCfg.sendingPeriod & 0xff;
	gTxPacket.payload[8] = (gCustomerCfg.sendingPeriod >> 8) & 0xff;
	gTxPacket.payload[9] = (gCustomerCfg.sendingPeriod >> 16) & 0xff;
	gTxPacket.payload[10] = (gCustomerCfg.sendingPeriod >> 24) & 0xff;
	gTxPacket.payload[11] = gRxPacket.rssi;
	gTxPacket.payload[12] = 0;
	gTxPacket.len = 13;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
}

static bool rfCmd_configSavePeriod(void)
{
#if 0
	if (gRxPacket.payload[6] == 4)
	{
		gCustomerCfg.savingPeriod = gRxPacket.payload[7] + ((uint32_t) gRxPacket.payload[8] << 8) + ((uint32_t) gRxPacket.payload[9] << 16) + ((uint32_t) gRxPacket.payload[10] << 24);
	}
	else if (gRxPacket.payload[6] == 2)	// for backward compatibility
	{
		gCustomerCfg.savingPeriod = gRxPacket.payload[7] + ((uint32_t) gRxPacket.payload[8] << 8);
	}
	flashData_savegCustomerCfg();
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	return true;
#endif
return true;
}

static bool rfCmd_readSavePeriod(void)
{
#if 0
	gTxPacket.payload[6] = 4;
	gTxPacket.payload[7] = gCustomerCfg.savingPeriod & 0xff;
	gTxPacket.payload[8] = (gCustomerCfg.savingPeriod >> 8) & 0xff;
	gTxPacket.payload[9] = (gCustomerCfg.savingPeriod >> 16) & 0xff;
	gTxPacket.payload[10] = (gCustomerCfg.savingPeriod >> 24) & 0xff;
	gTxPacket.payload[11] = gRxPacket.rssi;
	gTxPacket.payload[12] = 0;
	gTxPacket.len = 13;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
}

static bool rfCmd_configState(void)
{
#if 0
	gCustomerCfg.workingMode = gRxPacket.payload[7];
	flashData_savegCustomerCfg();
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	return true;
#endif
return true;
}

static bool rfCmd_readState(void)
{
#if 0
	gTxPacket.payload[6] = 1;
	gTxPacket.payload[7] = gCustomerCfg.workingMode;
	gTxPacket.payload[8] = 0;
	gTxPacket.payload[9] = 0;
	gTxPacket.len = 10;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
}

static bool rfCmd_outConfig(void)
{
#if 0
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	return true;
#endif
return true;
}

static bool rfCmd_setCustomerSer(void)
{
#if 0
	memcpy(&(gCustomerCfg.customerSer), &(gRxPacket.payload[7]), sizeof(ProductSerial_s));
	flashData_savegCustomerCfg();
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	return true;
#endif
return true;
}

static bool rfCmd_getCustomerSer(void)
{
#if 0
	return true;
#endif
return true;
}

static bool rfCmd_callStatus(void)
{
#if 0
//	callStatus = 1;
//	Event_post(stateEvent, STATEEVENT_SENSORDISCOVERED);
	return true;
#endif
return true;
}

static bool rfCmd_sysCall(void)	//reply to 
{
#if 0
#ifdef UART_DEBUG_DISPLAY
	Display_printf(displayHandle, 0, 0, "sysCall.\r\n");
#endif
return true;
	if (gCustomerCfg.customerSer.year == defaultCustomerCfg.customerSer.year
		&&gCustomerCfg.customerSer.flag[0] == defaultCustomerCfg.customerSer.flag[0]
		&&gCustomerCfg.customerSer.flag[1] == defaultCustomerCfg.customerSer.flag[1]
		&&gCustomerCfg.customerSer.batch == defaultCustomerCfg.customerSer.batch
		&&gCustomerCfg.customerSer.number == defaultCustomerCfg.customerSer.number
			)	//if default, return as old version.
		replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	else
	{
		gTxPacket.payload[6] = 5;
		memcpy(&(gTxPacket.payload[7]), &(gCustomerCfg.customerSer), sizeof(ProductSerial_s));
		gTxPacket.payload[12] = gRxPacket.rssi;
		gTxPacket.payload[13] = 0;
		gTxPacket.len = 14;
		EasyLink_transmit(&gTxPacket);
	}
	return true;
#endif
return true;
}

static bool rfCmd_caliTemp(void)
{
#if 0
	int32_t k_temp, y_temp;
#ifdef UART_DEBUG_DISPLAY
	Display_printf(displayHandle, 0, 0, "debug mode %d.\r\n", gFactoryCfg.debugMode);
#endif
return true;
	if (gFactoryCfg.debugMode == 0xff || gFactoryCfg.debugMode == 0xfe)	//only change calibrating parameter in debug mode
	{
		memcpy(&(k_temp), &(gRxPacket.payload[7]), 4);
		memcpy(&(y_temp), &(gRxPacket.payload[11]), 4);
		gFactoryCfg.calibration.temperature_k = (float) k_temp / 1.0e8;
		gFactoryCfg.calibration.temperature_y = (float) y_temp / 1.0e4;
#ifdef UART_DEBUG_DISPLAY
		Display_printf(displayHandle, 0, 0, "change temperature k %f, y %f.\r\n", gFactoryCfg.calibration.temperature_k, gFactoryCfg.calibration.temperature_y);
#endif
return true;
		flashData_savegFactoryCfg();
		replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	}
	else
	{
		replyRfWriteCmdStatus(RF_WRITE_CMD_FAIL);
	}
	return true;
#endif
return true;
}

static bool rfCmd_getCaliTempPara(void)
{
#if 0
	int32_t k_t, y_t;
	k_t = (int32_t)(gFactoryCfg.calibration.temperature_k * 1.0e8);
	y_t = (int32_t)(gFactoryCfg.calibration.temperature_y * 1.0e4);
	gTxPacket.payload[6] = 8;
	gTxPacket.payload[7] = BYTE0_OF(k_t);
	gTxPacket.payload[8] = BYTE1_OF(k_t);
	gTxPacket.payload[9] = BYTE2_OF(k_t);
	gTxPacket.payload[10] = BYTE3_OF(k_t);
	gTxPacket.payload[11] = BYTE0_OF(y_t);
	gTxPacket.payload[12] = BYTE1_OF(y_t);
	gTxPacket.payload[13] = BYTE2_OF(y_t);
	gTxPacket.payload[14] = BYTE3_OF(y_t);
	gTxPacket.payload[15] = gRxPacket.rssi;
	gTxPacket.len = 16;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
} 
static bool rfCmd_setCaliTemp4T(void)
{
#if 0
//	if (gFactoryCfg.debugMode != 0)	//only change calibrating parameter in debug mode
//	{
	//used payload[7] as temperature point status:
	uint8_t index;
	index = gRxPacket.payload[7];
#ifdef UART_DEBUG_DISPLAY
	Display_printf(displayHandle, 0, 0, "receive cali temp hex: %d, %d.\r\n", gRxPacket.payload[8] + (gRxPacket.payload[9] << 8), gRxPacket.payload[10] + (gRxPacket.payload[11] << 8));
#endif
return true;
	switch (index)
	{
		case 0:
			gFactoryCfg.calibration.adcTemperatureM40 = gRxPacket.payload[8] + (gRxPacket.payload[9] << 8);
			gFactoryCfg.calibration.temperature1 = gRxPacket.payload[10] + (gRxPacket.payload[11] << 8) - 10000;
			break;
		case 1:
			gFactoryCfg.calibration.adcTemperatureM10 = gRxPacket.payload[8] + (gRxPacket.payload[9] << 8);
			gFactoryCfg.calibration.temperature2 = gRxPacket.payload[10] + (gRxPacket.payload[11] << 8) - 10000;
			break;
		case 2:
			gFactoryCfg.calibration.adcTemperatureP25 = gRxPacket.payload[8] + (gRxPacket.payload[9] << 8);
			gFactoryCfg.calibration.temperature3 = gRxPacket.payload[10] + (gRxPacket.payload[11] << 8) - 10000;
			break;
		case 3:
			gFactoryCfg.calibration.adcTemperatureP60 = gRxPacket.payload[8] + (gRxPacket.payload[9] << 8);
			gFactoryCfg.calibration.temperature4 = gRxPacket.payload[10] + (gRxPacket.payload[11] << 8) - 10000;
			break;
	}
	flashData_savegFactoryCfg();
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
//	}
//	else
//	{
//		replyRfWriteCmdStatus(RF_WRITE_CMD_FAIL);
//	}
	return true;
#endif
return true;
}
static bool rfCmd_getCaliTemp4T(void)
{
#if 0
	gTxPacket.payload[6] = 16;
	//TODO: plus 10000
	gTxPacket.payload[7] = BYTE0_OF(gFactoryCfg.calibration.adcTemperatureM40);
	gTxPacket.payload[8] = BYTE1_OF(gFactoryCfg.calibration.adcTemperatureM40);
	gTxPacket.payload[9] = BYTE0_OF(gFactoryCfg.calibration.temperature1);
	gTxPacket.payload[10] = BYTE1_OF(gFactoryCfg.calibration.temperature1);
	gTxPacket.payload[12] = BYTE0_OF(gFactoryCfg.calibration.adcTemperatureM10);
	gTxPacket.payload[13] = BYTE1_OF(gFactoryCfg.calibration.adcTemperatureM10);
	gTxPacket.payload[14] = BYTE0_OF(gFactoryCfg.calibration.temperature2);
	gTxPacket.payload[15] = BYTE1_OF(gFactoryCfg.calibration.temperature2);
	gTxPacket.payload[16] = BYTE0_OF(gFactoryCfg.calibration.adcTemperatureP25);
	gTxPacket.payload[17] = BYTE1_OF(gFactoryCfg.calibration.adcTemperatureP25);
	gTxPacket.payload[18] = BYTE0_OF(gFactoryCfg.calibration.temperature3);
	gTxPacket.payload[19] = BYTE1_OF(gFactoryCfg.calibration.temperature3);
	gTxPacket.payload[20] = BYTE0_OF(gFactoryCfg.calibration.adcTemperatureP60);
	gTxPacket.payload[21] = BYTE1_OF(gFactoryCfg.calibration.adcTemperatureP60);
	gTxPacket.payload[22] = BYTE0_OF(gFactoryCfg.calibration.temperature4);
	gTxPacket.payload[23] = BYTE1_OF(gFactoryCfg.calibration.temperature4);
	gTxPacket.payload[24] = gRxPacket.rssi;
	gTxPacket.payload[25] = 0;
	gTxPacket.len = 26;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
}
static bool rfCmd_setTempCaliMethod(void)
{
#if 0
	gFactoryCfg.calibration.temperatureConvertingMethod = gRxPacket.payload[7];
	flashData_savegFactoryCfg();
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	return true;
#endif
return true;
}
static bool rfCmd_getTempCaliMethod(void)
{
#if 0
	gTxPacket.payload[6] = 1;
	gTxPacket.payload[7] = gFactoryCfg.calibration.temperatureConvertingMethod;
	gTxPacket.payload[8] = gRxPacket.rssi;
	gTxPacket.payload[9] = 0;
	gTxPacket.len = 10;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
}
static bool rfCmd_caliHumi(void)
{
#if 0
//#if 0
	int16_t a_shift, b_shift;	// shift value 
	
	if (gFactoryCfg.debugMode == 0xff || gFactoryCfg.debugMode == 0xfe)	//only change calibrating parameter in debug mode
	{
		a_shift = JOINT_TO_INT16(gRxPacket.payload, 7);
		b_shift = JOINT_TO_INT16(gRxPacket.payload, 9);
#ifdef UART_DEBUG_DISPLAY
		Display_printf(displayHandle, 0, 0, "receive Humi hex: %x, %x, %x, %x.\r\n", gRxPacket.payload[7], gRxPacket.payload[8], gRxPacket.payload[9], gRxPacket.payload[10]);
#endif
return true;
#ifdef UART_DEBUG_DISPLAY
		Display_printf(displayHandle, 0, 0, "a_shift %d, b_shift %d.\r\n", a_shift, b_shift);
#endif
return true;
		gFactoryCfg.calibration.humidity_a_shift = (float)a_shift / 1.0e5;
		gFactoryCfg.calibration.humidity_b_shift = (float)b_shift / 1.0e6;
#ifdef UART_DEBUG_DISPLAY
		Display_printf(displayHandle, 0, 0, "a_shift %f, b_shift %f.\r\n", gFactoryCfg.calibration.humidity_a_shift, gFactoryCfg.calibration.humidity_b_shift);
#endif
return true;
		flashData_savegFactoryCfg();
		replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	}
	else
	{
		replyRfWriteCmdStatus(RF_WRITE_CMD_FAIL);
	}
//#endif
return true;
#if 0
	//change to segment linear
	uint8_t index;
	index = gRxPacket.payload[7];
	if (gFactoryCfg.debugMode == 0xff || gFactoryCfg.debugMode == 0xfe)	//only change calibrating parameter in debug mode
	{
		switch (index)
		{
			case 0:
				gFactoryCfg.calibration.adcHumidity20T25 = JOINT_TO_INT16(gRxPacket.payload, 8);
				gFactoryCfg.calibration.h20t25_real = JOINT_TO_INT16(gRxPacket.payload, 10);
				break;
			case 1:
				gFactoryCfg.calibration.adcHumidity40T25 = JOINT_TO_INT16(gRxPacket.payload, 8);
				gFactoryCfg.calibration.h40t25_real = JOINT_TO_INT16(gRxPacket.payload, 10);
				break;
			case 2:
				gFactoryCfg.calibration.adcHumidity60T25 = JOINT_TO_INT16(gRxPacket.payload, 8);
				gFactoryCfg.calibration.h60t25_real = JOINT_TO_INT16(gRxPacket.payload, 10);
				break;
			case 3:
				gFactoryCfg.calibration.adcHumidity20T50 = JOINT_TO_INT16(gRxPacket.payload, 8);
				gFactoryCfg.calibration.h20t50_real = JOINT_TO_INT16(gRxPacket.payload, 10);
				break;
			case 4:
				gFactoryCfg.calibration.adcHumidity40T50 = JOINT_TO_INT16(gRxPacket.payload, 8);
				gFactoryCfg.calibration.h40t50_real = JOINT_TO_INT16(gRxPacket.payload, 10);
				break;
			case 5:
			default:
				gFactoryCfg.calibration.adcHumidity60T50 = JOINT_TO_INT16(gRxPacket.payload, 8);
				gFactoryCfg.calibration.h60t50_real = JOINT_TO_INT16(gRxPacket.payload, 10);
				break;
		}
//		gFactoryCfg.calibration.adc5VT25 = JOINT_TO_INT16(gRxPacket.payload, 7);
//		gFactoryCfg.calibration.adc5VT50 = JOINT_TO_INT16(gRxPacket.payload, 9);
//		gFactoryCfg.calibration.adcHumidity40T25 = JOINT_TO_INT16(gRxPacket.payload, 13);
//		gFactoryCfg.calibration.adcHumidity60T25 = JOINT_TO_INT16(gRxPacket.payload, 15);
//		gFactoryCfg.calibration.adcHumidity20T50 = JOINT_TO_INT16(gRxPacket.payload, 17);
//		gFactoryCfg.calibration.adcHumidity40T50 = JOINT_TO_INT16(gRxPacket.payload, 19);
//		gFactoryCfg.calibration.adcHumidity60T50 = JOINT_TO_INT16(gRxPacket.payload, 21);
//		gFactoryCfg.calibration.h40t25_real = JOINT_TO_INT16(gRxPacket.payload, 25);
//		gFactoryCfg.calibration.h60t25_real = JOINT_TO_INT16(gRxPacket.payload, 27);
//		gFactoryCfg.calibration.h20t50_real = JOINT_TO_INT16(gRxPacket.payload, 29);
//		gFactoryCfg.calibration.h40t50_real = JOINT_TO_INT16(gRxPacket.payload, 31);
//		gFactoryCfg.calibration.h60t50_real = JOINT_TO_INT16(gRxPacket.payload, 33);
#ifdef UART_DEBUG_DISPLAY
		Display_printf(displayHandle, 0, 0, "adchumidity %d, h_real %d.\r\n", JOINT_TO_INT16(gRxPacket.payload, 8), JOINT_TO_INT16(gRxPacket.payload, 10));
#endif
return true;
		flashData_savegFactoryCfg();
		replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	}
	else
	{
		replyRfWriteCmdStatus(RF_WRITE_CMD_FAIL);
	}
#endif
return true;
	return true;
#endif
return true;
}

static bool rfCmd_getCaliHumiPara(void)
{
#if 0
	int16_t a_shift, b_shift;
	a_shift = (int16_t)(gFactoryCfg.calibration.humidity_a_shift * 1.0e5);
	b_shift = (int16_t)(gFactoryCfg.calibration.humidity_b_shift * 1.0e6);
	gTxPacket.payload[6] = 4;
	gTxPacket.payload[7] = BYTE0_OF(a_shift);
	gTxPacket.payload[8] = BYTE1_OF(a_shift);
	gTxPacket.payload[9] =  BYTE0_OF(b_shift);
	gTxPacket.payload[10] = BYTE1_OF(b_shift);
	gTxPacket.len = 11;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
}

static bool rfCmd_caliPres(void)
{
#if 0
	int32_t k_press, y_press;
#ifdef UART_DEBUG_DISPLAY
	Display_printf(displayHandle, 0, 0, "debug mode %d.\r\n", gFactoryCfg.debugMode);
#endif
return true;
	if (gFactoryCfg.debugMode == 0xff || gFactoryCfg.debugMode == 0xfe)	//only change calibrating parameter in debug mode
	{
		memcpy(&(k_press), &(gRxPacket.payload[7]), 4);
		memcpy(&(y_press), &(gRxPacket.payload[11]), 4);
		gFactoryCfg.calibration.pressure_k = (float) k_press / 1.0e8;
		gFactoryCfg.calibration.pressure_y = (float) y_press / 1.0e4;
#ifdef UART_DEBUG_DISPLAY
		Display_printf(displayHandle, 0, 0, "change presserature k %f, y %f.\r\n", gFactoryCfg.calibration.pressure_k, gFactoryCfg.calibration.pressure_y);
#endif
return true;
		flashData_savegFactoryCfg();
		replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	}
	else
	{
		replyRfWriteCmdStatus(RF_WRITE_CMD_FAIL);
	}
	return true;
#endif
return true;
}

static bool rfCmd_getCaliPresPara(void)
{
#if 0
	int32_t k_t, y_t;
	k_t = (int32_t)(gFactoryCfg.calibration.pressure_k * 1.0e8);
	y_t = (int32_t)(gFactoryCfg.calibration.pressure_y * 1.0e4);
	gTxPacket.payload[6] = 8;
	gTxPacket.payload[7] = BYTE0_OF(k_t);
	gTxPacket.payload[8] = BYTE1_OF(k_t);
	gTxPacket.payload[9] = BYTE2_OF(k_t);
	gTxPacket.payload[10] = BYTE3_OF(k_t);
	gTxPacket.payload[11] = BYTE0_OF(y_t);
	gTxPacket.payload[12] = BYTE1_OF(y_t);
	gTxPacket.payload[13] = BYTE2_OF(y_t);
	gTxPacket.payload[14] = BYTE3_OF(y_t);
	gTxPacket.payload[15] = gRxPacket.rssi;
	gTxPacket.payload[16] = 0;
	gTxPacket.len = 17;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
}

static bool rfCmd_setCalipressureMethod(void)
{
#if 0
	uint16_t method;
	gFactoryCfg.calibration.pressConvertingMethod = gRxPacket.payload[7];
	flashData_savegFactoryCfg();
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	return true;
#endif
return true;
}

static bool rfCmd_getCalipressureMethod(void)
{
#if 0
	gTxPacket.payload[6] = 1;
	gTxPacket.payload[7] = gFactoryCfg.calibration.pressConvertingMethod;
	gTxPacket.payload[8] = gRxPacket.rssi;
	gTxPacket.payload[9] = 0;
	gTxPacket.len = 10;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
}

static bool rfCmd_setPressureCaliParaWithTemperature(void)
{
#if 0
#if 0
	int32_t k1, y1, k2, y2, k3, y3, k4, y4;
	memcpy(&(k1), &(gRxPacket.payload[7]), 4);
	memcpy(&(y1), &(gRxPacket.payload[11]), 4);
	memcpy(&(k2), &(gRxPacket.payload[15]), 4);
	memcpy(&(y2), &(gRxPacket.payload[19]), 4);
	memcpy(&(k3), &(gRxPacket.payload[23]), 4);
	memcpy(&(y3), &(gRxPacket.payload[27]), 4);
	gFactoryCfg.calibration.pressure_k_M10 = (float) k1 / 1.0e8;
	gFactoryCfg.calibration.pressure_y_M10 = (float) y1 / 1.0e4;
	gFactoryCfg.calibration.pressure_k_P25 = (float) k2 / 1.0e8;
	gFactoryCfg.calibration.pressure_y_P25 = (float) y2 / 1.0e4;
	gFactoryCfg.calibration.pressure_k_P50 = (float) k3 / 1.0e8;
	gFactoryCfg.calibration.pressure_y_P50 = (float) y3 / 1.0e4;
#ifdef UART_DEBUG_DISPLAY
	Display_printf(displayHandle, 0, 0, "change presserature k1 %f, y1 %f.\r\n", gFactoryCfg.calibration.pressure_k_M10, gFactoryCfg.calibration.pressure_y_M10);
	Display_printf(displayHandle, 0, 0, "change presserature k4 %f, y4 %f.\r\n", gFactoryCfg.calibration.pressure_k_P50, gFactoryCfg.calibration.pressure_y_P50);
#endif
return true;
		flashData_savegFactoryCfg();
#endif
return true;
	uint8_t index;
	int32_t kt, yt;
	index = gRxPacket.payload[7];
	memcpy(&(kt), &(gRxPacket.payload[8]), 4);
	memcpy(&(yt), &(gRxPacket.payload[12]), 4);
	switch (index)
	{
		case 0:
			gFactoryCfg.calibration.pressure_k_M10 = (float) kt / 1.0e8;
			gFactoryCfg.calibration.pressure_y_M10 = (float) yt / 1.0e4;
			break;
		case 1:
			gFactoryCfg.calibration.pressure_k_P25 = (float) kt / 1.0e8;
			gFactoryCfg.calibration.pressure_y_P25 = (float) yt / 1.0e4;
			break;
		case 2:
		default:
			gFactoryCfg.calibration.pressure_k_P50 = (float) kt / 1.0e8;
			gFactoryCfg.calibration.pressure_y_P50 = (float) yt / 1.0e4;
			break;
	}
#ifdef UART_DEBUG_DISPLAY
	Display_printf(displayHandle, 0, 0, "index %d, change presserature k1 %f, y1 %f.\r\n", index, (float) kt / 1.0e8, (float) yt / 1.0e4);
#endif
return true;
	flashData_savegFactoryCfg();
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);

	return true;
#endif
return true;
}

static bool rfCmd_getPressureCaliParaWithTemperature(void)
{
#if 0
	int32_t k_t, y_t;
	uint8_t index;
	index = gRxPacket.payload[7];
	switch (index)
	{
		case 0:
			k_t = (int32_t)(gFactoryCfg.calibration.pressure_k_M10 * 1.0e8);
			y_t = (int32_t)(gFactoryCfg.calibration.pressure_y_M10 * 1.0e4);
			break;
		case 1:
			k_t = (int32_t)(gFactoryCfg.calibration.pressure_k_P25 * 1.0e8);
			y_t = (int32_t)(gFactoryCfg.calibration.pressure_y_P25 * 1.0e4);
			break;
		case 2:
		default:
			k_t = (int32_t)(gFactoryCfg.calibration.pressure_k_P50 * 1.0e8);
			y_t = (int32_t)(gFactoryCfg.calibration.pressure_y_P50 * 1.0e4);
			break;
	}
	gTxPacket.payload[6] = 9;
	gTxPacket.payload[7] = index;

	gTxPacket.payload[8] = BYTE0_OF(k_t);
	gTxPacket.payload[9] = BYTE1_OF(k_t);
	gTxPacket.payload[10] = BYTE2_OF(k_t);
	gTxPacket.payload[11] = BYTE3_OF(k_t);
	gTxPacket.payload[12] = BYTE0_OF(y_t);
	gTxPacket.payload[13] = BYTE1_OF(y_t);
	gTxPacket.payload[14] = BYTE2_OF(y_t);
	gTxPacket.payload[15] = BYTE3_OF(y_t);
	gTxPacket.payload[16] = gRxPacket.rssi;
	gTxPacket.len = 17;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
}

static bool rfCmd_setDebugMode(void)
{
#if 0
	//TODO: add redunt value check for safe operation
	gFactoryCfg.debugMode = gRxPacket.payload[7];
	if (gFactoryCfg.debugMode == 0x00)
	{
//	    gSensorState = sysStateSleep;	
		gFactoryCfg.idleTimeout = 5;	//default changed to 5 min idle timeout
	}
	else if (gFactoryCfg.debugMode == 0xfe)
	{
		gFactoryCfg.idleTimeout = 30;
	}
	else if (gFactoryCfg.debugMode == 0xff)
	{
		PIN_setOutputValue(pinHandle, PIN_5V_EN, 1);	//always enable adc for pressure calibration.
	}
	flashData_savegFactoryCfg();
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	return true;
#endif
return true;
}

static bool rfCmd_getDebugMode(void)
{
#if 0
	gTxPacket.payload[6] = 1;
	gTxPacket.payload[7] = gFactoryCfg.debugMode;
	gTxPacket.payload[8] = gRxPacket.rssi;
	gTxPacket.payload[9] = 0;
	gTxPacket.len = 10;
	EasyLink_transmit(&gTxPacket);
	
	return true;
#endif
return true;
}

static bool rfCmd_setSniffingInterval(void)
{
#if 0
	gCustomerCfg.sniffingPeriod = gRxPacket.payload[7];
#ifdef UART_DEBUG_DISPLAY
	Display_printf(displayHandle, 0, 0, "set sniffing %d\r\n", gCustomerCfg.sniffingPeriod);
#endif
return true;
	
	//limited to 5~60
	if (gCustomerCfg.sniffingPeriod < 5)
	{
		gCustomerCfg.sniffingPeriod = 5;
	}
	else if (gCustomerCfg.sniffingPeriod > 60)
	{
		gCustomerCfg.sniffingPeriod = 60;
	}
	flashData_savegCustomerCfg();
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	return true;
#endif
return true;
}

static bool rfCmd_getSniffingInterval(void)
{
#if 0
#ifdef UART_DEBUG_DISPLAY
	Display_printf(displayHandle, 0, 0, "get sniffing %d\r\n", gCustomerCfg.sniffingPeriod);
#endif
return true;
	gTxPacket.payload[6] = 1;
	gTxPacket.payload[7] = gCustomerCfg.sniffingPeriod;
	gTxPacket.payload[8] = gRxPacket.rssi;
	gTxPacket.payload[9] = 0;
	gTxPacket.len = 10;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
}

static bool rfCmd_setFlashIndex(void)
{
#if 0
	//only for debug purpose
	gDataOperationParameter.saveIndex = JOINT_TO_INT32(gRxPacket.payload, 7);
	gDataOperationParameter.sendIndex = JOINT_TO_INT32(gRxPacket.payload, 11);
	flashData_savegDataOperationParameter();
#ifdef UART_DEBUG_DISPLAY
	Display_printf(displayHandle, 0, 0, "set Index save: %d, send: %d\r\n", gDataOperationParameter.saveIndex, gDataOperationParameter.sendIndex);
#endif
return true;
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	return true;
#endif
return true;
}

static bool rfCmd_getFlashIndex(void)
{
#if 0
	//only for debug purpose
#ifdef UART_DEBUG_DISPLAY
	Display_printf(displayHandle, 0, 0, "set Index save: %d, send: %d\r\n", gDataOperationParameter.saveIndex, gDataOperationParameter.sendIndex);
#endif
return true;
	gTxPacket.payload[6] = 8;
	gTxPacket.payload[7] = BYTE0_OF(gDataOperationParameter.saveIndex);
	gTxPacket.payload[8] = BYTE1_OF(gDataOperationParameter.saveIndex);
	gTxPacket.payload[9] = BYTE2_OF(gDataOperationParameter.saveIndex);
	gTxPacket.payload[10] = BYTE3_OF(gDataOperationParameter.saveIndex);
	gTxPacket.payload[11] = BYTE0_OF(gDataOperationParameter.sendIndex);
	gTxPacket.payload[12] = BYTE1_OF(gDataOperationParameter.sendIndex);
	gTxPacket.payload[13] = BYTE2_OF(gDataOperationParameter.sendIndex);
	gTxPacket.payload[14] = BYTE3_OF(gDataOperationParameter.sendIndex);
	gTxPacket.len = 15;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
}

static bool rfCmd_resetFlashIndex(void)
{
#if 0
	gDataOperationParameter.saveIndex = 0;
	gDataOperationParameter.sendIndex = 0;
	gDataOperationParameter.firstTimeFull = 0;
	gDataOperationParameter.flashFull = 0;
	flashData_savegDataOperationParameter();
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	return true;
#endif
return true;
}

static bool rfCmd_setIdleTimeout(void)
{
#if 0
	gFactoryCfg.idleTimeout = gRxPacket.payload[7];
#ifdef UART_DEBUG_DISPLAY
	Display_printf(displayHandle, 0, 0, "idleTimeout: %d\r\n", gFactoryCfg.idleTimeout);
#endif
return true;
	replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	return true;
#endif
return true;
}

static bool rfCmd_getIdleTimeout(void)
{
#if 0
	gTxPacket.payload[6] = 1;
	gTxPacket.payload[7] = gFactoryCfg.idleTimeout;
	gTxPacket.len = 8;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
}
#if 0	//try for accelormeter, to be complete
static bool rfCmd_getAccelerometer(void)
{
#if 0
	ADXL355_dataScan();
	gTxPacket.payload[6] = 12;
	gTxPacket.payload[7 ] = BYTE0_OF(i32SensorX);
	gTxPacket.payload[8 ] = BYTE1_OF(i32SensorX);
	gTxPacket.payload[9 ] = BYTE2_OF(i32SensorX);
	gTxPacket.payload[10] = BYTE3_OF(i32SensorX);
	gTxPacket.payload[11] = BYTE0_OF(i32SensorY);
	gTxPacket.payload[12] = BYTE1_OF(i32SensorY);
	gTxPacket.payload[13] = BYTE2_OF(i32SensorY);
	gTxPacket.payload[14] = BYTE3_OF(i32SensorY);
	gTxPacket.payload[15] = BYTE0_OF(i32SensorZ);
	gTxPacket.payload[16] = BYTE1_OF(i32SensorZ);
	gTxPacket.payload[17] = BYTE2_OF(i32SensorZ);
	gTxPacket.payload[18] = BYTE3_OF(i32SensorZ);
	gTxPacket.payload[19] = gRxPacket.rssi;
	gTxPacket.payload[20] = 0;
	gTxPacket.len = 21;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
}
#endif

static bool rfCmd_version(void)
{
#if 0
	gTxPacket.payload[6] = 3;
	gTxPacket.payload[7] = VER_MAJOR;
	gTxPacket.payload[8] = VER_MINOR;
	gTxPacket.payload[9] = VER_PATCH;
	gTxPacket.payload[10] = gRxPacket.rssi;
	gTxPacket.payload[11] = 0;
	gTxPacket.len = 12;
	EasyLink_transmit(&gTxPacket);
	return true;
#endif
return true;
}

static bool rfCmd_testRf(void)
{
#if 0
	gTxPacket.payload[6] = 3;
	gTxPacket.payload[7] = 1;
	gTxPacket.payload[8] = gRxPacket.payload[8];
	gTxPacket.payload[9] = gRxPacket.payload[9];
	gTxPacket.payload[10] = gRxPacket.rssi;
	gTxPacket.payload[11] = 0;
	gTxPacket.len = 12;
	EasyLink_transmit(&gTxPacket);
#ifdef UART_DEBUG_DISPLAY
	Display_printf(displayHandle, 0, 0, "test %d, %d\r\n", gTxPacket.payload[8], gTxPacket.payload[9]);
#endif
return true;
	return true;
#endif
return true;
}

static bool rfCmd_keepRfSending(void)
{
#if 0
	int i;
	uint8_t last_sec;
	last_sec = gRxPacket.payload[7];
#ifdef UART_DEBUG_DISPLAY
	Display_printf(displayHandle, 0, 0, "last_sec %d\r\n", last_sec);
#endif
return true;
	for (i = 0; i < (200 * last_sec); i++)		//according to test: 1000 times is about 5s
	{
		replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	}
	return true;
#endif
return true;
}

static bool rfCmd_restoreCalibrationDefault(void)
{
#if 0
	if (gFactoryCfg.debugMode == 0xff)	//only restore to default in factory mode.
	{
		//flashData_reset();
	    memcpy(&(gFactoryCfg.calibration), &(defaultFactoryCfg.calibration),sizeof(Calibration_s));
		replyRfWriteCmdStatus(RF_WRITE_CMD_SUCCESS);
	}
	else
		replyRfWriteCmdStatus(RF_WRITE_CMD_FAIL);
	return true;
#endif
return true;
}

/*------end of cmd process function---------------------*/

/* -----command lookup dictitonary when sensor is in wakeup state-----------------------*/
/* for simple protocol, just sort commands according to number.
 * TODO: for mass amount of commands, may use hash table or binary search table.*/

CmdDict_s cmdDict [] = 
{
//	sysGlobalWKUPCmd,	rfCmd_wakeup,
	sysGlobalSleepCmd,	rfCmd_sleep,
	sysGlobalReadDBCmd,	rfCmd_readDB,

   sysLocationDataSet,		rfCmd_locationDataSet,
   sysLocationDataReadAll,	rfCmd_locationDataReadAll,
   sysLocationDataDeleteAll, rfCmd_locationDataDeleteAll,

//	sysWKUPCmd,			rfCmd_wakeup,
	sysOnceSampleCmd,	rfCmd_onceSample,
	sysReadADCCmd,		rfCmd_readADC,
	sysReadDBCmd,		rfCmd_readDB,
	sysSleepCmd,		rfCmd_sleep,

	sysInConfigCmd,		rfCmd_inConfig,
	sysConfigIDCmd,		rfCmd_configID,
	sysConfigTimeCmd,	rfCmd_configTime,
	sysConfigSendPeriodCmd,	rfCmd_configSendPeriod,
	sysReadSendPeriodCmd,	rfCmd_readSendPeriod,
	sysConfigSavePeriodCmd,	rfCmd_configSavePeriod,
	sysReadSavePeriodCmd,	rfCmd_readSavePeriod,
	sysConfigStateCmd,	rfCmd_configState,
	sysReadStateCmd,	rfCmd_readState,
	sysOutConfigCmd,	rfCmd_outConfig,

	sysSetCustomerSer,	rfCmd_setCustomerSer,
	sysGetCustomerSer,	rfCmd_getCustomerSer,

	sysCallStatusCmd,	rfCmd_callStatus,
	sysCallCmd,			rfCmd_sysCall,

	sysCaliTempCmd,     rfCmd_caliTemp,
	sysGetCaliTempParaCmd, rfCmd_getCaliTempPara, 
	sysSetCaliTemp4TCmd,	rfCmd_setCaliTemp4T,
	sysGetCaliTemp4TCmd,	rfCmd_getCaliTemp4T,
	sysSetTempCaliMethodCmd,	rfCmd_setTempCaliMethod,
	sysGetTempCaliMethodCmd,	rfCmd_getTempCaliMethod,
	sysCaliHumiCmd,	rfCmd_caliHumi,
	sysGetCaliHumiParaCmd,	rfCmd_getCaliHumiPara,
	sysCaliPresCmd,	rfCmd_caliPres,
	sysGetCaliPressureParaCmd,	rfCmd_getCaliPresPara,
	sysSetCaliPressureMethodCmd, rfCmd_setCalipressureMethod,
	sysGetCaliPressureMethodCmd, rfCmd_getCalipressureMethod,
	sysSetPressureCaliParaWithTemperature, rfCmd_setPressureCaliParaWithTemperature,
	sysGetPressureCaliParaWithTemperature, rfCmd_getPressureCaliParaWithTemperature,

	sysSetDebugModeCmd, rfCmd_setDebugMode,
	sysGetDebugModeCmd, rfCmd_getDebugMode,

	sysSetSniffingInterval, rfCmd_setSniffingInterval,
	sysGetSniffingInterval, rfCmd_getSniffingInterval,
	sysSetFlashIndex,	rfCmd_setFlashIndex,
	sysGetFlashIndex,	rfCmd_getFlashIndex,
	sysResetFlashIndex,	rfCmd_resetFlashIndex,
	sysSetIdleTimeout,	rfCmd_setIdleTimeout,
	sysGetIdleTimeout,	rfCmd_getIdleTimeout,
//	sysGetAccelerometer,	rfCmd_getAccelerometer,

	sysVersionCmd,		rfCmd_version,
	sysTestRfCmd,		rfCmd_testRf,
	sysKeepRfSendingCmd, rfCmd_keepRfSending,
	sysRestoreCalibrationDefault, rfCmd_restoreCalibrationDefault,
};

/*------end of cmd command lookup dictitonary---------------------*/

/* ---------------pulic functions---------------- */
void rfCmdProc_processCmd()
{
#if 0
	int i;
#ifdef UART_DEBUG_DISPLAY
//	Display_printf(displayHandle, 0, 0, "payload[4-5]: %02x %02x", gRxPacket.payload[4], gRxPacket.payload[5]);
#endif

	if (isOwnAddr())
	{
		cmdType = CMDTYPE_SINGLE;
	}
	else if (isBroadcastAddr())
	{
		cmdType = CMDTYPE_BROADCAST;
	}
	else
	{
		cmdType = CMDTYPE_NONE;
	    return ;    //do nothing
	}


	if (gSensorState == sysStateSleep)
	{
		//only accept wakeup command.
		if (gRxPacket.payload[RF_CMD_INDEX] == sysGlobalWKUPCmd ||
			gRxPacket.payload[RF_CMD_INDEX] == sysWKUPCmd)
		{
			rfCmd_wakeup();
			Event_post(stateEvent, STATEEVENT_WAKEUP);
		}
	}
	else
	{
		for (i = 0; i < sizeof(cmdDict) / sizeof(CmdDict_s); i++)
		{
			if (gRxPacket.payload[RF_CMD_INDEX] == cmdDict[i].cmd)
			{
				gTxPacket.payload[RF_CMD_INDEX] = gRxPacket.payload[RF_CMD_INDEX];
				cmdDict[i].pFunc();
				Event_post(stateEvent, STATEEVENT_VALID_COMMAND_RECEIVED);
				break;
			}
		}
	}
#endif
}

void rfCmdProc_sendOneRecord(Record_s record)
{
#if 0
	gTxPacket.dstAddr[0] = 0xdd;
	gTxPacket.dstAddr[1] = 0x66;

	gTxPacket.len = 24;

	gTxPacket.payload[5] = 16;//TODO: the definition of data len
	memcpy(&gTxPacket.payload[6], (void *)&record, sizeof(SensorData_s));
	gTxPacket.payload[14] = record.rtcTime.year;
	gTxPacket.payload[15] = record.rtcTime.month;
	gTxPacket.payload[16] = record.rtcTime.date;
	gTxPacket.payload[17] = record.rtcTime.hour;
	gTxPacket.payload[18] = record.rtcTime.minute;
	gTxPacket.payload[19] = record.rtcTime.second;
	gTxPacket.payload[20] = frame_count & 0xff;
	gTxPacket.payload[21] = (frame_count >> 8) & 0xff;
	frame_count++;
//	crc = cal_crc
	gTxPacket.payload[22] = 0;
	gTxPacket.payload[23] = 0;
	EasyLink_transmit(&gTxPacket);

	//switch tx addr back
	gTxPacket.dstAddr[0] = 0xaa;
	gTxPacket.dstAddr[1] = 0x55;
#endif
}

void rfCmdProc_activeSendRecord(Record_s record)
{
#if 0
	gTxPacket.dstAddr[0] = 0xdd;
	gTxPacket.dstAddr[1] = 0x66;

	gTxPacket.len = 24;

	gTxPacket.payload[5] = 16;//TODO: the definition of data len
//	memcpy(&gTxPacket.payload[6], (void *)&record, sizeof(Record_s));
	memcpy(&gTxPacket.payload[6], (void *)&record, sizeof(SensorData_s));
	gTxPacket.payload[14] = record.rtcTime.year;
	gTxPacket.payload[15] = record.rtcTime.month;
	gTxPacket.payload[16] = record.rtcTime.date;
	gTxPacket.payload[17] = record.rtcTime.hour;
	gTxPacket.payload[18] = record.rtcTime.minute;
	gTxPacket.payload[19] = record.rtcTime.second;
	gTxPacket.payload[20] = 0xff;		//TODO, for period sending, frame count always 0xffff 
	gTxPacket.payload[21] = 0xff;
	frame_count++;
//	crc = cal_crc
	gTxPacket.payload[22] = 0;
	gTxPacket.payload[23] = 0;
	Event_post(rfEvent, RFEVENT_TRANSFER);

	//TODO, add handshake protocol to confirm successful sending.
#endif
}
/* --------------------end fo public function------------------------- */

bool isOwnAddr(void)
{
#if 0
	//for backward compatibility, just check payload[4]: number
//	return (gRxPacket.payload[4] == gFactoryCfg.manufacturerSer.number);
	return ((gRxPacket.payload[0] == 0)
		&& (gRxPacket.payload[1] == 0)
		&& (gRxPacket.payload[2] == 0)
		&& (gRxPacket.payload[4] == gFactoryCfg.manufacturerSer.number))
		|| ((gRxPacket.payload[0] == gFactoryCfg.manufacturerSer.year)
		&& (gRxPacket.payload[1] == gFactoryCfg.manufacturerSer.flag[0])
		&& (gRxPacket.payload[2] == gFactoryCfg.manufacturerSer.flag[1])
		&& (gRxPacket.payload[3] == gFactoryCfg.manufacturerSer.batch)
		&& (gRxPacket.payload[4] == gFactoryCfg.manufacturerSer.number));
//		|| ((gRxPacket.payload[0] == gCustomerCfg.customerSer.year)
//		&& (gRxPacket.payload[1] == gCustomerCfg.customerSer.flag[0])
//		&& (gRxPacket.payload[2] == gCustomerCfg.customerSer.flag[1])
//		&& (gRxPacket.payload[3] == gCustomerCfg.customerSer.batch)
//		&& (gRxPacket.payload[4] == gCustomerCfg.customerSer.number));
#endif
}
bool isBroadcastAddr(void)
{
#if 0
//	return (gRxPacket.payload[0] == 0x00 && gRxPacket.payload[1] == 0x00 && gRxPacket.payload[2] == 0x00 && gRxPacket.payload[3] == 0x00 && gRxPacket.payload[4] == 0x00);
	if (gRxPacket.payload[0] == 0x00 && gRxPacket.payload[1] == 0x00 && gRxPacket.payload[2] == 0x00 && gRxPacket.payload[3] == 0x00 && gRxPacket.payload[4] == 0x00)
	{
		return true;
	}
	else if (gFactoryCfg.manufacturerSer.flag[0] > 0 && gFactoryCfg.manufacturerSer.flag[1] == 0)	//8359 id
	{
		return (gRxPacket.payload[0] == 0x00) && (gRxPacket.payload[1] == 0x01) && (gRxPacket.payload[2] == 0x00) && (gRxPacket.payload[3] == 0x00) && (gRxPacket.payload[4] == 0x00);
	}
	else if (gFactoryCfg.manufacturerSer.flag[0] >= 'A' && gFactoryCfg.manufacturerSer.flag[1] > 0) //206 id
	{
		return (gRxPacket.payload[0] == 0x00) && (gRxPacket.payload[1] >= 0x41) && (gRxPacket.payload[2] == 0x00) && (gRxPacket.payload[3] == 0x00) && (gRxPacket.payload[4] == 0x00);
	}
	else
		return false;
#endif
}

bool isBatchAddr(void)
{
#if 0
	if (gFactoryCfg.manufacturerSer.flag[0] > 0 && gFactoryCfg.manufacturerSer.flag[1] == 0)	//8359 id
	{
		return (gRxPacket.payload[0] == gFactoryCfg.manufacturerSer.year)
			&& (gRxPacket.payload[1] == gFactoryCfg.manufacturerSer.flag[0])
			&& (gRxPacket.payload[2] == 0)
			&& (gRxPacket.payload[3] == 0)
			&& (gRxPacket.payload[4] == 0);
	}
	else if (gFactoryCfg.manufacturerSer.flag[0] >= 'A' && gFactoryCfg.manufacturerSer.flag[1] > 0) //206 id
	{
		return (gRxPacket.payload[0] == gFactoryCfg.manufacturerSer.year)
			&& (gRxPacket.payload[1] == gFactoryCfg.manufacturerSer.flag[0])
			&& (gRxPacket.payload[2] == gFactoryCfg.manufacturerSer.flag[1])
			&& (gRxPacket.payload[3] == 0)
			&& (gRxPacket.payload[4] == 0);
	}
	else
		return false;
#endif
}

uint16_t cal_crc(unsigned char *ptr, unsigned char len)
{
#if 0
    unsigned char i;
    uint16_t crc=0;
    while(len-- != 0)
    {
        for(i=0x80; i!=0; i>>=1)
        {
            if((crc&0x8000)!=0)
            {
               crc<<=1;
               crc^=(crc_mul);
            }else{
               crc<<=1;
            }
            if((*ptr&i)!=0)
            {
               crc ^= (crc_mul);
            }
        }
        ptr ++;
    }
    return (crc);
#endif
}

static void replyRfWriteCmdStatus(RFCmdStatus_e cmd_status)
{
#if 0
	gTxPacket.payload[6] = 0x01;
	gTxPacket.payload[7] = (uint8_t) cmd_status;
	//TODO: the last two byte originally used as CRC, change to rssi later.
	gTxPacket.payload[8] = gRxPacket.rssi;
	gTxPacket.payload[9] = 0;
	gTxPacket.len = 10;
	EasyLink_transmit(&gTxPacket);
	//TODO: for broadcast command, replay with TDMA
#endif
}

/* finish rf read command and then send via rf
 * tail_index must be the last data byte index + 1*/
static void replyRfReadCmd(int tail_index)
{
#if 0
	gTxPacket.payload[tail_index] = gRxPacket.rssi;
	gTxPacket.payload[tail_index + 1] = 0;
	gTxPacket.len = tail_index + 2;
	EasyLink_transmit(&gTxPacket);
#endif
}
