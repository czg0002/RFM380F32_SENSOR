/******************************************************************************/
/** \file crc.c
 **
 ** Common API of crc.
 ** @link crcGroup Some description @endlink
 **
 **   - 2017-05-16
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "crc.h"
/**
 *******************************************************************************
 ** \addtogroup CrcGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/


/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *****************************************************************************
 ** \brief CRC16 编码(字节填充方式)
 **
 ** 该函数主要用于生成CRC16编码.
 **
 ** \param [in]  pu8Data          待编码数据指针（字节方式输入）
 ** \param [in]  u32Len           待编码数据长度（字节数）
 ** 
 ** \retval CRC16                 CRC16编码值.                                
 *****************************************************************************/
uint16_t CRC16_Get8(uint8_t* pu8Data, uint32_t u32Len)
{
    uint32_t u32Index;
    
    M0P_CRC->RESULT_f.RESULT = 0xFFFF;
    for (u32Index=0; u32Index<u32Len; u32Index++)
    {
        *((volatile uint8_t*)(&(M0P_CRC->DATA_f))) = pu8Data[u32Index];
    }
  
    return (M0P_CRC->RESULT_f.RESULT);
}

/**
 *****************************************************************************
 ** \brief CRC16 编码(半字填充方式)
 **
 ** 该函数主要用于生成CRC16编码.
 **
 ** \param [in]  pu16Data          待编码数据指针（半字方式输入）
 ** \param [in]  u32Len           待编码数据长度（半字数）
 ** 
 ** \retval CRC16                 CRC16编码值.                                
 *****************************************************************************/
uint16_t CRC16_Get16(uint16_t* pu16Data, uint32_t u32Len)
{
    uint32_t u32Index;
    
    M0P_CRC->RESULT_f.RESULT = 0xFFFF;
    for (u32Index=0; u32Index<u32Len; u32Index++)
    {
        *((volatile uint16_t*)(&(M0P_CRC->DATA_f))) = pu16Data[u32Index];
    }
  
    return (M0P_CRC->RESULT_f.RESULT);
}

/**
 *****************************************************************************
 ** \brief CRC16 编码(字填充方式)
 **
 ** 该函数主要用于生成CRC16编码.
 **
 ** \param [in]  pu32Data          待编码数据指针（字方式输入）
 ** \param [in]  u32Len            待编码数据长度（字数）
 ** 
 ** \retval CRC16                  CRC16编码值.                                
 *****************************************************************************/
uint16_t CRC16_Get32(uint32_t* pu32Data, uint32_t u32Len)
{
    uint32_t u32Index;
    
    M0P_CRC->RESULT_f.RESULT = 0xFFFF;
    for (u32Index=0; u32Index<u32Len; u32Index++)
    {
        M0P_CRC->DATA_f.DATA = pu32Data[u32Index];
    }
  
    return (M0P_CRC->RESULT_f.RESULT);
}

/**
 *****************************************************************************
 ** \brief CRC16 校验(字节填充方式)
 **
 ** 该函数主要用于对数据及CRC16值进行校验.
 **
 ** \param [in]  pu8Data          待校验数据指针（字节方式输入）
 ** \param [in]  u32Len           待校验数据长度（字节数）
 ** \param [in]  u16CRC           待校验CRC16值
 ** 
 ** \retval Ok                    CRC校验正确
 ** \retval Error                 CRC校验错误                                
 *****************************************************************************/
en_result_t CRC16_Check8(uint8_t* pu8Data, uint32_t u32Len, uint16_t u16CRC)
{
    en_result_t enResult = Ok;
    uint32_t    u32Index;
    
    M0P_CRC->RESULT_f.RESULT = 0xFFFF;
    for (u32Index=0; u32Index<u32Len; u32Index++)
    {
        *((volatile uint8_t*)(&(M0P_CRC->DATA_f))) = pu8Data[u32Index];
    }
    
     *((volatile uint16_t*)(&(M0P_CRC->DATA_f))) = u16CRC;
    
    enResult = M0P_CRC->RESULT_f.FLAG ? Ok : Error;
    
    return (enResult); 
}

/**
 *****************************************************************************
 ** \brief CRC16 校验(半字填充方式)
 **
 ** 该函数主要用于对数据及CRC16值进行校验.
 **
 ** \param [in]  pu16Data         待校验数据指针（半字方式输入）
 ** \param [in]  u32Len           待校验数据长度（半字数）
 ** \param [in]  u16CRC           待校验CRC16值
 ** 
 ** \retval Ok                    CRC校验正确
 ** \retval Error                 CRC校验错误                                
 *****************************************************************************/
en_result_t CRC16_Check16(uint16_t* pu16Data, uint32_t u32Len, uint16_t u16CRC)
{
    en_result_t enResult = Ok;
    uint32_t    u32Index;
    
    M0P_CRC->RESULT_f.RESULT = 0xFFFF;
    for (u32Index=0; u32Index<u32Len; u32Index++)
    {
        *((volatile uint16_t*)(&(M0P_CRC->DATA_f))) = pu16Data[u32Index];
    }
    
     *((volatile uint16_t*)(&(M0P_CRC->DATA_f))) = u16CRC;
    
    enResult = M0P_CRC->RESULT_f.FLAG ? Ok : Error;
    
    return (enResult); 
}

/**
 *****************************************************************************
 ** \brief CRC16 校验(字填充方式)
 **
 ** 该函数主要用于对数据及CRC16值进行校验.
 **
 ** \param [in]  pu32Data         待校验数据指针（字方式输入）
 ** \param [in]  u32Len           待校验数据长度（字数）
 ** \param [in]  u16CRC           待校验CRC16值
 ** 
 ** \retval Ok                    CRC校验正确
 ** \retval Error                 CRC校验错误                                
 *****************************************************************************/
en_result_t CRC16_Check32(uint32_t* pu32Data, uint32_t u32Len, uint16_t u16CRC)
{
    en_result_t enResult = Ok;
    uint32_t    u32Index;
    
    M0P_CRC->RESULT_f.RESULT = 0xFFFF;
    for (u32Index=0; u32Index<u32Len; u32Index++)
    {
        *((volatile uint32_t*)(&(M0P_CRC->DATA_f))) = pu32Data[u32Index];
    }
    
     *((volatile uint16_t*)(&(M0P_CRC->DATA_f))) = u16CRC;
    
    enResult = M0P_CRC->RESULT_f.FLAG ? Ok : Error;
    
    return (enResult);
}


//@} // CrcGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
