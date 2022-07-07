/*
 * mHal_I2C.c
 *
 *  Created on: Jun 18, 2022
 *      Author: Garmoosh
 */



#include <mHal_I2C.h>



#if defined( MHAL_I2C_NON_BLOCKING_Q) && !defined(QUEUE_BUFF_SIZE)
#define QUEUE_BUFF_SIZE 128
#warning "I2C Queue Buffer Size set to 128 by default"
#endif

#if !defined( MHAL_I2C_NON_BLOCKING_Q ) && !defined( MHAL_I2C_BLOCKING )
#define MHAL_I2C_BLOCKING
#warning "I2C Set to Blocking by default"
#endif

#if defined( MHAL_I2C_NON_BLOCKING_Q ) && !defined( MHAL_I2C_NON_BLOCKING_Q_I2C1 ) && !defined(MHAL_I2C_NON_BLOCKING_Q_I2C2)
#error "I2C Blocking Queue Enabled and no I2C Peripheral Specified"
#endif













mHal_I2C_Config_Struct mHal_I2C_SM_Struct = {

	 .mode = MHAL_I2C_SM ,    								  /* i2c mode , standard mode (SM) or Fast Mode (FM) */
	 .bus_mode = MHAL_I2C_I2CBUS ,       					  /* I2C BUS OR SMBUS */
	 .smbus_type = 0 , 										  /* SMBUS Type only if bus is selected to be smbus */
	 .ccr =   MHAL_I2C_SM_GET_CCR(100000 , 42000000 ) , 	  /* Determines SCL Period */
	 .fm_duty = 0 ,        									  /* FM mode duty cycle tlow / thigh */
	 .tp_freq = 42UL ,               						  /* Peripheral Frequency 2<= fph <=50 */
	 .oaddr1  = 0x0000 ,          							  /* own address  1 */
	 .oaddr2  = 0x0000  ,            						  /* own address 2 if dual addressing is enabled */
	 .addr10bit = Disabled,       									  /* ten bit address enable */
	 .dual_addren = Disabled ,       									  /* dual address enable */
	 .tr  = MHAL_I2C_SM_MAX_TR ,                								  /* Rise time */
	 .analog_f_en = Enabled ,  									      /* Analog Filter Enable */
	 .digita_f = MHAL_I2C_DF_4TP ,           									  /* Digital Filter 0b0000 = Disabled , else it filters up to value * Peripheral clock */
	/* Interrupts */
	 .err_int_en  = Disabled ,          /* Error Interrupt Enable */
	 .event_int_en = Disabled ,         /* Event Interrupt Enable */
	 .buff_int_en  = Disabled,       /* Buffer interrupt Enable */
	 .dma_rq_en    = Disabled,        /* DMA Requests enabled */
	/* Control Switches */
	 .gcall_en = Disabled,            /* General Call enable (responds to general call on the bus */
     .pec_en  = Disabled ,             /* error check packet enable */
     .arp_en  = Disabled ,          /* SMBUS Related  */
     .clk_stretch_en = Enabled ,     /* Enable Clock Stretching */

};




void mHal_I2C_Reset( I2C_TypeDef* i2c )
{
	i2c->CR1 |= (  I2C_CR1_SWRST_Msk );
	i2c->CR1 &= ~( I2C_CR1_SWRST_Msk );
}


void mHal_I2C_Stop_Gen ( I2C_TypeDef * i2c ) {

	i2c->CR1 |= ( I2C_CR1_STOP_Msk );
}

static inline void mHal_I2C_ACK( I2C_TypeDef * i2c )
{
	i2c->CR1 |= ( I2C_CR1_ACK_Msk );
}

static inline void mHal_I2C_NACK( I2C_TypeDef * i2c )
{
	i2c->CR1 &= ~( I2C_CR1_ACK_Msk );
}

 void mHal_I2C_Start_Gen( I2C_TypeDef * i2c  )
{

	i2c->CR1 |= ( I2C_CR1_START_Msk );
#ifdef MHAL_I2C_BLOCKING
	while ( !(i2c->SR1 & (I2C_SR1_SB_Msk)));
#endif

}



void mHal_I2C_Disable (I2C_TypeDef * i2c )
 {
	 i2c->CR1 &= ~( I2C_CR1_PE_Msk );
 }

 void mHal_I2C_Enable (I2C_TypeDef * i2c )
 {
	 i2c->CR1 |= ( I2C_CR1_PE_Msk );
 }


 void mHal_I2C_Errata_WA( I2C_TypeDef * i2c  , uint8_t scl , uint8_t sda , GPIO_TypeDef * gpio )
 {

	 /* Disable The Peripheral */
	 mHal_I2C_Disable(i2c);
	 /* SCL ,SDA OPEN DRAIN HIGH */
	 gpio->ODR   |= (( 1 << scl )|( 1 << sda ));
	 gpio->MODER |= (( 1 << scl*2 )|( 1 << sda*2 ));
	 gpio->OTYPER|= (( 1 << scl )|( 1 << sda ));
	 /* Check in IDR */
	 while( !(gpio->IDR&(1<<scl)) || !(gpio->IDR&(1<<sda)) );
	 /* SDA Goes Low */
	 gpio->ODR &= ~ ( 1 << sda );
	 /* Check SDA Low */
	 while( gpio->IDR & ( 1 << sda ));
	 /* SCL Goes Low */
	 gpio->ODR &= ~ ( 1 << scl );
		 /* Check SCL Low */
	 while( gpio->IDR & ( 1 << scl ));
	 /* SCL Goes High */
	 gpio->ODR |=  ( 1 << scl );
		 /* Check SCL High */
	 while( !(gpio->IDR & ( 1 << scl )));
	 /* SDA Goes High */
	 gpio->ODR |=  ( 1 << sda );
		 /* Check SDA High */
	 while( !(gpio->IDR & ( 1 << sda )));
	 /* Configure in AF mode */
	 uint32_t temp = gpio->MODER ;
	 temp &= ~(( 0B11 << scl*2)  | (0B11 << sda*2 ));
	 temp|= ( (0B10 << scl*2 )|(0B10 << sda*2 ) );
	 gpio->MODER =  temp ;
	 /* Do a soft Reset */
	 mHal_I2C_Reset(i2c);
	 /* Enable Peripheral */
	 mHal_I2C_Enable(i2c);



 }






















#ifdef MHAL_I2C_NON_BLOCKING_Q

static inline void mHal_I2C_EventHandler( I2C_TypeDef * i2c , mHal_TRX_Buffer * i2c_buff )
{

	uint32_t s_w  = i2c->SR1 ;
	uint32_t s_w2 = i2c->SR2 ;
	uint8_t temp ;
	uint8_t ret ;
	if ( s_w & I2C_SR1_SB_Msk || s_w & I2C_SR1_ADD10_Msk )
	{
		/* Start Generation Event */
		ret =  mHal_Queue_Pop(&i2c_buff->txb , &temp , 1UL    ) ;

		if ( !ret )
			mHal_I2C_Stop_Gen(i2c);
		else
			i2c->DR = temp ;

	}

	if ( s_w & I2C_SR1_AF_Msk )
	{

		mHal_I2C_Stop_Gen(i2c);

	}

	if ( s_w & I2C_SR1_ADDR_Msk )
	{


	}




}


#if defined (MHAL_I2C_NON_BLOCKING_Q_I2C1 )


mHal_TRX_Buffer i2c1_buff = {
		.txb = {} ,
		.rxb = {} ,
};



void __attribute__((weak)) I2C1_EV_Handler_CallBack ( void  )
{


}

void I2C1_EV_IRQHandler ( void ) {

	mHal_I2C_EventHandler(I2C1 , &i2c1_buff  );



	I2C1_EV_Handler_CallBack();


}


#endif

#if defined (MHAL_I2C_NON_BLOCKING_Q_I2C2 )


mHal_TRX_Buffer i2c2_buff = {
		.txb = {} ,
		.rxb = {} ,
};


#endif



#endif

















int32_t mHal_I2C_Master_Begin( I2C_TypeDef * i2c  , uint8_t* data , int32_t len , uint16_t addr  , mHal_I2C_Master_Mode i2c_mm , mHal_I2C_Params tparams )
{



#ifdef MHAL_I2C_NON_BLOCKING_Q

	mHal_TRX_Buffer * i2c_buff ;

	switch ( (uint32_t)i2c )
	{
	case (uint32_t)I2C1 :
#if defined( MHAL_I2C_NON_BLOCKING_Q_I2C1 )
		i2c_buff = &i2c1_buff ;
#endif
		break;
	case (uint32_t)I2C2 :
#if defined( MHAL_I2C_NON_BLOCKING_Q_I2C2 )
	i2c_buff = &i2c2_buff ;
#endif
	break;
	default : break ;

	}


#endif

#ifdef MHAL_I2C_BLOCKING

	/* Generate a Start Condition to Enter into  Master Mode */
	if( tparams& MHAL_I2C_PARAM_START )
	mHal_I2C_Start_Gen(i2c);

	/* Send Address ( 10 or 8 - bit Modes */

	if( tparams& MHAL_I2C_PARAM_ADDR ) {

	if ( addr&0x0380 ){ // 10 bit address mode

		i2c->DR = ( addr&0X0300 >> 7UL )|MHAL_I2C_10B_HEADER ;

		while(!(i2c->SR1 & I2C_SR1_ADD10_Msk ));

		i2c->DR = ( addr&0x00FF );
		while(!(i2c->SR1 & I2C_SR1_ADDR_Msk ));
		if( i2c->SR2 ){}

		if( i2c_mm == MHAL_I2C_MASTER_RECEIVER  ){
			mHal_I2C_Start_Gen(i2c);
			i2c->DR = ( addr&0X0300 >> 7UL )|MHAL_I2C_10B_HEADER ;
			while(!(i2c->SR1 & I2C_SR1_ADDR_Msk ));
			if( i2c->SR2 ){}


	}
}
	else {

		i2c->DR = (uint8_t) ( ((addr&0X7F)  << 1UL ) |  i2c_mm ) ;

		while( !(i2c->SR1 & I2C_SR1_ADDR_Msk) );
		if( i2c->SR2 ){}


	}

	}


	if ( i2c_mm == MHAL_I2C_MASTER_RECEIVER ){

		if ( len > 1UL )
			mHal_I2C_ACK(i2c);
		else
			mHal_I2C_NACK(i2c);

	}

#endif

	/* Data is Sent */

#ifdef MHAL_I2C_BLOCKING
	for ( ; len-- ;  )
	{
			if ( i2c_mm == MHAL_I2C_MASTER_TRANSMITTER )
			{
				while(!( i2c->SR1 & I2C_SR1_TXE_Msk));
				i2c->DR = *(data++);
			}
			else
			{
				while(!( i2c->SR1 & I2C_SR1_RXNE_Msk));
				*(data++) = i2c->DR ;
				if( len == 1UL )
					mHal_I2C_NACK(i2c);
			}

	}

	if( tparams&MHAL_I2C_PARAM_STOP ){
		if ( i2c_mm == MHAL_I2C_MASTER_TRANSMITTER )
			while( !(i2c->SR1 & I2C_SR1_BTF_Msk) );

		mHal_I2C_Stop_Gen(i2c);
	}


	return 1 ;

#endif


}



int32_t mHal_I2C_Fill ( mHal_I2C_Config_Struct* i2c_s ,  I2C_TypeDef* i2c )
{

	int32_t ret = NO_ERR ;
	int32_t temp = 0UL ;


#ifdef CHECK_PARAMS

	if ( ( i2c_s->tr > 63UL ) || ( i2c_s->fm_duty > 1UL ) || ( i2c_s->tp_freq < 2UL ) ||
		 ( i2c_s->tp_freq > 50UL ) || ( i2c_s->analog_f_en > 1UL) || ( i2c_s->bus_mode > 1UL )||
		 ( i2c_s->arp_en > 1UL ) || ( i2c_s->digita_f > 15UL ) || ( i2c_s->buff_int_en > 1UL )||
		 ( i2c_s->dma_rq_en > 1UL ) || ( i2c_s->dual_addren > 1UL ) || ( i2c_s->err_int_en > 1UL )||
		 ( i2c_s->event_int_en > 1UL )||( i2c_s->gcall_en > 1UL ) || ( i2c_s->mode > 1UL )||
		 ( i2c_s->pec_en > 1UL ) || ( i2c_s->smbus_type > 1UL ) || ( i2c_s->ccr > 4095UL ) ||
		 ( i2c_s->ccr < 4UL ) )
		ret = PARAM_ASSERT_ERR ;

#endif

	if ( i2c->CR1 & I2C_CR1_PE_Msk )
		i2c->CR1 &= ~( I2C_CR1_PE_Msk );

	temp = i2c->FLTR ;
	temp &= ~( I2C_FLTR_ANOFF_Msk | I2C_FLTR_DNF_Msk );
	temp |= (( INVERT_SWITCH(i2c_s->analog_f_en) << I2C_FLTR_ANOFF_Pos )|
				 ( i2c_s->digita_f << I2C_FLTR_DNF_Pos )) ;

	i2c->FLTR = temp ;

	temp = i2c->TRISE ;
	temp &= ~( I2C_TRISE_TRISE_Msk );
	temp |= (( i2c_s->tr & I2C_TRISE_TRISE_Msk ) << I2C_TRISE_TRISE_Pos ) ;

	i2c->TRISE =  temp ;


	temp = i2c->CCR ;
	temp &= ~( I2C_CCR_DUTY_Msk | I2C_CCR_FS_Msk | I2C_CCR_CCR_Msk );
	temp |= ( ( i2c_s->fm_duty << I2C_CCR_DUTY_Pos ) | ( i2c_s->ccr << I2C_CCR_CCR_Pos ) | ( i2c_s->mode << I2C_CCR_FS_Pos ) ) ;

	i2c->CCR = temp ;

	temp = i2c->OAR2 ;
	temp &= ~( I2C_OAR2_ADD2_Msk | I2C_OAR2_ENDUAL_Msk );
	temp |= ( ( i2c_s->oaddr2 << I2C_OAR2_ADD2_Pos ) | ( i2c_s->dual_addren << I2C_OAR2_ENDUAL_Pos ) ) ;

	i2c->OAR2 = temp ;


	temp = i2c->OAR1 ;
	temp &= ~( 0X3FF | I2C_OAR1_ADDMODE_Msk );
	if ( i2c_s->dual_addren == Disabled )
		temp |= (( ( i2c_s->oaddr1 & 0x7F ) << I2C_OAR1_ADD1_Pos )|( i2c_s->dual_addren << I2C_OAR1_ADDMODE_Pos ) );
	else
		temp |= (( ( i2c_s->oaddr1 & 0x3FF ) << I2C_OAR1_ADD0_Pos )|( i2c_s->dual_addren << I2C_OAR1_ADDMODE_Pos ) );

	i2c->OAR1 = temp ;


	temp = i2c->CR2 ;
	temp &= ~( I2C_CR2_FREQ_Msk | I2C_CR2_ITERREN_Msk | I2C_CR2_ITEVTEN_Msk | I2C_CR2_ITBUFEN_Msk | I2C_CR2_DMAEN_Msk );
	temp |= ( ( i2c_s->dma_rq_en << I2C_CR2_DMAEN_Pos ) | ( i2c_s->buff_int_en << I2C_CR2_ITBUFEN_Pos )|
			  ( i2c_s->event_int_en << I2C_CR2_ITEVTEN_Pos ) | ( i2c_s->err_int_en << I2C_CR2_ITERREN_Pos ) |
			  ( (i2c_s->tp_freq&0x3F) <<  I2C_CR2_FREQ_Pos )) ;

	i2c->CR2 = temp ;


	temp = i2c->CR1 ;
	temp &= ~( I2C_CR1_SMBUS_Msk | I2C_CR1_SMBTYPE_Msk | I2C_CR1_ENARP_Msk | I2C_CR1_ENGC_Msk | I2C_CR1_ENPEC_Msk | I2C_CR1_NOSTRETCH_Msk );
	temp |= ( ( i2c_s->smbus_type <<  I2C_CR1_SMBTYPE_Pos ) | ( i2c_s->bus_mode << I2C_CR1_SMBUS_Pos )|( i2c_s->arp_en <<  I2C_CR1_ENARP_Pos )|
			  ( i2c_s->gcall_en << I2C_CR1_ENGC_Pos ) | ( i2c_s->pec_en << I2C_CR1_ENPEC_Pos ) | ( INVERT_SWITCH(i2c_s->clk_stretch_en) << I2C_CR1_NOSTRETCH_Pos )|
			  ( 1UL << I2C_CR1_PE_Pos ));

	i2c->CR1 = temp ;



	return ret  ;



}
