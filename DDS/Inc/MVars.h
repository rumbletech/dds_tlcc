/*
 * MVars.h
 *
 *  Created on: Jul 6, 2022
 *      Author: Garmoosh
 */

#ifndef MVARS_H_
#define MVARS_H_

#include "main.h"



OS_STK SysInit_Stack[SYS_INIT_TASK_STCKSZ] = {0} ;
OS_STK Idle_Task_Stack[IDLE_TASK_STCKSZ] = {0};
OS_STK Get_Press_Stack[GET_PRESS_TASK_STCKSZ] = {0} ;
OS_STK Process_Press_Stack[PROCESS_PRESS_TASK_STCKSZ] = {0} ;
OS_STK Display_Update_Stack[DISPLAY_UPDATE_TASK_STCKSZ] = {0} ;
OS_STK Generate_Stack[GENERATE_TASK_STCKSZ] = {0} ;

const uint8_t keyp_m[12] = { 0 , 7 , 4 , 1 , SK_1 , 8 , 5 , 2 , SK_2 , 9 , 6 , 3 } ;
uint8_t logoch[8] = { 1 , 0xA , 0xC , 0x4E , 0x0F , 0x0F , 0x0F , 0x0F };
uint8_t modoch[8] = { 1 , 0x09 , 0x47 , 0x0F ,  0x0F , 0x0F , 0x0F , 0x0F };
void* kpr_q[KEY_PRESS_QUEUE_SIZE] ;


uint8_t v_buff[VARIABLE_WAVE_BUFF_LEN] = {0} ;



State_Struct prg_str = {
		.key_prev = 0UL ,
		.prg_s = PROGRAM_STATE_IDLE,
		.disp_s = DISPLAY_STATE_CH ,
		.serial_s = SERIAL_NOT_CONNECTED ,
		.ch_s = 1UL ,
		.ch = { { 0UL , 0UL } } ,
		.kq = NULL ,
		.du = NULL ,

};


#endif /* MVARS_H_ */
