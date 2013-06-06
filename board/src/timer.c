#include "timer.h"

//��84MHz��TIM3ʱ��ԴΪ168MHz/2������TIM_scale��Ƶ����Ϊ����ʱ��
void TIM2_Init(u32 TIM_scale, u32 TIM_Period)//TIM_PeriodΪ16λ����
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	NVIC_InitTypeDef  NVIC_InitStructure; 
	 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
  	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);					
  	TIM_TimeBaseStructure.TIM_Period = TIM_Period;//��������װֵ
  	TIM_TimeBaseStructure.TIM_Prescaler = 0;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  	TIM_PrescalerConfig(TIM2, (TIM_scale-1), TIM_PSCReloadMode_Immediate);
  	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  	TIM_Cmd(TIM2, ENABLE);
}
/******************************************************************/
//���ö�ʱ��3���ͨ��
void TIM3_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//ʹ�ܶ�ʱ��3
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//GPIOAʱ��ʹ��
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//����PB5Ϊ��ʱ��3��2ͨ���������
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  	GPIO_Init(GPIOB, &GPIO_InitStructure); 
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);//���Ӹ�������
}

static TIM_OCInitTypeDef  TIM_OCInitStructure;
void PWM_Init(void)
{
	uint16_t PrescalerValue=0;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	

	TIM3_Init();  
	//����Ԥ��Ƶ����	
  	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 28000000) - 1;
  	//����ʱ��
  	TIM_TimeBaseStructure.TIM_Period = 665;
  	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//PWM1ģʽ���ã�ͨ��2
  	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStructure.TIM_Pulse = 500;
  	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
  	TIM_ARRPreloadConfig(TIM3, ENABLE);
  	TIM_Cmd(TIM3, ENABLE);//����ʹ��
}

void PWM_SetPulse(u16 Pulse)
{
	TIM_Cmd(TIM3, DISABLE);//������ֹ
	TIM_OCInitStructure.TIM_Pulse = Pulse;
  	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
  	TIM_ARRPreloadConfig(TIM3, ENABLE);
  	TIM_Cmd(TIM3, ENABLE);//����ʹ��
}


