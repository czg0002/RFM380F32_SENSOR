/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, CMOSTEK SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (C) CMOSTEK SZ.
 */

/*!
 * @file    cmt2300a_hal.c
 * @brief   CMT2300A hardware abstraction layer
 *
 * @version 1.2
 * @date    Jul 17 2017
 * @author  CMOSTEK R@D
 */
#include "ddl.h" 
#include "cmt2300a_hal.h"
#include "cmt2300a_params.h"
#include "common.h"
#include "gpio_setting.h"

/* *********************************************************
 * static spi read write function
 * *********************************************************/
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

/***************************************************************
* static spi write function from original refcode, may be improved later          
****************************************************************/
/*
uint8_t SPI_Read(uint8_t Data)
{	
      Write_SPI8Bit(Data|0x80);
      return(Read_SPI8Bit());
}
*/

static uint8_t Read_SPI8Bit(void)
{
uint8_t num=0;
uint8_t i;						
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
/*
void SPI_Write(uint8_t adder,uint8_t Data)
{
      Write_SPI8Bit(adder&0x7F);
      Write_SPI8Bit(Data);
      CSB_H();
}
*/
static void Write_SPI8Bit(uint8_t Data)
{
uint8_t i;
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
static void Write_FIFO(uint8_t date)
{
uint8_t i;
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

static uint8_t Read_FIFO(void)
{
uint8_t i;
uint8_t date=0;
      
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


static void cmt_spi_init(void)	//initialize, empty function, use gpio simulate, gpio setting moved to io setting.c
{
	return ;
}

static void cmt_spi_read(uint8_t _addr, uint8_t *_dat)
{
	Write_SPI8Bit(_addr | 0x80);
	*_dat = Read_SPI8Bit();
	return ;
}

static void cmt_spi_write(uint8_t _addr, uint8_t _dat)
{
	Write_SPI8Bit(_addr&0x7F);
	Write_SPI8Bit(_dat);
	CSB_H();
	return ;
}

void cmt_spi_write_fifo(const uint8_t* p_buf, uint16_t len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		Write_FIFO(p_buf[i]);
	}
	return ;
}

void cmt_spi_read_fifo(uint8_t* p_buf, uint16_t len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		p_buf[i] = Read_FIFO();
	}
	return ;
}
/* ***********end of static spi function*********************/
/*! ********************************************************
* @name    CMT2300A_InitGpio
* @desc    Initializes the CMT2300A interface GPIOs.
* *********************************************************/
void CMT2300A_InitGpio(void)
{
//	CMT2300A_SetGpio1In();
//    CMT2300A_SetGpio2In();
//    CMT2300A_SetGpio3In();
//	
    cmt_spi_init();
}

/*! ********************************************************
* @name    CMT2300A_ReadReg
* @desc    Read the CMT2300A register at the specified address.
* @param   addr: register address
* @return  Register value
* *********************************************************/
uint8_t CMT2300A_ReadReg(uint8_t addr)
{
    uint8_t dat = 0xFF;
    cmt_spi_read(addr, &dat);
	
    return dat;
}

/*! ********************************************************
* @name    CMT2300A_WriteReg
* @desc    Write the CMT2300A register at the specified address.
* @param   addr: register address
*          dat: register value
* *********************************************************/
void CMT2300A_WriteReg(uint8_t addr, uint8_t dat)
{
    cmt_spi_write(addr, dat);
}

/*! ********************************************************
* @name    CMT2300A_ReadFifo
* @desc    Reads the contents of the CMT2300A FIFO.
* @param   buf: buffer where to copy the FIFO read data
*          len: number of uint8_ts to be read from the FIFO
* *********************************************************/
void CMT2300A_ReadFifo(uint8_t buf[], uint16_t len)
{
    cmt_spi_read_fifo(buf, len);
}

/*! ********************************************************
* @name    CMT2300A_WriteFifo
* @desc    Writes the buffer contents to the CMT2300A FIFO.
* @param   buf: buffer containing data to be put on the FIFO
*          len: number of uint8_ts to be written to the FIFO
* *********************************************************/
void CMT2300A_WriteFifo(const uint8_t buf[], uint16_t len)
{
    cmt_spi_write_fifo(buf, len);
}
