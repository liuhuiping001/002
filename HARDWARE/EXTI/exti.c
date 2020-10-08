#include "stm32f4xx.h" 
#include "stm32f4xx_syscfg.h"

//KEY1 --- PA0
void Exti_Init(void)
{
	//1����ʼ��GPIO, PA0
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//2����ʼ��PA --->EXTI0
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0); //ѡ��PA0��Ϊ�ⲿ�ж�0���ж�Դ 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd= ENABLE;
	EXTI_Init(&EXTI_InitStructure);     //��ʼ������EXTI�Ĵ���

	
	//3.��ʼ��NVIC
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //ֻ��1�η������þ�ok
	
	NVIC_InitStructure.NVIC_IRQChannel= EXTI0_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0x01;  //��ռ���ȼ�2��0~3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0x02;    //��Ӧ���ȼ�2�� 0~3
	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;      //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);   //�ж����ȼ������ʼ��

}


void EXTI0_IRQHandler(void)
{
	GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
	
	EXTI_ClearITPendingBit(EXTI_Line0);
	
}



void EXTI9_5_IRQHandler(void)
{
	//�жϾ�����ж�Դ
	if(RESET != EXTI_GetITStatus(EXTI_Line5))
	{
		//printf("exti9  action\n");
		EXTI_ClearITPendingBit(EXTI_Line5);
	
	}
	if(RESET != EXTI_GetITStatus(EXTI_Line6))
	{
		//printf("exti9  action\n");
		EXTI_ClearITPendingBit(EXTI_Line6);
	
	}
}
