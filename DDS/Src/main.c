
#include "main.h"
#include "MVars.h"






void Idle_Task ( void *p)
{



	while(1)
	{
		if ( prg_str.prg_s == PROGRAM_STATE_GENERATE )
			OSTaskCreate(Wave_Generator , NULL , &Generate_Stack[GENERATE_TASK_STCKSZ-1] , GENERATE_TASK_PRIO ) ;

		OSTimeDlyHMSM(0, 0, 0, 100);

	}

}




void Display_Update ( void* p )
{



	while(1)
	{
		uint8_t err  = 0 ;
		uint32_t duv = (uint32_t)OSMboxPend(prg_str.du, 0 , &err) ;
		if ( err == OS_ERR_NONE  )
		{

			switch( prg_str.disp_s )
			{
			case DISPLAY_STATE_CH :
				logoch[0] = (uint8_t)duv > NUM_CHANNELS ? 1 : (uint8_t)duv ;
				D_MAX7219_WriteDigits( &logoch[0] , 0xF7);
				break ;
			case DISPLAY_STATE_TYPE :
					modoch[0] = (uint8_t) duv-1 ;
					D_MAX7219_WriteDigits( &modoch[0] , 0xF9 );
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

#if !defined(NUM_CHANNELS)
#error "Channel number undefined "
#endif

	uint32_t cpu_sr = 0UL ;

	/***************************************************************************************
	 * **************-- Generation Variables --**************************************************
	 */

#if NUM_CHANNELS >= 1UL

	uint32_t ph_acc[NUM_CHANNELS] = {0} ;
	uint32_t m[NUM_CHANNELS]  = {0} ;
	double cal_val[NUM_CHANNELS] = {0} ;
	uint8_t *rptr[NUM_CHANNELS] = {NULL} ;
	uint8_t ch_en[NUM_CHANNELS] = {0} ;
	int16_t corr[NUM_CHANNELS] = {0} ;
	int16_t ph_corr[NUM_CHANNELS] = {0} ;
	uint8_t ph_shift[NUM_CHANNELS] = {0} ;
	uint8_t ch_en_num = 0 ;

	/* Disable Warnings */
	ph_corr[NUM_CHANNELS-1]++ ;

	/* Initialize Variables */
	for ( int32_t i = 0 ; i < NUM_CHANNELS ; i++ )
	{
		ph_acc[i] = 0 ;
		m[i] = 0 ;
		cal_val[i] = 0 ;
		rptr[i] = NULL ;
		ch_en[i] = 0 ;
		corr[i] = 0 ;
		ph_corr[i] = 0 ;
		ph_shift[i] = 0 ;


	}

	ch_en_num = 0 ;

	/* List Enabled Channels */

	for ( int32_t i = 0 ; i < NUM_CHANNELS ; i++ )
	{
		if ( prg_str.ch[i].gen_s == GENERATE_STATE_DISABLE ){
			DAC_Switch(i);
			DAC_Write(0U);
		}
		else{
			ch_en[ch_en_num++] = i ;
		}
	}


	/***************************************************************************************
	 * **************-- Load Each Channel --**************************************************
	 */

for ( int32_t i = 0 ; i < ch_en_num ; i++ ){
	switch( prg_str.ch[ch_en[i]].gen_s )
		{
			case GENERATE_STATE_SINE:
				cal_val[ch_en[i]] = SINE_CAL_VAL ;
				rptr[ch_en[i]] = &s_rom[0] ;
				ph_shift[ch_en[i]] = PH_ACC_MSB_SHIFT ;
				break;
			case GENERATE_STATE_SAWTOOTH :
				cal_val[ch_en[i]] = SAWTOOTH_CAL_VAL;
				rptr[ch_en[i]] = &swt_rom[0] ;
				ph_shift[ch_en[i]] = PH_ACC_MSB_SHIFT ;
				break;
			case GENERATE_STATE_SQUARE :
				cal_val[ch_en[i]] = SQUARE_CAL_VAL ;
				rptr[ch_en[i]] = &sqr_rom[0];
				ph_shift[ch_en[i]] = PHASE_ACC_NUM_BITS-1 ;
				break;
			case GENERATE_STATE_TRIANGULAR:
				cal_val[ch_en[i]] = TRIANGLE_CAL_VAL;
				rptr[ch_en[i]] = &t_rom[0] ;
				ph_shift[ch_en[i]] = PH_ACC_MSB_SHIFT ;
				break;
			case GENERATE_STATE_SERIAL :
				cal_val[ch_en[i]] = SERIAL_CAL_VAL ;
				rptr[ch_en[i]] = &v_buff[0] ;
				ph_shift[ch_en[i]] = PH_ACC_MSB_SHIFT+1u ;

			default:
				break;
	}


	m[ch_en[i]] = get_tunew(BCDtoUInt(prg_str.ch[ch_en[i]].ik) , NUM_PRECISION_DIGITS , &corr[ch_en[i]] , cal_val[ch_en[i]] ) ;


}

	/* Disable Specific Interrupts */

	Wave_Gen_IntDisable();
	OS_ENTER_CRITICAL();


	/* Generate */

	for ( int32_t i = 0 ; i < ch_en_num && prg_str.prg_s == PROGRAM_STATE_GENERATE ; i++ , i%=ch_en_num  )
	{
		/* Select Channel */

		DAC_Switch(ch_en[i]) ;
		/* Switch Memory Bank by dummy access */
		volatile uint8_t fd = *((uint32_t*)0x08000000);
		/* Load Actual Sample*/
		fd = *(rptr[ch_en[i]] + ((ph_acc[ch_en[i]]&PHASE_ACC_MSK) >> (ph_shift[ch_en[i]]) ) );
		/* Write to DAC */
		DAC_Write( fd );
#if PHASE_CORRECTION != 0U
		/* Calculate Phase Correction value */
		ph_corr[ch_en[i]]+= corr[ch_en[i]] ;
		/* Correct phase accumlator */
	    ph_acc[ch_en[i]]+=( m[ch_en[i]] + ph_corr[ch_en[i]]/1000 ) ;
	    /* Re-accumlate phase error */
	    ph_corr[ch_en[i]]%=1000;
#else

	    ph_acc[ch_en[i]]+= m[ch_en[i]];
#endif


	}


	prg_str.prg_s = PROGRAM_STATE_IDLE ;

	OS_EXIT_CRITICAL();

	Wave_Gen_IntEnable();

#endif




	/* Trash Task */
	OSTaskDel( OS_PRIO_SELF );



}



void Get_Press ( void* p  )
{

	while ( 1 ){

	/* Get Press Status Word */
	uint32_t ks =   MPR121_GetStatus();
	OSQPost( prg_str.kq , (void*)ks);
	OSTaskSuspend(OS_PRIO_SELF);

	}




}


void Process_Press ( void* p )
{
	static  uint32_t sk_2_cnt = 0UL ;

	while(1)
	{
		uint8_t err = 0 ;
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

					OSMboxPost( prg_str.du , (void*)(prg_str.ch[prg_str.ch_s-1].gen_s+1)  );

					break ;

				case DISPLAY_STATE_FREQ :
					if ( keyp_m[kpr] <= SK_1  ){
					if ( keyp_m[kpr] <= 9 ){
					prg_str.ch[prg_str.ch_s-1].ik <<= 4 ;
					prg_str.ch[prg_str.ch_s-1].ik |= keyp_m[kpr]&0x0F ;
					}
					else{
						prg_str.ch[prg_str.ch_s-1].ik = 0 ;
					}
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


			}
			prg_str.key_prev = ks ;

		}





	}




}




void Sys_Init( void* p )
{

	 uint8_t  msg[] = "Initializing......" ;
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

	GPIOB->MODER |= 1 ;


	/* Exit Critical */
	OS_EXIT_CRITICAL();
	/* CreateTasks */
	OSTaskCreate(Get_Press , NULL , &Get_Press_Stack[GET_PRESS_TASK_STCKSZ-1], GET_PRESS_TASK_PRIO );
	OSTaskCreate(Process_Press , NULL , &Process_Press_Stack[PROCESS_PRESS_TASK_STCKSZ-1], PROCESS_PRESS_TASK_PRIO );
	OSTaskCreate(Idle_Task , NULL , &Idle_Task_Stack[IDLE_TASK_STCKSZ-1], IDLE_TASK_PRIO );
	OSTaskCreate(Display_Update , NULL , &Display_Update_Stack[DISPLAY_UPDATE_TASK_STCKSZ-1] , DISPLAY_UPDATE_PRIO );
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
	OSInit();
	/* Create Init Task */
	OSTaskCreate(Sys_Init, NULL , &SysInit_Stack[SYS_INIT_TASK_STCKSZ-1], SYS_INIT_TASK_PRIO );
	OSStart();

	while(1){}




}
