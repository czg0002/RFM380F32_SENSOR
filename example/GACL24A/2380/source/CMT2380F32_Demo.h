
#ifndef	_CMT2380F32_DEMO_
#define _CMT2380F32_DEMO_
#include "ddl.h"
#include "cmt2300a_defs.h"
//#include "cmt2300a_params.h"
//#define  RF_STATUS   1//1为接收，0为发射

typedef unsigned char byte;
typedef unsigned int  word;
typedef unsigned long lword;

typedef union 					
{
 struct 
 	{
 	byte  _FlagBit0:1;			
 	byte  _FlagBit1:1;	 		
 	byte  _FlagBit2:1;			
 	byte  _FlagBit3:1;			
 	byte  _FlagBit4:1;			
 	byte  _FlagBit5:1;			
 	byte  _FlagBit6:1;			
 	byte  _FlagBit7:1;			
 	}BBITS;
 byte BBYTE;
}FlagSTR;


//RF
#define CMT_Bank_Origin     0x00     //CMT
#define CMT_Bank_End        0x0B     //CMT

#define System_Origin       0x0C    //System
#define System_End          0x17    //System

#define Frequency_Origin    0x18    //Frequency
#define Frequency_End       0x1F    //Frequency

#define Data_Rete_Origin    0x20    //Data_Rete
#define Data_Rete_End       0x37    //Data_Rete

#define Baseband_Origin     0x38    //Baseband
#define Baseband_End        0x54    //Baseband

#define TX_Origin           0x55    //TX
#define TX_End              0x5F    //TX

#define PackNum_N           0x56
#define SyncNum_N           0x23

#define Time_100Ms          100

/*RFM380F32S2*/
#define	FCSB_H()    Gpio_SetIO(3,2,1)
#define	FCSB_L()    Gpio_SetIO(3,2,0)

#define	CSB_H()	    Gpio_SetIO(0,3,1)//
#define	CSB_L()	    Gpio_SetIO(0,3,0)

#define	SCLK_H()	 	Gpio_SetIO(2,4,1)
#define	SCLK_L()	 	Gpio_SetIO(2,4,0)

#define	SDIO_H()	 	Gpio_SetIO(2,3,1)
#define	SDIO_L()	 	Gpio_SetIO(2,3,0)



#define SDIO_Output() (M0P_GPIO->P2DIR&=(~SDIO))
#define	SDIO_Input() (M0P_GPIO->P2DIR|=SDIO)

#define Test_SDIO()  (M0P_GPIO->P2IN&SDIO)
#define Test_GPIO1() (M0P_GPIO->P0IN&GPIO1)
#define Test_GPIO2() (M0P_GPIO->P3IN&GPIO2)

extern void Write_SPI8Bit(byte Data);
extern void SPI_Write(byte adder,byte Data);
extern byte Read_SPI8Bit(void);
extern byte SPI_Read(byte Data);
extern byte Read_FIFO(void);
extern void Write_FIFO(byte date);

#endif

