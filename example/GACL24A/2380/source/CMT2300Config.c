#include "CMT2380F32_Demo.h"
#include "cmt2300a_params.h"
#include "cmt2300a.h"
#include "gpio_setting.h"
//refcode from producer, only retain the basic SPI R/W functions
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

