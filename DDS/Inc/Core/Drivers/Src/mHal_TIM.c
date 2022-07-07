/*
 * mHal_TIM.c
 *
 *  Created on: Jun 23, 2022
 *      Author: Garmoosh
 */



#include "../Inc/mHal_TIM.h"



void mHal_TIM_Update(TIM_TypeDef* tim )
{
	tim->EGR |= ( TIM_EGR_UG_Msk );
}
void mHal_TIM_Start( TIM_TypeDef* tim )
{
	tim->CR1 |= 1UL ;
}

void mHal_TIM_Stop( TIM_TypeDef* tim )
{
	tim->CR1 &= ~1UL ;
}



int32_t mHal_TIM_Fill( mHal_TIM_Config_Struct* tim_c , TIM_TypeDef * tim )
{

	uint32_t temp = 0UL ;
	uint32_t ret  = 0UL ;


#ifdef CHECK_PARAMS
	uint32_t file , line ;





#endif


	if ( tim->CR1  & TIM_CR1_CEN_Msk )
		tim->CR1 &= ~ ( TIM_CR1_CEN_Msk );



	uint32_t cc_cr2[4]   = { 0UL , 0UL , 0UL , 0UL } ;
	uint32_t cc_dier[4]  = { 0UL , 0UL , 0UL , 0UL } ;
	uint32_t cc_CCMR[4]  = { 0UL , 0UL , 0UL , 0UL } ;
	uint32_t cc_CCER[4]  = { 0UL , 0UL , 0UL , 0UL } ;

	if ( tim_c->cc1_config != NULL ){

		cc_cr2[0]  = ( tim_c->cc1_config->out_idle_val  << TIM_CR2_OIS1_Pos |
			          tim_c->cc1_config->inverted_out_idle_val << TIM_CR2_OIS1N_Pos );

		cc_dier[0] = ( tim_c->cc1_config->cc_dma_req_en  << TIM_DIER_CC1DE_Pos |
					  tim_c->cc1_config->cc_ie << TIM_DIER_CC1IE_Pos );

		if( tim_c->cc1_config->cc_s )
		{
			//INPUT MODE
			cc_CCMR[0] = ( tim_c->cc1_config->cc_s << TIM_CCMR1_CC1S_Pos |
						   tim_c->cc1_config->ic_pre << TIM_CCMR1_IC1PSC_Pos |
						   tim_c->cc1_config->ic_filter << TIM_CCMR1_IC1F_Pos );



		}
		else{
			// OUTPUT MODE
			cc_CCMR[0] = ( tim_c->cc1_config->cc_s << TIM_CCMR1_CC1S_Pos |
						   tim_c->cc1_config->cc_oc_clear_en << TIM_CCMR1_OC1CE_Pos |
						   tim_c->cc1_config->cc_oc_fast_en << TIM_CCMR1_OC1FE_Pos |
						   tim_c->cc1_config->cc_om << TIM_CCMR1_OC1M_Pos |
						   tim_c->cc1_config->cc_oc_preload_en << TIM_CCMR1_OC1PE_Pos );

			cc_CCER[0] = ( tim_c->cc1_config->inverted_out_en << TIM_CCER_CC1NE_Pos |
						   tim_c->cc1_config->out_en << TIM_CCER_CC1E_Pos |
						   tim_c->cc1_config->inverted_out_pol << TIM_CCER_CC1NP_Pos |
						   tim_c->cc1_config->out_pol << TIM_CCER_CC1P_Pos ) ;

			tim->CCR1 = tim_c->cc1_config->cc_r ;

		}

	}

	if ( tim_c->cc2_config != NULL ){
		cc_cr2[1]  = ( tim_c->cc2_config->out_idle_val  << TIM_CR2_OIS2_Pos |
			          tim_c->cc2_config->inverted_out_idle_val << TIM_CR2_OIS2N_Pos );

		cc_dier[1] = ( tim_c->cc2_config->cc_dma_req_en  << TIM_DIER_CC2DE_Pos |
					    tim_c->cc2_config->cc_ie << TIM_DIER_CC2IE_Pos );

		if( tim_c->cc2_config->cc_s  )
			{
				//INPUT MODE
				cc_CCMR[1] = ( tim_c->cc2_config->cc_s << TIM_CCMR1_CC2S_Pos |
							   tim_c->cc2_config->ic_pre << TIM_CCMR1_IC2PSC_Pos |
							   tim_c->cc2_config->ic_filter << TIM_CCMR1_IC2F_Pos );



			}
			else{
				// OUTPUT MODE
				cc_CCMR[1] = ( tim_c->cc2_config->cc_s << TIM_CCMR1_CC2S_Pos |
							   tim_c->cc2_config->cc_oc_clear_en << TIM_CCMR1_OC2CE_Pos |
							   tim_c->cc2_config->cc_oc_fast_en << TIM_CCMR1_OC2FE_Pos |
							   tim_c->cc2_config->cc_om << TIM_CCMR1_OC2M_Pos |
							   tim_c->cc2_config->cc_oc_preload_en << TIM_CCMR1_OC2PE_Pos );

				cc_CCER[1] = ( tim_c->cc2_config->inverted_out_en << TIM_CCER_CC2NE_Pos |
								   tim_c->cc2_config->out_en << TIM_CCER_CC2E_Pos |
								   tim_c->cc2_config->inverted_out_pol << TIM_CCER_CC2NP_Pos |
								   tim_c->cc2_config->out_pol << TIM_CCER_CC2P_Pos ) ;

				tim->CCR2 = tim_c->cc2_config->cc_r ;

			}

	}


	if ( tim_c->cc3_config != NULL ){
		cc_cr2[2]  = ( tim_c->cc3_config->out_idle_val  << TIM_CR2_OIS3_Pos |
			          tim_c->cc3_config->inverted_out_idle_val << TIM_CR2_OIS3N_Pos );

		cc_dier[2] = ( tim_c->cc3_config->cc_dma_req_en  << TIM_DIER_CC3DE_Pos |
					   tim_c->cc3_config->cc_ie << TIM_DIER_CC3IE_Pos );

		if( tim_c->cc3_config->cc_s  )
			{
				//INPUT MODE
				cc_CCMR[2] = ( tim_c->cc3_config->cc_s << TIM_CCMR2_CC3S_Pos |
							   tim_c->cc3_config->ic_pre << TIM_CCMR2_IC3PSC_Pos |
							   tim_c->cc3_config->ic_filter << TIM_CCMR2_IC3F_Pos );



			}
			else{
				// OUTPUT MODE
				cc_CCMR[2] = ( tim_c->cc3_config->cc_s << TIM_CCMR2_CC3S_Pos |
							   tim_c->cc3_config->cc_oc_clear_en << TIM_CCMR2_OC3CE_Pos |
							   tim_c->cc3_config->cc_oc_fast_en << TIM_CCMR2_OC3FE_Pos |
							   tim_c->cc3_config->cc_om << TIM_CCMR2_OC3M_Pos |
							   tim_c->cc3_config->cc_oc_preload_en << TIM_CCMR2_OC3PE_Pos );


				cc_CCER[2] = ( tim_c->cc3_config->inverted_out_en << TIM_CCER_CC3NE_Pos |
								   tim_c->cc3_config->out_en << TIM_CCER_CC3E_Pos |
								   tim_c->cc3_config->inverted_out_pol << TIM_CCER_CC3NP_Pos |
								   tim_c->cc3_config->out_pol << TIM_CCER_CC3P_Pos ) ;

				tim->CCR3 = tim_c->cc3_config->cc_r ;

			}

	}


	if ( tim_c->cc4_config != NULL ){

		cc_cr2[3]  = ( tim_c->cc4_config->out_idle_val  << TIM_CR2_OIS4_Pos  );

		cc_dier[3] = ( tim_c->cc4_config->cc_dma_req_en  << TIM_DIER_CC4DE_Pos |
					  tim_c->cc4_config->cc_ie << TIM_DIER_CC4IE_Pos );

		if( tim_c->cc4_config->cc_s )
			{
				//INPUT MODE
				cc_CCMR[3] = ( tim_c->cc4_config->cc_s << TIM_CCMR2_CC4S_Pos |
							   tim_c->cc4_config->ic_pre << TIM_CCMR2_IC4PSC_Pos |
							   tim_c->cc4_config->ic_filter << TIM_CCMR2_IC4F_Pos );



			}
			else{
				// OUTPUT MODE
				cc_CCMR[3] = ( tim_c->cc4_config->cc_s << TIM_CCMR2_CC4S_Pos |
							   tim_c->cc4_config->cc_oc_clear_en << TIM_CCMR2_OC4CE_Pos |
							   tim_c->cc4_config->cc_oc_fast_en << TIM_CCMR2_OC4FE_Pos |
							   tim_c->cc4_config->cc_om << TIM_CCMR2_OC4M_Pos |
							   tim_c->cc4_config->cc_oc_preload_en << TIM_CCMR2_OC4PE_Pos );

				cc_CCER[3] = (     tim_c->cc4_config->out_en << TIM_CCER_CC4E_Pos |
								   tim_c->cc4_config->out_pol << TIM_CCER_CC4P_Pos ) ;

				tim->CCR4 = tim_c->cc4_config->cc_r ;

			}

	}


	temp = tim->CR2 ;
	temp &= ~ ( TIM_CR2_OIS1N_Msk | TIM_CR2_OIS1_Msk |
			    TIM_CR2_OIS2N_Msk | TIM_CR2_OIS2_Msk |
				TIM_CR2_OIS3N_Msk | TIM_CR2_OIS3_Msk |
				TIM_CR2_OIS4_Msk  | TIM_CR2_TI1S_Msk |
				TIM_CR2_MMS_Msk   | TIM_CR2_CCDS_Msk |
				TIM_CR2_CCUS_Msk  | TIM_CR2_CCPC_Msk ) ;


	temp|= ( tim_c->ti1_select  << TIM_CR2_TI1S_Pos | cc_cr2[0] | cc_cr2[1] | cc_cr2[2] | cc_cr2[3]|
			 tim_c->mms  << TIM_CR2_MMS_Pos |
			 tim_c->cc_dma_reqs_en <<  TIM_CR2_CCDS_Pos |
			 tim_c->cc_update_select << TIM_CR2_CCUS_Pos |
			 tim_c->cc_preload_en << TIM_CR2_CCPC_Pos ) ;

	tim->CR2 = (uint16_t)temp ;


	temp = tim->SMCR ;

	temp&= ~ ( TIM_SMCR_ETP_Msk | TIM_SMCR_ECE_Msk | TIM_SMCR_ETPS_Msk |
			   TIM_SMCR_ETF_Msk | TIM_SMCR_MSM_Msk | TIM_SMCR_TS_Msk   |
			   TIM_SMCR_SMS_Msk );

	if( tim_c->etrg_config != NULL )
		temp|=  ( tim_c->etrg_config->etrg_pol << TIM_SMCR_ETP_Pos  |
				  tim_c->etrg_config->etrg_pre << TIM_SMCR_ETPS_Pos |
				  tim_c->etrg_config->etrg_filter <<  TIM_SMCR_ETF_Pos );

	temp|= ( tim_c->ext_clk_mod2_en << TIM_SMCR_ECE_Pos |
			 tim_c->trgi_delay_en << TIM_SMCR_MSM_Pos |
			 tim_c->trgi_s << TIM_SMCR_TS_Pos |
			 tim_c->slave_mode_s << TIM_SMCR_SMS_Pos );

	tim->SMCR = (uint16_t)temp  ;


	temp = tim->DIER ;

	temp&= ~( TIM_DIER_TDE_Msk | TIM_DIER_COMDE_Msk | TIM_DIER_CC4DE_Msk |
			  TIM_DIER_CC3DE_Msk | TIM_DIER_CC2DE_Msk | TIM_DIER_CC1DE_Msk |
			  TIM_DIER_UDE_Msk | TIM_DIER_BIE_Msk | TIM_DIER_TIE_Msk |
			  TIM_DIER_COMIE_Msk | TIM_DIER_CC4IE_Msk | TIM_DIER_CC3IE_Msk |
			  TIM_DIER_CC2IE_Msk | TIM_DIER_CC1IE_Msk | TIM_DIER_UIE_Msk );


	temp|= ( tim_c->trig_dma_req_en << TIM_DIER_TDE_Pos | cc_dier[0] | cc_dier[1] | cc_dier[2] | cc_dier[3] |
			 tim_c->com_dma_req_en << TIM_DIER_COMDE_Pos |
			 tim_c->update_dma_req_en << TIM_DIER_UDE_Pos |
			 tim_c->break_ie << TIM_DIER_BIE_Pos |
			 tim_c->trig_ie << TIM_DIER_TIE_Pos |
			 tim_c->com_ie << TIM_DIER_COMIE_Pos |
			 tim_c->update_ie << TIM_DIER_UIE_Pos );

	tim->DIER = (uint16_t)temp ;


	tim->CCMR1 = (uint16_t)( cc_CCMR[0] | cc_CCMR[1] << 8UL );
	tim->CCMR2 = (uint16_t)( cc_CCMR[2] | cc_CCMR[3] << 8UL );

	temp = tim->CCER ;

	temp &= ~( TIM_CCER_CC4P_Msk | TIM_CCER_CC4E_Msk |
			   TIM_CCER_CC3NP_Msk | TIM_CCER_CC3NE_Msk | TIM_CCER_CC3P_Msk | TIM_CCER_CC3E_Msk |
			   TIM_CCER_CC2NP_Msk | TIM_CCER_CC2NE_Msk | TIM_CCER_CC2P_Msk | TIM_CCER_CC2E_Msk |
			   TIM_CCER_CC1NP_Msk | TIM_CCER_CC1NE_Msk | TIM_CCER_CC1P_Msk | TIM_CCER_CC1E_Msk );

	temp |= ( cc_CCER[0] | cc_CCER[1] | cc_CCER[2] | cc_CCER[3] );

	tim->CCER = (uint16_t) temp ;


	tim->PSC = tim_c->psc ;
	tim->ARR = tim_c->arr ;
	tim->CNT = tim_c->cnt ;

	temp = tim->RCR ;

	temp&= ~ ( TIM_RCR_REP_Msk );

	temp|= tim_c->rcr ;

	tim->RCR = (uint16_t) temp ;

	tim->BDTR = ( tim_c->main_output_en << TIM_BDTR_MOE_Pos |
				  tim_c->auto_output_en << TIM_BDTR_AOE_Pos |
				  tim_c->break_pol << TIM_BDTR_BKP_Pos |
				  tim_c->break_en << TIM_BDTR_BKE_Pos |
				  tim_c->o_off_idle_ss << TIM_BDTR_OSSI_Pos |
				  tim_c->o_off_run_ss << TIM_BDTR_OSSR_Pos |
				  tim_c->lock_bits << TIM_BDTR_LOCK_Pos |
				  tim_c->dead_time << TIM_BDTR_DTG_Pos );


	temp = tim->CR1 ;

	temp&= ~ ( TIM_CR1_CEN_Msk | TIM_CR1_UDIS_Msk | TIM_CR1_URS_Msk | TIM_CR1_OPM_Msk |
			   TIM_CR1_DIR_Msk | TIM_CR1_CMS_Msk | TIM_CR1_ARPE_Msk | TIM_CR1_CKD_Msk ) ;

	temp|= ( tim_c->s_clk_pre << TIM_CR1_CKD_Pos |
			 tim_c->arr_preload_en << TIM_CR1_ARPE_Pos |
			 tim_c->count_align << TIM_CR1_CMS_Pos |
			 tim_c->onepulse_en << TIM_CR1_OPM_Pos |
			 INVERT_SWITCH(tim_c->uev_gen_en) << TIM_CR1_UDIS_Pos |
			 tim_c->uev_src_en << TIM_CR1_URS_Pos ) ;

	if ( tim_c->count_align == 0UL )
		temp|= tim_c->count_dir << TIM_CR1_DIR_Pos ;

	tim->CR1 = temp ;



return ret ;


}
