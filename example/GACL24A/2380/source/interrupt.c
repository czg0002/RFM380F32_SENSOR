#include "CMT2380F32_Demo.h"

uint16_t led_time_cnt1=0;

//��ʱ��0�жϷ�����
void Time0_Irq(void)
{
		Bt_ClearIntFlag(TIM0);
		SysTime++;
	  if(SysTime>10000)
			SysTime=10000;
//		LedTime++;
	//	ResetTime++;
	
}

//��ʱ��1�жϷ�����
void Time1_Irq(void)
{
		Bt_ClearIntFlag(TIM1);
		SysTime++;
	  if(SysTime>10000)
			SysTime=10000;
//		LedTime++;
//		ResetTime++;
	
}


