


#include "../Inc/mHal.h"






 void  __attribute__((naked))   __fpu_s( void )
{


	__asm__ volatile
	(
				"PUSH {R0-R1} \n\t"
				"LDR.W R0 , =0xE000ED88 \n\t"
				"LDR R1 , [R0] \n\t"
		    	"ORR R1 , R1 , #( 0B1111 << 20 ) \n\t"
		    	"STR R1 , [R0] \n\t"
		    	"DSB \n\t"
		    	"ISB \n\t"
		    	"POP {R0-R1} \n\t"
				"MOV PC , LR \n\t"
	);


}




uint16_t mHal_Queue_Len ( mHal_Queue * b )
{

	if ( b->y_ptr > b->x_ptr )
		return b->y_ptr - b->x_ptr ;
	else
		return QUEUE_BUFF_SIZE - ( b->x_ptr - b->y_ptr ) ;

}


void mHal_Queue_Push ( mHal_Queue * b , uint8_t * d , uint16_t len  )
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


int16_t mHal_Queue_Pop ( mHal_Queue * b , uint8_t * d , uint16_t len  )
{
	int16_t ret = 0 ;
	if ( abs( b->x_ptr - b->y_ptr ) == 0  ) {
		return 0  ;
	}
	for ( ; len-- && (b->y_ptr != b->x_ptr) ; ret++   , b->y_ptr %= QUEUE_BUFF_SIZE )
		*(d++) = (b->xb[b->y_ptr++]) ;

	return ret ;
}


int32_t mHal_EXTI_Fill( mHal_EXTI_Config_Struct* exti )
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


void mHal_EXTI_SWInterrupt (  mHal_EXTI_Num exti_num )
{
	EXTI->SWIER |= ( 1UL << exti_num );
}

void mHal_EXTI_PendingClear( mHal_EXTI_Num exti_num  )
{
	EXTI->PR |= ( 1UL << exti_num );
}


int32_t mHal_Systick_Config ( uint32_t tickps , uint8_t div  )
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
