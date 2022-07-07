/*
 * irqh.c
 *
 *  Created on: Jun 7, 2022
 *      Author: Garmoosh
 */



#include "main.h"


extern State_Struct  prg_str ;



void EXTI9_5_IRQHandler ( void )
{

	uint32_t cpu_sr = 0UL ;

	OS_ENTER_CRITICAL();
	OSIntEnter();
	OS_EXIT_CRITICAL();
	/* Clear IRQ Flag */
	mHal_EXTI_PendingClear( MHAL_EXTI5 );
	/* Get Key Press */
	prg_str.prg_s = PROGRAM_STATE_GETKEY;
	OSTaskResume(GET_PRESS_TASK_PRIO);

	OSIntExit();


}





