#ifndef __CH9828__
#define __CH9828__
#include "main.h"
#include "usart.h"
void Send_left(void);
void Send_right(void);
void Send_up(void);
void Send_down(void);
void Send_left_noup(void);
void Send_right_noup(void);
void Send_up_noup(void);
void Send_down_noup(void);
//
void Send_release(void);
void send_l_u_noup(void);
void send_r_u_noup(void);
void send_r_d_noup(void);
void send_l_d_noup(void);
//
void Send_bigger(void);
void Send_smaller(void);
void Send_page_up(void);
void Send_page_down(void);
void Send_password(void);
#endif
