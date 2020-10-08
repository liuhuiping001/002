#include "stm32f4xx.h" 
#include "Delay.h" 
#include "Led.h" 
#include "Key.h" 
#include "Beep.h" 
#include "exti.h"
#include "usart.h"
#include "stdio.h"
#include "timer.h"

uint8_t uart1_rx_buf[5]={0xAA,0x00,0x00,0x00,0x00};
uint8_t uart1_flag = 0;

void USART1_IRQHandler(void)
{
	static uint32_t count = 0;
	uint32_t i, check_sum = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收数据产生中断
	{
		uart1_rx_buf[count] = (char)USART_ReceiveData(USART1); //从接收缓存寄存器读取数据
		if(uart1_rx_buf[0] == 0xAA)
			count ++;
		if(count == 5)
		{
			for(i=0;i<(5-1); i++)
				check_sum = check_sum + uart1_rx_buf[i];
			check_sum = check_sum & 0xFF;
			if(check_sum == uart1_rx_buf[4])
				uart1_flag = 1;
			else
				uart1_flag = 0;
			
			count = 0;
		}
		//USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}




int main(void)
{ 
	uint16_t data;

	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //只做1次分组设置就ok
	Led_Init();
	Key_Init();
	Beep_Init();
	Exti_Init();
	Uart1_Init(9600);
	Timer6_Init();
		
	
	while(1)
	{
		
		if(uart1_flag == 1)
		{
			data = (uart1_rx_buf[3]<<8) + uart1_rx_buf[2];
			switch(uart1_rx_buf[1])
			{
				case 0x01:
					if(data == 0x0001)
						printf("LED1 on\n");
					else if(data == 0x0000)
						printf("LED1 off\n");
					uart1_flag = 0; //一包数据处理结束
					break;
				default:
					uart1_flag = 0;
					break;
				
			}
			
		}

	}
	
	return 0;
}
 



