
#include "main.h"
#include "MVars.h"






void Idle_Task ( void *p)
{



	while(1)
	{


		OSTimeDlyHMSM(0, 0, 0, 100);

	}

}




void Display_Update ( void* p )
{



	while(1)
	{
		uint8_t err  = 0 ;
		uint32_t duv = OSMboxPend(prg_str.du, 0 , &err) ;
		if ( err == OS_ERR_NONE  )
		{

			switch( prg_str.disp_s )
			{
			case DISPLAY_STATE_CH :
				logoch[0] = (uint8_t)duv > NUM_CHANNELS ? 1 : (uint8_t)duv ;
				D_MAX7219_WriteDigits( &logoch , 0xF7);
				break ;
			case DISPLAY_STATE_TYPE :
					modoch[0] = (uint8_t) duv-1 ;
					D_MAX7219_WriteDigits( &modoch , 0xF9 );
					break;
			case DISPLAY_STATE_FREQ :
				D_MAX7219_WriteNum(BCDtoUInt((uint32_t)(duv == DISPLAY_CLEAR ? 0UL :duv)), ( 1 << PRECISION_POS) );
				break;

			default:
				break;

			}




		}


	}



}

void Wave_Generator ( void* p  )
{

	uint32_t cpu_sr = 0UL ;

#if NUM_CHANNELS == 1UL
	uint32_t ph_acc1 = 0;
	uint32_t m1 = 0 ;
#endif

#if NUM_CHANNELS == 2UL

	uint32_t ph_acc2 = 0 ;
	uint32_t m2 = 0 ;


#endif

	double cal_val1 ;
	switch( prg_str.ch[0].gen_s )
	{
	case GENERATE_STATE_SINE:
		cal_val1 = SINE_CAL_VAL ;
		break;
	case GENERATE_STATE_SAWTOOTH :
		cal_val1 = SAWTOOTH_CAL_VAL;
		break;
	case GENERATE_STATE_SQUARE :
		cal_val1 = SQUARE_CAL_VAL ;
		break;
	case GENERATE_STATE_TRIANGULAR:
		cal_val1 = TRIANGLE_CAL_VAL;
		break;
	default:
		cal_val1 = SINE_CAL_VAL ;
		break;
	}
	m1 = get_tunew(BCDtoUInt((uint32_t)p) , NUM_PRECISION_DIGITS , cal_val1 ) ;

	Wave_Gen_IntDisable();

	OS_ENTER_CRITICAL();
	/* Generate */
#if !defined(NUM_CHANNELS)
#error "Channel number undefined "
#endif

	if ( prg_str.ch[0].gen_s == GENERATE_STATE_DISABLE )
	{
		DAC_Write(0UL);

	}
	else if ( prg_str.ch[0].gen_s == GENERATE_STATE_SINE ){

	for( int32_t sine; prg_str.prg_s == PROGRAM_STATE_GENERATE ; ph_acc1+=m1  ){

#if defined( SINE_CALC )
			DAC_Write(round(255*calc_sinex(((double)ph_acc1/(double)PHASE_ACC_MAX) *2.0*M_PI)));

#else
			DAC_Write(s_rom[(ph_acc1&PHASE_ACC_MSK) >> PH_ACC_MSB_SHIFT]);

			__asm__ volatile ( "NOP \n\t"
							   "NOP \n\t"
							   "NOP \n\t"
							   "NOP \n\t"
							   "NOP \n\t" );
			__asm__ volatile ( "NOP \n\t"
							   "NOP \n\t"
							   "NOP \n\t"
							   "NOP \n\t"
							   "NOP \n\t" );



#endif


	}

	}
	else if ( prg_str.ch[0].gen_s == GENERATE_STATE_SQUARE )
	{
			for (  int32_t square ;prg_str.prg_s == PROGRAM_STATE_GENERATE; ph_acc1+=m1 )
			{
				DAC_Write( (ph_acc1&PHASE_ACC_MSK) < (PHASE_ACC_MAX/2UL) ? 0 : 0xFF );

					__asm__ volatile ( "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t" );
					__asm__ volatile ( "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t" );
			}
	}

	else if ( prg_str.ch[0].gen_s == GENERATE_STATE_SAWTOOTH )
	{
		for ( int32_t sawtooth ;prg_str.prg_s == PROGRAM_STATE_GENERATE; ph_acc1+=m1 )
			{
#if defined( SAW_TOOTH_CALC )
				DAC_Write( round(255.0 * ((float)(ph_acc1&PHASE_ACC_MSK)/(float)(PHASE_ACC_MAX-1.0))));
#else
				DAC_Write(swt_rom[(ph_acc1&PHASE_ACC_MSK) >> PH_ACC_MSB_SHIFT]);

				__asm__ volatile ( "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t" );
				__asm__ volatile ( "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t" );
#endif


			}

	}
	else if ( prg_str.ch[0].gen_s == GENERATE_STATE_TRIANGULAR )
	{
		for (  int32_t triangle ;prg_str.prg_s == PROGRAM_STATE_GENERATE; ph_acc1+=m1 )
			{
#if defined( TRIANGLE_CALC )
				if( (ph_acc1&PHASE_ACC_MSK) < (PHASE_ACC_MAX/2UL) )
				DAC_Write( round(255.0 * ((float)(ph_acc1&PHASE_ACC_MSK)/(float)(PHASE_ACC_MAX/2UL))));
				else
				DAC_Write( 255 - round(255.0 * ((float)((ph_acc1&PHASE_ACC_MSK)%PHASE_ACC_MAX/2UL)/(float)(PHASE_ACC_MAX/2UL))));
#else

				DAC_Write(t_rom[(ph_acc1&PHASE_ACC_MSK) >> PH_ACC_MSB_SHIFT]);

				__asm__ volatile ( "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t" );
				__asm__ volatile ( "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t"
									   "NOP \n\t" );

#endif
			}

	}

	else if ( prg_str.ch[0].gen_s == GENERATE_STATE_SERIAL )
	{
		for( int32_t s; prg_str.prg_s == PROGRAM_STATE_GENERATE ; ph_acc1+=m1  ){


				DAC_Write(v_buff[(ph_acc1&PHASE_ACC_MSK) >> (PH_ACC_MSB_SHIFT+1)]);

				__asm__ volatile ( "NOP \n\t"
								   "NOP \n\t"
								   "NOP \n\t"
								   "NOP \n\t"
								   "NOP \n\t" );
				__asm__ volatile ( "NOP \n\t"
								   "NOP \n\t"
								   "NOP \n\t"
								   "NOP \n\t"
								   "NOP \n\t" );



		}


	}
	else{
		(void)(0);
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
	static  uint32_t sk_2_cnt = 0UL ;

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
				if ( keyp_m[kpr] == SK_1 || keyp_m[kpr] == SK_2 ){

						if ( keyp_m[kpr] == SK_2  ){

							sk_2_cnt++;sk_2_cnt%=SK2_NUM_MODES ;

							if ( sk_2_cnt == 0UL )
								prg_str.disp_s = DISPLAY_STATE_CH ;
							else if ( sk_2_cnt == 1UL )
								prg_str.disp_s = DISPLAY_STATE_TYPE ;
							else if ( sk_2_cnt == 2UL )
								prg_str.disp_s = DISPLAY_STATE_FREQ ;
							else{
								prg_str.disp_s = DISPLAY_STATE_CH ;
							}

					}

				}

				switch( prg_str.disp_s )
				{
				case DISPLAY_STATE_CH :

					if(  keyp_m[kpr] && keyp_m[kpr] <= NUM_CHANNELS  )
					prg_str.ch_s = keyp_m[kpr];

					OSMboxPost( prg_str.du , (void*)prg_str.ch_s );
					break;
				case DISPLAY_STATE_TYPE :
					if ( keyp_m[kpr] <= GENERATE_STATE_SERIAL )
						prg_str.ch[prg_str.ch_s-1].gen_s =  keyp_m[kpr];

						OSMboxPost( prg_str.du , prg_str.ch[prg_str.ch_s-1].gen_s+1  );

					break ;

				case DISPLAY_STATE_FREQ :
					if ( keyp_m[kpr] <= 9U ){
					prg_str.ch[prg_str.ch_s-1].ik <<= 4 ;
					prg_str.ch[prg_str.ch_s-1].ik |= keyp_m[kpr]&0x0F ;
					}
					OSMboxPost( prg_str.du , (void*)( prg_str.ch[prg_str.ch_s-1].ik ? prg_str.ch[prg_str.ch_s-1].ik : DISPLAY_CLEAR ));
					break;

				default :
					break;

				}

				/*Beep BOOP */
				GPIOB->ODR |= (  GPIO_ODR_OD4_Msk );
				OSTimeDlyHMSM(0, 0, 0, CLICK_SOUND_TIME);
				GPIOB->ODR &= ~(  GPIO_ODR_OD4_Msk );

				if ( keyp_m[kpr] == SK_1 ){
					prg_str.prg_s = PROGRAM_STATE_GENERATE ;
					OSTaskCreate(Wave_Generator , (void*)prg_str.ch[prg_str.ch_s-1].ik , &Generate_Stack[GENERATE_TASK_STCKSZ-1] , GENERATE_TASK_PRIO ) ;
											}

			}
			prg_str.key_prev = ks ;

		}





	}




}



void Process_Serial( void*p )
{

	while(1)
	{
		uint8_t s = 'D';
		int8_t timeout = 10 ;
		int16_t w = 0 ;

		mHal_USART_Send(&s, 1u , USART1 );
		for ( ; timeout-- && (( w = mHal_USART_NBReceive(USART1) ) == -1 ) ; )
			OSTimeDly(1);

		mHal_USART_Clear();
		mHal_USART_NBReceive(USART1);

		if ( timeout != -1 && w == 'S' )
			mHal_USART_Receive( &v_buff[0] , VARIABLE_WAVE_BUFF_LEN , USART1 );



		if( prg_str.prg_s == PROGRAM_STATE_PROCESS_SERIAL )
			prg_str.prg_s = PROGRAM_STATE_IDLE ;

		OSTaskSuspend(OS_PRIO_SELF);


	}


}

void Sys_Init( void* p )
{

	const uint8_t  msg[] = "Initializing......" ;
	OS_CPU_SR cpu_sr  = 0 ;
	/* Init Variables */
	prg_str.kq = OSQCreate((void**)&kpr_q , sizeof(kpr_q)/sizeof(void*)) ;
	prg_str.du = OSMboxCreate(NULL);
	/* Enter Critical */
	OS_ENTER_CRITICAL();
	/* Initialize Everything */
	do {

	RCC_Init();
	USARTX_Init();
	mHal_USART_Send( &msg[0]  , sizeof(msg)/sizeof(uint8_t) , USART1);
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
	OSTaskCreate(Display_Update , NULL , &Display_Update_Stack[DISPLAY_UPDATE_TASK_STCKSZ-1] , DISPLAY_UPDATE_PRIO );
	OSTaskCreate(Process_Serial , NULL , &Process_Serial_Stack[PROCESS_SERIAL_TASK_STCKSZ-1] , PROCESS_SERIAL_PRIO );
	OSTaskSuspend(PROCESS_SERIAL_PRIO);
	OSTaskSuspend(GET_PRESS_TASK_PRIO);
	OSMboxPost( prg_str.du ,(void*)prg_str.ch_s);

	OSTaskDel(OS_PRIO_SELF);

	while(1)
	{

	}


}







int main(void)
{

	/* Init UCOSII */
`	OSInit();
	/* Create Init Task */
	OSTaskCreate(Sys_Init, NULL , &SysInit_Stack[SYS_INIT_TASK_STCKSZ-1], INIT_TASK_PRIO );
	OSStart();

	while(1){}




}
