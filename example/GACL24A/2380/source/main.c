/******************************************************************************/
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2017-05-28     First Version
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "adc.h"
#include "gpio.h"
#include "ddl.h"
#include "uart.h"
#include "CMT2380F32_Demo.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/
//#define DEBUG_PRINT

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')                                         
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
 uint8_t CheckFlg=0,CoutNum=0,Uartlen=10;
 
 #define MAX_UART 100 //串口接收缓存
 uint8_t u8RxData[MAX_UART]={0xAA,0x55,0x11,0x22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
 uint8_t u8RxFlg=0;

void RxIntCallback(void)  //串口接收守护程序 根据结束符合0x0D 0x0a来判断一包完整数据
{
	u8RxData[CoutNum]=M0P_UART1->SBUF; //将收到的数据，存入u8RxData数组
	if(CoutNum>=2)
	{
		if((u8RxData[CoutNum-1]==0x0d)&&(u8RxData[CoutNum]==0x0a))
		{
			u8RxFlg=1;
		}
  }

	 if(CoutNum>=MAX_UART)
	 {
		u8RxFlg=1;
	 } 
	 else{
	 CoutNum++;		
	 }		 
}


void ErrIntCallback(void)
{  	  byte i=0;
	
	    Uart_DisableIrq(UARTCH1,UartRxIrq);//当成功收到一包数据后，首先关掉串口中断服务程序，
	    u8RxFlg=0;
	    CoutNum=0;										    //防止应用程序再处理发射和打印任务的时候，串口还在接收出�
      for(i=0;i<MAX_UART;i++)
			{
				u8RxData[i]=0;
			}
	    /*当处理完发射和打印任务的时候，串口中断再次打开去接收数据*/				
				Uart_EnableIrq(UARTCH1,UartRxIrq); //UART通信中断使能函数设置
				Uart_ClrStatus(UARTCH1,UartRxFull); //UART通道通信状态清除
				Uart_EnableFunc(UARTCH1,UartRx);  //UART通道发送或接收使能设置	
}

void UartP3536Int(void)
{

		uint16_t timer=0;
		uint32_t pclk=0;
		
		stc_uart_config_t  stcConfig;
		stc_uart_irq_cb_t stcUartIrqCb;
		stc_uart_multimode_t stcMulti;
		stc_uart_baud_config_t stcBaud;
		stc_bt_config_t stcBtConfig;
		
		DDL_ZERO_STRUCT(stcUartIrqCb);
		DDL_ZERO_STRUCT(stcMulti);
		DDL_ZERO_STRUCT(stcBaud);
		DDL_ZERO_STRUCT(stcBtConfig);  	

    Gpio_InitIOExt(3,5,GpioDirOut,TRUE,FALSE,FALSE,FALSE);   
    Gpio_InitIOExt(3,6,GpioDirOut,TRUE,FALSE,FALSE,FALSE); 
    
    //通道端口配置
    Gpio_SetFunc_UART1TX_P35();
    Gpio_SetFunc_UART1RX_P36();

		
		//外设时钟使能
		Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);//模式0/2可以不使能
		Clk_SetPeripheralGate(ClkPeripheralUart1,TRUE);    	
		
		stcUartIrqCb.pfnRxIrqCb = RxIntCallback;//循环等待串口的输入
		stcUartIrqCb.pfnTxIrqCb = NULL;
		stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback;
		
		stcConfig.pstcIrqCb = &stcUartIrqCb;
		stcConfig.bTouchNvic = TRUE;      	
		stcConfig.enRunMode = UartMode3;//测试项，更改此处来转换4种模式测试         	
		stcMulti.enMulti_mode = UartNormal;//测试项，更改此处来转换多主机模式，mode2/3才有多主机模式  	
		stcConfig.pstcMultiMode = &stcMulti; 		
		
		stcBaud.bDbaud = 0u;//双倍波特率功能
		stcBaud.u32Baud = 19200;//更新波特率位置
		stcBaud.u8Mode = UartMode3; //计算波特率需要模式参数
		pclk = Clk_GetPClkFreq();
		timer=Uart_SetBaudRate(UARTCH1,pclk,&stcBaud);
		
		stcBtConfig.enMD = BtMode2;
		stcBtConfig.enCT = BtTimer;
		Bt_Init(TIM1, &stcBtConfig);//调用basetimer1设置函数产生波特率
		Bt_ARRSet(TIM1,timer);
		Bt_Cnt16Set(TIM1,timer);
		Bt_Run(TIM1);
		
		Uart_Init(UARTCH1, &stcConfig);
		Uart_EnableIrq(UARTCH1,UartRxIrq); //UART通信中断使能函数设置
		Uart_ClrStatus(UARTCH1,UartRxFull); //UART通道通信状态清除
		Uart_EnableFunc(UARTCH1,UartRx);  //UART通道发送或接收使能设置
	
}
/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
 
/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample
 **
 ******************************************************************************/
int32_t main(void)
{  
	byte i=0;
	uint32_t k=1;
	MCU_Init(); 
  CMT2300A_Init();

//  UartP3536Int();	//初始化串口 19200 MODE3 even校验
	
	#if RF_STATUS  
	GO_STBY();	  
	CMT2300A_EnableReadFifo();
	Clr_INT(); //清除中断
	Clr_FIFO(); //清除FIFO	
	GO_RX();
	#else 
	while(1)
		Send_Pack(u8RxData,8,0);
	#endif	
	while(1)
	 {		 
		 	#if RF_STATUS  	
			KeyScan_Rx();  
			#else 	
			if(u8RxFlg)
			{
				Uart_DisableIrq(UARTCH1,UartRxIrq);//当成功收到一包数据后，首先关掉串口中断服务程序，
				//防止应用程序再处理发射和打印任务的时候，串口还在接收出错
				u8RxFlg = 0;	
		//		if(u8RxData[1]==0x32)
		//		KeyScan_Tx();	//这个地方发射的数据量尽量短,这个地方后续优化一下，当这个在处理的时候关掉
				Send_Pack(u8RxData,CoutNum,0);
				for(i=0;i<CoutNum;i++)
				{
					Uart_SendData(UARTCH1,u8RxData[i]);	
					u8RxData[i]=0;
				}
					CoutNum=0;
         /*当处理完发射和打印任务的时候，串口中断再次打开去接收数据*/				
				Uart_EnableIrq(UARTCH1,UartRxIrq); //UART通信中断使能函数设置
				Uart_ClrStatus(UARTCH1,UartRxFull); //UART通道通信状态清除
				Uart_EnableFunc(UARTCH1,UartRx);  //UART通道发送或接收使能设置				
			}	   		
			 #endif	
	  }
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


