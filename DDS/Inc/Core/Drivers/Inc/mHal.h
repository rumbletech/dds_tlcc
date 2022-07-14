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









void  __attribute__((naked))   __fpu_s( void );
uint16_t mHal_Queue_Len ( mHal_Queue * b );
void mHal_Queue_Push ( mHal_Queue * b , uint8_t * d , uint16_t len  );
int16_t mHal_Queue_Pop ( mHal_Queue * b , uint8_t * d , uint16_t len  );
int32_t mHal_EXTI_Fill( mHal_EXTI_Config_Struct* exti );
void mHal_EXTI_SWInterrupt (  mHal_EXTI_Num exti_num );
void mHal_EXTI_PendingClear( mHal_EXTI_Num exti_num  );
int32_t mHal_Systick_Config ( uint32_t tickps , uint8_t div  );





#include "mHal_RCC.h"
#include "mHal_SPI.h"
#include "mHal_USBD.h"
#include "mHal_TIM.h"
#include "mHal_I2C.h"
#include "mHal_USART.h"













#endif /* CORE_DRIVERS_INC_MHAL_H_ */
