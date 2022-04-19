#include "CMT2380F32_Demo.h"
#include "cmt2300a_params.h"
#include "uart.h"
#include "flash.h"
#include "Flash_ID.h"

#define KEYCODE_MAX 30
byte KeyCode[KEYCODE_MAX];


#define  P23_VALUE 0x01
#define  P00_VALUE 0x02

uint8_t   app_learn_flag=0;
uint16_t  sleep_time_cnt=0;
uint8_t   app_timeout_cnt=200;


uint8_t    txbuff[63]={   0x00,0x11,0x12,0x14,0x14,0x15,0x16,0x17,0x18,0x19,
												  0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,
	                        0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,
												  0x00,0x11,0x12,0x14,0x14,0x15,0x16,0x17,0x18,0x19,
												  0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,
	                        0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39, 
                      	  0x00,0x00,0x01,
		                  };
uint8_t   txlen;
/**************************************
**名字:KeyScan_Rx
**功能:略
**入参:无        
**出参:无          
***************************************/
uint32_t num=0;
void KeyScan_Rx(void)
{
	unsigned char i=0;	
	uint32_t   u32Data  = 0,length=0;
	uint8_t    len;	
#if RF_STATUS	
	if(Test_GPIO2())    //检测数据接收完成中断
#else
	if(Test_GPIO1())    //检测数据接收完成中断
#endif
	{
		GO_STBY();      
		KeyCode[0]=Read_FIFO();
		length=KeyCode[0];
		if(length>=KEYCODE_MAX||length<=0)
		{
		}
		else
		{
			for(i=0;i<length;i++)
			{
			KeyCode[1+i]=Read_FIFO(); 
			Uart_SendData(UARTCH1,KeyCode[i]);	
//			KeyCode[i]=0; 
			}	
	  }		
		GO_SLEEP();
		GO_STBY();	  
		Clr_INT(); //清除中断
		Clr_FIFO(); //清除FIFO	
		GO_RX();
	}	
}


void Send_Tx_Data(uint8_t Value)
{int i=0;
  txlen=10;
	Send_Pack(txbuff,txlen,0);
	
}

/**************************************
**名字:KeyScan_Tx
**功能:略
**入参:无        
**出参:无          
***************************************/
void KeyScan_Tx(void)
{
			Send_Tx_Data(P23_VALUE);
	
}
/**************************************
**名称:ReadKey
**功能:读取按键
**入参:无      
**出参:无          
***************************************/
byte ReadKey(void)
{
    return(((M0P_GPIO->P2IN^0x08)&0x08));
}

/**************************************
**名字:RxOKDelay
**功能:接收成功处理
**入参:无            
**出参:无           
***************************************/
void RxOKDelay(void)
{	
//		TX_Buf[0]=KeyCode[0];
//		TX_Buf[1]=KeyCode[1];
//		TX_Buf[2]=KeyCode[2];
//		TX_Buf[3]=0x80;//KeyCode[3];
//    Send_Pack(TX_Buf,4,0);     //回传         
    GO_RX();  
}
