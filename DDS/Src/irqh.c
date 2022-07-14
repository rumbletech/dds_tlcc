/*
 * irqh.c
 *
 *  Created on: Jun 7, 2022
 *      Author: Garmoosh
 */



#include "main.h"




static uint32_t v_buff_count = 0 ;



void EXTI9_5_IRQHandler ( void )
{

	uint32_t cpu_sr = 0UL ;

	OS_ENTER_CRITICAL();
	OSIntEnter();
	OS_EXIT_CRITICAL();
	/* Clear IRQ Flag */
	mHal_EXTI_PendingClear( MHAL_EXTI5 );
	/* Get Key Press */
	OSTaskResume(GET_PRESS_TASK_PRIO);

	OSIntExit();


}

void EXTI3_IRQHandler( void )
{

	uint32_t cpu_sr = 0UL ;

	OS_ENTER_CRITICAL();
	OSIntEnter();
	OS_EXIT_CRITICAL();
	/* Clear IRQ Flag */
	mHal_EXTI_PendingClear( MHAL_EXTI3 );

	if ( prg_str.prg_s == PROGRAM_STATE_GENERATE )
	{
		prg_str.prg_s = PROGRAM_STATE_IDLE ;
	}
	else{
		prg_str.prg_s = PROGRAM_STATE_GENERATE ;
	}


	OSIntExit();



}


void USART1_IRQHandler ( void )
{
	uint32_t cpu_sr = 0UL ;
	uint8_t t_data = 0 ;
	OS_ENTER_CRITICAL();
	OSIntEnter();
	OS_EXIT_CRITICAL();

	t_data = mHal_USART_NBReceive(USART1);

	if ( prg_str.serial_s == SERIAL_NOT_CONNECTED  ) {
		/* Send Handshake and wait for commands */
		if ( t_data ==  'D'  ){
		mHal_USART_Send(&t_data, 1u , USART1 );
		prg_str.serial_s = SERIAL_CONNECTED ;
		}


	}
	else if (  prg_str.serial_s  == SERIAL_CONNECTED )
	{	/* Valid Command after handshake , else Disconnect */
		if ( t_data == 'S' ){
			prg_str.serial_s = SERIAL_START_CMD ;
			v_buff_count = 0 ;
		}
		else
			prg_str.serial_s = SERIAL_NOT_CONNECTED ;

	}
	else if (  prg_str.serial_s  == SERIAL_START_CMD )
	{
		v_buff[v_buff_count++] = t_data ;

		/* Command Complete */
		if ( v_buff_count == VARIABLE_WAVE_BUFF_LEN )
			prg_str.serial_s = SERIAL_NOT_CONNECTED ;


	}


	OSIntExit();


}





