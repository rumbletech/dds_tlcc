/*
 * mHal_TIM.h
 *
 *  Created on: Jun 23, 2022
 *      Author: Garmoosh
 */

#ifndef CORE_DRIVERS_INC_MHAL_TIM_H_
#define CORE_DRIVERS_INC_MHAL_TIM_H_




#include "mHal.h"










/* Defines Sample Clocking( used for digital filters ) division ratio from Counter Clock */
typedef enum {

	MHAL_TIM_SCLK_DIV1 = 0UL ,  /* Tsample = Tclk   */
	MHAL_TIM_SCLK_DIV2 = 1UL ,  /* Tsample = 2*Tclk */
	MHAL_TIM_SCLK_DIV4 = 2UL ,  /* Tsample = 4*Tclk */

} mHal_TIM_SCLK_Pre ;



/* Defines Center aligned mode or edge aligned mode */
typedef enum {

	MHAL_TIM_COUNT_MODE_EA  = 0B00 ,  /* Edge Aligned ( up / down counter ) */
	MHAL_TIM_COUNT_MODE_CA1 = 0B01 ,  /* Center Aligned 1 mode , output compare triggers on down count */
	MHAL_TIM_COUNT_MODE_CA2 = 0B10 ,  /* Center Aligned 2 mode , output compare triggers on up count */
	MHAL_TIM_COUNT_MODE_CA3 = 0B11 ,  /* Center Aligned 3 mode , output compare triggers on both up and down */


} mHal_TIM_COUNT_Align ;


/* Defines Count Direction */
typedef enum {

	MHAL_TIM_COUNT_DIR_UP   = 0B00 ,  /* Edge Aligned ( up / down counter ) */
	MHAL_TIM_COUNT_DIR_DOWN = 0B01 ,  /* Center Aligned 1 mode , output compare triggers on down count */


} mHal_TIM_COUNT_Dir ;

typedef enum {


	MHAL_TIM_TI1_TIM1CH           = 0B0 ,
	MHAL_TIM_TI1_TIMCH1XORCH2CH3  = 0B1 ,

} mHal_TIM_TI1_Select ;


typedef enum {

	MHAl_TIM_MSTR_MODE_RESET    = 0B000 ,              /* TRGO on UG Set */
	MHAL_TIM_MSTR_MODE_ENABLE   = 0B001 ,              /* TRGO on TIMXEN */
	MHAL_TIM_MSTR_MODE_UPDATE   = 0B010 ,              /* TRGO ON UEV */
	MHAl_TIM_MSTR_MODE_CCI      = 0B011 ,              /* TRGO ON CCIF */
	MHAl_TIM_MSTR_MODE_OC1      = 0B100 ,              /* TRGO ON OC1 */
	MHAl_TIM_MSTR_MODE_OC2      = 0B101 ,			   /* TRGO ON OC2 */
	MHAl_TIM_MSTR_MODE_OC3      = 0B110 ,			   /* TRGO ON OC3 */
	MHAl_TIM_MSTR_MODE_OC4      = 0B111 ,			   /* TRGO ON OC4 */



} mHal_TIM_Mstr_Mode_Select ;


typedef enum {

	MHAL_TIM_CC_UPDATE_COMG      = 0B0 , /* When COMG Bit is set */
	MHAL_TIM_CC_UPDATE_COMG_TRGI = 0B1 , /* When COMG Bit is set or a Rising edge on TRGI */


} mHal_TIM_CC_Update_Select ;

typedef enum {

	MHAL_TIM_EXT_TRG_POL_NON_INVERT = 0B0 ,
	MHAL_TIM_EXT_TRG_POL_INVERT     = 0B1 ,

} mHal_TIM_EXT_TRG ;


typedef enum {

	MHAL_TIM_EXT_TRG_PRE_NODIV = 0B00 ,
	MHAL_TIM_EXT_TRG_PRE_DIV2  = 0B01 ,
	MHAL_TIM_EXT_TRG_PRE_DIV4  = 0B10 ,
	MHAL_TIM_EXT_TRG_PRE_DIV8  = 0B11 ,


} mHal_TIM_EXT_TRG_Pre ;


typedef enum {

	MHAL_TIM_EXT_TRG_FILTER_FDTS_NOF        = 0B0000 ,
	MHAL_TIM_EXT_TRG_FILTER_FCLK_FN2        = 0B0001 ,
	MHAL_TIM_EXT_TRG_FILTER_FCLK_FN4        = 0B0010 ,
	MHAL_TIM_EXT_TRG_FILTER_FCLK_FN8        = 0B0011 ,
	MHAL_TIM_EXT_TRG_FILTER_FDTSDIV2_FN6    = 0B0100 ,
	MHAL_TIM_EXT_TRG_FILTER_FDTSDIV2_FN8    = 0B0101 ,
	MHAL_TIM_EXT_TRG_FILTER_FDTSDIV4_FN6    = 0B0110 ,
	MHAL_TIM_EXT_TRG_FILTER_FDTSDIV4_FN8    = 0B0111 ,
	MHAL_TIM_EXT_TRG_FILTER_FDTSDIV8_FN6    = 0B1000 ,
	MHAL_TIM_EXT_TRG_FILTER_FDTSDIV8_FN8    = 0B1001 ,
	MHAL_TIM_EXT_TRG_FILTER_FDTSDIV16_FN5   = 0B1010 ,
	MHAL_TIM_EXT_TRG_FILTER_FDTSDIV16_FN6   = 0B1011 ,
	MHAL_TIM_EXT_TRG_FILTER_FDTSDIV16_FN8   = 0B1100 ,
	MHAL_TIM_EXT_TRG_FILTER_FDTSDIV32_FN5   = 0B1101 ,
	MHAL_TIM_EXT_TRG_FILTER_FDTSDIV32_FN6   = 0B1110 ,
	MHAL_TIM_EXT_TRG_FILTER_FDTSDIV32_FN8   = 0B1111 ,




} mHaL_TIM_EXT_TRG_FILTER ;



typedef enum {

	MHAL_TIM_TRIGGER_ITR0     = 0B000 ,
	MHAL_TIM_TRIGGER_ITR1     = 0B001 ,
	MHAL_TIM_TRIGGER_ITR2     = 0B010 ,
	MHAL_TIM_TRIGGER_ITR3     = 0B011 ,
	MHAL_TIM_TRIGGER_TI1F_ED  = 0B100 ,
	MHAL_TIM_TRIGGER_TI1F     = 0B101 ,
	MHAL_TIM_TRIGGER_TI2F     = 0B110 ,
	MHAL_TIM_TRIGGER_ETRF     = 0B111 ,


} mHal_TIM_Trigger_Select ;


typedef enum {

	MHAL_TIM_SLAVE_MODE_DISABLED  = 0B000 ,
	MHAL_TIM_SLAVE_MODE_ENCODER1  = 0B001 ,
	MHAL_TIM_SLAVE_MODE_ENCODER2  = 0B010 ,
	MHAL_TIM_SLAVE_MODE_ENCODER3  = 0B011 ,
	MHAL_TIM_SLAVE_MODE_RESET     = 0B100 ,
	MHAL_TIM_SLAVE_MODE_GATED     = 0B101 ,
	MHAL_TIM_SLAVE_MODE_TRIGGER   = 0B110 ,
	MHAL_TIM_SLAVE_MODE_EXTCLOCK1 = 0B111 ,


} mHal_TIM_SlaveMode_Select ;

typedef enum  {

	MHAL_TIM_OC_MODE_FROZEN                = 0B000 ,
	MHAL_TIM_OC_MODE_ACTIVE_ONMATCH        = 0B001 ,
	MHAL_TIM_OC_MODE_INACTIVE_ONMATCH      = 0B010 ,
	MHAL_TIM_OC_MODE_TOGGLE                = 0B011 ,
	MHAL_TIM_OC_MODE_FORCE_INACTIVE        = 0B100 ,
	MHAL_TIM_OC_MODE_FORCE_ACTIVE          = 0B101 ,
	MHAL_TIM_OC_MODE_PWM1                  = 0B110 ,
	MHAL_TIM_OC_MODE_PWM2                  = 0B111 ,


} mHal_TIM_OC_Mode ;


typedef  enum {

	MHAL_TIM_CC_OUTPUT = 0B00 ,
	MHAL_TIM_CC_TI1    = 0B01 ,
	MHAL_TIM_CC_TI2    = 0B10 ,
	MHAL_TIM_CC_TRC    = 0B11 ,


} mHal_TIM_CC_Select  ;


typedef enum {

	MHAL_TIM_IC_FILTER_FDTS_NOF        = 0B0000 ,
	MHAL_TIM_IC_FILTER_FCLK_FN2        = 0B0001 ,
	MHAL_TIM_IC_FILTER_FCLK_FN4        = 0B0010 ,
	MHAL_TIM_IC_FILTER_FCLK_FN8        = 0B0011 ,
	MHAL_TIM_IC_FILTER_FDTSDIV2_FN6    = 0B0100 ,
	MHAL_TIM_IC_FILTER_FDTSDIV2_FN8    = 0B0101 ,
	MHAL_TIM_IC_FILTER_FDTSDIV4_FN6    = 0B0110 ,
	MHAL_TIM_IC_FILTER_FDTSDIV4_FN8    = 0B0111 ,
	MHAL_TIM_IC_FILTER_FDTSDIV8_FN6    = 0B1000 ,
	MHAL_TIM_IC_FILTER_FDTSDIV8_FN8    = 0B1001 ,
	MHAL_TIM_IC_FILTER_FDTSDIV16_FN5   = 0B1010 ,
	MHAL_TIM_IC_FILTER_FDTSDIV16_FN6   = 0B1011 ,
	MHAL_TIM_IC_FILTER_FDTSDIV16_FN8   = 0B1100 ,
	MHAL_TIM_IC_FILTER_FDTSDIV32_FN5   = 0B1101 ,
	MHAL_TIM_IC_FILTER_FDTSDIV32_FN6   = 0B1110 ,
	MHAL_TIM_IC_FILTER_FDTSDIV32_FN8   = 0B1111 ,




} mHal_TIM_IC_FILTER ;


typedef enum {

	MHAL_TIM_IC_PRE_CAPTURE_ON_EDGE     = 0B00 ,
	MHAL_TIM_IC_PRE_CAPTURE_ONCE_IN_2EV = 0B01 ,
	MHAL_TIM_IC_PRE_CAPTURE_ONCE_IN_4EV = 0B10 ,
	MHAL_TIM_IC_PRE_CAPTURE_ONCE_IN_8EV = 0B11 ,


} mHal_TIM_IC_Pre ;



typedef struct {

	mHal_TIM_CC_Select cc_s ;
	/* Case CC_S Selects and OUTPUT CC */
	mHal_TIM_OC_Mode cc_om ;
	flip_switch cc_oc_preload_en ;
	flip_switch cc_oc_fast_en  ;
	flip_switch cc_oc_clear_en ;
	/* Case CC_S Selects Input Capture */
	mHal_TIM_IC_FILTER ic_filter ;
	mHal_TIM_IC_Pre ic_pre ;
	/* Output Enable / Polarity / and idle values */
	flip_switch out_en ;
	Edge_Pol out_pol ;
	flip_switch inverted_out_en     ;
	Edge_Pol inverted_out_pol     ;
	Bitv out_idle_val  ;
	Bitv inverted_out_idle_val ;

	flip_switch cc_dma_req_en ;
	flip_switch cc_ie ;

	uint16_t cc_r ;






} mHal_TIM_CC_Config_Struct ;


typedef enum {

	MHAL_TIM_LOCK_OFF   = 0B00 ,
	MHAL_TIM_LOCK_LVL1  = 0B01 ,
	MHAL_TIM_LOCK_LVL2  = 0B10 ,
	MHAL_TIM_LOCK_LVL3  = 0B11 ,


} mHal_TIM_LOCK ;


typedef struct {

	mHal_TIM_EXT_TRG etrg_pol ;				   /* ETRG Polarity */
	mHal_TIM_EXT_TRG_Pre etrg_pre ;			   /* ETRG Prescaler */
	mHaL_TIM_EXT_TRG_FILTER etrg_filter ;	   /* ETRG Filter */



} mHal_ETRG_Config_Struct ;





typedef struct {


mHal_TIM_SCLK_Pre s_clk_pre ;			   /* Sample Clock PreDivision */
flip_switch arr_preload_en ;               /* AutoReload Register Preload enable */
mHal_TIM_COUNT_Align count_align ;         /* Alignment */
mHal_TIM_COUNT_Dir   count_dir ;		   /* Specifies the counter direction , it is readonly in center aligned mode as it alternates */
flip_switch onepulse_en ;                  /* If enabled the counter stops at the next update event */
flip_switch uev_src_en  ;                  /* if Disabled UEV is generated on counter over flow or underflow only */
flip_switch uev_gen_en  ;                  /* UEV generation on events */


mHal_TIM_TI1_Select ti1_select ;           /* TI1 Selection */
mHal_TIM_Mstr_Mode_Select mms  ;           /* Master Mode Selection */
flip_switch ext_clk_mod2_en ;			   /* External Clock mode 2 */

mHal_ETRG_Config_Struct* etrg_config ;     /* ETRG Configuration if not used then NULL */


flip_switch trgi_delay_en ;                /* Introduces a delay on the effect of TRGI until TRGO reaches slave timers , this is to synchronize the timers*/
mHal_TIM_Trigger_Select trgi_s  ;          /* Trigger Selection */
mHal_TIM_SlaveMode_Select slave_mode_s ;     /* Slave Mode Selection */

/* DMA REQ SWITCHES */

flip_switch trig_dma_req_en ;			   /* Trigger dma request enable */
flip_switch com_dma_req_en ;               /* COM dma request enable */
flip_switch update_dma_req_en ;            /* Update dma request enable */

/* INTERRUPT SWITCHES */

flip_switch break_ie ;                     /* Break Interrupt enable */
flip_switch trig_ie ;                      /* Trigger Interrupt enable */
flip_switch com_ie ;					   /* COM Interrupt enable */
flip_switch update_ie ;                    /* Update Interrupt enable */



mHal_TIM_CC_Config_Struct* cc1_config ;         /* CC1 Configuration */
mHal_TIM_CC_Config_Struct* cc2_config ;			/* CC2 Configuration */
mHal_TIM_CC_Config_Struct* cc3_config ;			/* CC3 Configuration */
mHal_TIM_CC_Config_Struct* cc4_config ;			/* CC4 Configuration */

flip_switch cc_dma_reqs_en     ;                /* DMA Request sending on Capture/Compare */
mHal_TIM_CC_Update_Select cc_update_select ;    /* CC Update on COMG or COMG And Rising Edge on TRGI*/
flip_switch cc_preload_en      ;                /* CC Bits preload enable */
flip_switch main_output_en ;                    /* Main Output Enable */
flip_switch auto_output_en ;                    /* Auto Output Enable , when SET MOE can be set by update events on break not active */
flip_switch o_off_idle_ss ; 					/* Off state in idle mode when OCE OCNE are not set */
flip_switch o_off_run_ss  ;						/* Off state in run mode when OCE OCNE are not set */


uint16_t cnt ;                                  /* Counter */
uint16_t arr ;                                  /* Auto Reload Register */
uint16_t psc ;                                  /* Prescaler */
uint8_t rcr  ;                                  /* Repitition count register */


Bitv  break_pol ; 								/* Break polarity */
flip_switch break_en ;                          /* Break Enable */
mHal_TIM_LOCK lock_bits ;                       /* Lock Value */


uint8_t dead_time ;                            /* Dead Time Generator value */


} mHal_TIM_Config_Struct ;



int32_t mHal_TIM_Fill( mHal_TIM_Config_Struct* tim_c , TIM_TypeDef * tim );
void mHal_TIM_Start( TIM_TypeDef* tim );
void mHal_TIM_Stop( TIM_TypeDef* tim );
void mHal_TIM_Update(TIM_TypeDef* tim );




#endif /* CORE_DRIVERS_INC_MHAL_TIM_H_ */
