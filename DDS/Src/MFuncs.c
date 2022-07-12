
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



#ifdef CALC_SINE

/* O(N) Polynomial Calculation */
double polycalc ( int deg , double x , double* coeffs ) {

    double res = 0 ; coeffs+= deg ;
    for ( int i = 0  ; i < deg ; res+= *(coeffs) ,res*=x, i++ , coeffs-- ) ;

 return res+*(coeffs);

}

/* Calculate Nth Factorial */
uint32_t calc_factorial ( uint32_t x ) {

    uint32_t res = 1;
    for ( ; x ; res*=x , x-- );
    return res;
}

/* Sine X Calculation  Using Tayler series expansion */

double calc_sinex ( double x , uint32_t poly_deg ) {

    double a[poly_deg+1] ;

    for ( int32_t i = 0 , c = 1 ; i <= poly_deg ; i++ ) {
        if ( i%2 == 1 ) {
            a[i] = (c?(1.0):(-1.0))/(double(calc_factorial(i) )) ;
            c=!c;
        }
        else{
        a[i] = 0;
        }
    }

    return polycalc( poly_deg , x , &a[0] );




}

#endif




void Wave_Gen_IntDisable( void )
{
	__NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
	__NVIC_DisableIRQ(EXTI9_5_IRQn);
}
