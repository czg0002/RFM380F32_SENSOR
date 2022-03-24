/******************************************************************************/
/** \file I2C.c
 **
 ** WDT function driver API.
 ** @link SampleGroup Some description @endlink
 **
 **   - 2017-06-13  1.0   First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "i2c.h"

/**
 *******************************************************************************
 ** \addtogroup I2cGroup
 ******************************************************************************/
//@{

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
static func_ptr_t pfnI2ctCallback = NULL;
/**
 ******************************************************************************
 ** \brief  I2C设置波特率配置寄存器
 **
 ** \param [in] u8Tm 波特率配置值
 **
 ** \retval enRet 成功或失败
 **
 ******************************************************************************/
 en_result_t I2C_SetBaud(uint8_t u8Tm)
 {
     en_result_t enRet = Error;
     M0P_I2C->TM = u8Tm;
     enRet = Ok;
     return enRet;
 }
 /**
 ******************************************************************************
 ** \brief  I2C功能设置相关函数
 **
 ** \param [in] enFunc功能参数
 **
 ** \retval enRet 成功或失败
 **
 ******************************************************************************/
en_result_t I2C_SetFunc(en_i2c_func_t enFunc)
{
    en_result_t enRet = Error;
    switch(enFunc)
    {
        case I2cMode_En:
            M0P_I2C->CR_f.ENS = 1;
            break;
        case I2cStart_En:
            M0P_I2C->CR_f.STA = 1;
            break;
        case I2cStop_En:
            M0P_I2C->CR_f.STO = 1;
            break;
        case I2cAck_En:
            M0P_I2C->CR_f.AA = 1;
            break;
        case I2cHlm_En:
            M0P_I2C->CR_f.H1M = 1;
            break;
        case I2cBaud_En:
            M0P_I2C->TMRUN = 0x01;
            break;
        default:
            return ErrorInvalidParameter;
    }
    enRet = Ok;
    return enRet;
}
 /**
 ******************************************************************************
 ** \brief  I2C功能清除相关函数
 **
 ** \param [in] enFunc功能参数
 **
 ** \retval enRet 成功或失败
 **
 ******************************************************************************/
 en_result_t I2C_ClearFunc(en_i2c_func_t enFunc)
 {
    en_result_t enRet = Error;
    switch(enFunc)
    {
        case I2cMode_En:
            M0P_I2C->CR_f.ENS = 0;
            break;
        case I2cStart_En:
            M0P_I2C->CR_f.STA = 0;
            break;
        case I2cStop_En:
            M0P_I2C->CR_f.STO = 0;
            break;
        case I2cAck_En:
            M0P_I2C->CR_f.AA = 0;
            break;
        case I2cHlm_En:
            M0P_I2C->CR_f.H1M = 0;
            break;
        case I2cBaud_En:
            M0P_I2C->TMRUN = 0x00;
            break;
        default:
            return ErrorInvalidParameter;
    }
    enRet = Ok;
    return enRet; 
 }
 /**
 ******************************************************************************
 ** \brief  I2C获取中断标记函数
 **
 ** \param 无
 **
 ** \retval bIrq中断标记
 **
 ******************************************************************************/
boolean_t I2C_GetIrq(void)
{
    boolean_t bIrq = FALSE;
    bIrq = M0P_I2C->CR_f.SI;
    return bIrq;  
}
/**
 ******************************************************************************
 ** \brief  I2C清除中断标记函数
 **
 ** \param 无
 **
 ** \retval bIrq中断标记
 **
 ******************************************************************************/
en_result_t I2C_ClearIrq(void)
{
    en_result_t enRet = Error;
    M0P_I2C->CR_f.SI = 0;
    enRet = Ok;
    return enRet; 
}
 /**
 ******************************************************************************
 ** \brief  I2C获取相关状态
 **
 ** \param 无
 **
 ** \retval I2C状态
 **
 ******************************************************************************/
uint8_t I2C_GetState(void)
{
  uint8_t u8State = 0;
  u8State = M0P_I2C->STAT;
  return u8State;
}
/**
 ******************************************************************************
 ** \brief  I2C写从机地址函数
 **
 ** \param u8SlaveAddr从机地址
 **
 ** \retval I2C写成功与否状态
 **
 ******************************************************************************/
 en_result_t I2C_WriteSlaveAddr(stc_i2c_addr_t stcSlaveAddr)
{
    en_result_t enRet = Error;
    M0P_I2C->ADDR_f.I2CADR = stcSlaveAddr.Addr;
    M0P_I2C->ADDR_f.GC = stcSlaveAddr.Gc;
    enRet = Ok;
    return enRet;     
}
/**
 ******************************************************************************
 ** \brief  字节写从机函数
 **
 ** \param u8Data写数据
 **
 ** \retval 写数据是否成功
 **
 ******************************************************************************/
en_result_t I2C_WriteByte(uint8_t u8Data)
{
    en_result_t enRet = Error;
    M0P_I2C->DATA = u8Data;
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  字节读从机函数
 **
 ** \param 无
 **
 ** \retval 读取数据
 **
 ******************************************************************************/
uint8_t I2C_ReadByte(void)
{
    uint8_t u8Data = 0;
    u8Data = M0P_I2C->DATA;
    return u8Data;
}
 /**
 ******************************************************************************
 ** \brief  主机发送函数
 **
 ** \param u8Addr从机内存地址，pu8Data写数据，u32Len写数据长度
 **
 ** \retval 写数据是否成功
 **
 ******************************************************************************/
en_result_t I2C_MasterWriteData(uint8_t u8Addr,uint8_t *pu8Data,uint32_t u32Len)
{
    en_result_t enRet = Error;
    uint8_t u8i,u8State;
    
    I2C_SetFunc(I2cStart_En);
    
    while(0 == I2C_GetIrq())
    {}
    u8State = I2C_GetState();
    while(0x08 != u8State)//起始信号发送成功
    {}
    I2C_ClearFunc(I2cStart_En);
    I2C_WriteByte(I2c_SlaveWt);//从机地址发送OK
    I2C_ClearIrq();
    
    while(0 == I2C_GetIrq())
    {}
    u8State = I2C_GetState();
    while(0x18 != u8State)
    {}
    I2C_WriteByte(u8Addr);
    I2C_ClearIrq();
    
    while(0 == I2C_GetIrq())
    {}
    u8State = I2C_GetState();
    while(0x28 != u8State)
    {}
    for(u8i = 0;u8i<u32Len;u8i++)
    {
        I2C_WriteByte(pu8Data[u8i]);
        I2C_ClearIrq();
        while(0 == I2C_GetIrq())
        {}
        u8State = I2C_GetState();
        while(0x28 != u8State)
        {}
        
    }
    if(u32Len == u8i)
    {
        I2C_SetFunc(I2cStop_En);
        I2C_ClearIrq();
    }
    enRet = Ok;
    return enRet;
}
 /**
 ******************************************************************************
 ** \brief  从机发送函数
 **
 ** \param pu8Data发送数据缓存，u32Len发送数据长度
 **
 ** \retval 发送数据是否成功
 **
 ******************************************************************************/
 en_result_t I2C_SlaveWriteData(uint8_t *pu8Data,uint32_t *u32Len)
 {
    //en_result_t enRet = Error;
    uint8_t u8i=0,u8State;
    //
    while(1)
    {

        while(0 == I2C_GetIrq())
        {}
        u8State = I2C_GetState();
        switch(u8State)
        {
            case 0xA8:
            case 0xB0:
                I2C_WriteByte(pu8Data[u8i++]);
                I2C_ClearIrq();
                break;
            case 0xB8: 
            case 0xC8:               
                I2C_WriteByte(pu8Data[u8i++]);
                I2C_ClearIrq();                 
                break;
            
            case 0xF8:
                I2C_ClearIrq();
                *u32Len = u8i;
                break;
             //case 0xC0:   
            default:
                I2C_ClearIrq();
                return ErrorInvalidParameter; 
        }
        u8State = I2C_GetState();
    } 
 }
 /**
 ******************************************************************************
 ** \brief  从机接收函数
 **
 ** \param pu8Data接收数据存放缓存，u32Len接收数据指针
 **
 ** \retval 接收数据是否成功
 **
 ******************************************************************************/
en_result_t I2C_SlaveReadData(uint8_t *pu8Data,uint32_t *pu32Len)
{
#if 1
    //en_result_t enRet = Error;
    uint8_t u8i=0,u8State;
    while(0 == I2C_GetIrq())
    {}
    while(1)
   {
        while(0 == I2C_GetIrq())
        {}
        u8State = I2C_GetState();
        switch(u8State)
        {
            case 0x60:
            case 0x68:
            case 0x70:
            case 0x78:
              I2C_ClearIrq();
              break;
            case 0x80:
            case 0x90:               
                pu8Data[u8i++] = I2C_ReadByte();
                I2C_ClearIrq();                 
                break;
            //case 0x88:
            //case 0x98:
                //return Error;
            case 0xA0:
                I2C_ClearIrq();
                *pu32Len = u8i;
                break;
            default:
                return ErrorInvalidParameter; 
        }
        if(0xA0 == u8State)
        {
          return Ok;
        }
   }
#endif
} 

/**
 ******************************************************************************
 ** \brief  主机接收函数
 **
 ** \param u8Addr从机内存地址，pu8Data读数据存放缓存，u32Len读数据长度
 **
 ** \retval 读数据是否成功
 **
 ******************************************************************************/
 en_result_t I2C_MasterReadData(uint8_t *pu8Data,uint8_t u8Addr,uint32_t u32Len)
{
    en_result_t enRet = Error;
    uint8_t u8i,u8State;
    
    I2C_SetFunc(I2cStart_En);
    
    while(0 == I2C_GetIrq())
    {}
    u8State = I2C_GetState();
    while(0x08 != u8State)//起始信号发送成功
    {}
    I2C_ClearFunc(I2cStart_En);
    
#if 1 //根据从机端是E2还是MCU模拟I2C来进行处理
    I2C_WriteByte(I2c_SlaveWt);//从机地址发送OK
    I2C_ClearIrq();

    while(0 == I2C_GetIrq())
    {}
    u8State = I2C_GetState();
    while(0x18 != u8State)
    {}
    I2C_WriteByte(u8Addr);
    I2C_ClearIrq();
    
    while(0 == I2C_GetIrq())
    {}
    u8State = I2C_GetState();
    while(0x28 != u8State)
    {}
    I2C_SetFunc(I2cStart_En);
    I2C_ClearIrq();
 
    while(0 == I2C_GetIrq())
    {}
    u8State = I2C_GetState();
    while(0x10 != u8State)//起始信号发送成功
    {}

    I2C_ClearFunc(I2cStart_En);
#endif
    I2C_WriteByte(I2c_SlaveRd);//从机地址发送OK
    I2C_ClearIrq();
    
    while(0 == I2C_GetIrq())
    {}
    u8State = I2C_GetState();
    while(0x40 != u8State)
    {}
    
    I2C_SetFunc(I2cAck_En);
    
    I2C_ClearIrq();
    while(0 == I2C_GetIrq())
    {}
    for(u8i=0;u8i<u32Len;u8i++)
    {
      pu8Data[u8i] = I2C_ReadByte();
      I2C_ClearIrq();
      while(0 == I2C_GetIrq())
      {}
      u8State = I2C_GetState();
      while(0x50 != u8State)
      {}
    }
    I2C_ClearFunc(I2cAck_En);
    I2C_SetFunc(I2cStop_En);
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  I2C模块初始化
 **
 ** \param stcI2CCfg初始化配置结构体
 **
 ** \retval 初始化是否成功
 **
 ******************************************************************************/
en_result_t I2C_Init(stc_i2c_config_t stcI2CCfg)
{
   en_result_t enRet = Error;
   enRet = I2C_SetFunc(stcI2CCfg.enFunc);
   enRet = I2C_SetBaud(stcI2CCfg.u8Tm);
   enRet = I2C_WriteSlaveAddr(stcI2CCfg.stcSlaveAddr);
   pfnI2ctCallback = stcI2CCfg.pfnI2cCb;
   if(TRUE == stcI2CCfg.bTouchNvic)
   {
        EnableNvic(I2C_IRQn,DDL_IRQ_LEVEL_DEFAULT,TRUE);
   }  
   return enRet;
}
/**
 ******************************************************************************
 ** \brief  I2C模块关闭初始化
 **
 ** \param 无
 **
 ** \retval 设置是否成功
 **
 ******************************************************************************/
 en_result_t I2C_DeInit(void)
 {
     en_result_t enRet = Error;
     M0P_I2C->CR = 0x00;
     enRet = Ok;
     return enRet;
 }
 /**
 ******************************************************************************
 ** \brief  I2C模块中断处理函数
 **
 ** \param u8Param 无意义
 **
 ** \retval  无
 **
 ******************************************************************************/
void I2c_IRQHandler(uint8_t u8Param)
{
    pfnI2ctCallback();
}

//@} // I2cGroup
