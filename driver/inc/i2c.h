/*****************************************************************************/
/** \file i2c.h
 **
 ** Headerfile for I2C functions
 **  
 **
 ** History:
 **   - 2017-06-13       First Version
 **
 *****************************************************************************/

#ifndef __I2C_H__
#define __I2C_H__

#include "ddl.h"
#include "interrupts_CMT2380F32.h"

/**
 *******************************************************************************
 ** \defgroup I2cGroup Inter-Integrated Circuit (I2C)
 **
 **
 ******************************************************************************/
//@{

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/
//#define I2c_SlaveAddr    0xA0
#define I2c_SlaveWt        0x40///<0xA0//此处从机地址根据需要进行配置
#define I2c_SlaveRd        0x41///<0xA1
/******************************************************************************
 * Global type definitions
 ******************************************************************************/
/**
 ******************************************************************************
 ** \brief I2C功能配置
 *****************************************************************************/
typedef enum en_i2c_func
{
    I2cMode_En  = 0, ///<I2C模块使能
    I2cStart_En = 1, ///<开始信号
    I2cStop_En  = 2, ///<结束信号
    I2cAck_En   = 3, ///<应答信号
    I2cHlm_En   = 4, ///<高速使能
    I2cBaud_En  = 5, ///<波特率使能
}en_i2c_func_t; 
/**
 ******************************************************************************
 ** \brief I2C从机地址配置
 *****************************************************************************/
typedef struct stc_i2c_addr
{
    uint8_t Addr;       ///<从机模式地址
    uint8_t Gc;         ///<广播地址使能

}stc_i2c_addr_t;
/**
 ******************************************************************************
 ** \brief I2C初始化配置结构
 *****************************************************************************/
typedef struct stc_i2c_config
{
    en_i2c_func_t   enFunc;      ///<功能使能
    uint8_t         u8Tm;        ///<波特率计数器配置
    stc_i2c_addr_t  stcSlaveAddr;///<从机地址
    func_ptr_t      pfnI2cCb;    ///<中断服务函数指针
    boolean_t       bTouchNvic;  ///<是否使能NVIC
}stc_i2c_config_t;

/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 *****************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 *****************************************************************************/
 //I2C初始化函数
 en_result_t I2C_Init(stc_i2c_config_t stcI2CCfg);
  //I2C模块关闭函数
 en_result_t I2C_DeInit(void);
 //设置波特率配置寄存器
 en_result_t I2C_SetBaud(uint8_t u8Tm);
 //I2C功能设置函数
 en_result_t I2C_SetFunc(en_i2c_func_t enFunc);
  //I2C功能清除函数
 en_result_t I2C_ClearFunc(en_i2c_func_t enFunc);
 //获取中断标记SI
 boolean_t I2C_GetIrq(void);
  //清除中断标记SI
 en_result_t I2C_ClearIrq(void);
 //获取状态
 uint8_t I2C_GetState(void);
 //写从机地址函数
 en_result_t I2C_WriteSlaveAddr(stc_i2c_addr_t stcSlaveAddr);
 //主发送函数
 en_result_t I2C_MasterWriteData(uint8_t u8Addr,uint8_t *pu8Data,uint32_t u32Len);
  //从发送函数
 en_result_t I2C_SlaveWriteData(uint8_t *pu8Data,uint32_t *u32Len);
 //字节写函数
 en_result_t I2C_WriteByte(uint8_t u8Data);
 //主接收函数
 en_result_t I2C_MasterReadData(uint8_t *pu8Data,uint8_t u8Addr,uint32_t u32Len);
  //主接收函数
 en_result_t I2C_SlaveReadData(uint8_t *pu8Data,uint32_t *pu32Len);
  //字节读函数
uint8_t I2C_ReadByte(void);
 
//@} // I2cGroup

#ifdef __cplusplus
#endif

#endif /* __I2C_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/


