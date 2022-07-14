







#include <D_MPR121.h>














MPR121_Config_Struct MPR121_Default_Struct = {

		 .ele_blv = {} ,

		 .ele_cdc = {},

		 .glob_cdc = MPR121_CDC_DISABLED ,


		 .ele_cdt = {} ,


		 .glob_cdt = MPR121_CDT_DISABLE ,


		 .ele_MHD_rising  = MPR121_MHD_NOMINAL,
		 .ele_NHD_rising  = MPR121_NHD_NOMINAL,
		 .ele_NCL_rising  = MPR121_NCL_NOMINAL,
		 .ele_FDL_rising  = MPR121_FDL_NOMINAL,

		 .ele_MHD_falling = MPR121_MHD_NOMINAL ,
		 .ele_NHD_falling = MPR121_NHD_NOMINAL ,
		 .ele_NCL_falling = MPR121_NCL_NOMINAL ,
		 .ele_FDL_falling = MPR121_FDL_NOMINAL ,

		 .ele_NHD_touched = MPR121_NHD_NOMINAL ,
		 .ele_NCL_touched = MPR121_NCL_NOMINAL ,
		 .ele_FDL_touched = MPR121_FDL_NOMINAL ,



		 .eleprx_MHD_rising = MPR121_MHD_NOMINAL ,
		 .eleprx_NHD_rising = MPR121_NHD_NOMINAL ,
		 .eleprx_NCL_rising = MPR121_NCL_NOMINAL ,
		 .eleprx_FDL_rising = MPR121_FDL_NOMINAL ,

		 .eleprx_MHD_falling = MPR121_MHD_NOMINAL ,
		 .eleprx_NHD_falling = MPR121_NHD_NOMINAL ,
		 .eleprx_NCL_falling = MPR121_NCL_NOMINAL ,
		 .eleprx_FDL_falling = MPR121_FDL_NOMINAL ,


		 .eleprx_NHD_touched = MPR121_NHD_NOMINAL ,
		 .eleprx_NCL_touched = MPR121_NCL_NOMINAL ,
		 .eleprx_FDL_touched = MPR121_FDL_NOMINAL ,


		 .ele_tr_threshhold = { (MPR121_RELEASE_THRESH_NOMINAL << 8UL )|MPR121_TOUCH_THRESH_NOMINAL,
				                (MPR121_RELEASE_THRESH_NOMINAL << 8UL )|MPR121_TOUCH_THRESH_NOMINAL,
								(MPR121_RELEASE_THRESH_NOMINAL << 8UL )|MPR121_TOUCH_THRESH_NOMINAL,
								(MPR121_RELEASE_THRESH_NOMINAL << 8UL )|MPR121_TOUCH_THRESH_NOMINAL,
								(MPR121_RELEASE_THRESH_NOMINAL << 8UL )|MPR121_TOUCH_THRESH_NOMINAL,
								(MPR121_RELEASE_THRESH_NOMINAL << 8UL )|MPR121_TOUCH_THRESH_NOMINAL,
								(MPR121_RELEASE_THRESH_NOMINAL << 8UL )|MPR121_TOUCH_THRESH_NOMINAL,
								(MPR121_RELEASE_THRESH_NOMINAL << 8UL )|MPR121_TOUCH_THRESH_NOMINAL,
								(MPR121_RELEASE_THRESH_NOMINAL << 8UL )|MPR121_TOUCH_THRESH_NOMINAL,
								(MPR121_RELEASE_THRESH_NOMINAL << 8UL )|MPR121_TOUCH_THRESH_NOMINAL,
								(MPR121_RELEASE_THRESH_NOMINAL << 8UL )|MPR121_TOUCH_THRESH_NOMINAL,
								(MPR121_RELEASE_THRESH_NOMINAL << 8UL )|MPR121_TOUCH_THRESH_NOMINAL,
								(MPR121_RELEASE_THRESH_NOMINAL << 8UL )|MPR121_TOUCH_THRESH_NOMINAL,
		 	 } ,


		 .ff = MPR121_FF_SAMPLES_6 ,
		 .sf = MPR121_SF_SAMPLES_4 ,
		 .si = MPR121_SAMPLE_INTV_1MS ,

		 .blten = MPR121_BLT_EN_INIT_10MSB ,

		 .gpio_config  = {} ,
		 .gpio_data = 0 ,
		 .gpio_en = 0 ,

		 .prox = MPR121_ELEPRX_DISABLE ,
		 .elecen = MPR121_ELE0_TO_ELE11_ENABLE ,
		 .debounce_touch  = MPR121_TOUCH_DEBOUNCE_NOMINAL ,
		 .debounce_release = MPR121_RELEASE_DEBOUNCE_NOMINAL ,



		 .auto_config_en = Enabled,
		 .ac_blv = MPR121_AUTOCONFIG_BLV_ACFVAL ,
		 .ac_retry = MPR121_AUTOCONFIG_RETRY_2TIMES ,
		 .ac_nsamples = MPR121_AUTOCONFIG_AFES_10SAMPLE ,

		 .skip_cdt_ien = Disabled ,
		 .out_of_range_ien = Disabled ,
		 .auto_reconfig_fail_ien = Disabled ,
		 .auto_config_fail_ien = Disabled ,

		 .USL = MPR121_USL_NOMINAL ,
		 .LSL = MPR121_LSL_NOMINAL ,
		 .TL  = MPR121_TL_NOMINAL ,


};





void MPR121_WriteCommand(  uint8_t* vptr , uint8_t len , uint8_t starting_addr )
{

	/* Set Starting Address  */
	mHal_I2C_Master_Begin( MPR121_I2C , &starting_addr , 1 , MPR121_ADDRESS, MHAL_I2C_MASTER_TRANSMITTER , MHAL_I2C_PARAM_ADDR|MHAL_I2C_PARAM_START );
	/* Continue Transmission on len bytes then stop */
	mHal_I2C_Master_Begin( MPR121_I2C , vptr , len , MPR121_ADDRESS, MHAL_I2C_MASTER_TRANSMITTER , MHAL_I2C_PARAM_STOP );


}

void MPR121_ReadCommand(  uint8_t* vptr , uint8_t len , uint8_t starting_addr )
{

	/* Set Read Address */
	mHal_I2C_Master_Begin( MPR121_I2C , &starting_addr , 1 , MPR121_ADDRESS, MHAL_I2C_MASTER_TRANSMITTER ,  MHAL_I2C_PARAM_ADDR|MHAL_I2C_PARAM_START );
	/* Repeated Start Followed by len reads */
	mHal_I2C_Master_Begin( MPR121_I2C , vptr , len , MPR121_ADDRESS , MHAL_I2C_MASTER_RECEIVER , MHAL_I2C_PARAM_START|MHAL_I2C_PARAM_ADDR|MHAL_I2C_PARAM_STOP );


}

void MPR121_SoftReset ( void )
{
	uint8_t reset = MPR121_SOFT_RESET_W ;
	MPR121_WriteCommand(&reset, 1, MPR121_SOFT_RESET_RADDR );

}

void MPR121_Stop( void )
{
	uint8_t stop = 0x00 ;
	MPR121_WriteCommand(&stop, 1, MPR121_ECR_RADDR );

}



void MPR121_Read( uint8_t * ptr )
{

	 MPR121_ReadCommand( ptr , MPR121_NUM_REGS , MPR121_ELE_TS_L_RADDR );

}


uint32_t MPR121_GetStatus( void )
{
	uint32_t temp = 0 ;
	 MPR121_ReadCommand( (uint8_t*)&temp , 4 , MPR121_ELE_TS_L_RADDR );
	 return temp ;

}







void MPR121_Fill ( MPR121_Config_Struct* mpr )
{

	/* Registers can be written only in stop mode */
	MPR121_Stop();

	uint8_t temp ;
	//Write Base Line Value Registers
	MPR121_WriteCommand( &mpr->ele_blv[0] , 13 , MPR121_ELE0_BASEV_RADDR );
	//Write Half Delta Registers
	MPR121_WriteCommand( &mpr->ele_MHD_rising , 22 , MPR121_ELEPRX_MHD_RISE_RADDR );
	//Touch and Relase Registers
	MPR121_WriteCommand( (uint8_t*)&mpr->ele_tr_threshhold , 26 , MPR121_ELE0_TOUCH_THRSH_RADDR );
	//Debounce
	temp = ( (mpr->debounce_touch << MPR121_DEBOUNCE_DT_Pos )| (mpr->debounce_release << MPR121_DEBOUNCE_DR_Pos ));
	MPR121_WriteCommand( &temp , 1 , MPR121_ELE_DEBOUNCE_RADDR  );
	// AFE 1
	temp = (( mpr->glob_cdc << MPR121_AFE_CONFIG1_CDC_Pos )|( mpr->ff << MPR121_AFE_CONFIG1_FFI_Pos ));
	MPR121_WriteCommand( &temp , 1 , MPR121_AFE_CONFIG1_RADDR   );
	// AFE 2
	temp = (( mpr->glob_cdt << MPR121_AFE_CONFIG2_CDT_Pos )|
			( mpr->si << MPR121_AFE_CONFIG2_ESI_Pos )|
			( mpr->sf << MPR121_AFE_CONFIG2_SFI_Pos ));

	MPR121_WriteCommand( &temp , 1 , MPR121_AFE_CONFIG2_RADDR   );
	// CDC Individual
	MPR121_WriteCommand( &mpr->ele_cdc[0] , 13 , MPR121_ELE0_CURRENT_RADDR );
	// CDT Individual
	uint8_t temp_a[8];
	for ( uint8_t i = 1 ; i < 12 ; i++ ){
		if ( i%2 == 1 )
			temp_a[i/2] = (( mpr->ele_cdt[i] ) | ( mpr->ele_cdt[i-1] << 4 ));

	}
	temp_a[6] = mpr->ele_cdt[12];
	MPR121_WriteCommand( &temp_a[0] , 7 , MPR121_ELE01_CHARGET_RADDR );
	//GPIO
	for ( uint8_t i = 0 ; i < 5 ; i++ )
		temp_a[i] = 0 ;

	for ( uint8_t i = 0  ; i < 5 ; i++ )
	{
		temp_a[0] |= (( mpr->gpio_config[i]&MPR121_GPIO_BIT0_Msk ) << i ) ;
		temp_a[1] |= (( (mpr->gpio_config[i]&MPR121_GPIO_BIT1_Msk) ? 1 : 0 ) << i ) ;
		temp_a[3] |= (( (mpr->gpio_config[i]&MPR121_GPIO_BIT2_Msk) ? 1 : 0 ) << i ) ;
	}

	temp_a[2] = mpr->gpio_data ;
	temp_a[4] = mpr->gpio_en ;
	MPR121_WriteCommand( &temp_a[0] , 5 , MPR121_GPIO_CTRL0_RADDR );

	//Auto Configuration Registers

	MPR121_WriteCommand( &mpr->USL , 3 , MPR121_AUTOCONFIG_USL_RADDR );

	//AUTO CONFIG1
	temp = ( ( mpr->ac_blv << MPR121_AUTOCONFIG1_BVA_Pos )|
			( mpr->ac_retry << MPR121_AUTOCONFIG1_RETRY_Pos )|
			( mpr->ac_nsamples << MPR121_AUTOCONFIG1_AFES_Pos )|
			 ( mpr->auto_config_en << MPR121_AUTOCONFIG1_ACE_Pos ));

	MPR121_WriteCommand( &temp , 1 , MPR121_AUTOCONFIG1_RADDR  );
	//AUTO CONFIG2
	temp = ( ( mpr->skip_cdt_ien << MPR121_AUTOCONFIG2_SCTS_Pos )|
			( mpr->out_of_range_ien << MPR121_AUTOCONFIG2_OORIE_Pos )|
			( mpr->auto_config_fail_ien << MPR121_AUTOCONFIG2_ACFIE_Pos )|
			( mpr->auto_config_fail_ien << MPR121_AUTOCONFIG2_ARFIE_Pos ));

	MPR121_WriteCommand( &temp , 1 , MPR121_AUTOCONFIG2_RADDR  );

	//ECR REGISTER

	temp = (
			( mpr->blten << MPR121_ECR_CL_Pos )|
			( mpr->elecen << MPR121_ECR_ELE_Pos )|
			( mpr->prox << MPR121_ECR_ELEPROX_Pos ) );

	//EXIT STOP MODE
	MPR121_WriteCommand( &temp , 1 , MPR121_ECR_RADDR  );


}

