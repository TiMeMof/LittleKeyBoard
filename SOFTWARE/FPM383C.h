#ifndef __FPM383C__
#define __FPM383C__
#include "main.h"
typedef struct fpm_s_{
	uint8_t cmd_1;
	uint8_t cmd_2;
	uint16_t result;
	uint8_t error_code;
	uint16_t accuracy;
	uint8_t id;
}fpm_s;

extern uint8_t fpm_cmd[18];
extern uint8_t fpm_rec[28];
extern fpm_s fpm_data;

void clean_cmd(void);
void fingerprint_matching_synchronization_cmd(void);
uint8_t Rearrange_Rec(void);
uint8_t Verify_result(void);
#endif
