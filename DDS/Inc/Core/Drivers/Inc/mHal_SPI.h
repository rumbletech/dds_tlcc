/*
 * mHal_SPI.h
 *
 *  Created on: Jun 17, 2022
 *      Author: Garmoosh
 */

#ifndef CORE_DRIVERS_INC_MHAL_SPI_H_
#define CORE_DRIVERS_INC_MHAL_SPI_H_



#include "mHal.h"


#define MHAL_SPI_CLK_POL_ENUMX 1UL
#define MHAL_SPI_CLK_POL_ENUMM 0UL

#define MHAL_SPI_CLK_PHA_ENUMX 1UL
#define MHAL_SPI_CLK_PHA_ENUMM 0UL

#define MHAL_SPI_MODE_ENUMX 1UL
#define MHAL_SPI_MODE_ENUMM 0UL

#define MHAL_SPI_PRE_ENUMX 7UL
#define MHAL_SPI_PRE_ENUMM 0UL

#define MHAL_SPI_FRAME_ENUMX 1UL
#define MHAL_SPI_FRAME_ENUMM 0UL

#define MHAL_SPI_DATAF_ENUMX 1UL
#define MHAL_SPI_DATAF_ENUMM 0UL

#define MHAL_SPI_FRAME_MODE_ENUMX 1UL
#define MHAL_SPI_FRAME_MODE_ENUMM 0UL


typedef enum {

	MHAL_SPI_INT_TXBUFF_E    = SPI_SR_TXE_Msk ,
	MHAL_SPI_INT_RXBUFF_NE   = SPI_SR_RXNE_Msk ,
	MHAL_SPI_INT_CRCRERR     = SPI_SR_CRCERR_Msk ,
	MHAL_SPI_INT_MODEFAULT   = SPI_SR_MODF_Msk ,
	MHAL_SPI_INT_OVERRUN     = SPI_SR_OVR_Msk ,
	MHAL_SPI_INT_BUSY        = SPI_SR_BSY_Msk ,
	MHAL_SPI_INT_FRAME_ERROR = SPI_SR_FRE_Msk ,

} mHal_SPI_INT ;
typedef enum {

	MHAL_SPI_CLK_POL_LOW  = 0B0 , /* CLOCK IS LOW WHEN IDLE */
	MHAL_SPI_CLK_POL_HIGH = 0B1 , /* CLOCK IS HIGH WHEN IDLE */



} mHal_SPI_CLK_POL ;

typedef enum {

	MHAL_SPI_CLK_PHA_FE = 0B0 , /* Sampled at First edge of the clock */
	MHAL_SPI_CLK_PHA_SE = 0B1 , /* Sampled at Second edge of the clock */



} mHal_SPI_CLK_PHA ;

typedef enum {

	MHAL_SPI_MODE_MSTR  = 0B1 , /* SPI MASTER */
	MHAL_SPI_MODE_SLAVE = 0B0 , /* SPI SLAVE */



} mHal_SPI_MODE ;


typedef enum {

	MHAL_SPI_DIV2   = 0B000 ,      /* SPI_CLK = SERIAL_PERIPHERAL_CLK / 2 */
	MHAL_SPI_DIV4   = 0B001 ,      /* SPI_CLK = SERIAL_PERIPHERAL_CLK / 4 */
	MHAL_SPI_DIV8   = 0B010 ,      /* SPI_CLK = SERIAL_PERIPHERAL_CLK / 8 */
	MHAL_SPI_DIV16  = 0B011 ,      /* SPI_CLK = SERIAL_PERIPHERAL_CLK / 16 */
	MHAL_SPI_DIV32  = 0B100 ,      /* SPI_CLK = SERIAL_PERIPHERAL_CLK / 32 */
	MHAL_SPI_DIV64  = 0B101 ,      /* SPI_CLK = SERIAL_PERIPHERAL_CLK / 64 */
	MHAL_SPI_DIV128 = 0B110 ,      /* SPI_CLK = SERIAL_PERIPHERAL_CLK / 128 */
	MHAL_SPI_DIV256 = 0B111 ,      /* SPI_CLK = SERIAL_PERIPHERAL_CLK / 256 */



} mHal_SPI_Pre ;

typedef enum {

	MHAL_SPI_FRAME_MSB = 0B0 , /* MSB Transmitted First */
	MHAL_SPI_FRAME_LSB = 0B1 , /* LSB Transmitted First */



} mHal_SPI_FRAME ;

typedef enum {

	MHAL_SPI_DATA_8BF  = 0B0 , /* 8 BIT DATA FRAME */
	MHAL_SPI_DATA_16BF = 0B1 , /* 16 BIT DATA FRAME */



} mHal_SPI_DATAF ;

typedef enum {

	MHAL_SPI_FRAME_MODE_MOTO = 0B0 , /* MOTOROLLA FRAME MODE  */
	MHAL_SPI_FRAME_MODE_TI   = 0B1 , /* TEXAS INSTRUMENTS FRAME MODE */


} mHal_SPI_FRAME_MODE ;



typedef struct {



	mHal_SPI_MODE   spi_mode ;           /* Master or Slave Mode */
	mHal_SPI_CLK_PHA ck_phase ;          /* Phase of clock */
	mHal_SPI_CLK_POL ck_pol ;            /* Polarity of clock */
	mHal_SPI_Pre   spi_ckdiv ;           /* SPI Prescaler */
	mHal_SPI_FRAME spi_f_format ;        /* frame format LSB OR MSB First */
	mHal_SPI_DATAF spi_f_data ;          /* Frame Data 8BIT OR 16 BITS */
	mHal_SPI_FRAME_MODE spi_f_mode ;     /* Frame mode motorolla or TI */
	flip_switch soft_cs_v ;              /* Software CS Initial Value */
	flip_switch soft_cs_en ;             /* Software CS Management */
	flip_switch spi_rxonly ;
	flip_switch hw_crcen ;               /* Hardware CRC */
	flip_switch bi_en ;                  /* BI Directional data mode enable */
	flip_switch bi_oe ;                  /* BI Directional Output enable */
	flip_switch sso_en ;
	uint16_t crc_poly ;                  /* CRC Polynomial */


	/* DMA */

	flip_switch tx_dma_en ;       /* TX BUFFER DMA EN */
	flip_switch rx_dma_en ;       /* RX BUFFER DMA EN */

	/* Interrupt Masks */
	flip_switch tx_buff_e ;       /* TX BUFFER EMPTY IE */
	flip_switch rx_buff_ne ;      /* RX BUFFER NOT EMPTY IE */
	flip_switch err ;             /* ERROR(CRC,OVF) IE */



} mHal_SPI_Config_Struct ;


int32_t mHal_SPI_Fill ( mHal_SPI_Config_Struct* spi_s , SPI_TypeDef * spi );
int32_t mHal_SPI_GetRXCRC ( SPI_TypeDef * spi );
int32_t mHal_SPI_GetTXCRC ( SPI_TypeDef * spi );
void  mHal_SPI_Disable(  SPI_TypeDef * spi );
void  mHal_SPI_Enable(  SPI_TypeDef * spi );
int32_t mHal_SPI_Write( void * tx_dptr  , void* rx_dptr , int32_t len  , SPI_TypeDef * spi );
int32_t mHal_SPI_Read( void* rptr , int32_t len , SPI_TypeDef * spi );
int32_t  mHal_SPI_NBWrite ( uint16_t* data_ptr  , SPI_TypeDef * spi );
int32_t mHal_SPI_NBRead ( uint16_t * rptr , SPI_TypeDef * spi );


static inline int32_t __attribute__((always_inline)) mHal_SPI_ISGet (  SPI_TypeDef * spi  )
{
	return spi->SR ;

}

static inline void __attribute__((always_inline)) mHal_SPI_Set_CS ( SPI_TypeDef * spi )
{
	spi->CR1 |= ( 1UL << SPI_CR1_SSI_Pos );
}

static inline void __attribute__((always_inline)) mHal_SPI_Clear_CS ( SPI_TypeDef * spi )
{
	spi->CR1 &= ~( SPI_CR1_SSI_Msk );
}


#endif /* CORE_DRIVERS_INC_MHAL_SPI_H_ */
