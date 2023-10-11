#include "tim_callback.h"
uint32_t timcount=0;
uint32_t uart1_rx_flag=0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==TIM1){
		if(htim->Channel==TIM_CHANNEL_1){
			timcount++;
//			if(timcount==1000){
//				timcount=0;
//			}
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance==USART1){
		uart1_rx_flag=Rearrange_Rec();
		Verify_result();
		
		HAL_UART_Receive_IT(&huart1,uart1_rx,28);
	}
	if(huart->Instance==USART2){
		HAL_UART_Receive_IT(&huart2,uart2_rx,1);
	}
}
