#ifndef GXHT3x_H
#define GXHT3x_H
//modify by czg for cc1352 platform.
//==============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//==============================================================================
// Project   :  GXHT3x Sample Code (V1.2)
// File      :  GXHT3x.h
// Author    :  MST
// Controller:  NEC V850/SG3 (uPD70F3740)
// Compiler  :  IAR compiler for V850 (3.50A)
// Brief     :  Sensor layer. Definitions of commands and registers,
//              functions for sensor access
//==============================================================================
//---------- Includes ----------------------------------------------------------
#include <stdint.h>
//---------- Defines -----------------------------------------------------------
//#define SHT25_I2C_ADDR 0x40
#define GHXT30_I2C_ADDR1_LEFT_SHIFTED 0x88	//I2C1
#define GHXT30_I2C_ADDR2_LEFT_SHIFTED 0x88	//I2C2
#define GHXT30_I2C_ADDR3_LEFT_SHIFTED 0x8a	//I2C2
// sensor command
typedef enum{
  TRIG_T_MEASUREMENT_HM    = 0xE3, // command trig. temp meas. hold master
  TRIG_RH_MEASUREMENT_HM   = 0xE5, // command trig. humidity meas. hold master
  TRIG_T_MEASUREMENT_POLL  = 0xF3, // command trig. temp meas. no hold master
  TRIG_RH_MEASUREMENT_POLL = 0xF5, // command trig. humidity meas. no hold master
  USER_REG_W               = 0xE6, // command writing user register
  USER_REG_R               = 0xE7, // command reading user register
  SOFT_RESET               = 0xFE  // command soft reset
}etGXHT3xCommand;

typedef enum {
  GXHT3x_RES_12_14BIT       = 0x00, // RH=12bit, T=14bit
  GXHT3x_RES_8_12BIT        = 0x01, // RH= 8bit, T=12bit
  GXHT3x_RES_10_13BIT       = 0x80, // RH=10bit, T=13bit
  GXHT3x_RES_11_11BIT       = 0x81, // RH=11bit, T=11bit
  GXHT3x_RES_MASK           = 0x81  // Mask for res. bits (7,0) in user reg.
} etGXHT3xResolution;

typedef enum {
  GXHT3x_EOB_ON             = 0x40, // end of battery
  GXHT3x_EOB_MASK           = 0x40, // Mask for EOB bit(6) in user reg.
} etGXHT3xEob;

typedef enum {
  GXHT3x_HEATER_ON          = 0x04, // heater on
  GXHT3x_HEATER_OFF         = 0x00, // heater off
  GXHT3x_HEATER_MASK        = 0x04, // Mask for Heater bit(2) in user reg.
} etGXHT3xHeater;

// measurement signal selection
typedef enum{
  HUMIDITY,
  TEMP
}etGXHT3xMeasureType;

typedef enum{
  I2C_ADR_W                = 128,   // sensor I2C address + write bit
  I2C_ADR_R                = 129    // sensor I2C address + read bit
}etI2cHeader;


//==============================================================================
int8_t GXHT3x_CheckCrc(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum);
//==============================================================================
// calculates checksum for n bytes of data and compares it with expected
// checksum
// input:  data[]       checksum is built based on this data
//         nbrOfBytes   checksum is built for n bytes of data
//         checksum     expected checksum
// return: error:       CHECKSUM_ERROR = checksum does not match
//                      0              = checksum matches

//==============================================================================
int8_t GXHT3x_ReadUserRegister(uint8_t *pRegisterValue);
//==============================================================================
// reads the GXHT3x user register (8bit)
// input : -
// output: *pRegisterValue
// return: error

//==============================================================================
int8_t GXHT3x_WriteUserRegister(uint8_t *pRegisterValue);
//==============================================================================
// writes the GXHT3x user register (8bit)
// input : *pRegisterValue
// output: -
// return: error

//==============================================================================
int8_t GXHT3x_MeasurePoll(etGXHT3xMeasureType eGXHT3xMeasureType, uint16_t *pMeasurand);
//==============================================================================
// measures humidity or temperature. This function polls every 10ms until
// measurement is ready.
// input:  eGXHT3xMeasureType
// output: *pMeasurand:  humidity / temperature as raw value
// return: error
// note:   timing for timeout may be changed

//==============================================================================
int8_t GXHT3x_MeasureHM(etGXHT3xMeasureType eGXHT3xMeasureType, uint16_t *pMeasurand);
//==============================================================================
// measures humidity or temperature. This function waits for a hold master until
// measurement is ready or a timeout occurred.
// input:  eGXHT3xMeasureType
// output: *pMeasurand:  humidity / temperature as raw value
// return: error
// note:   timing for timeout may be changed

//==============================================================================
int8_t GXHT3x_SoftReset();
//==============================================================================
// performs a reset
// input:  -
// output: -
// return: error

//==============================================================================
float GXHT3x_CalcRH(uint16_t u16sRH);
//==============================================================================
// calculates the relative humidity
// input:  sRH: humidity raw value (16bit scaled)
// return: pHumidity relative humidity [%RH]

//==============================================================================
float GXHT3x_CalcTemperatureC(uint16_t u16sT);
//==============================================================================
// calculates temperature
// input:  sT: temperature raw value (16bit scaled)
// return: temperature [°C]

//==============================================================================
int8_t GXHT3x_GetSerialNumber(uint8_t u8SerialNumber[]);
//==============================================================================
// gets serial number of GXHT3x according application note "How To
// Read-Out the Serial Number"
// note:   readout of this function is not CRC checked
//
// input:  -
// output: u8SerialNumber: Array of 8 bytes (64Bits)
//         MSB                                         LSB
//         u8SerialNumber[7]             u8SerialNumber[0]
//         SNA_1 SNA_0 SNB_3 SNB_2 SNB_1 SNB_0 SNC_1 SNC_0
// return: error
#endif
