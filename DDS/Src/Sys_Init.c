


#include "main.h"




#include "stm32f4xx.h"
#include <stdint.h>
#include <stdlib.h>


#define PRIO_GROUP 3UL
#define SYSTICK_PREM_PRIO 1UL
#define SYSTICK_SUB_PRIO 0UL

/* Main System Initialization Function */
 int32_t systick_cnt;

 extern OS_STK task1_stk[511];
 void task1( void *p);

void SysTick_Delay( int32_t ticks )
{

	int32_t val = systick_cnt ;
	for ( ; (systick_cnt - val) < ticks ;  );
}

void mHal_CSEN_CallBack( void )
{
	GPIOB->ODR &= ~ ( GPIO_ODR_OD12_Msk );
}

void mHal_CSNE_CallBack( void )
{
	GPIOB->ODR |= ( 1 << GPIO_ODR_OD12_Pos );
}




void RCC_Init( void )
{

	 mHal_RCC_Config_Struct RCC_Struct =
	    {
	    		 .HSI_Trim_Val    =  0UL ,
	    		 .Main_Clk        = MHAL_RCC_CLK_SRC_PLL ,
	    		 .CSS_sw 	      = Enabled ,
	    		 .HSI_sw	      = Disabled ,
	    		 .HSE_sw	      = Enabled ,
	    		 .MPLL_sw         = Enabled ,
	    		 .PLLI2S_sw       = Enabled ,
	    		 .HSEBYP_sw       = Disabled ,
	    		 .PLLQ_Divider    = MHAL_RCC_PLLQ_DIV7 ,
	    		 .PLLN_Multiplier = MHAL_RCC_PLLN_MUL432 ,
	    	     .PLLM_Divider    = MHAL_RCC_PLLM_DIV25 ,
	    	     .PLLP_Divider    = MHAL_RCC_PLLP_DIV4 ,
	    		 .MCO1_Src        = MHAL_MCO1_PLL   ,
	    		 .MCO2_Src        = MHAL_MCO2_SYSCLK ,
	    		 .MCO1_Pre        = MHAL_RCC_MCO_DIV5 ,
	    		 .MCO2_Pre        = MHAL_RCC_MCO_DIV5 ,
	    		 .I2S_Src 	      = MHAL_I2S_SRC_PLLI2S ,
				 .PLL_Src         = MHAL_PLL_SRC_HSE ,
	    		 .RTC_Pre         = MHAL_RCC_RTC_NOCLK ,
	    		 .APB2_Pre        = MHAL_RCC_APB_DIV1 ,
	    		 .APB1_Pre        = MHAL_RCC_APB_DIV4 ,
	    		 .AHB_Pre         = MHAL_RCC_AHB_DIV1 ,
	    		 .PLLI2SRDY_IE    = Disabled ,
	    		 .PLLMAINRDY_IE   = Disabled ,
	    		 .HSIRDY_IE       = Disabled ,
	    		 .HSERDY_IE       = Disabled ,
	    		 .LSERDY_IE       = Disabled ,
	    		 .LSIRDY_IE       = Disabled ,
	    };

	    mHal_Flash_Config_Struct Flash_Struct =
	    {

	    		.dcache_sw   = Enabled ,
				.icache_sw   = Enabled ,
				.prefetch_sw = Enabled ,
	    		.fwcycles    = 3UL ,
	    };


	    mHal_RCC_Fill( &RCC_Struct , &Flash_Struct  );

	    /* Set Priority Grouping for application */
		__NVIC_SetPriorityGrouping((uint32_t)PRIO_GROUP);
		/* Configure Systick */
		//mHal_Systick_Config(CPU_FREQ , 1);
	//	__NVIC_EnableIRQ(SysTick_IRQn);



}


void I2CX_Init( void )
{

	/* Copy Struct */
	mHal_I2C_Config_Struct I2C1_Struct = mHal_I2C_SM_Struct ;

	I2C1_Struct.tp_freq = 27 ;  /* Peripheral Frequency , APB1 FREQ */
	I2C1_Struct.ccr = MHAL_I2C_SM_GET_CCR( 50000 , 27000000 ); /* SET SCL FREQ TO 50 KHZ */
	/* I2C1 CLKEN */

	RCC->APB1ENR |= (  RCC_APB1ENR_I2C1EN_Msk );

	/* GPIO Config */

	/* Enable GPIOB */
	RCC->AHB1ENR |= ( RCC_AHB1ENR_GPIOBEN_Msk  ) ;
	/* Open Drain  , AFSEL = 4 , I2C1 , HIGH SPEED  */
	GPIOB->AFR[0] |= ( ( 4 << GPIO_AFRL_AFSEL6_Pos ) | ( 4 << GPIO_AFRL_AFSEL7_Pos ) );
	GPIOB->OSPEEDR |= ( ( 0B10 << GPIO_OSPEEDR_OSPEED6_Pos ) | ( 0B10 << GPIO_OSPEEDR_OSPEED7_Pos )) ;
	GPIOB->OTYPER |= ( GPIO_OTYPER_OT6_Msk | GPIO_OTYPER_OT7_Msk  );

	/* Set as OUTPUTS  */

	GPIOB->ODR |=  ( GPIO_ODR_OD7_Msk | GPIO_ODR_OD6_Msk ) ;
	GPIOB->MODER |= (( 0B01 << GPIO_MODER_MODE7_Pos )|(0B01 << GPIO_MODER_MODE6_Pos ) );

	/* Make Sure SDA Line is not Hanging by pulsing SCL */
	while ( !(GPIOB->IDR & GPIO_IDR_ID7_Msk ) ){
		//Pulse SCL
		GPIOB->ODR &= ~ ( GPIO_ODR_OD6_Msk );
		for ( int32_t i = 0 ; i < 100 ; i++);
		GPIOB->ODR |=  ( GPIO_ODR_OD6_Msk );
		for ( int32_t i = 0 ; i < 100 ; i++);
	}

	/* ERRATA SHEET Workaround for analog filter causing hang up */
	mHal_I2C_Errata_WA( I2C1 , 6 , 7 , GPIOB );

	/* Init */
	mHal_I2C_Fill( &I2C1_Struct  , I2C1 );






}


void GPIO_Init ( void )
{

	mHal_EXTI_Config_Struct EXTI5_Config_Struct =
	{
		.exti_ie = Enabled ,
		.exti_num = MHAL_EXTI5 ,
		.gpio = GPIOB ,
		.trig = MHAL_EXTI_FALLING_EDGE
	};

	RCC->AHB1ENR |= ( RCC_AHB1ENR_GPIOBEN_Msk);
	/* IRQN PIN INPUT WITH PULLUPS */
	GPIOB->MODER &= ~ ( GPIO_MODER_MODE5_Msk );
	GPIOB->PUPDR &= ~ ( GPIO_PUPDR_PUPD5_Msk );
	GPIOB->PUPDR |= ( 0B01 << GPIO_PUPDR_PUPD5_Pos );

	mHal_EXTI_Fill(&EXTI5_Config_Struct);






	/* Buzzer driver pin */
	GPIOB->ODR &= ~GPIO_ODR_OD4_Msk ;
	GPIOB->PUPDR &= ~ ( GPIO_PUPDR_PUPD4_Msk );
	GPIOB->MODER &= ~ ( GPIO_MODER_MODE4_Msk );
	GPIOB->MODER |= ( 0B01 << GPIO_MODER_MODE4_Pos );




}

void D_MPR121_Init( void ) {

	MPR121_Config_Struct MPR121_Config = MPR121_Default_Struct ;
	/* Configure the Parameters as needed*/

	/* Use Default as is */
	MPR121_Fill(&MPR121_Config);


}


void USARTX_Init( void )
{


	mHal_USART_Config_Struct USART1_Config_Struct = mHal_USART_FDUPLEX_UART_Struct ;


	USART1_Config_Struct.baud_div = mHal_USART_BaudCalc( CPU_FREQ, 115200 , 0UL );
	USART1_Config_Struct.rxn_ie = Enabled ;


	/* GPIO CONFIG */

	RCC->AHB1ENR |= ( RCC_AHB1ENR_GPIOAEN_Msk );

	GPIOA->MODER &= ~ ( 0B11 << GPIO_MODER_MODE9_Pos );
	GPIOA->MODER &= ~ ( 0B11 << GPIO_MODER_MODE10_Pos );

	GPIOA->OTYPER &= ~( GPIO_OTYPER_OT9_Msk );
	GPIOA->OTYPER &= ~( GPIO_OTYPER_OT10_Msk );

	GPIOA->OSPEEDR &= ~( 0B11 << GPIO_OSPEEDR_OSPEED9_Pos );
	GPIOA->OSPEEDR &= ~( 0B11 << GPIO_OSPEEDR_OSPEED10_Pos );

	GPIOA->OSPEEDR |= ( 0B10 << GPIO_OSPEEDR_OSPEED9_Pos );
	GPIOA->OSPEEDR |= ( 0B10 << GPIO_OSPEEDR_OSPEED10_Pos );

	GPIOA->AFR[1] &= ( 0B1111 << GPIO_AFRH_AFSEL9_Pos );
	GPIOA->AFR[1] &= ( 0B1111 << GPIO_AFRH_AFSEL10_Pos );

	GPIOA->AFR[1] |= ( 7UL << GPIO_AFRH_AFSEL9_Pos );
	GPIOA->AFR[1] |= ( 7UL << GPIO_AFRH_AFSEL10_Pos );

	GPIOA->MODER |= ( 0B10 << GPIO_MODER_MODE9_Pos );
	GPIOA->MODER |= ( 0B10 << GPIO_MODER_MODE10_Pos );


	/* Enable USART1 Clock */
	RCC->APB2ENR |= ( 1 << RCC_APB2ENR_USART1EN_Pos );


	mHal_USART_Fill( &USART1_Config_Struct , USART1 );



}



void SPIX_Init( void )
{

	  mHal_SPI_Config_Struct SPI2_Struct =
	    {

	    		.bi_en = Disabled ,
				.bi_oe = Disabled ,
				.ck_phase = MHAL_SPI_CLK_PHA_FE ,
				.ck_pol = MHAL_SPI_CLK_POL_LOW ,
				.crc_poly = 0X0007 ,
				.err = Disabled ,
				.hw_crcen = Disabled ,
				.rx_buff_ne = Disabled ,
				.rx_dma_en = Disabled ,
				.soft_cs_en = Enabled ,
				.soft_cs_v = Enabled ,
				.spi_ckdiv = MHAL_SPI_DIV64 ,
				.spi_f_data = MHAL_SPI_DATA_16BF ,
				.spi_f_format = MHAL_SPI_FRAME_MSB ,
				.spi_f_mode = MHAL_SPI_FRAME_MODE_MOTO ,
				.spi_mode = MHAL_SPI_MODE_MSTR ,
				.spi_rxonly = Disabled ,
				.sso_en = Disabled ,
				.tx_buff_e = Disabled ,
				.tx_dma_en = Disabled ,
	    };


	  /* PINT INIT FOR SPI2 */


	    RCC->AHB1ENR |= ( 1 <<  RCC_AHB1ENR_GPIOBEN_Pos ) ;

	    GPIOB->AFR[1] |= ( 5UL << GPIO_AFRH_AFSEL13_Pos ) ;
	    GPIOB->AFR[1] |= ( 5UL << GPIO_AFRH_AFSEL15_Pos ) ;


	    GPIOB->MODER &= ~( 0B11 << GPIO_MODER_MODER12_Pos );
	    GPIOB->MODER &= ~( 0B11 << GPIO_MODER_MODER13_Pos );
	    GPIOB->MODER &= ~( 0B11 << GPIO_MODER_MODER15_Pos );

	    GPIOB->ODR |= ( 1 << GPIO_ODR_OD12_Pos );
	    GPIOB->ODR |= ( 1 << GPIO_ODR_OD13_Pos );
	    GPIOB->ODR |= ( 1 << GPIO_ODR_OD15_Pos );



	    GPIOB->MODER |= ( 0B01 << GPIO_MODER_MODER12_Pos ) ;
	    GPIOB->MODER |= ( 0B10 << GPIO_MODER_MODER13_Pos ) ;
	    GPIOB->MODER |= ( 0B10 << GPIO_MODER_MODER15_Pos ) ;

	    GPIOB->OSPEEDR |= ( 0B10 << GPIO_OSPEEDR_OSPEED12_Pos) ;
	    GPIOB->OSPEEDR |= ( 0B10 << GPIO_OSPEEDR_OSPEED13_Pos) ;
	    GPIOB->OSPEEDR |= ( 0B10 << GPIO_OSPEEDR_OSPEED15_Pos) ;



	    RCC->APB1ENR |= ( 1 << RCC_APB1ENR_SPI2EN_Pos ) ;

	    mHal_SPI_Fill ( &SPI2_Struct , SPI2 );








}


void MAX7219_Init ( void )
{

    D_MAX7219_Config_Struct MAX7219_Struct = {
    		.Code_B = 0XFF , /* ALL CODE B Enabled */
    		.Intensity = 7 ,
			.Scan_Limit = 7 ,
			.d = {
					0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
			},
    };


    D_MAX7219_Init(&MAX7219_Struct);

    D_MAX7219_ScanLimit(7);
    D_MAX7219_WakeUp();



}


void DAC_Init ( void )
{


	 /* Setup AD7840 Pins */

	    RCC->AHB1ENR |= ( 1 << RCC_AHB1ENR_GPIOAEN_Pos );
	    RCC->AHB1ENR |= ( 1 << RCC_AHB1ENR_GPIOCEN_Pos );

	    /* Control Signals */
	    GPIOC->ODR |= ( 1 << GPIO_ODR_OD15_Pos );
	    GPIOC->ODR |= ( 1 << GPIO_ODR_OD14_Pos );
	    GPIOC->ODR |= ( 1 << GPIO_ODR_OD13_Pos );

	    GPIOC->MODER |= ( 1 << GPIO_MODER_MODER15_Pos );
	    GPIOC->MODER |= ( 1 << GPIO_MODER_MODER14_Pos );
	    GPIOC->MODER |= ( 1 << GPIO_MODER_MODER13_Pos );

	    GPIOC->OSPEEDR |= ( 0B10 << GPIO_OSPEEDR_OSPEED15_Pos ) ;
	    GPIOC->OSPEEDR |= ( 0B10 << GPIO_OSPEEDR_OSPEED14_Pos) ;
	    GPIOC->OSPEEDR |= ( 0B10 << GPIO_OSPEEDR_OSPEED13_Pos) ;

	    /*Data */

	    GPIOA->MODER |= ( 1 << GPIO_MODER_MODER0_Pos );
	    GPIOA->MODER |= ( 1 << GPIO_MODER_MODER1_Pos );
	    GPIOA->MODER |= ( 1 << GPIO_MODER_MODER2_Pos );
	    GPIOA->MODER |= ( 1 << GPIO_MODER_MODER3_Pos );
	    GPIOA->MODER |= ( 1 << GPIO_MODER_MODER4_Pos );
	    GPIOA->MODER |= ( 1 << GPIO_MODER_MODER5_Pos );
	    GPIOA->MODER |= ( 1 << GPIO_MODER_MODER6_Pos );
	    GPIOA->MODER |= ( 1 << GPIO_MODER_MODER7_Pos );

	    GPIOA->OSPEEDR |= ( 0B10 << GPIO_OSPEEDR_OSPEED0_Pos );
	    GPIOA->OSPEEDR |= ( 0B10 << GPIO_OSPEEDR_OSPEED1_Pos );
	    GPIOA->OSPEEDR |= ( 0B10 << GPIO_OSPEEDR_OSPEED2_Pos );
	    GPIOA->OSPEEDR |= ( 0B10 << GPIO_OSPEEDR_OSPEED3_Pos );
	    GPIOA->OSPEEDR |= ( 0B10 << GPIO_OSPEEDR_OSPEED4_Pos );
	    GPIOA->OSPEEDR |= ( 0B10 << GPIO_OSPEEDR_OSPEED5_Pos );
	    GPIOA->OSPEEDR |= ( 0B10 << GPIO_OSPEEDR_OSPEED6_Pos );
	    GPIOA->OSPEEDR |= ( 0B10 << GPIO_OSPEEDR_OSPEED7_Pos );





}


void TIMX_Init ( void )
{


	 mHal_TIM_CC_Config_Struct Tim1_OC1_Struct = {

	    		.cc_dma_req_en = Disabled ,
				.cc_ie = Disabled ,
				.cc_oc_clear_en = Disabled ,
				.cc_oc_fast_en = Disabled ,
				.cc_oc_preload_en = Disabled ,
				.cc_om = MHAL_TIM_OC_MODE_PWM1 ,
				.cc_s = MHAL_TIM_CC_OUTPUT ,
				.cc_r = 0x0080 ,
				.out_en = Enabled ,
				.inverted_out_en = Disabled ,
				.out_pol = ACTIVE_HIGH ,
				.out_idle_val = LOW ,
				.inverted_out_idle_val = LOW ,
				.inverted_out_pol = ACTIVE_HIGH ,

	    };
	    mHal_TIM_Config_Struct  Tim1_Struct  = {

	    		.arr_preload_en = Disabled ,
				.auto_output_en = Disabled ,
				.break_en = Disabled ,
				.break_ie = Disabled ,
				.break_pol = LOW ,
				.cc_dma_reqs_en = Disabled ,
				.cc_preload_en = Disabled ,
				.cc_update_select = MHAL_TIM_CC_UPDATE_COMG,
				.com_dma_req_en = Disabled ,
				.com_ie = Disabled ,
				.count_align = MHAL_TIM_COUNT_MODE_EA ,
				.count_dir = MHAL_TIM_COUNT_DIR_UP ,
				.dead_time = 0 ,
				.ext_clk_mod2_en = Disabled ,
				.lock_bits = MHAL_TIM_LOCK_OFF ,
				.main_output_en = Enabled ,
				.mms = MHAL_TIM_MSTR_MODE_ENABLE ,
				.o_off_idle_ss = Disabled ,
				.o_off_run_ss = Disabled ,
				.onepulse_en = Disabled ,
				.psc = 215 ,
				.rcr = 0 ,
				.arr = 5000 ,
				.s_clk_pre = MHAL_TIM_SCLK_DIV1 ,
				.slave_mode_s = MHAL_TIM_SLAVE_MODE_DISABLED ,
				.ti1_select = MHAL_TIM_TI1_TIM1CH ,
				.trgi_delay_en = Disabled ,
				.trgi_s = MHAL_TIM_TRIGGER_ITR0 ,
				.trig_dma_req_en = Disabled ,
				.trig_ie = Disabled ,
				.uev_gen_en = Enabled ,
				.uev_src_en = Disabled ,
				.update_dma_req_en = Disabled ,
				.update_ie = Enabled ,

				.etrg_config = NULL ,
				.cc1_config = NULL ,
				.cc2_config = NULL ,
				.cc3_config = NULL ,
				.cc4_config = NULL ,
	    };


		/* Configure Timer  PINS */

		RCC->APB2ENR |= ( 1UL << RCC_APB2ENR_TIM1EN_Pos );
		/* Int */


		/* Output Pins */

		/* Start Timer */

		mHal_TIM_Fill ( &Tim1_Struct , TIM1 ) ;
		mHal_TIM_Start( TIM1 );
		//mHal_TIM_Update( TIM1 );




}


void USBOTG_Init ( void )
{

	/* USB GPIO */

    RCC->AHB1ENR |= ( 1 <<  RCC_AHB1ENR_GPIOAEN_Pos ) ;
    GPIOA->MODER |= ( 0B10 << GPIO_MODER_MODER11_Pos ) ;
    GPIOA->MODER |= ( 0B10 << GPIO_MODER_MODER12_Pos ) ;
    GPIOA->AFR[1] |= ( 10UL << GPIO_AFRH_AFSEL11_Pos );
    GPIOA->AFR[1] |= ( 10UL << GPIO_AFRH_AFSEL12_Pos );


    RCC->AHB2ENR |= ( 1UL << RCC_AHB2ENR_OTGFSEN_Pos );

    ((USB_OTG_DeviceTypeDef*)((void*)USB_OTG_DEVICE_BASE + USB_OTG_FS_PERIPH_BASE ))->DCFG |= ( 0B11 << USB_OTG_DCFG_DSPD_Pos );
    USB_OTG_FS->GUSBCFG |= ( 1 << USB_OTG_GUSBCFG_FDMOD_Pos  );
    USB_OTG_FS->GCCFG |= ( 1 << USB_OTG_GCCFG_NOVBUSSENS_Pos ) | ( 1 << USB_OTG_GCCFG_PWRDWN_Pos );




}


void IntXEnable ( void )
{
	/* EXTI5 */
	__NVIC_SetPriority(EXTI9_5_IRQn ,NVIC_EncodePriority(PRIO_GROUP, 1UL , 0UL) );
	__NVIC_EnableIRQ(EXTI9_5_IRQn);
	/* TIM1 */
	__NVIC_SetPriority(TIM1_UP_TIM10_IRQn ,NVIC_EncodePriority(PRIO_GROUP, 0UL , 0UL) );
	__NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	/*USART1*/
	__NVIC_SetPriority(USART1_IRQn ,NVIC_EncodePriority(PRIO_GROUP, 2UL , 0UL) );
	__NVIC_EnableIRQ(USART1_IRQn);
}


