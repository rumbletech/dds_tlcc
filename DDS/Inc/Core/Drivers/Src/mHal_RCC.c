/*
 * mHal_RCC.c
 *
 *  Created on: Jun 7, 2022
 *      Author: Garmoosh
 */


#include "../Inc/mHal_RCC.h"





int32_t mHal_RCC_Fill ( mHal_RCC_Config_Struct* rccptr , mHal_Flash_Config_Struct* flashptr  )
{

	int32_t ret = NO_ERR ;

#ifdef CHECK_PARAMS
	if (( rccptr->PLLM_Divider > MHAL_RCC_PLLM_DIVIDER_MAX || rccptr->PLLM_Divider < MHAL_RCC_PLLM_DIVIDER_MIN                ) ||
	    ( rccptr->PLLN_Multiplier > MHAL_RCC_PLLN_MULTIPLIER_MAX || rccptr->PLLN_Multiplier > MHAL_RCC_PLLN_MULTIPLIER_MAX    ) ||
		( rccptr->PLLM_Divider > MHAL_RCC_PLLM_DIVIDER_MAX || rccptr->PLLM_Divider < MHAL_RCC_PLLM_DIVIDER_MIN                ) ||
		( rccptr->PLLQ_Divider < MHAL_RCC_PLLQ_DIVIDER_MIN || rccptr->fwcycles > MHAL_FWCYCLES_MAX ) )
		ret = PARAM_ASSERT_ERR ;


#endif

	uint32_t temp = 0UL ;

	/* Enable ClkSource Ready Interrupt Flags */
	temp = (( rccptr->HSERDY_IE << RCC_CIR_HSERDYIE_Pos      ) |
		   ( rccptr->HSIRDY_IE << RCC_CIR_HSIRDYIE_Pos       ) |
		   ( rccptr->PLLMAINRDY_IE << RCC_CIR_PLLRDYIE_Pos   ) |
		   ( rccptr->PLLI2SRDY_IE << RCC_CIR_PLLI2SRDYIE_Pos ) |
		   ( rccptr->LSERDY_IE << RCC_CIR_LSERDYIE_Pos       ) |
		   ( rccptr->LSIRDY_IE << RCC_CIR_LSIRDYIE_Pos )     ) ;

	RCC->CIR = temp ;

	/* Write Pre_Scalers and Multipliers */

	temp = (( rccptr->MCO1_Src << RCC_CFGR_MCO1_Pos    ) |
		   ( rccptr->MCO2_Src << RCC_CFGR_MCO2_Pos     ) |
		   ( rccptr->MCO1_Pre << RCC_CFGR_MCO1PRE_Pos  ) |
		   ( rccptr->MCO2_Pre << RCC_CFGR_MCO2PRE_Pos  ) |
		   ( rccptr->MCO1_Src << RCC_CFGR_MCO1_Pos     ) |
		   ( rccptr->RTC_Pre << RCC_CFGR_RTCPRE_Pos    ) |
		   ( rccptr->APB2_Pre << RCC_CFGR_PPRE2_Pos    ) |
		   ( rccptr->APB1_Pre << RCC_CFGR_PPRE1_Pos    ) |
		   ( rccptr->AHB_Pre << RCC_CFGR_HPRE_Pos      ) |
		   ( rccptr->I2S_Src << RCC_CFGR_I2SSRC_Pos    ));


	RCC->CFGR = temp ;

	/* PLL Configuration */

	temp = (( rccptr->PLLM_Divider << RCC_PLLCFGR_PLLM_Pos    )|
			( rccptr->PLLP_Divider << RCC_PLLCFGR_PLLP_Pos    )|
			( rccptr->PLLN_Multiplier << RCC_PLLCFGR_PLLN_Pos )|
			( rccptr->PLLQ_Divider << RCC_PLLCFGR_PLLQ_Pos    )|
			( rccptr->PLL_Src << RCC_PLLCFGR_PLLSRC_Pos       ));

	RCC->PLLCFGR = temp ;

	/* Set Clk Sources */

	temp = ( (rccptr->CSS_sw << RCC_CR_CSSON_Pos       ) |
		     (rccptr->HSE_sw << RCC_CR_HSEON_Pos       ) |
			 (rccptr->HSEBYP_sw << RCC_CR_HSEBYP_Pos   ) |
		     (rccptr->HSI_sw << RCC_CR_HSION_Pos       ) |
		     (rccptr->MPLL_sw << RCC_CR_PLLON_Pos      ) |
		     (rccptr->PLLI2S_sw << RCC_CR_PLLI2SON_Pos ) |
			 (rccptr->HSI_Trim_Val << RCC_CR_HSITRIM_Pos));



	RCC->CR = temp ;

	if ( rccptr->HSI_sw == Enabled )
		while( ! (RCC->CR & RCC_CR_HSIRDY_Msk ) ) ;

	if (  rccptr->PLLI2S_sw == Enabled )
		while(!( RCC->CR & RCC_CR_PLLI2SRDY_Msk ));

	 if ( rccptr->HSE_sw == Enabled || rccptr->HSEBYP_sw == Enabled  )
		while( ! (RCC->CR & RCC_CR_HSERDY_Msk ) ) ;

	 if ( rccptr->MPLL_sw == Enabled )
			while( ! (RCC->CR & RCC_CR_PLLRDY_Msk ) ) ;



	/* Adjust CPU Wait cycles for flash  */

	temp = ( ( flashptr->dcache_sw << FLASH_ACR_DCEN_Pos     ) |
			 ( flashptr->icache_sw << FLASH_ACR_ICEN_Pos     ) |
			 ( flashptr->prefetch_sw << FLASH_ACR_PRFTEN_Pos ) |
			 ( flashptr->fwcycles << FLASH_ACR_LATENCY_Pos   ) ) ;

	FLASH->ACR = temp  ;
	while( (FLASH->ACR & FLASH_ACR_LATENCY_Msk) != (flashptr->fwcycles ));



	temp = RCC->CFGR ;
	temp&= ~( RCC_CFGR_SW_Msk );
	temp|= ( rccptr->Main_Clk << RCC_CFGR_SW_Pos );
	RCC->CFGR = temp ;
	while( ( RCC->CFGR & RCC_CFGR_SWS_Msk ) != ( rccptr->Main_Clk << RCC_CFGR_SWS_Pos  ) );



	return ret ;



}


