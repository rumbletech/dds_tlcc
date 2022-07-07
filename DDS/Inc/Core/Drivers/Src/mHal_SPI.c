/*
 * mHal_SPI.c
 *
 *  Created on: Jun 17, 2022
 *      Author: Garmoosh
 */

#include "../Inc/mHal_SPI.h"



void __attribute__((weak)) mHal_CSEN_CallBack ( void )
{

}


void __attribute__((weak)) mHal_CSNE_CallBack ( void )
{

}



int32_t  mHal_SPI_NBWrite ( uint16_t* data_ptr  , SPI_TypeDef * spi )
{
	int ret = 0UL ;
	if ( (mHal_SPI_ISGet(spi)&MHAL_SPI_INT_TXBUFF_E) ){
			mHal_CSEN_CallBack();
			spi->DR = *(data_ptr) ;

	}
	else{
		ret = BUSY_ERR ;
	}

	return ret ;

}





int32_t mHal_SPI_Write( void * tx_dptr  , void* rx_dptr , int32_t len  , SPI_TypeDef * spi )
{

	int32_t ret = 0UL ;
	int32_t db = spi->CR1 & SPI_CR1_DFF_Msk ? 2UL : 1UL ;
	int32_t temp = 0UL ;

	if ( len < db )
		db = 1UL ;

	while ( mHal_SPI_ISGet(spi) & MHAL_SPI_INT_BUSY );
	if ( mHal_SPI_ISGet(spi) & MHAL_SPI_INT_RXBUFF_NE )
		temp = spi->DR ;



	for ( ; len ; len-=db )
	{

		while ( !(mHal_SPI_ISGet(spi) & MHAL_SPI_INT_TXBUFF_E ));

		ret = mHal_SPI_NBWrite( (uint16_t*)tx_dptr , spi );
		while(!(mHal_SPI_ISGet(spi) & MHAL_SPI_INT_RXBUFF_NE));
		mHal_CSNE_CallBack();

		if ( db == 2UL  )
			*( ((uint16_t*)rx_dptr)) =  spi->DR;
		else
			*( ((uint8_t*)rx_dptr)) =  spi->DR & 0x00FF ;

		rx_dptr = (uint8_t*)(rx_dptr) + db ;
		tx_dptr = (uint8_t*)(tx_dptr) + db ;

	}


	return (ret|( temp << 16UL))  ;
}

int32_t mHal_SPI_Read( void* rptr , int32_t len , SPI_TypeDef * spi ){

	int32_t ret  = 0UL ;
	int32_t db = spi->CR1 & SPI_CR1_DFF_Msk ? 2UL : 1UL ;

	if ( len < db )
		db = len ;

	for ( ; len ; len-=db )
	{
		while( !( mHal_SPI_ISGet(spi)&MHAL_SPI_INT_RXBUFF_NE ) );

		if ( db == 2UL  )
			*( ((uint16_t*)rptr)) =  spi->DR;
		else
			*( ((uint8_t*)rptr)) =  spi->DR & 0x00FF ;

		rptr = (uint8_t*)(rptr) + db ;

	}


	return ret ;


}


int32_t mHal_SPI_NBRead ( uint16_t * rptr , SPI_TypeDef * spi )
{
	int32_t ret = 0UL ;

	if ( !( mHal_SPI_ISGet(spi)&MHAL_SPI_INT_RXBUFF_NE ) )
		return ret = EMPTY_BUFF_ERR ;

	*(rptr) = spi->DR ;

	return ret ;



}





void  mHal_SPI_Enable(  SPI_TypeDef * spi )
{
	spi->CR1 |= ( 1UL << SPI_CR1_SPE_Pos );
}

void mHal_SPI_Disable(  SPI_TypeDef * spi )
{
	spi->CR1 &= ~( SPI_CR1_SPE_Msk );
}

int32_t mHal_SPI_GetTXCRC ( SPI_TypeDef * spi )
{
	return ( spi->TXCRCR ) ;
}


int32_t mHal_SPI_GetRXCRC ( SPI_TypeDef * spi )
{
	return ( spi->RXCRCR ) ;
}



int32_t mHal_SPI_Fill ( mHal_SPI_Config_Struct* spi_s , SPI_TypeDef * spi )
{

	int32_t ret = NO_ERR ;
#ifdef CHECK_PARAMS
	if ( ( spi_s->spi_mode > 1UL ) || ( spi_s->ck_phase > 1UL ) || ( spi_s->ck_pol > 1UL ) ||
		 ( spi_s->spi_ckdiv > 7UL ) || ( spi_s->spi_f_format > 1UL ) || ( spi_s->spi_f_data > 1UL ) ||
		 ( spi_s->spi_f_mode > 1UL ) || ( spi_s->soft_cs_en > 1UL ) || ( spi_s->hw_crcen > 1UL ) ||
		 ( spi_s->bi_en > 1UL ) || ( spi_s->bi_oe > 1UL ) || ( spi_s->tx_dma_en > 1UL ) ||
		 ( spi_s->rx_dma_en > 1UL ) || ( spi_s->tx_buff_e > 1UL ) || ( spi_s->rx_buff_ne > 1UL ) ||
		 ( spi_s->err > 1UL ) )
		ret = PARAM_ASSERT_ERR





#endif

		if ( spi->CR1 & SPI_CR1_SPE_Msk )
			spi->CR1 &= ~( SPI_CR1_SPE_Msk );

	/* CR2 */
	uint16_t temp ;
	temp = ( ( spi_s->tx_buff_e << SPI_CR2_TXEIE_Pos ) | ( spi_s->rx_buff_ne << SPI_CR2_RXNEIE_Pos ) |
			 ( spi_s->err << SPI_CR2_ERRIE_Pos ) | ( spi_s->spi_f_mode << SPI_CR2_FRF_Pos ) |
			 ( spi_s->sso_en << SPI_CR2_SSOE_Pos ) | ( spi_s->tx_dma_en << SPI_CR2_TXDMAEN_Pos ) |
			 ( spi_s->rx_dma_en << SPI_CR2_RXDMAEN_Pos ) ) ;

	spi->CR2 = temp ;

	/* CRC Polynomial */
	spi->CRCPR = spi_s->crc_poly ;

	/* CR1 */

	temp = ( ( spi_s->bi_en << SPI_CR1_BIDIMODE_Pos ) | ( spi_s->bi_oe << SPI_CR1_BIDIOE_Pos ) |
			 ( spi_s->hw_crcen << SPI_CR1_CRCEN_Pos ) | ( spi_s->spi_f_data << SPI_CR1_DFF_Pos ) |
			 ( spi_s->spi_rxonly << SPI_CR1_RXONLY_Pos ) | ( spi_s->soft_cs_en << SPI_CR1_SSM_Pos ) |
			 ( spi_s->spi_f_format << SPI_CR1_LSBFIRST_Pos ) | ( spi_s->spi_ckdiv << SPI_CR1_BR_Pos ) |
			 ( spi_s->spi_mode << SPI_CR1_MSTR_Pos ) | ( spi_s->ck_phase << SPI_CR1_CPHA_Pos ) |
			 ( spi_s->ck_pol << SPI_CR1_CPOL_Pos ) | ( spi_s->soft_cs_v << SPI_CR1_SSI_Pos ) |
			 ( 1UL << SPI_CR1_SPE_Pos ) );

	spi->CR1 = temp ;






	return ret ;

}
