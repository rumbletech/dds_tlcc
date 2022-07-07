





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




#define CPU_FREQ 108000000UL
#define NUM_DIGITS 8UL
#define NUM_PRECISION_DIGITS 2UL
#define CAL_VAL (double)0.0211000

#define CLICK_SOUND_TIME 225
#define SK_1 10
#define SK_2 11

#define S_ROM_SIZE 16384
#define QUAD_VAL 16384
#define ZERO_DC_OFFSET 0X80


/* Task Priorities */
/***********************************************************************************************
 * *********************************************************************************************
 */
#define SYS_INIT_TASK_PRIO 1UL
#define GET_PRESS_TASK_PRIO 12UL
#define PROCESS_PRESS_TASK_PRIO 13UL
#define GENERATE_TASK_PRIO 1UL
#define IDLE_TASK_PRIO 14UL

/* Task Stack Params */
/***********************************************************************************************
 * *********************************************************************************************
 */

#define SYS_INIT_TASK_STCKSZ 256UL
#define GET_PRESS_TASK_STCKSZ 256UL
#define PROCESS_PRESS_TASK_STCKSZ 512UL
#define GENERATE_TASK_STCKSZ 512UL
#define IDLE_TASK_STCKSZ 128UL

/* Queue Params */
/***********************************************************************************************
 * *********************************************************************************************
 */

#define KEY_PRESS_QUEUE_SIZE 128UL

#define INIT_TASK_PRIO 2UL

#define IDLE_TASK_PRIO 14UL

#define QUAD_BITS 0
#define PHASE_ACC_NUM_BITS 28UL
#define PHASE_ACC_MSK  (( 1UL << (PHASE_ACC_NUM_BITS-QUAD_BITS) )-1L)
#define S_ROM_NUM_BITS 16
#define PH_ACC_MSB_SHIFT (PHASE_ACC_NUM_BITS-QUAD_BITS-S_ROM_NUM_BITS)


#define QUAD_MSK 0X0C000000
#define FIRST_QUAD  0X00000000
#define SECOND_QUAD (1 << (PHASE_ACC_NUM_BITS-QUAD_BITS))
#define THIRD_QUAD  (2 << (PHASE_ACC_NUM_BITS-QUAD_BITS))
#define FOURTH_QUAD (3 << (PHASE_ACC_NUM_BITS-QUAD_BITS))

#define GET_QUAD(X) (X&QUAD_MSK)

#define PROGRAM_STATE_PROCESS_PRESS 4UL
#define PROGRAM_STATE_GENERATE 3UL
#define PROGRAM_STATE_GETKEY 1UL
#define PROGRAM_STATE_IDLE 0UL


typedef struct {

	uint32_t prg_s ;
	uint32_t key_prev ;
	OS_EVENT* kq ;
	uint32_t ik ;


	} State_Struct ;

extern uint8_t s_rom[65536];
extern uint8_t s_rom2[65536];


void static inline gpio_on( void ) {


    GPIOC->ODR |= ( GPIO_ODR_OD13_Msk  ) ;

}
void static inline gpio_off( void ) {

     GPIOC->ODR &= ~( GPIO_ODR_OD13_Msk ) ;

}


void static inline __attribute__ ((always_inline)) DAC_CS_Low( void )
{


	GPIOC->ODR &= ~( GPIO_ODR_OD14_Msk );
}

void static inline __attribute__ ((always_inline)) DAC_CS_High( void )
{

	GPIOC->ODR |= ( GPIO_ODR_OD14_Msk );

}

void static inline __attribute__ ((always_inline)) DAC_WR_High( void )
{


	GPIOC->ODR |= (  GPIO_ODR_OD15_Msk );

}

void static inline __attribute__ ((always_inline)) DAC_WR_Low( void )
{

	GPIOC->ODR &= ~( GPIO_ODR_OD15_Msk );

}

void static inline __attribute__ ((always_inline)) DAC_Switch_DACA( void )
{

	GPIOC->ODR &=  ~( GPIO_ODR_OD13_Msk  );

}

void static inline __attribute__ ((always_inline)) DAC_Switch_DACB( void )
{

	GPIOC->ODR |= ( GPIO_ODR_OD13_Msk  );

}

void static inline __attribute__((always_inline)) DAC_Latch ( void )
{
	//DAC_CS_Low();
	DAC_WR_Low();
	DAC_WR_High();
	//DAC_CS_High();


}

void static inline __attribute__ ((always_inline)) DAC_Write ( uint8_t val )
{

	/* Using BSRR takes two load instructions instead of a rmw */
		GPIOA->BSRR = 0X00FF0000;
		GPIOA->BSRR = val ;
		DAC_Latch();


}

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



#endif
