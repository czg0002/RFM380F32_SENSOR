//modify by czg for rfm380 platform.
//==============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//==============================================================================
// Project   :  GXHT3x Sample Code (V1.2)
// File      :  GXHT3x.c
// Author    :  MST
// Controller:  NEC V850/SG3 (uPD70F3740)
// Compiler  :  IAR compiler for V850 (3.50A)
// Brief     :  Sensor layer. Functions for sensor access
//==============================================================================

//---------- Includes ----------------------------------------------------------
#include "stdbool.h"
//#include "i2cComm.h"
#include "i2c.h"
#include "GXHT3x.h"

uint8_t ghxt_i2c_addr = GHXT30_I2C_ADDR1_LEFT_SHIFTED;
//  CRC
const uint16_t POLYNOMIAL = 0x131;  //P(x)=x^8+x^5+x^4+1 = 100110001

//==============================================================================
int8_t GXHT3x_CheckCrc(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum)
//==============================================================================
{
  uint8_t crc = 0;	
  uint8_t byteCtr;
  uint8_t bit;
  //calculates 8-Bit checksum with given polynomial
  for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
  { crc ^= (data[byteCtr]);
    for (bit = 8; bit > 0; --bit)
    { if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
      else crc = (crc << 1);
    }
  }
  if (crc != checksum) return false;
  else return true;
}

//===========================================================================
int8_t GXHT3x_ReadUserRegister(uint8_t *pRegisterValue)
//===========================================================================
{
  uint8_t checksum;   //variable for checksum byte
  int8_t error=0;    //variable for error code

//  I2c_StartCondition();
//  error |= I2c_WriteByte (I2C_ADR_W);
//  error |= I2c_WriteByte (USER_REG_R);
//  I2c_StartCondition();
//  error |= I2c_WriteByte (I2C_ADR_R);
//  *pRegisterValue = I2c_ReadByte(ACK);
//  checksum=I2c_ReadByte(NO_ACK);
//  error |= GXHT3x_CheckCrc (pRegisterValue,1,checksum);
//  I2c_StopCondition();

  error = I2C_MasterReadData(ghxt_i2c_addr, USER_REG_R, pRegisterValue, 1);
  return error;
}

//===========================================================================
int8_t GXHT3x_WriteUserRegister(uint8_t *pRegisterValue)
//===========================================================================
{
  int8_t error=0;   //variable for error code
//
//  I2c_StartCondition();
//  error |= I2c_WriteByte (I2C_ADR_W);
//  error |= I2c_WriteByte (USER_REG_W);
//  error |= I2c_WriteByte (*pRegisterValue);
//  I2c_StopCondition();
  error = I2C_MasterWriteData(ghxt_i2c_addr, USER_REG_W, pRegisterValue, 1);
  return error;
}

//===========================================================================
int8_t GXHT3x_MeasureHM(etGXHT3xMeasureType eGXHT3xMeasureType, uint16_t *pMeasurand)
//===========================================================================
{
  uint8_t  checksum;   //checksum
  uint8_t  data[3];    //data array for checksum verification
  int8_t  error=0;    //error variable
  uint16_t i;          //counting variable
#if 0
  //-- write I2C sensor address and command --
  I2c_StartCondition();
  error |= I2c_WriteByte (I2C_ADR_W); // I2C Adr
  switch(eGXHT3xMeasureType)
  { case HUMIDITY: error |= I2c_WriteByte (TRIG_RH_MEASUREMENT_HM); break;
    case TEMP    : error |= I2c_WriteByte (TRIG_T_MEASUREMENT_HM);  break;
    default: assert(0);
  }
  //-- wait until hold master is released --
  I2c_StartCondition();
  error |= I2c_WriteByte (I2C_ADR_R);
  SCL=HIGH;                     // set SCL I/O port as input
  for(i=0; i<1000; i++)         // wait until master hold is released or
  { DelayMicroSeconds(1000);    // a timeout (~1s) is reached
    if (SCL_CONF==1) break;
  }
  //-- check for timeout --
  if(SCL_CONF==0) error |= TIME_OUT_ERROR;

  //-- read two data bytes and one checksum byte --
  pMeasurand->s16.u8H = data[0] = I2c_ReadByte(ACK);
  pMeasurand->s16.u8L = data[1] = I2c_ReadByte(ACK);
  checksum=I2c_ReadByte(NO_ACK);

  //-- verify checksum --
  error |= GXHT3x_CheckCrc (data,2,checksum);
  I2c_StopCondition();
#endif
  uint8_t subaddr;
  switch(eGXHT3xMeasureType)
  {
	  case HUMIDITY:
		  subaddr = TRIG_RH_MEASUREMENT_HM;
		  break;
	  case TEMP:
		  subaddr = TRIG_T_MEASUREMENT_HM;
		  break;
	  default:
		  return false;
  }
  error = I2C_MasterReadData(ghxt_i2c_addr, subaddr, data, 3);
  *pMeasurand = data[1] + ((uint16_t)data[0] << 8);
  checksum = data[2];
  error |= GXHT3x_CheckCrc (data,2,checksum);
  return error;
}

//===========================================================================
int8_t GXHT3x_MeasurePoll(etGXHT3xMeasureType eGXHT3xMeasureType, uint16_t *pMeasurand)
//===========================================================================
{
  uint8_t  checksum;   //checksum
  uint8_t  data[2];    //data array for checksum verification
  int8_t  error=0;    //error variable
  uint16_t i=0;        //counting variable

#if 0
  //-- write I2C sensor address and command --
  I2c_StartCondition();
  error |= I2c_WriteByte (I2C_ADR_W); // I2C Adr
  switch(eGXHT3xMeasureType)
  { case HUMIDITY: error |= I2c_WriteByte (TRIG_RH_MEASUREMENT_POLL); break;
    case TEMP    : error |= I2c_WriteByte (TRIG_T_MEASUREMENT_POLL);  break;
    default: assert(0);
  }
  //-- poll every 10ms for measurement ready. Timeout after 20 retries (200ms)--
  do
  { I2c_StartCondition();
    DelayMicroSeconds(10000);  //delay 10ms
    if(i++ >= 20) break;
  } while(I2c_WriteByte (I2C_ADR_R) == ACK_ERROR);
  if (i>=20) error |= TIME_OUT_ERROR;

  //-- read two data bytes and one checksum byte --
  pMeasurand->s16.u8H = data[0] = I2c_ReadByte(ACK);
  pMeasurand->s16.u8L = data[1] = I2c_ReadByte(ACK);
  checksum=I2c_ReadByte(NO_ACK);

  //-- verify checksum --
  error |= GXHT3x_CheckCrc (data,2,checksum);
  I2c_StopCondition();
#endif
#if 0
  uint8_t subaddr;
  switch(eGXHT3xMeasureType)
  {
	  case HUMIDITY:
		  subaddr = TRIG_RH_MEASUREMENT_HM;
		  break;
	  case TEMP:
		  subaddr = TRIG_T_MEASUREMENT_HM;
		  break;
	  default:
		  return false;
		  break;
  }
  error = I2C_MasterReadData(ghxt_i2c_addr, subaddr, data, 3);
  *pMeasurand = data[1] + ((uint16_t)data[0] << 8);
  checksum = data[3];
  error |= GXHT3x_CheckCrc (data,2,checksum);
#endif
  return error;
}

//===========================================================================
int8_t GXHT3x_SoftReset()
//===========================================================================
{
  int8_t  error=0;           //error variable
#if 0
  I2c_StartCondition();
  error |= I2c_WriteByte (I2C_ADR_W); // I2C Adr
  error |= I2c_WriteByte (SOFT_RESET);                            // Command
  I2c_StopCondition();

  DelayMicroSeconds(15000); // wait till sensor has restarted
#endif
  return error;
}

//==============================================================================
float GXHT3x_CalcRH(uint16_t u16sRH)
//==============================================================================
{
  float humidityRH;              // variable for result

  u16sRH &= ~0x0003;          // clear bits [1..0] (status bits)
  //-- calculate relative humidity [%RH] --

  humidityRH = -6.0 + 125.0/65536 * (float)u16sRH; // RH= -6 + 125 * SRH/2^16
  return humidityRH;
}

//==============================================================================
float GXHT3x_CalcTemperatureC(uint16_t u16sT)
//==============================================================================
{
  float temperatureC;            // variable for result

  u16sT &= ~0x0003;           // clear bits [1..0] (status bits)

  //-- calculate temperature [°C] --
  temperatureC= -46.85 + 175.72/65536 *(float)u16sT; //T= -46.85 + 175.72 * ST/2^16
  return temperatureC;
}

//==============================================================================
int8_t GXHT3x_GetSerialNumber(uint8_t u8SerialNumber[])
//==============================================================================
{
  int8_t  error=0;                          //error variable
#if 0

  //Read from memory location 1
  I2c_StartCondition();
  error |= I2c_WriteByte (I2C_ADR_W);    //I2C address
  error |= I2c_WriteByte (0xFA);         //Command for readout on-chip memory
  error |= I2c_WriteByte (0x0F);         //on-chip memory address
  I2c_StartCondition();
  error |= I2c_WriteByte (I2C_ADR_R);    //I2C address
  u8SerialNumber[5] = I2c_ReadByte(ACK); //Read SNB_3
  I2c_ReadByte(ACK);                     //Read CRC SNB_3 (CRC is not analyzed)
  u8SerialNumber[4] = I2c_ReadByte(ACK); //Read SNB_2
  I2c_ReadByte(ACK);                     //Read CRC SNB_2 (CRC is not analyzed)
  u8SerialNumber[3] = I2c_ReadByte(ACK); //Read SNB_1
  I2c_ReadByte(ACK);                     //Read CRC SNB_1 (CRC is not analyzed)
  u8SerialNumber[2] = I2c_ReadByte(ACK); //Read SNB_0
  I2c_ReadByte(NO_ACK);                  //Read CRC SNB_0 (CRC is not analyzed)
  I2c_StopCondition();

  //Read from memory location 2
  I2c_StartCondition();
  error |= I2c_WriteByte (I2C_ADR_W);    //I2C address
  error |= I2c_WriteByte (0xFC);         //Command for readout on-chip memory
  error |= I2c_WriteByte (0xC9);         //on-chip memory address
  I2c_StartCondition();
  error |= I2c_WriteByte (I2C_ADR_R);    //I2C address
  u8SerialNumber[1] = I2c_ReadByte(ACK); //Read SNC_1
  u8SerialNumber[0] = I2c_ReadByte(ACK); //Read SNC_0
  I2c_ReadByte(ACK);                     //Read CRC SNC0/1 (CRC is not analyzed)
  u8SerialNumber[7] = I2c_ReadByte(ACK); //Read SNA_1
  u8SerialNumber[6] = I2c_ReadByte(ACK); //Read SNA_0
  I2c_ReadByte(NO_ACK);                  //Read CRC SNA0/1 (CRC is not analyzed)
  I2c_StopCondition();
#endif
  return error;
}
