#include "stm32f4xx.h"
#include "stdio.h"

void Uart1_Init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//1.����ʱ��ʹ��
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	
	//2. �������Ÿ���
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_USART1); //GPIOA9-->USART1_TX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10-->USART1_RX
	
	
	//3.GPIO�˿ڳ�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//4.���ڲ�����ʼ��
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	
	//5. ��ʼ���жϿ�����--NVIC
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn ; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0x02;  //��ռ���ȼ�2��0~3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0x02;    //��Ӧ���ȼ�2�� 0~3
	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;      //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);   //�ж����ȼ������ʼ��
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�򿪴��ڽ����ж�
	
	//6.ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1

}

void Put_Char(USART_TypeDef* USARTx, uint16_t Data)
{	
	USART_SendData(USARTx,Data);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); //�ȴ����ݷ���
}

uint16_t Get_Char(USART_TypeDef* USARTx)
{	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET); //�ȴ���������
	return USART_ReceiveData(USARTx);
}

int fputc(int c, FILE *stream)
{
	USART_SendData(USART1,c); //��������
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

	return c;
}


 int fgetc(FILE *stream)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return USART_ReceiveData(USART1);
}

/*
uint32_t data;
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		data = USART_ReceiveData(USART1); //������
		//USART_ClearITPendingBit(USART1, USART_IT_RXNE); --->����ȡ���ڽ��յ����ݺ��ж��Զ�����ģ�����Ҫ�����ж�
	}
} 

*/
