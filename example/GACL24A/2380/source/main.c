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
 
 #define MAX_UART 100 //´®¿Ú½ÓÊÕ»º´æ
 uint8_t u8RxData[MAX_UART]={0xAA,0x55,0x11,0x22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
 uint8_t u8RxFlg=0;

void RxIntCallback(void)  //´®¿Ú½ÓÊÕÊØ»¤³ÌĞò ¸ù¾İ½áÊø·ûºÏ0x0D 0x0aÀ´ÅĞ¶ÏÒ»°üÍêÕûÊı¾İ
{
	u8RxData[CoutNum]=M0P_UART1->SBUF; //½«ÊÕµ½µÄÊı¾İ£¬´æÈëu8RxDataÊı×é
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
	
	    Uart_DisableIrq(UARTCH1,UartRxIrq);//µ±³É¹¦ÊÕµ½Ò»°üÊı¾İºó£¬Ê×ÏÈ¹Øµô´®¿ÚÖĞ¶Ï·şÎñ³ÌĞò£¬
	    u8RxFlg=0;
	    CoutNum=0;										    //·ÀÖ¹Ó¦ÓÃ³ÌĞòÔÙ´¦Àí·¢ÉäºÍ´òÓ¡ÈÎÎñµÄÊ±ºò£¬´®¿Ú»¹ÔÚ½ÓÊÕ³ö´
      for(i=0;i<MAX_UART;i++)
			{
				u8RxData[i]=0;
			}
	    /*µ±´¦ÀíÍê·¢ÉäºÍ´òÓ¡ÈÎÎñµÄÊ±ºò£¬´®¿ÚÖĞ¶ÏÔÙ´Î´ò¿ªÈ¥½ÓÊÕÊı¾İ*/				
				Uart_EnableIrq(UARTCH1,UartRxIrq); //UARTÍ¨ĞÅÖĞ¶ÏÊ¹ÄÜº¯ÊıÉèÖÃ
				Uart_ClrStatus(UARTCH1,UartRxFull); //UARTÍ¨µÀÍ¨ĞÅ×´Ì¬Çå³ı
				Uart_EnableFunc(UARTCH1,UartRx);  //UARTÍ¨µÀ·¢ËÍ»ò½ÓÊÕÊ¹ÄÜÉèÖÃ	
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
    
    //Í¨µÀ¶Ë¿ÚÅäÖÃ
    Gpio_SetFunc_UART1TX_P35();
    Gpio_SetFunc_UART1RX_P36();

		
		//ÍâÉèÊ±ÖÓÊ¹ÄÜ
		Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);//Ä£Ê½0/2¿ÉÒÔ²»Ê¹ÄÜ
		Clk_SetPeripheralGate(ClkPeripheralUart1,TRUE);    	
		
		stcUartIrqCb.pfnRxIrqCb = RxIntCallback;//Ñ­»·µÈ´ı´®¿ÚµÄÊäÈë
		stcUartIrqCb.pfnTxIrqCb = NULL;
		stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback;
		
		stcConfig.pstcIrqCb = &stcUartIrqCb;
		stcConfig.bTouchNvic = TRUE;      	
		stcConfig.enRunMode = UartMode3;//²âÊÔÏî£¬¸ü¸Ä´Ë´¦À´×ª»»4ÖÖÄ£Ê½²âÊÔ         	
		stcMulti.enMulti_mode = UartNormal;//²âÊÔÏî£¬¸ü¸Ä´Ë´¦À´×ª»»¶àÖ÷»úÄ£Ê½£¬mode2/3²ÅÓĞ¶àÖ÷»úÄ£Ê½  	
		stcConfig.pstcMultiMode = &stcMulti; 		
		
		stcBaud.bDbaud = 0u;//Ë«±¶²¨ÌØÂÊ¹¦ÄÜ
		stcBaud.u32Baud = 19200;//¸üĞÂ²¨ÌØÂÊÎ»ÖÃ
		stcBaud.u8Mode = UartMode3; //¼ÆËã²¨ÌØÂÊĞèÒªÄ£Ê½²ÎÊı
		pclk = Clk_GetPClkFreq();
		timer=Uart_SetBaudRate(UARTCH1,pclk,&stcBaud);
		
		stcBtConfig.enMD = BtMode2;
		stcBtConfig.enCT = BtTimer;
		Bt_Init(TIM1, &stcBtConfig);//µ÷ÓÃbasetimer1ÉèÖÃº¯Êı²úÉú²¨ÌØÂÊ
		Bt_ARRSet(TIM1,timer);
		Bt_Cnt16Set(TIM1,timer);
		Bt_Run(TIM1);
		
		Uart_Init(UARTCH1, &stcConfig);
		Uart_EnableIrq(UARTCH1,UartRxIrq); //UARTÍ¨ĞÅÖĞ¶ÏÊ¹ÄÜº¯ÊıÉèÖÃ
		Uart_ClrStatus(UARTCH1,UartRxFull); //UARTÍ¨µÀÍ¨ĞÅ×´Ì¬Çå³ı
		Uart_EnableFunc(UARTCH1,UartRx);  //UARTÍ¨µÀ·¢ËÍ»ò½ÓÊÕÊ¹ÄÜÉèÖÃ
	
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
	word k=1;
	MCU_Init(); 
  CMT2300A_Init();   

  UartP3536Int();	//³õÊ¼»¯´®¿Ú 19200 MODE3 evenĞ£Ñé
	
	#if RF_STATUS  
	GO_STBY();	  
	CMT2300A_EnableReadFifo();
	Clr_INT(); //Çå³ıÖĞ¶Ï
	Clr_FIFO(); //Çå³ıFIFO	
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
				Uart_DisableIrq(UARTCH1,UartRxIrq);//µ±³É¹¦ÊÕµ½Ò»°üÊı¾İºó£¬Ê×ÏÈ¹Øµô´®¿ÚÖĞ¶Ï·şÎñ³ÌĞò£¬
				//·ÀÖ¹Ó¦ÓÃ³ÌĞòÔÙ´¦Àí·¢ÉäºÍ´òÓ¡ÈÎÎñµÄÊ±ºò£¬´®¿Ú»¹ÔÚ½ÓÊÕ³ö´í
				u8RxFlg = 0;	
		//		if(u8RxData[1]==0x32)
		//		KeyScan_Tx();	//Õâ¸öµØ·½·¢ÉäµÄÊı¾İÁ¿¾¡Á¿¶Ì,Õâ¸öµØ·½ºóĞøÓÅ»¯Ò»ÏÂ£¬µ±Õâ¸öÔÚ´¦ÀíµÄÊ±ºò¹Øµô
				Send_Pack(u8RxData,CoutNum,0);
				for(i=0;i<CoutNum;i++)
				{
					Uart_SendData(UARTCH1,u8RxData[i]);	
					u8RxData[i]=0;
				}
					CoutNum=0;
         /*µ±´¦ÀíÍê·¢ÉäºÍ´òÓ¡ÈÎÎñµÄÊ±ºò£¬´®¿ÚÖĞ¶ÏÔÙ´Î´ò¿ªÈ¥½ÓÊÕÊı¾İ*/				
				Uart_EnableIrq(UARTCH1,UartRxIrq); //UARTÍ¨ĞÅÖĞ¶ÏÊ¹ÄÜº¯ÊıÉèÖÃ
				Uart_ClrStatus(UARTCH1,UartRxFull); //UARTÍ¨µÀÍ¨ĞÅ×´Ì¬Çå³ı
				Uart_EnableFunc(UARTCH1,UartRx);  //UARTÍ¨µÀ·¢ËÍ»ò½ÓÊÕÊ¹ÄÜÉèÖÃ				
			}	   		
			 #endif	
	  }
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


