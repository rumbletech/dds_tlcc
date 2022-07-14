





#ifndef __MAINH_TOKEN
#define __MAINH_TOKEN

#include <stdint.h>
#include <stdlib.h>
#include "core_cm4.h"
#include "stm32f4xx.h"
#include <math.h>
#include "glob.h"
#include "Core/Drivers/Inc/mHal.h"
#include "D_MAX7219.h"
#include "D_MPR121.h"
#include <ucos_ii.h>



/* Currenct CPU FREQ */
#define CPU_FREQ 108000000UL
/* Number of digits used by the display */
#define NUM_DIGITS 8UL
/* Number of precision point digits used */
#define NUM_PRECISION_DIGITS 1UL
/* Precision '.' placement */
#define PRECISION_POS 1UL
/* Phase correction ON/OFF Switch */
#define PHASE_CORRECTION 0U
/* Number of Channels USED */
#define NUM_CHANNELS 1UL

/* Calibration Values */
#ifndef NUM_CHANNELS
#error "NUM_CHANNELS  not defined -error "
#endif

#ifndef PHASE_CORRECTION
#error "PHASE_CORRECTION not defined -error "
#endif

#if NUM_CHANNELS == 1UL

#if PHASE_CORRECTION == 0UL
/* Calibration Values for NO PHASE CORRECTION */
#define SINE_CAL_VAL (double)0.002874141123
#define SQUARE_CAL_VAL (double)0.002874141123
#define SAWTOOTH_CAL_VAL (double) 0.002874141123
#define TRIANGLE_CAL_VAL (double) 0.002874141123
#define SERIAL_CAL_VAL (double) 0.003468802384

#else
/* Calibration Values for  PHASE CORRECTION */
#define SINE_CAL_VAL (double)0.002453475177
#define SQUARE_CAL_VAL (double)0.002453475177
#define SAWTOOTH_CAL_VAL (double) 0.002453475177
#define TRIANGLE_CAL_VAL (double) 0.002453475177
#define SERIAL_CAL_VAL (double) 0.002453475177


#endif


#elif NUM_CHANNELS == 2UL

#if PHASE_CORRECTION == 0UL

#define SINE_CAL_VAL (double)0.002453475177
#define SQUARE_CAL_VAL (double)0.002453475177
#define SAWTOOTH_CAL_VAL (double) 0.002453475177
#define TRIANGLE_CAL_VAL (double) 0.002453475177
#define SERIAL_CAL_VAL (double) 0.002453475177



#else

/* Calibration Values for  PHASE CORRECTION */
#define SINE_CAL_VAL (double)0.002453475177
#define SQUARE_CAL_VAL (double)0.002453475177
#define SAWTOOTH_CAL_VAL (double) 0.002453475177
#define TRIANGLE_CAL_VAL (double) 0.002453475177
#define SERIAL_CAL_VAL (double) 0.002453475177

#endif


#endif



/* Handshake Word for Serial */
#define SERIAL_START_W 'D'
/* Number of Function button modes after each click */
#define SK2_NUM_MODES 3UL
/* Click sound time in ms */
#define CLICK_SOUND_TIME 225
/* Special function buttons numbers */
#define SK_1 10
#define SK_2 11
/* Taylor seires polynomial degree for sine calculation */
#define SINE_CALC_DEG 4
/* Serial SAMPLE DEPTH */
#define VARIABLE_WAVE_BUFF_LEN 32768


/* Task Priorities */
/***********************************************************************************************
 * *********************************************************************************************
 */
#define SYS_INIT_TASK_PRIO 1UL
#define GET_PRESS_TASK_PRIO 10UL
#define PROCESS_PRESS_TASK_PRIO 12UL
#define GENERATE_TASK_PRIO 1UL
#define IDLE_TASK_PRIO 14UL
#define DISPLAY_UPDATE_PRIO  11UL


/* Task Stack Params */
/***********************************************************************************************
 * *********************************************************************************************
 */

#define SYS_INIT_TASK_STCKSZ 256UL
#define GET_PRESS_TASK_STCKSZ 256UL
#define PROCESS_PRESS_TASK_STCKSZ 512UL
#define GENERATE_TASK_STCKSZ 512UL
#define IDLE_TASK_STCKSZ 128UL
#define DISPLAY_UPDATE_TASK_STCKSZ 128UL


/* Queue Params */
/***********************************************************************************************
 * *********************************************************************************************
 */




/* QUEUE SIZES */
#define KEY_PRESS_QUEUE_SIZE 128UL




/* PHASE ACCUMULATOR CONFIGURATION */
#define QUAD_BITS 0                  // NUMBER OF QUAD BITS USED IN THE PHASE ACCUMLATOR
#define PHASE_ACC_NUM_BITS 30UL      // PHASE ACCUMLATOR BITS USED
#define PHASE_ACC_MAX  (uint32_t)(( 1UL << PHASE_ACC_NUM_BITS )) // MAX PH_ACC VALUE
#define PHASE_ACC_MSK  (( 1UL << (PHASE_ACC_NUM_BITS-QUAD_BITS) )-1L) //MASK
#define S_ROM_NUM_BITS 16			 // NUMBER OF ROM BITS USED
#define PH_ACC_MSB_SHIFT (PHASE_ACC_NUM_BITS-QUAD_BITS-S_ROM_NUM_BITS) // SHIFTED VALUE TO GET ROM MSB BITS


/* REDUCED ROM QUAD MAP */
#define QUAD_MSK 0X0C000000
#define FIRST_QUAD  0X00000000
#define SECOND_QUAD (1 << (PHASE_ACC_NUM_BITS-QUAD_BITS))
#define THIRD_QUAD  (2 << (PHASE_ACC_NUM_BITS-QUAD_BITS))
#define FOURTH_QUAD (3 << (PHASE_ACC_NUM_BITS-QUAD_BITS))
#define GET_QUAD(X) (X&QUAD_MSK)



/* PROGRAM STATES */

#define PROGRAM_STATE_GENERATE 3UL
#define PROGRAM_STATE_IDLE 0UL


/* DISPLAY STATES */
#define DISPLAY_STATE_CH   0UL
#define DISPLAY_STATE_TYPE 1UL
#define DISPLAY_STATE_FREQ 2UL
#define DISPLAY_CLEAR 0xFFFFFFFF

/* GENERATION STATES */
#define GENERATE_STATE_DISABLE    0UL
#define GENERATE_STATE_SINE       1UL
#define GENERATE_STATE_SQUARE     2UL
#define GENERATE_STATE_SAWTOOTH   3UL
#define GENERATE_STATE_TRIANGULAR 4UL
#define GENERATE_STATE_SERIAL 5UL

/* SERIAL STATES */
#define SERIAL_CONNECTED 1UL
#define SERIAL_NOT_CONNECTED 0UL
#define SERIAL_START_CMD 2UL




/* Channel DATA */
typedef struct {

	uint32_t gen_s ;         // type
	uint32_t ik ;            //freq word


} Channel_X;

/* Main Program DATA Struct */
typedef struct {

	uint32_t prg_s ;							// Program State
	uint32_t disp_s ;                           // Display State
	uint32_t serial_s ;
	Channel_X ch[NUM_CHANNELS] ;                // Channel/Channels states
	uint32_t ch_s ;                             // channel selection
	uint32_t key_prev ;                         // Previous key pressed
	OS_EVENT* kq ;                              // keypress queue
	OS_EVENT* du ;                              // display mailbox
	OS_EVENT* sq ;                              // serial queue


	} State_Struct ;



#ifndef SINE_CALC
extern uint8_t s_rom[65536];         // SINE TABLE
#endif
#ifndef SAWTOOTH_CALC
extern uint8_t swt_rom[65536];       // SAWTOOTH TABLE
#endif
#ifndef TRIANGLE_CALC
extern uint8_t t_rom[65536];         // Triangle TABLE
#endif
extern uint8_t sqr_rom[2] ;        // 2 Byte Square TABLE


/* CS Low */
void static inline __attribute__ ((always_inline)) DAC_CS_Low( void )
{


	GPIOC->ODR &= ~( GPIO_ODR_OD14_Msk );
}
/* CS High */
void static inline __attribute__ ((always_inline)) DAC_CS_High( void )
{

	GPIOC->ODR |= ( GPIO_ODR_OD14_Msk );

}
/* WR High */
void static inline __attribute__ ((always_inline)) DAC_WR_High( void )
{
	GPIOC->BSRR = ( GPIO_BSRR_BS15_Msk );

}
/* WR Low */
void static inline __attribute__ ((always_inline)) DAC_WR_Low( void )
{
	GPIOC->BSRR = ( GPIO_BSRR_BR15_Msk );
}

/* DACA Switch */
void static inline __attribute__ ((always_inline)) DAC_Switch_DACA( void )
{

	GPIOC->BSRR = ( GPIO_BSRR_BR13_Msk );

}

/* DACB Switch */
void static inline __attribute__ ((always_inline)) DAC_Switch_DACB( void )
{

	GPIOC->BSRR = ( GPIO_BSRR_BS13_Msk );

}

/* Switches to Nth DAC ( ONLY 2 are implemented */

void static inline __attribute__((always_inline)) DAC_Switch( uint8_t ch )
{
	if ( ch == 0 )
	GPIOC->BSRR = ( GPIO_BSRR_BS13_Msk );
	else
	GPIOC->BSRR = ( GPIO_BSRR_BR13_Msk );

}


/* Pulses WR Low -- minimum pulse width is accounted for */

void static inline __attribute__((always_inline)) DAC_Latch ( void )
{
	//DAC_CS_Low();
	DAC_WR_Low();
	__asm__ volatile ( "NOP \n\t"
					   "NOP \n\t"
					   "NOP \n\t"
					   "NOP \n\t"
					   "NOP \n\t" );
	DAC_WR_High();
	//DAC_CS_High();


}


/* Writes VAL to DAC */

void static inline __attribute__ ((always_inline)) DAC_Write ( uint8_t val )
{

	/* Using BSRR takes two load instructions instead of a rmw */
		GPIOA->BSRR = 0X00FF0000;
		GPIOA->BSRR = val ;
		DAC_Latch();


}


/* Initialization Routines */

void RCC_Init( void );
void I2CX_Init( void );
void GPIO_Init ( void );
void D_MPR121_Init( void );
void SPIX_Init( void );
void MAX7219_Init ( void );
void DAC_Init ( void );
void TIMX_Init ( void );
void USBOTG_Init ( void );
void IntXEnable( void );
void USARTX_Init( void );


/* Calculation Functons    -- KILL WARNINGS  */

uint8_t  F_SquareCalc( double n_sample  );
float FreqW ( uint32_t freq , uint8_t num_prec );
uint32_t BCDtoUInt( uint32_t degs );
uint32_t get_tunew( uint32_t freq , uint8_t prec , int16_t* corr , double d_f );
void Wave_Gen_IntEnable( void );
void Wave_Gen_IntDisable( void );

/* Tasks */


void Idle_Task ( void *p);
void Display_Update ( void* p );
void Wave_Generator ( void* p  );
void Get_Press ( void* p  );
void Process_Press ( void* p );
void Sys_Init( void* p );


/* Externs */

extern State_Struct  prg_str ;
extern uint8_t v_buff[VARIABLE_WAVE_BUFF_LEN];
extern OS_STK Generate_Stack[GENERATE_TASK_STCKSZ];



#endif
