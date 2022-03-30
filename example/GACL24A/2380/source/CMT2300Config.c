#include "CMT2380F32_Demo.h"

void CMT2300A_ConfigGpio(byte nGpioSel)
{
    SPI_Write(CUS_IO_SEL,nGpioSel);  //����GPIO
}
/*! ********************************************************
* @name    SetPayloadLength
* @desc    Set payload length.
* @param   nLength
* *********************************************************/
void SetPayloadLength(word nLength)
{ 
    byte tmp = SPI_Read(CUS_PKT14);
    
    tmp &= ~MASK_PAYLOAD_LENG_10_8;
    tmp |= (nLength >> 4) & MASK_PAYLOAD_LENG_10_8;
    SPI_Write(CUS_PKT14, tmp);
    
    tmp = nLength & MASK_PAYLOAD_LENG_7_0;
    SPI_Write(CUS_PKT15, tmp);
}

/*! ********************************************************
* @name    EnableFifoMerge
* @desc    Enable FIFO merge.
* @param   bEnable(TRUE): use a single 64-byte FIFO for either Tx or Rx
*          bEnable(FALSE): use a 32-byte FIFO for Tx and another 32-byte FIFO for Rx(default)
* *********************************************************/
void EnableFifoMerge(byte bEnable)
{
    byte tmp = SPI_Read(CUS_FIFO_CTL);

    if(bEnable)
        tmp |= MASK_FIFO_MERGE_EN;
    else
        tmp &= ~MASK_FIFO_MERGE_EN;

    SPI_Write(CUS_FIFO_CTL, tmp);
}
/*! ********************************************************
* @name    CMT2300A_EnableReadFifo
* @desc    Enable SPI to read the FIFO.
* *********************************************************/
void CMT2300A_EnableReadFifo(void)
{
    byte tmp = SPI_Read(CUS_FIFO_CTL);
    tmp &= ~SPI_FIFO_RD_WR_SEL; 
    tmp &= ~FIFO_RX_TX_SEL;
    SPI_Write(CUS_FIFO_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableWriteFifo
* @desc    Enable SPI to write the FIFO.
* *********************************************************/
void CMT2300A_EnableWriteFifo(void)
{
    byte tmp = SPI_Read(CUS_FIFO_CTL);
    tmp |= SPI_FIFO_RD_WR_SEL;
    tmp |= FIFO_RX_TX_SEL;
    SPI_Write(CUS_FIFO_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableLfosc
* @desc    If you need use sleep timer, you should enable LFOSC.
* @param   bEnable(TRUE): Enable it(default)
*          bEnable(FALSE): Disable it
* *********************************************************/
void CMT2300A_EnableLfosc(byte bEnable)
{
    byte tmp = SPI_Read(CUS_SYS2);
    
    if(bEnable) {
        tmp |= CMT2300A_MASK_LFOSC_RECAL_EN;
        tmp |= CMT2300A_MASK_LFOSC_CAL1_EN;
        tmp |= CMT2300A_MASK_LFOSC_CAL2_EN;
    }
    else {
        tmp &= ~CMT2300A_MASK_LFOSC_RECAL_EN;
        tmp &= ~CMT2300A_MASK_LFOSC_CAL1_EN;
        tmp &= ~CMT2300A_MASK_LFOSC_CAL2_EN;
    }
    
    SPI_Write(CUS_SYS2, tmp);
}
/***********************************
**����:CMT2300A_Init
**����:2300��ʼ��
**���:��
**����:��
************************************/
void CMT2300A_Init(void)
{
	byte i,tmp;
	byte cnt;	
	Reset();                       //��λ
	delay1ms(30);
	GO_STBY();
	for(i=CMT_Bank_Origin,cnt=0;i<=CMT_Bank_End;i++,cnt++)    	
		SPI_Write(i,CMT_Bank[cnt]);
	for(i=System_Origin,cnt=0;i<=System_End;i++,cnt++)    	
		SPI_Write(i,System_Bank[cnt]);
	for(i=Frequency_Origin,cnt=0;i<=Frequency_End;i++,cnt++)    	
		SPI_Write(i,Frequency_Bank[cnt]);
	for(i=Data_Rete_Origin,cnt=0;i<=Data_Rete_End;i++,cnt++)
		SPI_Write(i,Data_Rate_Bank[cnt]);
	for(i=Baseband_Origin,cnt=0;i<=Baseband_End;i++,cnt++)    	
		SPI_Write(i,Baseband_Bank[cnt]);
	for(i=TX_Origin,cnt=0;i<=TX_End;i++,cnt++)
		SPI_Write(i,TX_Bank[cnt]);	
	
#if RF_STATUS
	CMT2300A_ConfigGpio((GPIO3_DATA|GPIO2_INT2|GPIO1_INT1));   	//test by czg, switch for receiving test
#else
	CMT2300A_ConfigGpio((GPIO3_DATA|GPIO2_INT1|GPIO1_INT2));
#endif
	SPI_Write(CUS_INT_EN,(PKT_DONE_EN|TX_DONE_EN|CRC_OK_EN));            //����������ɺͷ�������ж�
	i = SPI_Read(CUS_INT2_CTL);
	SPI_Write(CUS_INT2_CTL,(i&0xE0)|INT_PKT_DONE);             //INT2ӳ����ճɹ��ж�            //
	i = SPI_Read(CUS_INT1_CTL);
	SPI_Write(CUS_INT1_CTL,(i&0xE0)|INT_TX_DONE);              //INT1ӳ�䷢������ж� 
	Clr_FIFO();                                                //���FIFO
	Clr_INT();                                                 //����ж�
	CMT2300A_EnableLfosc(0);
	EnableFifoMerge(1); //�ϲ�FIFO
	CMT2300A_EnableReadFifo();
	GO_SLEEP();        	    	                            		//����˯��
	ResetTime=0;
}

/*****************************************************************
**����: Send_Pack					
**����: ����TX�������ݰ�
**���: ���ݣ����ȣ��ظ���������
**����: ��
******************************************************************/
void Send_Pack(byte Str[],byte length,byte Pack_Cnt)
{
		byte buf;  
		GO_STBY();   
		CMT2300A_EnableWriteFifo();  
		SPI_Write(CUS_FIFO_CLR,FIFO_CLR_TX);            //���TX FIFO      
		Clr_INT(); 
		SetPayloadLength(length);
		for(buf=0;buf<length;buf++)                     //FIFOд��
		Write_FIFO(Str[buf]);    	                   
		buf=SPI_Read(CUS_PKT29);
		SPI_Write(CUS_PKT29,buf|0x80);                  //��restore TX FIFO
		GO_TX();			                            			//
		while(1)
		{
				if(Test_GPIO2())                            //�ȴ����
				break;
		}
		GO_SLEEP();
}

/***************************************************************
**����:Clr_FIFO
**����:���FIFO
**���:��
**����:��  
***************************************************************/
void Clr_FIFO(void)
{
    SPI_Write(CUS_FIFO_CLR,(FIFO_CLR_RX|FIFO_CLR_TX));
}

/***************************************************************
**����:Clr_INT
**����:����ж�
**���:��
**����:��  
***************************************************************/
void Clr_INT(void)
{
    SPI_Write(CUS_INT_CLR1,(RX_TMO_CLR|SL_TMO_CLR|TX_DONE_CLR));
    SPI_Write(CUS_INT_CLR2,(LBD_CLR|PREAM_OK_CLR|SYNC_OK_CLR|NODE_OK_CLR|CRC_OK_CLR|PKT_DONE_CLR));
}


/****************************************************************
**����:GO_TX
**����:����TXģʽ
**���:��
**����:��  
****************************************************************/
void GO_TX(void)
{   
byte START;	
  SPI_Write(CUS_MODE_CTL,MODE_GO_TX);    //����TXģʽ
	for(SysTime=0;SysTime<2;)
	{
				START=SPI_Read(CUS_MODE_STA);
				if((START&0x0F) == MODE_STA_TX)
				break;
	}
}

/****************************************************************
**����:GO_RX
**����:����RXģʽ
**���:��
**����:��  
****************************************************************/
void GO_RX(void)
{  
byte START;	
	SPI_Write(CUS_MODE_CTL,MODE_GO_RX);    //����RXģʽ
	for(SysTime=0;SysTime<2;)
	{
		START=SPI_Read(CUS_MODE_STA);
		if((START&0x0F) == MODE_STA_RX)
			break;
	}   
}


/****************************************************************
**����:GO_STBY
**����:����STBģʽ
**���:��
**����:��  
****************************************************************/
void GO_STBY(void)
{
byte START;
   SPI_Write(CUS_MODE_CTL,MODE_GO_STBY);    //����STBYģʽ 

	 for(SysTime=0;SysTime<2;)
	 {
				START=SPI_Read(CUS_MODE_STA);
				if((START&0x0F) == MODE_STA_STBY)
				break;
	 } 

}

/****************************************************************
**����:GO_SLEEP
**����:����SLEEPģʽ
**���:��
**����:��  
****************************************************************/
void GO_SLEEP(void)
{
byte START;	
   SPI_Write(CUS_MODE_CTL,MODE_GO_SLEEP);    //����SLEEPģʽ
	 for(SysTime=0;SysTime<2;)
	 {
				START=SPI_Read(CUS_MODE_STA);
				if((START&0x0F) == MODE_STA_SLEEP)
				break;
	 }   
}


/****************************************************************
**����:Reset
**����:��λ
**���:��
**����:��  
****************************************************************/
void Reset(void)
{
   SPI_Write(CUS_SOFTRST,0xFF);    
}

/***************************************************************
**����:Write_FIFO
**����:д��FIFO
**���:����
**����:��               
****************************************************************/
void Write_FIFO(byte date)
{
byte i;
        CSB_H();
        FCSB_L();
        SDIO_L();
        SDIO_Output();
        SCLK_L();
        			
       for(i=0;i<8;i++)  
       {
          SCLK_L();
         if(date&0x80)
            SDIO_H();
        else
            SDIO_L();
            Delay(2);
            SCLK_H();
            Delay(2);
            date<<=1;
       }
       SCLK_L();
       Delay(4);
       FCSB_H();
       SDIO_H();    
       Delay(4);                     
}

/***************************************************************
**����:Read_FIFO
**����:��ȡFIFO����
**���:��
**����:����                 
****************************************************************/
byte Read_FIFO(void)
{
byte i;
byte date=0;
      
      CSB_H();
      FCSB_L();
      SDIO_L();
      SDIO_Input();
      SCLK_L();
      
      for(i=0;i<8;i++)  
      {
            SCLK_L();
            date<<=1;
            Delay(2);         
            SCLK_H();
            Delay(2);
            if(Test_SDIO())
            date|=0x01;
            else
            date&=0xFE;
       }
       SCLK_L();
       Delay(2); 
       FCSB_H();
       SDIO_Output();
       SDIO_H();
       Delay(2);
       return date;

}

/***********************************
**����:SPI_Read
**����:SPI��ȡ
**���:��ַ
**����:����
************************************/
byte SPI_Read(byte Data)
{	
      Write_SPI8Bit(Data|0x80);
      return(Read_SPI8Bit());
}

/***********************************
**����:Read_SPI8Bit
**����:SPI��ȡ
**���:��
**����:����
************************************/
byte Read_SPI8Bit(void)
{
byte num=0;
byte i;						
    CSB_L();
		SDIO_Input();
		Delay(1);
    for(i=0;i<8;i++)		
		{
        SCLK_L();
        Delay(2);	
        num<<=1;
        SCLK_H();
        Delay(2);	
        if(Test_SDIO())
        num|=0x01;
        else
        num&=0xFE;
		}	
		SCLK_L();
    SDIO_Output();
    SDIO_H();
		CSB_H();
		return num;
}

/***********************************
**����:Write_SPI
**����:SPIд��
**���:��ַ������
**����:��
************************************/
void SPI_Write(byte adder,byte Data)
{
      Write_SPI8Bit(adder&0x7F);
      Write_SPI8Bit(Data);
      CSB_H();
}


/***********************************
**����:Write_SPI8Bit
**����:SPIд��8Bit
**���:����
**����:��
************************************/
void Write_SPI8Bit(byte Data)
{
byte i;
    FCSB_H();
    SDIO_Output();
    SDIO_H();
    Delay(1);						
    CSB_L();
    SCLK_L();
    for(i=0;i<8;i++)			
    {			
        SCLK_L();
        if(Data&0x80)
        SDIO_H();	
        else
        SDIO_L();
        Delay(2);
        SCLK_H();
        Delay(2);
        Data<<=1;
    }
    SCLK_L();
    SDIO_H();	
}


