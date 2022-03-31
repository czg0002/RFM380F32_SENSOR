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
 
 #define MAX_UART 100 //���ڽ��ջ���
 uint8_t u8RxData[MAX_UART]={0xAA,0x55,0x11,0x22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
 uint8_t u8RxFlg=0;

void RxIntCallback(void)  //���ڽ����ػ����� ���ݽ�������0x0D 0x0a���ж�һ����������
{
	u8RxData[CoutNum]=M0P_UART1->SBUF; //���յ������ݣ�����u8RxData����
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
	
	    Uart_DisableIrq(UARTCH1,UartRxIrq);//���ɹ��յ�һ�����ݺ����ȹص������жϷ������
	    u8RxFlg=0;
	    CoutNum=0;										    //��ֹӦ�ó����ٴ�����ʹ�ӡ�����ʱ�򣬴��ڻ��ڽ��ճ��
      for(i=0;i<MAX_UART;i++)
			{
				u8RxData[i]=0;
			}
	    /*�������귢��ʹ�ӡ�����ʱ�򣬴����ж��ٴδ�ȥ��������*/				
				Uart_EnableIrq(UARTCH1,UartRxIrq); //UARTͨ���ж�ʹ�ܺ�������
				Uart_ClrStatus(UARTCH1,UartRxFull); //UARTͨ��ͨ��״̬���
				Uart_EnableFunc(UARTCH1,UartRx);  //UARTͨ�����ͻ����ʹ������	
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
    
    //ͨ���˿�����
    Gpio_SetFunc_UART1TX_P35();
    Gpio_SetFunc_UART1RX_P36();

		
		//����ʱ��ʹ��
		Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);//ģʽ0/2���Բ�ʹ��
		Clk_SetPeripheralGate(ClkPeripheralUart1,TRUE);    	
		
		stcUartIrqCb.pfnRxIrqCb = RxIntCallback;//ѭ���ȴ����ڵ�����
		stcUartIrqCb.pfnTxIrqCb = NULL;
		stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback;
		
		stcConfig.pstcIrqCb = &stcUartIrqCb;
		stcConfig.bTouchNvic = TRUE;      	
		stcConfig.enRunMode = UartMode3;//��������Ĵ˴���ת��4��ģʽ����         	
		stcMulti.enMulti_mode = UartNormal;//��������Ĵ˴���ת��������ģʽ��mode2/3���ж�����ģʽ  	
		stcConfig.pstcMultiMode = &stcMulti; 		
		
		stcBaud.bDbaud = 0u;//˫�������ʹ���
		stcBaud.u32Baud = 19200;//���²�����λ��
		stcBaud.u8Mode = UartMode3; //���㲨������Ҫģʽ����
		pclk = Clk_GetPClkFreq();
		timer=Uart_SetBaudRate(UARTCH1,pclk,&stcBaud);
		
		stcBtConfig.enMD = BtMode2;
		stcBtConfig.enCT = BtTimer;
		Bt_Init(TIM1, &stcBtConfig);//����basetimer1���ú�������������
		Bt_ARRSet(TIM1,timer);
		Bt_Cnt16Set(TIM1,timer);
		Bt_Run(TIM1);
		
		Uart_Init(UARTCH1, &stcConfig);
		Uart_EnableIrq(UARTCH1,UartRxIrq); //UARTͨ���ж�ʹ�ܺ�������
		Uart_ClrStatus(UARTCH1,UartRxFull); //UARTͨ��ͨ��״̬���
		Uart_EnableFunc(UARTCH1,UartRx);  //UARTͨ�����ͻ����ʹ������
	
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

  UartP3536Int();	//��ʼ������ 19200 MODE3 evenУ��
	
	#if RF_STATUS  
	GO_STBY();	  
	CMT2300A_EnableReadFifo();
	Clr_INT(); //����ж�
	Clr_FIFO(); //���FIFO	
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
				Uart_DisableIrq(UARTCH1,UartRxIrq);//���ɹ��յ�һ�����ݺ����ȹص������жϷ������
				//��ֹӦ�ó����ٴ�����ʹ�ӡ�����ʱ�򣬴��ڻ��ڽ��ճ���
				u8RxFlg = 0;	
		//		if(u8RxData[1]==0x32)
		//		KeyScan_Tx();	//����ط������������������,����ط������Ż�һ�£�������ڴ����ʱ��ص�
				Send_Pack(u8RxData,CoutNum,0);
				for(i=0;i<CoutNum;i++)
				{
					Uart_SendData(UARTCH1,u8RxData[i]);	
					u8RxData[i]=0;
				}
					CoutNum=0;
         /*�������귢��ʹ�ӡ�����ʱ�򣬴����ж��ٴδ�ȥ��������*/				
				Uart_EnableIrq(UARTCH1,UartRxIrq); //UARTͨ���ж�ʹ�ܺ�������
				Uart_ClrStatus(UARTCH1,UartRxFull); //UARTͨ��ͨ��״̬���
				Uart_EnableFunc(UARTCH1,UartRx);  //UARTͨ�����ͻ����ʹ������				
			}	   		
			 #endif	
	  }
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


