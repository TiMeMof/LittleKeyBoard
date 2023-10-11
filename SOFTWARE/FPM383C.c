#include "FPM383C.h"
uint8_t fpm_cmd[18];
uint8_t fpm_rec[28];
fpm_s fpm_data={1,1,1,1,1,1};

void clean_cmd(void){
	for(int i=0;i<17;i++){
		fpm_cmd[i]=0;
	}
}

//设置帧头及校验和
void Set_Frame_head(uint8_t datalen1,uint8_t datalen2){
	fpm_cmd[0]=0xF1;
	fpm_cmd[1]=0x1F;
	fpm_cmd[2]=0xE2;
	fpm_cmd[3]=0x2E;
	fpm_cmd[4]=0xB6;
	fpm_cmd[5]=0x6B;
	fpm_cmd[6]=0xA8;
	fpm_cmd[7]=0x8A;
	fpm_cmd[8]=datalen1;
	fpm_cmd[9]=datalen2;
	uint8_t tmp;
	for(int i=0;i<10;i++)tmp+=fpm_cmd[i];
	fpm_cmd[10]=(~tmp)+1;
//	fpm_cmd[10]=0x86;
}

//设置帧密码，默认为0
void Set_Frame_Password(void){
	fpm_cmd[11]=0;
	fpm_cmd[12]=0;
	fpm_cmd[13]=0;
	fpm_cmd[14]=0;
}

//设置帧命令
void Set_Frame_cmd(uint8_t cmd1,uint8_t cmd2){
	fpm_cmd[15]=cmd1;
	fpm_cmd[16]=cmd2;
}

//设置帧校验和，默认密码是0
void Set_Frame_sum(void){
	uint8_t tmp;
	tmp = fpm_cmd[15]+fpm_cmd[16];
	fpm_cmd[17]=(~tmp)+1;
//	fpm_cmd[17]=0xDC;
}
//指纹匹配命令(同步)
void fingerprint_matching_synchronization_cmd(void){
	Set_Frame_head(0x00,0x07);
	Set_Frame_Password();
	Set_Frame_cmd(0x01,0x23);
	Set_Frame_sum();
}

//重新整理uart1接收数据
uint8_t Rearrange_Rec(void){
	uint8_t begin;
	for(int i=0;i<28;i++){
		if(uart1_rx[i]==0xF1){
			begin = i;
			i = 30;
		}
	}
	for(int i=0;i<28;i++){
		if(begin+i<28)fpm_rec[i]=uart1_rx[i+begin];
		else fpm_rec[i]=uart1_rx[i+begin-28];
	}
	return begin;
}

//验证结果
uint8_t Verify_result(void){
//	uint8_t sum1=0,sum2=0;
//	for(int i=0;i<10;i++){
//		sum1+=fpm_rec[i];
//	}
//	for(int i=0;i<16;i++){
//		sum2+=fpm_rec[11+i];
//	}
//	if(fpm_rec[10]==((~sum1)+1)){
//		if(fpm_rec[27]==((~sum2)+1)){
			fpm_data.id=0;
			fpm_data.cmd_1			=		fpm_rec[15];
			fpm_data.cmd_2			=		fpm_rec[16];
			fpm_data.error_code	=		fpm_rec[20];
			fpm_data.result			=		fpm_rec[22];
			uint8_t acc_array[2]={fpm_rec[24],fpm_rec[23]};
			uint16_t *acc = (uint16_t*)&acc_array[0];
			fpm_data.accuracy		=		*acc;
			fpm_data.id					=		fpm_rec[26];
			return 0;
//		}
//		else return 1;
//	}
//	else return 1;
}
