/*
 * mHal_UART.h
 *
 *  Created on: Jul 9, 2022
 *      Author: Garmoosh
 */

#ifndef CORE_DRIVERS_INC_MHAL_USART_H_
#define CORE_DRIVERS_INC_MHAL_USART_H_

#include "mHal.h"








static inline uint32_t mHal_USART_GetSR ( USART_TypeDef* usart )
{
	return usart->SR ;
}

static inline void mHal_USART_Send_Break ( USART_TypeDef* usart )
{
	while(usart->CR1 & USART_CR1_SBK_Msk);
	usart->CR1 |= USART_CR1_SBK_Msk ;
}



typedef enum {

	MHAL_USART_OVERSAMPLE_B16 = 0B0 ,
	MHAL_USART_OVERSAMPLE_B8  = 0B1 ,

} mHal_USART_OVSample ;


typedef enum {

	MHAL_USART_WORDLEN_1S8DNS = 0B0 ,
	MHAL_USART_WORDLEN_1S9DNS = 0B1 ,


} mHal_USART_Word_Len ;

typedef enum {

	MHAL_USART_WKUP_IDLE_LINE   = 0B0 ,
	MHAL_USART_WKUP_ADDESS_MARK = 0B1 ,


} mHal_USART_WKUP_Method ;


typedef enum {

	MHAL_USART_PARITY_EVEN = 0B0 ,
	MHAL_USART_PARITY_ODD  = 0B1 ,

} mHal_USART_Parity_S;



typedef enum {

	MHAL_USRAT_STOPB_1SB    = 0B00 ,
	MHAL_USART_STOPB_0P5SB  = 0B01 ,
	MHAL_USART_STOPB_2SB    = 0B10 ,
	MHAL_USART_STOPB_1P5SB  = 0B11 ,



} mHal_USART_STOPB_S;


typedef enum {

	MHAL_USART_CLOCK_PH_FTR  = 0B0 ,               /* Second Clock Transition based on chosen Clock Polarity */
	MHAL_USART_CLOCK_POL_STR = 0B1 ,               /* Second Clock Transition based on chosen Clock Polarity */


} mHal_USART_Clock_Phase ;


typedef enum {

	MHAL_USART_CLOCK_POL_LOW  = 0B0 ,          /* Low CLK Value outside of transmission */
	MHAL_USART_CLOCK_POL_HIGH = 0B1 ,          /* High CLK Value outside of transmission */

} mHal_USART_Clock_Polarity ;



typedef enum {

	MHAL_USART_LINBREAK_DETLEN_10BIT = 0B0 ,
	MHAL_USART_LINBREAK_DETLEN_11BIT = 0B1 ,


} mHal_USART_LinBreak_DetectLen ;


typedef enum {

	MHAL_USART_SMC_DIV2   = 0B00001 ,
	MHAL_USART_SMC_DIV4   = 0B00010 ,
	MHAL_USART_SMC_DIV6   = 0B00011 ,
	MHAL_USART_SMC_DIV8   = 0B00100 ,
	MHAL_USART_SMC_DIV10   = 0B00101 ,
	MHAL_USART_SMC_DIV12   = 0B00110 ,
	MHAL_USART_SMC_DIV14   = 0B00111 ,
	MHAL_USART_SMC_DIV16   = 0B01000 ,
	MHAL_USART_SMC_DIV18   = 0B01001 ,
	MHAL_USART_SMC_DIV20  = 0B01010 ,
	MHAL_USART_SMC_DIV22  = 0B01011 ,
	MHAL_USART_SMC_DIV24  = 0B01100 ,
	MHAL_USART_SMC_DIV26  = 0B01101 ,
	MHAL_USART_SMC_DIV28  = 0B01110 ,
	MHAL_USART_SMC_DIV30  = 0B01111 ,
	MHAL_USART_SMC_DIV32  = 0B10000 ,
	MHAL_USART_SMC_DIV34  = 0B10001 ,
	MHAL_USART_SMC_DIV36  = 0B10010 ,
	MHAL_USART_SMC_DIV38  = 0B10011 ,
	MHAL_USART_SMC_DIV40  = 0B10100 ,
	MHAL_USART_SMC_DIV42  = 0B10101 ,
	MHAL_USART_SMC_DIV44  = 0B10110 ,
	MHAL_USART_SMC_DIV46  = 0B10111 ,
	MHAL_USART_SMC_DIV48  = 0B11000 ,
	MHAL_USART_SMC_DIV50  = 0B11001 ,
	MHAL_USART_SMC_DIV52  = 0B11010 ,
	MHAL_USART_SMC_DIV54  = 0B11011 ,
	MHAL_USART_SMC_DIV56  = 0B11100 ,
	MHAL_USART_SMC_DIV58  = 0B11101 ,
	MHAL_USART_SMC_DIV60  = 0B11110 ,
	MHAL_USART_SMC_DIV62  = 0B11111 ,


} mHal_USART_SMC_Pre ;

typedef struct {

	flip_switch clk_en ;
	mHal_USART_Clock_Polarity clk_pol ;
	mHal_USART_Clock_Phase clk_ph ;
	flip_switch last_clk_pulse_en ;




} mHal_USART_Clock_Config_Struct ;


typedef struct {

	flip_switch lin_en ;
	flip_switch lin_brk_ie ;
	mHal_USART_LinBreak_DetectLen lin_brk_dtlen;



} mHal_USART_LIN_Config_Struct ;

typedef struct {

	flip_switch smart_card_en ;
	flip_switch smart_card_nack_en ;
	uint8_t smart_card_gt_v ;
	mHal_USART_SMC_Pre smc_pre ;


} mHal_USART_SMC_Config_Struct ;


typedef struct {

	flip_switch irdra_en ;
	flip_switch irda_low_power_en ;
	uint8_t irda_pre ;

} mHal_USART_IRDA_Config_Struct ;

typedef struct {

	mHal_USART_OVSample    ovsample ;
	mHal_USART_Word_Len    wordlen ;
	mHal_USART_WKUP_Method wkupm ;
	flip_switch parity_en ;
	mHal_USART_Parity_S parity_s ;
	flip_switch hw_parity_control_en ;
	flip_switch half_duplex_en ;
	mHal_USART_STOPB_S stopb_s ;
	uint8_t usart_addr ;
	flip_switch one_sample_method_en ;
	uint16_t baud_div ;

	/* Flow Control */
	flip_switch cts_en ;
	flip_switch rts_en ;
	flip_switch dma_tx_en ;
	flip_switch dma_rx_en ;

	/* Interrupt Switches */

	flip_switch parity_ie ;
	flip_switch tx_ie ;
	flip_switch tc_ie ;
	flip_switch rxn_ie ;
	flip_switch idle_ie ;
	flip_switch cts_ie ;
	flip_switch err_ie ;

	/* TX , RX EN */

	flip_switch tx_en ;
	flip_switch rx_en ;
	flip_switch rx_mute_en ;

	/* SYNCRHONOUS CLK , SMARTCARD , IRDA , AND LIN */

	mHal_USART_LIN_Config_Struct * lin_cfg_s ;
	mHal_USART_Clock_Config_Struct* clk_cfg_s ;
	mHal_USART_SMC_Config_Struct * smc_cfg_s ;
	mHal_USART_IRDA_Config_Struct* irda_cfg_s ;



} mHal_USART_Config_Struct ;








extern mHal_USART_Config_Struct mHal_USART_FDUPLEX_UART_Struct ;


uint32_t mHal_USART_Fill ( mHal_USART_Config_Struct* usart_s ,  USART_TypeDef* usart );
uint32_t mHal_USART_Send( uint8_t* dptr , int16_t len  ,USART_TypeDef* usart  );
uint32_t mHal_USART_Receive( uint8_t* dptr , int16_t len  ,USART_TypeDef* usart  );
uint16_t mHal_USART_BaudCalc( uint32_t p_freq ,  uint32_t baud , uint8_t ov8 );
int16_t  mHal_USART_NBReceive ( USART_TypeDef* usart );










#endif /* CORE_DRIVERS_INC_MHAL_USART_H_ */
