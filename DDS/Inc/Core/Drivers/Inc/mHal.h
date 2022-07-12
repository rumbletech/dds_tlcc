/*
 * mHal.h
 *	This File Includes necessary Headers for all the Peripheral specific header files and source files .
 *  Created on: Jun 7, 2022
 *      Author: Garmoosh
 */

#ifndef GLOB_MHAL_H_
#define GLOB_MHAL_H_

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdlib.h>


#define CHECK_PARAMS
#undef CHECK_PARAMS

/* ERRORS */

#define NO_ERR 0UL
#define PARAM_ASSERT_ERR 1
#define BUSY_ERR 2
#define EMPTY_BUFF_ERR 3


#define GET_ERR( ERR ) (ERR&0X0000FFFF)
#define INVERT_SWITCH(X) (( X == Enabled )?(Disabled):(Enabled))


typedef enum {
	Enabled  = 1UL  ,
	Disabled = 0UL  ,
} flip_switch ;

typedef enum {

	HIGH = 0B1 ,
	LOW  = 0B0 ,

} Bitv ;

typedef enum{

	ACTIVE_HIGH = 0B0 ,
	ACTIVE_LOW  = 0B1 ,


} Edge_Pol ;


typedef struct {

	flip_switch dcache_sw ;
	flip_switch icache_sw ;
	flip_switch prefetch_sw ;
	uint32_t fwcycles ;

} mHal_Flash_Config_Struct ;



typedef enum {

	MHAL_EXTI_NO_TRIGGER    =  0B00 ,
	MHAL_EXTI_RISING_EDGE   =  0B01 ,
	MHAL_EXTI_FALLING_EDGE  =  0B10 ,
	MHAL_EXTI_BOTH_EDGES    =  0B11 ,


} mHal_EXTI_Trigger ;

typedef enum {

	MHAL_EXTI0 = 0B00000 ,
	MHAL_EXTI1 = 0B00001 ,
	MHAL_EXTI2 = 0B00010 ,
	MHAL_EXTI3 = 0B00011 ,
	MHAL_EXTI4 = 0B00100 ,
	MHAL_EXTI5 = 0B00101 ,
	MHAL_EXTI6 = 0B00110 ,
	MHAL_EXTI7 = 0B00111 ,
	MHAL_EXTI8 = 0B01000 ,
	MHAL_EXTI9 = 0B01001 ,
	MHAL_EXTI10 = 0B01010 ,
	MHAL_EXTI11 = 0B01011 ,
	MHAL_EXTI12 = 0B01100 ,
	MHAL_EXTI13 = 0B01101 ,
	MHAL_EXTI14 = 0B01110 ,
	MHAL_EXTI15 = 0B01111 ,
	MHAL_EXTI16 = 0B10000 ,
	MHAL_EXTI17 = 0B10001 ,
	MHAL_EXTI18 = 0B10010 ,
	MHAL_EXTI21 = 0B10101 ,
	MHAL_EXTI22 = 0B10110 ,

} mHal_EXTI_Num ;

typedef struct {

	mHal_EXTI_Num exti_num ;
	GPIO_TypeDef * gpio ;
	mHal_EXTI_Trigger trig ;
	flip_switch exti_ie ;


} mHal_EXTI_Config_Struct ;



#define MHAL_I2C_BLOCKING
/*
#define MHAL_I2C_NON_BLOCKING_Q_I2C1
#define MHAL_I2C_NON_BLOCKING_Q
*/


#define QUEUE_BUFF_SIZE 128UL

typedef struct  {


	int32_t x_ptr ;
	int32_t y_ptr ;
	uint8_t xb [QUEUE_BUFF_SIZE] ;


} mHal_Queue ;

typedef struct {

	mHal_Queue txb ;
	mHal_Queue rxb ;

} mHal_TRX_Buffer  ;










static inline void  __attribute__((naked))   __fpu_s( void )
{


	__asm__ volatile ( "  PUSH {R0-R1} \n\t"
				"LDR.W R0 , =0xE000ED88 \n\t"
				"LDR R1 , [R0] \n\t"
		    	"ORR R1 , R1 , #( 0B1111 << 20 ) \n\t"
		    	"STR R1 , [R0] \n\t"
		    	"DSB \n\t"
		    	"ISB \n\t"
		    	"POP {R0-R1} \n\t"
				"MOV PC , LR \n\t");

}




static inline uint16_t mHal_Queue_Len ( mHal_Queue * b )
{

	if ( b->y_ptr > b->x_ptr )
		return b->y_ptr - b->x_ptr ;
	else
		return QUEUE_BUFF_SIZE - ( b->x_ptr - b->y_ptr ) ;

}

 static inline void mHal_Queue_Push ( mHal_Queue * b , uint8_t * d , uint16_t len  )
{
	if ( b->y_ptr > b->x_ptr )
	{
		if ( ( b->y_ptr - b->x_ptr ) < len ) {
			return;
		}
	}
	else{
		if ( QUEUE_BUFF_SIZE - ( b->x_ptr - b->y_ptr ) < len ){
			return ;
		}
	}
	for ( ; len--  && (b->y_ptr != b->x_ptr) ; b->x_ptr %= QUEUE_BUFF_SIZE  )
		(b->xb[b->x_ptr++]) = *(d++) ;


}
/* not done */
static inline int16_t mHal_Queue_Pop ( mHal_Queue * b , uint8_t * d , uint16_t len  )
{
	int16_t ret = 0 ;
	if ( abs( b->x_ptr - b->y_ptr ) == 0  ) {
		return 0  ;
	}
	for ( ; len-- && (b->y_ptr != b->x_ptr) ; ret++   , b->y_ptr %= QUEUE_BUFF_SIZE )
		*(d++) = (b->xb[b->y_ptr++]) ;

	return ret ;
}


static inline int32_t mHal_EXTI_Fill( mHal_EXTI_Config_Struct* exti )
{
	int32_t ret = 0 ;

#ifdef CHECK_PARAMS
	if ( ( exti->exti_ie > 1 )||
		 ( exti->exti_num > 22UL )||
		 ( exti->pin_num > 15UL )||
		 ( exti->trig > 3 UL ) ||
		 ( exti->gpio != GPIOA && exti->gpio != GPIOB && exti->gpio != GPIOC && exti->gpio != GPIOD && exti->gpio !=GPIOE && exti->gpio != GPIOH ))
		ret = PARAM_ASSERT_ERR
#endif

		/* Set Rising trigger */
		EXTI->RTSR &= ~( 1UL << exti->exti_num );
		EXTI->RTSR |= ( (exti->trig&1UL) <<  exti->exti_num );
		/* Set Falling Trigger */
		EXTI->FTSR &= ~( 1UL << exti->exti_num );
		EXTI->FTSR |= ( (exti->trig&2UL&&1) <<  exti->exti_num );
		/* UnMask Interrupt */
		EXTI->IMR &= ~( 1UL << exti->exti_num );
		EXTI->IMR |= ( exti->exti_ie <<  exti->exti_num  );

		if ( exti->exti_num <= 15UL ){
		/* Enable SYSCFG */
		RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN_Msk;
		/* Configure EXTI Source */
		uint8_t gpio_v = 0 ;
		if ( exti->gpio == GPIOH)
			gpio_v = 7  ;
		else
			gpio_v = ((int32_t)exti->gpio-(int32_t)GPIOA )/(0x400) ;

		SYSCFG->EXTICR[exti->exti_num/4] &= ~( 0B1111 << (exti->exti_num%4)*4 ) ;
		SYSCFG->EXTICR[exti->exti_num/4] |=  ( gpio_v << (exti->exti_num%4)*4 ) ;
		}


		return ret ;
}


static inline void mHal_EXTI_SWInterrupt (  mHal_EXTI_Num exti_num )
{
	EXTI->SWIER |= ( 1UL << exti_num );
}

static inline void mHal_EXTI_PendingClear( mHal_EXTI_Num exti_num  )
{
	EXTI->PR |= ( 1UL << exti_num );
}


static inline int32_t mHal_Systick_Config ( uint32_t tickps , uint8_t div  )
{


	if ( tickps-1 > 0x00FFFFFF && !div )
	{
		return PARAM_ASSERT_ERR ;
	}

	if ( (tickps-1)/8 > 0x00FFFFFF && div ){
		return PARAM_ASSERT_ERR ;
	}

	/* Initialize Delay Variable */
	 SysTick->LOAD  = (uint32_t)((tickps - 1UL)/(div?8UL:1UL));       /* set reload register */
	 NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
	 SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
	 SysTick->CTRL  = ( !div << SysTick_CTRL_CLKSOURCE_Pos ) |
	                   SysTick_CTRL_TICKINT_Msk   |
	                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
	  return (0UL);


}
/*


static inline int32_t mHal_Systick_Delay( uint32_t ms )
{



}


static inline  int32_t mHal_Systick_Inc( uint8_t reset )
{
	static uint32_t systick_cnt = 0UL ;

	if( reset )
	    return (systick_cnt = 0 );

	return ++systick_cnt  ;

}


*/



#include "mHal_RCC.h"
#include "mHal_SPI.h"
#include "mHal_USBD.h"
#include "mHal_TIM.h"
#include "mHal_I2C.h"
#include "mHal_USART.h"













#endif /* CORE_DRIVERS_INC_MHAL_H_ */
