
#include "main.h"
#include "MVars.h"






void Idle_Task ( void *p)
{



	while(1)
	{


		OSTimeDlyHMSM(0, 0, 0, 100);

	}

}


void Wave_Generator ( void* p  )
{

	uint32_t ph_acc = 0 ;
	uint32_t cpu_sr = 0UL ;

	uint32_t m = get_tunew(BCDtoUInt((uint32_t)p) , NUM_PRECISION_DIGITS , CAL_VAL ) ;

	Wave_Gen_IntDisable();

	OS_ENTER_CRITICAL();

	/* Generate */
	for(; prg_str.prg_s == PROGRAM_STATE_GENERATE ; ph_acc+=m ){

			DAC_Write(s_rom[(ph_acc&PHASE_ACC_MSK) >> PH_ACC_MSB_SHIFT]);


	}

	OS_EXIT_CRITICAL();




	/* Trash Task */
	OSTaskDel( OS_PRIO_SELF );







}



void Get_Press ( void* p  )
{

	while ( 1 ){

	/* Get Press Status Word */
	uint32_t ks =   MPR121_GetStatus();
	prg_str.prg_s = PROGRAM_STATE_PROCESS_PRESS ;
	OSQPost( prg_str.kq , (void*)ks);
	OSTaskSuspend(OS_PRIO_SELF);

	}




}


void Process_Press ( void* p )
{


	while(1)
	{
		int8_t err = 0 ;
		uint32_t ks  = 0UL ;
		ks = (uint32_t) OSQPend(prg_str.kq , 0 , &err );
		ks&= 0xFFF;
		if ( err == OS_ERR_NONE  )
		{
			if ( !(prg_str.key_prev) && ks )
			{
				/* Log First Key Pressed */
				uint32_t kst = ks&0XFFF ;
				uint8_t kpr = 0 ;
				for (; !(kst&1UL) ; kst>>=1UL , kpr++ );

				if ( keyp_m[kpr] == SK_1 || keyp_m[kpr] == SK_2 )
				{
					if ( keyp_m[kpr] == SK_2  )
						prg_str.ik =0UL ;

				}
				else {
				prg_str.ik <<= 4 ;
				prg_str.ik |= keyp_m[kpr]&0x0F ;
				}
				/* Update Display */
				D_MAX7219_WriteNum( BCDtoUInt( prg_str.ik) , 0 ) ;
				/*Beep BOOP */
				GPIOB->ODR |= (  GPIO_ODR_OD4_Msk );
				OSTimeDlyHMSM(0, 0, 0, CLICK_SOUND_TIME);
				GPIOB->ODR &= ~(  GPIO_ODR_OD4_Msk );

				if ( keyp_m[kpr] == SK_1 ){
					prg_str.prg_s = PROGRAM_STATE_GENERATE ;
					OSTaskCreate(Wave_Generator , (void*)prg_str.ik , &Generate_Stack[GENERATE_TASK_STCKSZ-1] , GENERATE_TASK_PRIO ) ;
											}

			}
			prg_str.key_prev = ks ;

		}





	}




}



void Sys_Init( void* p )
{

	OS_CPU_SR cpu_sr  = 0 ;
	/* Init Variables */
	prg_str.kq = OSQCreate((void**)&kpr_q , sizeof(kpr_q)/sizeof(void*)) ;
	/* Enter Critical */
	OS_ENTER_CRITICAL();
	/* Initialize Everything */
	do {

	RCC_Init();
	DAC_Init();
	DAC_CS_Low();
	DAC_WR_High();
	I2CX_Init();
	SPIX_Init();
	D_MPR121_Init();
	MAX7219_Init();
	GPIO_Init();
	TIMX_Init();
	IntXEnable();

	} while(0);

	/* Exit Critical */
	OS_EXIT_CRITICAL();
	/* Create More Tasks */
	OSTaskCreate(Get_Press , NULL , &Get_Press_Stack[GET_PRESS_TASK_STCKSZ-1], GET_PRESS_TASK_PRIO );
	OSTaskCreate(Process_Press , NULL , &Process_Press_Stack[PROCESS_PRESS_TASK_STCKSZ-1], PROCESS_PRESS_TASK_PRIO );
	OSTaskCreate(Idle_Task , NULL , &Idle_Task_Stack[IDLE_TASK_STCKSZ-1], IDLE_TASK_PRIO );
	OSTaskSuspend(GET_PRESS_TASK_PRIO);
	OSTaskDel(OS_PRIO_SELF);

	while(1)
	{

	}










}




/*

//1.83 , 0.0175
int32_t v = 0  ;
uint32_t m = 2335388; // 268435456/4;
uint32_t ph_acc = 0 ;
uint32_t period_track_count = 0 ;


double cal_delta_f = 0.021110 ;   // in Micro Hertz



*/


int main(void)
{

	/* Init UCOSII */
	OSInit();
	/* Create Init Task */
	OSTaskCreate(Sys_Init, NULL , &SysInit_Stack[SYS_INIT_TASK_STCKSZ-1], INIT_TASK_PRIO );
	OSStart();

	while(1){}




}
