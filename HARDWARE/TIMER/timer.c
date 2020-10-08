#include "stm32f4xx.h"

void Timer6_Init(void) //T = 1ms
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //TIM6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE); //NVIC
	
	//TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 99; //ARR=(99+1) 100KHz/100=1KHz  
	TIM_TimeBaseInitStructure.TIM_Prescaler = 839; //������Ƶ��=84MHz/(839+1)=100KHz
	//TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE); //��Timer6���������ж�
	
	NVIC_InitStructure.NVIC_IRQChannel= TIM6_DAC_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0x00;  //��ռ���ȼ�2��0~3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0x03;    //��Ӧ���ȼ�2�� 0~3
	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;      //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);   //�ж����ȼ������ʼ��
	
	TIM_Cmd(TIM6,ENABLE);
		
}


void TIM6_DAC_IRQHandler(void)
{
	static uint32_t count = 0;
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) != RESET) 
	{
		count=count + 1 ;
		if(count == 500)//0.5s
		{
			GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
			count = 0;
			
		}
	}
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
}



