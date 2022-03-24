#include "ddl.h"
#include "flash.h"
#include "Flash_ID.h"
#include "CMT2380F32_Demo.h"


static volatile uint32_t u32FlashTestFlag   = 0;

/*******************************************************************************
 * FLASH 
 ******************************************************************************/
 void FlashInt(void)
 {
    if (TRUE == Flash_GetIntFlag(flash_int0))
    {
        Flash_ClearIntFlag(flash_int0);
        u32FlashTestFlag |= 0x01;
        Flash_DisableIrq(flash_int0);
    }
    if (TRUE == Flash_GetIntFlag(flash_int1))
    {
        Flash_ClearIntFlag(flash_int1);
        u32FlashTestFlag |= 0x02;
        Flash_DisableIrq(flash_int1);
    }
      
 }
 
void Flash_ID_Init(void)
{
uint32_t          u32Data;
	
    Flash_Init(FlashInt, 0);
	
    u32Data=*((volatile uint32_t*)Prodution_ID_ADDR);
	  
    Prodution_ID[0]=(uint8_t)u32Data;
	  u32Data=u32Data>>8;
	  Prodution_ID[1]=(uint8_t)u32Data;
	  u32Data=u32Data >>8;
	  Prodution_ID[2]=(uint8_t)u32Data;

}	
