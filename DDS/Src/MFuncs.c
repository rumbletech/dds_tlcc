
#include "main.h"

uint32_t get_tunew( uint32_t freq , uint8_t prec , double d_f )
{
	 uint32_t div = 1UL ;

	 for ( ; prec-- ; div*=10UL );

	 return round(((double)freq)/((double)div)/d_f) ;


}



uint32_t BCDtoUInt( uint32_t degs )
{
	uint32_t ret  = 0UL ;
	for ( int32_t i = 0 ; i < 8UL ; ret*=10 , i++ )
		ret+= (( degs >> (28UL-i*4UL) )&15UL) ;

	return (ret/10UL) ;

}

float FreqW ( uint32_t freq , uint8_t num_prec )
{
	 uint32_t div = 1UL ;

	 for ( ; num_prec-- ; div*=10UL );

	 return (((float)freq)/((float)div)) ;


}


void Wave_Gen_IntDisable( void )
{
	__NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
	__NVIC_DisableIRQ(EXTI9_5_IRQn);
}
