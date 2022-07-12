

#include "../Inc/D_MAX7219.h"











uint16_t D_MAX7219_SendCMD( uint8_t addr , uint8_t data )
{
	uint16_t cmd = ( (uint16_t)addr << D_MAX7219_ADDR_Pos  ) | ( data ) ;
	mHal_SPI_Write( &cmd, &cmd ,  2UL , MX_SPI ) ;
	return cmd ;
}




uint16_t D_MAX7219_ShutDown( void )
{
	return D_MAX7219_SendCMD( D_MAX7219_SHUTD_ADDR , D_MAX7219_SHUTDOWN_CMD );
}

uint16_t D_MAX7219_WakeUp( void )
{
	return D_MAX7219_SendCMD( D_MAX7219_SHUTD_ADDR , D_MAX7219_WAKEUP_CMD );
}


void D_MAX7219_Test ( void )
{
	D_MAX7219_SendCMD( D_MAX7219_DISPT_ADDR , 1UL );
}

void D_MAX7219_Normal ( void )
{
	D_MAX7219_SendCMD( D_MAX7219_DISPT_ADDR , 0UL );
}

void D_MAX7219_Intensity( uint8_t pwm_val )
{
	D_MAX7219_SendCMD( D_MAX7219_INTS_ADDR , pwm_val );
}

void D_MAX7219_ScanLimit( uint8_t Limit_val )
{
	D_MAX7219_SendCMD( D_MAX7219_SCAN_ADDR , Limit_val );
}


void D_MAX7219_WriteNum ( uint32_t val , uint8_t dp_val )
{
	D_MAX7219_Normal ();
	D_MAX7219_SendCMD( D_MAX7219_DMODE_ADDR ,  0xFF);

	for ( int32_t i = 0  ; i <  8 ; i++ , val/=10  )
		D_MAX7219_SendCMD( D_MAX7219_DIG0_ADDR+i ,  (( val == 0 )?(0x0F):val%10)|(dp_val&(1<<i)?(1 << D_MAX7219_CODE_DP_Pos):0) );

	return ;


}


void D_MAX7219_WriteDigits ( uint8_t* Digits  , uint8_t code_B )
{
	D_MAX7219_Normal ();

	D_MAX7219_SendCMD( D_MAX7219_DMODE_ADDR ,  code_B  );

	for ( int32_t i = 0  ; i <  8 ; i++  )
		D_MAX7219_SendCMD( D_MAX7219_DIG0_ADDR+i ,  *(Digits+i) );

	return;

}

void D_MAX7219_Init ( D_MAX7219_Config_Struct * cfgptr )
{
	D_MAX7219_Normal ();
	D_MAX7219_ShutDown() ;


	/* Program all Digits */
	D_MAX7219_WriteDigits(cfgptr->d ,  cfgptr->Code_B  );

	D_MAX7219_SendCMD( D_MAX7219_DMODE_ADDR ,  cfgptr->Code_B  );
	D_MAX7219_SendCMD( D_MAX7219_INTS_ADDR ,  cfgptr->Intensity );
	D_MAX7219_SendCMD( D_MAX7219_SCAN_ADDR,  cfgptr->Scan_Limit  );




	return ;


}

