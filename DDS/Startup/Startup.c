/*
 * SysConfig.c
 *
 *  Created on: Jun 6, 2022
 *      Author: Garmoosh
 *  Configuration File for the Project
 *
 *      */


#include <stdint.h>
#include "main.h"


/* Avoid Compiler warnings */

extern void main( void );


/* Linker Variables , should always be acessed as &(var) */

extern int32_t _estack       ;
extern int32_t _etext        ;
extern int32_t _sbss         ;
extern int32_t _ebss         ;
extern int32_t _sidata       ;
extern int32_t _edata        ;
extern int32_t _sdata        ;
extern int32_t _sbss         ;
extern int32_t __bss_start__ ;
extern int32_t __bss_end__   ;

/* Copy Data from LOADADDR(.data) to VIRTUALADDR(.data) */
void Copy_Data ( void ) {
	int32_t * src = &_sidata ;
	int32_t * dst = &_sdata ;

	while ( dst != &_edata ){
		*dst++ = *src++;
	}

}
/* Fill Bss Action starting from VIRTUALADDR(.bss) */
void Fill_Bss ( void ){
	int32_t * strt = &__bss_start__ ;
	while ( strt != &__bss_end__ ) {
		 *strt++ = (int32_t)(0UL);
	}
}

/* Reset Handler */
void Reset_Handler( void ) {

	/* Copy Static Initialized Data to RAM */
	Copy_Data();
	/* Fill BSS with Zero */
	Fill_Bss();
	/* Enable FPU ACCESS Since (Privileged mode) */
	__fpu_s();
	/* Debounce Hardware Reset */
	for ( int32_t i = 0 ; i < 500000 ; i++ );
	/*Jump to main() */
	main();




}

/* Default_Handler */

void Default_Handler ( void ) {
	while(1);
}

/* Weak IRQ Names */

void __attribute__((weak , alias("Default_Handler"))) NMI_Handler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) HardFault_Handler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) MemManage_Handler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) BusFault_Handler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) UsageFault_Handler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) SVC_Handler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) DebugMon_Handler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) OS_CPU_PendSVHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) SysTick_Handler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) EXTI16_PVD_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) TAMP_STAMP_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) EXTI22_RTC_WKUP_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) FLASH_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) RCC_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) EXTI0_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) EXTI1_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) EXTI2_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) EXTI3_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) EXTI4_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) ADC_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) EXTI9_5_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) TIM1_BRK_TIM9_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) TIM1_UP_TIM10_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) TIM1_TRG_COM_TIM11_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) TIM1_CC_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) TIM2_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) TIM3_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) TIM4_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) I2C1_EV_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) I2C1_ER_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) I2C2_EV_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) I2C2_ER_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) SPI1_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) SPI2_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) USART1_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) USART2_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) EXTI15_10_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) EXTI17_RTC_Alarm_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) EXTI18_OTG_FS_WKUP_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) SDIO_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) TIM5_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) SPI3_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) OTG_FS_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) USART6_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) I2C3_EV_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) I2C3_ER_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) FPU_IRQHandler ( void ) ;
void __attribute__((weak , alias("Default_Handler"))) SPI4_IRQHandler ( void ) ;

/* Vector table with Vector table section */

void (*isr_vectors[]) ( void ) __attribute__((section(".isr_vector_table")))    = {

		    (void (*)( void ) )&_estack ,
		    Reset_Handler ,
		    NMI_Handler ,
		    HardFault_Handler ,
		    MemManage_Handler ,
		    BusFault_Handler ,
		    UsageFault_Handler ,
		    (void (*)( void )) (0UL),
		    (void (*)( void )) (0UL),
		    (void (*)( void )) (0UL),
		    (void (*)( void )) (0UL),
		    SVC_Handler ,
		    DebugMon_Handler ,
		    (void (*)( void )) (0UL),
			OS_CPU_PendSVHandler ,
		  	SysTick_Handler ,
			(void (*)( void )) (0UL),                            			/* Reserved                                                                        */
		  	EXTI16_PVD_IRQHandler ,        			/* EXTI Line 16 interrupt /PVD through EXTI line detection interrupt               */
		  	TAMP_STAMP_IRQHandler ,        			/* Tamper and TimeStamp interrupts through the EXTI line                           */
		  	EXTI22_RTC_WKUP_IRQHandler ,   			/* EXTI Line 22 interrupt /RTC Wakeup interrupt through the EXTI line              */
		  	FLASH_IRQHandler ,             			/* FLASH global interrupt                                                          */
		  	RCC_IRQHandler ,               			/* RCC global interrupt                                                            */
		  	EXTI0_IRQHandler ,             			/* EXTI Line0 interrupt                                                            */
		  	EXTI1_IRQHandler ,             			/* EXTI Line1 interrupt                                                            */
		  	EXTI2_IRQHandler ,            			/* EXTI Line2 interrupt                                                            */
		  	EXTI3_IRQHandler ,            			/* EXTI Line3 interrupt                                                            */
		  	EXTI4_IRQHandler ,             			/* EXTI Line4 interrupt                                                            */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
			(void (*)( void )) (0UL) ,                           			/* Reserved                                                                        */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	ADC_IRQHandler ,               			/* ADC1 global interrupt                                                           */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
			(void (*)( void )) (0UL) ,                           			/* Reserved                                                                        */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	EXTI9_5_IRQHandler ,           			/* EXTI Line[9:5] interrupts                                                       */
		  	TIM1_BRK_TIM9_IRQHandler ,     			/* TIM1 Break interrupt and TIM9 global interrupt                                  */
			OS_CPU_SysTickHandler ,     			/* TIM1 Update interrupt and TIM10 global interrupt                                */
		  	TIM1_TRG_COM_TIM11_IRQHandler ,			/* TIM1 Trigger and Commutation interrupts and TIM11 global interrupt              */
		  	TIM1_CC_IRQHandler ,           			/* TIM1 Capture Compare interrupt                                                  */
		  	TIM2_IRQHandler ,              			/* TIM2 global interrupt                                                           */
		  	TIM3_IRQHandler ,              			/* TIM3 global interrupt                                                           */
		  	TIM4_IRQHandler ,              			/* TIM4 global interrupt                                                           */
		  	I2C1_EV_IRQHandler ,           			/* I2C1 event interrupt                                                            */
		  	I2C1_ER_IRQHandler ,           			/* I2C1 error interrupt                                                            */
		  	I2C2_EV_IRQHandler ,           			/* I2C2 event interrupt                                                            */
		  	I2C2_ER_IRQHandler ,           			/* I2C2 error interrupt                                                            */
		  	SPI1_IRQHandler ,              			/* SPI1 global interrupt                                                           */
		  	SPI2_IRQHandler ,              			/* SPI2 global interrupt                                                           */
		  	USART1_IRQHandler ,            			/* USART1 global interrupt                                                         */
		  	USART2_IRQHandler ,            			/* USART2 global interrupt                                                         */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	EXTI15_10_IRQHandler ,         			/* EXTI Line[15:10] interrupts                                                     */
		  	EXTI17_RTC_Alarm_IRQHandler ,  			/* EXTI Line 17 interrupt / RTC Alarms (A and B) through EXTI line interrupt       */
		  	EXTI18_OTG_FS_WKUP_IRQHandler ,			/* EXTI Line 18 interrupt / USBUSB On-The-Go FS Wakeup through EXTI line interrupt */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	SDIO_IRQHandler ,              			/* SDIO global interrupt                                                           */
		  	TIM5_IRQHandler ,              			/* TIM5 global interrupt                                                           */
		  	SPI3_IRQHandler ,              			/* SPI3 global interrupt                                                           */
			(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	OTG_FS_IRQHandler ,            			/* USB On The Go FS global interrupt                                               */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	USART6_IRQHandler ,            			/* USART6 global interrupt                                                         */
		  	I2C3_EV_IRQHandler ,           			/* I2C3 event interrupt                                                            */
		  	I2C3_ER_IRQHandler ,           			/* I2C3 error interrupt                                                            */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	FPU_IRQHandler ,               			/* Floating point interrupt                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	(void (*)( void )) (0UL) ,                            			/* Reserved                                                                        */
		  	SPI4_IRQHandler                			/* SPI4 global interrupt                                                           */
};




