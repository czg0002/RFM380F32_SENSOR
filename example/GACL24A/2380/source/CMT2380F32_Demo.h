
#ifndef	_CMT2380F32_DEMO_
#define _CMT2380F32_DEMO_
#include "ddl.h"
#include "cmt2300a_defs.h"

#define  RF_STATUS   1//1为接收，0为发射

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

/***************************************************
Pxx_SEL:配置GPIO的功能(作为普通GPIO还是其他复用功能)
PxDIR:输入输出配置寄存器   1=输入  0=输出
PxIN:输入值寄存器(读取端口电平状态)
PxOUT:输出电平状态配置寄存器
PxADS:数模配置寄存器			1=模拟	0=数字(默认)
PxDR:驱动能力配置寄存器		1=低驱动	0=高驱动(默认)
PxPU:上拉使能配置寄存器		1=使能		0=禁止(默认)
PxPD:下拉使能配置寄存器		1=使能		0=禁止(默认)
PxOD:开漏输出配置寄存器		1=使能		0=禁止(默认)
****************************************************/
//P0													PxDIR		PxOUT		PxADS		PxDR	PxPU	PxPD	PxOD		
  #define GPIO1	0x04	//				1				0				0				0			0			0			0
	#define	GPIO2	0x10  //				1				0				0				0			0			0			0
  #define	SDIO	0x08	//此处修改为P2.3做sido，之前是P2.4
	
	
#define	P0DIR_Data	0x00000006//   0110---P01 P02输输入  P00 P03输出
#define	P1DIR_Data	0x00000000 //
#define	P2DIR_Data	0x00000000//0x00000008   
#define	P3DIR_Data	0x00000010  //P3.4 设置为输入

#define	P0OUT_Data	0x00000008 // P03输出高 其他输出低
#define	P1OUT_Data	0x00000000  
#define	P2OUT_Data	0x00000000 //P2.3CSB输出高 其他输低
#define	P3OUT_Data	0x00000004  //P3.2 FCSB输出高其他输出低

#define	P0PU_Data		0x00000000
#define	P1PU_Data		0x00000000
#define	P2PU_Data		0x00000000
#define	P3PU_Data		0x00000000


/*hm380*/



//#define	FCSB_H()    Gpio_SetIO(3,2,1)
//#define	FCSB_L()    Gpio_SetIO(3,2,0)

//#define	CSB_H()	    Gpio_SetIO(2,3,1)
//#define	CSB_L()	    Gpio_SetIO(2,3,0)

//#define	SCLK_H()	 	Gpio_SetIO(2,7,1)
//#define	SCLK_L()	 	Gpio_SetIO(2,7,0)

//#define	SDIO_H()	 	Gpio_SetIO(2,6,1)
//#define	SDIO_L()	 	Gpio_SetIO(2,6,0)

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
#define TestKey2()   (M0P_GPIO->P2IN&KEY2)



//变量
extern word SysTime;
extern byte CMT_Bank[12];
extern byte System_Bank[12];
extern byte Frequency_Bank[8];
extern byte Data_Rate_Bank[24];
extern byte Baseband_Bank[29];
extern byte TX_Bank[11];
extern byte Prodution_ID[3];
extern byte TX_Buf[64];
extern word ResetTime;
extern uint16_t  sleep_time_cnt;
extern uint8_t  app_learn_flag; //接收成功标志位
extern uint8_t  app_timeout_cnt;


//Base.c
extern void MCU_Config(void);
extern void IO_Init(void);
//extern void BaseTime0_Init(void);
extern void BaseTime1_Init(void);
extern void MCU_Init(void);
extern void Delay(word Cont);

//interrupt.c
extern void Time0_Irq(void);
extern void Time1_Irq(void);
//KeyScan.c
extern void KeyScan_Rx(void);
extern void KeyScan_Tx(void);
extern byte ReadKey(void);
extern void TestMode(void);
extern void KeyDelay(void);
extern void RxOKDelay(void);

//CMT2300Config.c
extern void Write_SPI8Bit(byte Data);
extern void SPI_Write(byte adder,byte Data);
extern byte Read_SPI8Bit(void);
extern byte SPI_Read(byte Data);
extern byte Read_FIFO(void);
extern void Write_FIFO(byte date);
extern void Reset(void);
extern void GO_SLEEP(void);
extern void GO_STBY(void);
extern void GO_RX(void);
extern void GO_TX(void);
extern void Clr_INT(void);
extern void Clr_FIFO(void);
extern void Send_Pack(byte Str[],byte length,byte Pack_Cnt);
extern void CMT2300A_Init(void);
extern void CMT2300A_EnableReadFifo(void);

#endif

