#include "CMT2380F32_Demo.h"

void CMT2300A_ConfigGpio(byte nGpioSel)
{
    SPI_Write(CUS_IO_SEL,nGpioSel);  //配置GPIO
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
**名字:CMT2300A_Init
**功能:2300初始化
**入参:无
**出参:无
************************************/
void CMT2300A_Init(void)
{
	byte i,tmp;
	byte cnt;	
	Reset();                       //软复位
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
	SPI_Write(CUS_INT_EN,(PKT_DONE_EN|TX_DONE_EN|CRC_OK_EN));            //开启接收完成和发射完成中断
	i = SPI_Read(CUS_INT2_CTL);
	SPI_Write(CUS_INT2_CTL,(i&0xE0)|INT_PKT_DONE);             //INT2映射接收成功中断            //
	i = SPI_Read(CUS_INT1_CTL);
	SPI_Write(CUS_INT1_CTL,(i&0xE0)|INT_TX_DONE);              //INT1映射发射完成中断 
	Clr_FIFO();                                                //清除FIFO
	Clr_INT();                                                 //清除中断
	CMT2300A_EnableLfosc(0);
	EnableFifoMerge(1); //合并FIFO
	CMT2300A_EnableReadFifo();
	GO_SLEEP();        	    	                            		//进入睡眠
	ResetTime=0;
}

/*****************************************************************
**名字: Send_Pack					
**功能: 进入TX发送数据包
**入参: 内容，长度，重复发报数量
**出参: 无
******************************************************************/
void Send_Pack(byte Str[],byte length,byte Pack_Cnt)
{
		byte buf;  
		GO_STBY();   
		CMT2300A_EnableWriteFifo();  
		SPI_Write(CUS_FIFO_CLR,FIFO_CLR_TX);            //清除TX FIFO      
		Clr_INT(); 
		SetPayloadLength(length);
		for(buf=0;buf<length;buf++)                     //FIFO写入
		Write_FIFO(Str[buf]);    	                   
		buf=SPI_Read(CUS_PKT29);
		SPI_Write(CUS_PKT29,buf|0x80);                  //打开restore TX FIFO
		GO_TX();			                            			//
		while(1)
		{
				if(Test_GPIO2())                            //等待完成
				break;
		}
		GO_SLEEP();
}

/***************************************************************
**名字:Clr_FIFO
**功能:清除FIFO
**入参:无
**出参:无  
***************************************************************/
void Clr_FIFO(void)
{
    SPI_Write(CUS_FIFO_CLR,(FIFO_CLR_RX|FIFO_CLR_TX));
}

/***************************************************************
**名字:Clr_INT
**功能:清除中断
**入参:无
**出参:无  
***************************************************************/
void Clr_INT(void)
{
    SPI_Write(CUS_INT_CLR1,(RX_TMO_CLR|SL_TMO_CLR|TX_DONE_CLR));
    SPI_Write(CUS_INT_CLR2,(LBD_CLR|PREAM_OK_CLR|SYNC_OK_CLR|NODE_OK_CLR|CRC_OK_CLR|PKT_DONE_CLR));
}


/****************************************************************
**名字:GO_TX
**功能:进入TX模式
**入参:无
**出参:无  
****************************************************************/
void GO_TX(void)
{   
byte START;	
  SPI_Write(CUS_MODE_CTL,MODE_GO_TX);    //进入TX模式
	for(SysTime=0;SysTime<2;)
	{
				START=SPI_Read(CUS_MODE_STA);
				if((START&0x0F) == MODE_STA_TX)
				break;
	}
}

/****************************************************************
**名字:GO_RX
**功能:进入RX模式
**入参:无
**出参:无  
****************************************************************/
void GO_RX(void)
{  
byte START;	
	SPI_Write(CUS_MODE_CTL,MODE_GO_RX);    //进入RX模式
	for(SysTime=0;SysTime<2;)
	{
		START=SPI_Read(CUS_MODE_STA);
		if((START&0x0F) == MODE_STA_RX)
			break;
	}   
}


/****************************************************************
**名字:GO_STBY
**功能:进入STB模式
**入参:无
**出参:无  
****************************************************************/
void GO_STBY(void)
{
byte START;
   SPI_Write(CUS_MODE_CTL,MODE_GO_STBY);    //进入STBY模式 

	 for(SysTime=0;SysTime<2;)
	 {
				START=SPI_Read(CUS_MODE_STA);
				if((START&0x0F) == MODE_STA_STBY)
				break;
	 } 

}

/****************************************************************
**名字:GO_SLEEP
**功能:进入SLEEP模式
**入参:无
**出参:无  
****************************************************************/
void GO_SLEEP(void)
{
byte START;	
   SPI_Write(CUS_MODE_CTL,MODE_GO_SLEEP);    //进入SLEEP模式
	 for(SysTime=0;SysTime<2;)
	 {
				START=SPI_Read(CUS_MODE_STA);
				if((START&0x0F) == MODE_STA_SLEEP)
				break;
	 }   
}


/****************************************************************
**名字:Reset
**功能:软复位
**入参:无
**出参:无  
****************************************************************/
void Reset(void)
{
   SPI_Write(CUS_SOFTRST,0xFF);    
}

/***************************************************************
**名字:Write_FIFO
**功能:写入FIFO
**入参:数据
**出参:无               
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
**名字:Read_FIFO
**功能:读取FIFO数据
**入参:无
**出参:数据                 
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
**名字:SPI_Read
**功能:SPI读取
**入参:地址
**出参:数据
************************************/
byte SPI_Read(byte Data)
{	
      Write_SPI8Bit(Data|0x80);
      return(Read_SPI8Bit());
}

/***********************************
**名字:Read_SPI8Bit
**功能:SPI读取
**入参:无
**出参:数据
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
**名字:Write_SPI
**功能:SPI写入
**入参:地址，数据
**出参:无
************************************/
void SPI_Write(byte adder,byte Data)
{
      Write_SPI8Bit(adder&0x7F);
      Write_SPI8Bit(Data);
      CSB_H();
}


/***********************************
**名字:Write_SPI8Bit
**功能:SPI写入8Bit
**入参:数据
**出参:无
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


