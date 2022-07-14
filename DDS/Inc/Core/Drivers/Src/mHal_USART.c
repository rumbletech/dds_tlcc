/*
 * mHal_UART.c
 *
 *  Created on: Jul 9, 2022
 *      Author: Garmoosh
 */

#ifndef CORE_DRIVERS_SRC_MHAL_UART_C_
#define CORE_DRIVERS_SRC_MHAL_UART_C_




#include "../Inc/mHal_USART.h"
#include <math.h>

mHal_USART_Config_Struct mHal_USART_FDUPLEX_UART_Struct = {

	 .ovsample = MHAL_USART_OVERSAMPLE_B16 ,
	 .wordlen = MHAL_USART_WORDLEN_1S8DNS ,
	 .wkupm  = MHAL_USART_WKUP_IDLE_LINE ,
	 .parity_en  = Disabled ,
	 .parity_s = MHAL_USART_PARITY_EVEN,
	 .hw_parity_control_en = Disabled ,
	 .half_duplex_en = Disabled ,
	 .stopb_s = MHAL_USRAT_STOPB_1SB ,
	 .usart_addr = 0x0F ,
	 .one_sample_method_en = Disabled ,
	 .baud_div = 0U ,

	/* Flow Control */
	 .cts_en = Disabled ,
	 .rts_en = Disabled ,
	 .dma_tx_en = Disabled ,
	 .dma_rx_en  = Disabled ,

	/* Interrupt Switches */

	 .parity_ie = Disabled ,
	 .tx_ie = Disabled  ,
	 .tc_ie = Disabled  ,
	 .rxn_ie = Disabled ,
	 .idle_ie = Disabled ,
	 .cts_ie = Disabled ,
	 .err_ie = Disabled ,

	/* TX , RX EN */

	 .tx_en = Enabled ,
	 .rx_en = Enabled ,
	 .rx_mute_en = Disabled ,

	/* SYNCRHONOUS CLK , SMARTCARD , IRDA , AND LIN */

	 .lin_cfg_s = NULL ,
	 .clk_cfg_s = NULL ,
	 .smc_cfg_s = NULL ,
	 .irda_cfg_s = NULL

};

/* Blocking */
uint32_t mHal_USART_Send( uint8_t* dptr , int16_t len  ,USART_TypeDef* usart  )
{

	for ( ; len-- ; )
	{
		while( !(usart->SR & (USART_SR_TXE_Msk)) );
		usart->DR = *(dptr++);
	}

	return 0 ;

}



/* Blocking */
uint32_t mHal_USART_Receive( uint8_t* dptr , int16_t len  ,USART_TypeDef* usart  )
{

	for ( ; len-- ; )
	{
		while( !(usart->SR & (USART_SR_RXNE_Msk)) );
		*(dptr++) = usart->DR ;

	}

	return 0  ;

}


void mHal_USART_Clear( USART_TypeDef* usart )
{
	usart->SR = 0 ;
}

/* Non blocking */


int16_t  mHal_USART_NBReceive ( USART_TypeDef* usart )
{
	if ( usart->SR & USART_SR_RXNE_Msk )
		return usart->DR ;

	return -1 ;

}


uint16_t mHal_USART_BaudCalc( uint32_t p_freq ,  uint32_t baud , uint8_t ov8 )
{

	uint8_t m = ov8 ? 8u : 16u ;
	uint32_t mantissa = 0 ;
	uint32_t fraction = 0 ;


	float fdiv = (float)p_freq / ( m * baud ) ;

	mantissa = (uint32_t) fdiv ;
	fraction = round ( (fdiv-mantissa )*m);

	if ( fraction > m-1 )
		mantissa++;

	return ( (mantissa << 4UL )|fraction);

}






uint32_t mHal_USART_Fill ( mHal_USART_Config_Struct* usart_s ,  USART_TypeDef* usart )
{


	uint32_t ret = 0UL ;
#ifdef CHECK_PARAMS

	/* Check Params */



#endif

	uint32_t temp = 0UL  ;

	if ( usart->CR1 & USART_CR1_UE_Msk )
		usart->CR1 &= ~ ( USART_CR1_UE_Msk );

/* BRR **************************************************************************************
 * ******************************************************************************************
 */

	temp = usart->BRR ;
	temp &= 0xFFFF0000 ;

	temp |= ((usart_s->baud_div ));

	usart->BRR = temp ;
/* CR3 **************************************************************************************
 * ******************************************************************************************
 */

	temp = usart->CR3 ;


	temp &= ~( USART_CR3_EIE_Msk| USART_CR3_HDSEL_Msk| USART_CR3_ONEBIT_Msk| USART_CR3_CTSE_Msk|
			    USART_CR3_CTSIE_Msk| USART_CR3_RTSE_Msk| USART_CR3_DMAR_Msk| USART_CR3_DMAT_Msk|
				USART_CR3_IRLP_Msk| USART_CR3_IREN_Msk| USART_CR3_NACK_Msk|  USART_CR3_SCEN_Msk );


	temp |= (( usart_s->err_ie << USART_CR3_EIE_Pos )|
			( usart_s->half_duplex_en << USART_CR3_HDSEL_Pos )|
			( usart_s->one_sample_method_en << USART_CR3_ONEBIT_Pos )|
			( usart_s->cts_en << USART_CR3_CTSE_Pos )|
			( usart_s->cts_ie << USART_CR3_CTSIE_Pos )|
			( usart_s->rts_en << USART_CR3_RTSE_Pos )|
			( usart_s->dma_rx_en << USART_CR3_DMAR_Pos )|
			( usart_s->dma_tx_en << USART_CR3_DMAT_Pos ));


	if( usart_s->irda_cfg_s != NULL )
	{
		usart->GTPR &= 0XFFFFFF00 ;
		usart->GTPR = usart_s->irda_cfg_s->irda_pre ;
		temp|= (( usart_s->irda_cfg_s->irda_low_power_en << USART_CR3_IRLP_Pos )|
			   ( usart_s->irda_cfg_s->irdra_en << USART_CR3_IREN_Pos ));
	}

	if( usart_s->smc_cfg_s != NULL )
	{
		usart->GTPR &= 0xFFFF00E0 ;
		usart->GTPR |= (( usart_s->smc_cfg_s->smart_card_gt_v << USART_GTPR_GT_Pos )|
				       ( usart_s->smc_cfg_s->smc_pre << USART_GTPR_PSC_Pos ));

		temp|= (( usart_s->smc_cfg_s->smart_card_nack_en << USART_CR3_NACK_Pos )|
			   ( usart_s->smc_cfg_s->smart_card_en << USART_CR3_SCEN_Pos ) );

	}

	usart->CR3 = temp ;


/* CR2 **************************************************************************************
 * ******************************************************************************************
 */


	temp = usart->CR2 ;
	temp &= ~ ( USART_CR2_ADD_Msk | USART_CR2_STOP_Msk |  USART_CR2_CPHA_Msk | USART_CR2_CPOL_Msk |
			USART_CR2_LBCL_Msk |  USART_CR2_CLKEN_Msk | USART_CR2_LBDL_Msk | USART_CR2_LBDIE_Msk |
			USART_CR2_LINEN_Msk );

	temp |= (( usart_s->usart_addr << USART_CR2_ADD_Pos )|
			( usart_s->stopb_s << USART_CR2_STOP_Pos ));

	if ( usart_s->clk_cfg_s != NULL )
		temp |= (( usart_s->clk_cfg_s->clk_ph << USART_CR2_CPHA_Pos )|
				( usart_s->clk_cfg_s->clk_pol << USART_CR2_CPOL_Pos )|
				( usart_s->clk_cfg_s->last_clk_pulse_en << USART_CR2_LBCL_Pos )|
				( usart_s->clk_cfg_s->clk_en << USART_CR2_CLKEN_Pos ));


	if (usart_s->lin_cfg_s != NULL )
		temp|= (( usart_s->lin_cfg_s->lin_brk_dtlen << USART_CR2_LBDL_Pos )|
			   ( usart_s->lin_cfg_s->lin_brk_ie << USART_CR2_LBDIE_Pos )|
			   ( usart_s->lin_cfg_s->lin_en << USART_CR2_LINEN_Pos ));


	usart->CR2 = temp ;

/* CR1 **************************************************************************************
 * ******************************************************************************************
 */


	temp = usart->CR1 ;
	temp&= ~( USART_CR1_RWU_Msk | USART_CR1_RE_Msk | USART_CR1_TE_Msk |  USART_CR1_IDLEIE_Msk |
			USART_CR1_RXNEIE_Msk | USART_CR1_TCIE_Msk | USART_CR1_TXEIE_Msk | USART_CR1_PEIE_Msk |
			USART_CR1_PS_Msk | USART_CR1_PCE_Msk  | USART_CR1_WAKE_Msk  | USART_CR1_M_Msk |
			USART_CR1_OVER8_Msk | USART_CR1_UE_Msk );

	temp|= (( usart_s->rx_mute_en << USART_CR1_RWU_Pos  )|
		   ( usart_s->rx_en <<  USART_CR1_RE_Pos )|
		   ( usart_s->tx_en << USART_CR1_TE_Pos )|
		   ( usart_s->idle_ie << USART_CR1_IDLEIE_Pos )|
		   ( usart_s->rxn_ie << USART_CR1_RXNEIE_Pos )|
		   ( usart_s->tc_ie << USART_CR1_TCIE_Pos )|
		   ( usart_s->tx_ie << USART_CR1_TXEIE_Pos )|
		   ( usart_s->parity_ie << USART_CR1_PEIE_Pos )|
		   ( usart_s->parity_s << USART_CR1_PS_Pos )|
		   ( usart_s->hw_parity_control_en << USART_CR1_PCE_Pos )|
		   ( usart_s->wkupm << USART_CR1_WAKE_Pos )|
		   ( usart_s->wordlen << USART_CR1_M_Pos )|
		   ( usart_s->ovsample << USART_CR1_OVER8_Pos )|
		   ( 1UL << USART_CR1_UE_Pos ) );

	usart->CR1 = temp ;



	return ret ;



}




#endif /* CORE_DRIVERS_SRC_MHAL_UART_C_ */
