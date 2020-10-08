#include "stm32f4xx.h"
#include "stdio.h"

void Uart1_Init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//1.串口时钟使能
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	
	//2. 设置引脚复用
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_USART1); //GPIOA9-->USART1_TX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10-->USART1_RX
	
	
	//3.GPIO端口初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//4.串口参数初始化
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	
	//5. 初始化中断控制器--NVIC
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn ; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0x02;  //抢占优先级2，0~3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0x02;    //响应优先级2， 0~3
	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;      //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);   //中断优先级分组初始化
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//打开串口接收中断
	
	//6.使能串口
	USART_Cmd(USART1, ENABLE);  //使能串口1

}

void Put_Char(USART_TypeDef* USARTx, uint16_t Data)
{	
	USART_SendData(USARTx,Data);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); //等待数据发走
}

uint16_t Get_Char(USART_TypeDef* USARTx)
{	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET); //等待接收数据
	return USART_ReceiveData(USARTx);
}

int fputc(int c, FILE *stream)
{
	USART_SendData(USART1,c); //发送数据
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
		data = USART_ReceiveData(USART1); //读数据
		//USART_ClearITPendingBit(USART1, USART_IT_RXNE); --->当读取串口接收到数据后，中断自动清除的，不需要再清中断
	}
} 

*/
