//V1.0.0
#include "stm32f4xx_it.h"
#include "main.h"

//NMI exception handler
void NMI_Handler(void)
{
}

//Hard Fault exception handler
void HardFault_Handler(void)
{
  	while (1)
  	{
  	}
}

//Memory Manage exception handler
void MemManage_Handler(void)
{
  	while (1)
  	{
  	}
}

//Bus Fault exception handler
void BusFault_Handler(void)
{
  	while (1)
  	{
  	}
}

//Usage Fault exception handler
void UsageFault_Handler(void)
{
  	while (1)
  	{
  	}
}

//SVCall exception handler
void SVC_Handler(void)
{
}

//Debug Monitor exception handler
void DebugMon_Handler(void)
{
}

//PendSVC exception handler
void PendSV_Handler(void)
{
}

//SysTick handler
extern u32 ntime;
void SysTick_Handler(void)//ϵͳ�����ж�,������ʱ
{
	ntime--;
}

void SDIO_IRQHandler(void)
{
  	SD_ProcessIRQSrc();
}

void SD_SDIO_DMA_IRQHANDLER(void)
{
  	SD_ProcessDMAIRQ();
}
//��ʱ�������ж�,��ʾ����
extern u16 scrpos;
void TIM2_IRQHandler(void)
{
  	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  	{ 	
		LEDTog(LED1);
		if(scrpos==40)RTC_TimeShow(98,11);	
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
  	}
}

void EXTI9_5_IRQHandler(void)//�����ж�
{
  	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
  	{
		//LEDTog(LED2);
		EXTI_ClearITPendingBit(EXTI_Line6);
  	}
}
//�����ж�,���н���
void EXTI15_10_IRQHandler(void)
{
  	if(EXTI_GetITStatus(EXTI_Line10) != RESET)
  	{
		LEDOn(LED4);
		Prt_Screen("0:/SYSTEM/SCRSHOT/Pic1.bmp");
		LEDOff(LED4);		
    	EXTI_ClearITPendingBit(EXTI_Line10);
  	}
}
//DCMI֡�ж�
void DCMI_IRQHandler(void)
{
	if(DCMI_GetITStatus(DCMI_IT_FRAME)!=RESET)LEDTog(LED3);
	DCMI_ClearITPendingBit(DCMI_IT_FRAME);
	if(Get_Key(KEY1)==0)
	{
		DCMI_Cmd(DISABLE);
		DCMI_CaptureCmd(DISABLE);
		Prt_Screen("0:/SYSTEM/SCRSHOT/Pic1.bmp");
	} 
}

