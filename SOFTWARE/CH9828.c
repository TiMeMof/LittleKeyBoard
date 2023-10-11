#include "CH9828.h"

void Send_left(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[2]=0x50;
	HAL_UART_Transmit(&huart2,ch9828,16,0xff);
}
void Send_left_noup(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[2]=0x50;
	HAL_UART_Transmit(&huart2,ch9828,8,0xff);
}
void Send_right(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[2]=0x4F;
	HAL_UART_Transmit(&huart2,ch9828,16,0xff);
}
void Send_right_noup(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[2]=0x4F;
	HAL_UART_Transmit(&huart2,ch9828,8,0xff);
}
void Send_up(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[2]=0x52;
	HAL_UART_Transmit(&huart2,ch9828,16,0xff);
}
void Send_up_noup(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[2]=0x52;
	HAL_UART_Transmit(&huart2,ch9828,8,0xff);
}
void Send_down(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[2]=0x51;
	HAL_UART_Transmit(&huart2,ch9828,16,0xff);
}
void Send_down_noup(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[2]=0x51;
	HAL_UART_Transmit(&huart2,ch9828,8,0xff);
}
//------
void Send_release(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	HAL_UART_Transmit(&huart2,ch9828,8,0xff);
}

void send_l_u_noup(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[2]=0x50;
	ch9828[3]=0x52;
	HAL_UART_Transmit(&huart2,ch9828,8,0xff);
}

void send_r_u_noup(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[2]=0x4f;
	ch9828[3]=0x52;
	HAL_UART_Transmit(&huart2,ch9828,8,0xff);
}

void send_r_d_noup(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[2]=0x4f;
	ch9828[3]=0x51;
	HAL_UART_Transmit(&huart2,ch9828,8,0xff);
}

void send_l_d_noup(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[2]=0x50;
	ch9828[3]=0x51;
	HAL_UART_Transmit(&huart2,ch9828,8,0xff);
}

//------
void Send_bigger(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[0]=0x01;ch9828[2]=0x57;
	HAL_UART_Transmit(&huart2,ch9828,16,0xff);
}

void Send_smaller(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[0]=0x01;ch9828[2]=0x56;
	HAL_UART_Transmit(&huart2,ch9828,16,0xff);
}

void Send_page_up(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[2]=0x4B;
	HAL_UART_Transmit(&huart2,ch9828,16,0xff);
}

void Send_page_down(void){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[2]=0x4E;
	HAL_UART_Transmit(&huart2,ch9828,16,0xff);
}

void Send_one_letter(char le){
	for(int i=0;i<16;i++)ch9828[i]=0x00;
	ch9828[2]=(le-65)+4;
	HAL_UART_Transmit(&huart2,ch9828,16,0xff);
}

void Send_one_num(char nu){
	if(nu>48){
		for(int i=0;i<16;i++)ch9828[i]=0x00;
		ch9828[2]=(nu-49)+0x1E;
		HAL_UART_Transmit(&huart2,ch9828,16,0xff);
	}
	else{
		for(int i=0;i<16;i++)ch9828[i]=0x00;
		ch9828[2]=0x27;
		HAL_UART_Transmit(&huart2,ch9828,16,0xff);
	}
}

void Send_password(void){
	Send_one_letter('L');
	HAL_Delay(30);
	Send_one_letter('E');
	HAL_Delay(30);
	Send_one_letter('G');
	HAL_Delay(30);
	Send_one_letter('O');
	HAL_Delay(30);
	Send_one_num('1');
	HAL_Delay(30);
	Send_one_num('9');
	HAL_Delay(30);
	Send_one_num('0');
	HAL_Delay(30);
	Send_one_num('0');
	HAL_Delay(30);
}

