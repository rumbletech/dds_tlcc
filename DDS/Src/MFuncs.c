
#include "main.h"

uint32_t get_tunew( uint32_t freq , uint8_t prec , int16_t* corr , double d_f )
{
	 uint32_t div = 1UL ;
	 double fract = 0 ;
	 double temp = 0 ;
	 for ( ; prec-- ; div*=10UL );
	 temp = ((double)freq)/((double)div)/d_f ;
	 fract = ( temp - (uint32_t)temp );
	 if ( fract < 0.5 )
	 *(corr) =(int16_t)(fract*1000.0);
	 else
	 *(corr) =(int16_t)((1.0-fract)*-1000.0);

	 return round(temp);

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

double calc_sinex ( double x  ) {

    double a[SINE_CALC_DEG+1] ;

    for ( int32_t i = 0 , c = 1 ; i <= SINE_CALC_DEG ; i++ ) {
        if ( i%2 == 1 ) {
            a[i] = (c?(1.0):(-1.0))/(double(calc_factorial(i) )) ;
            c=!c;
        }
        else{
        a[i] = 0;
        }
    }

    return polycalc( SINE_CALC_DEG , x , &a[0] );




}

/* Function that takes normalized sample input and returns an in range value (0-255) */

uint8_t F_SineCalc ( double n_sample )
{
	return round(127.0+127.0 * calc_sinex(n_sample*2*M_PI) ) ;
}


#endif

#ifdef  SAWTOOTH_CALC



uint8_t F_SawtoothCalc ( double n_sample )
{
	return round(255.0 *n_sample) ;
}

#endif

#ifdef TRIANGLE_CALC

uint8_t F_TriangleCalc ( double n_sample )
{
	double temp = 0 ;
	if ( n_sample < 0.5 )
		temp= 255.0*2.0*n_sample ;
	else
		temp= 255.0*2.0*(1.0 - n_sample) ;

	return round(temp);
}

#endif


uint8_t  F_SquareCalc( double n_sample  )
{
	if ( n_sample < 0.5 )
		return 255 ;
	else
		return 0 ;
}




void Wave_Gen_IntDisable( void )
{
	__NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
	__NVIC_DisableIRQ(EXTI9_5_IRQn);
	__NVIC_DisableIRQ(USART1_IRQn);
}

void Wave_Gen_IntEnable( void )
{
	__NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	__NVIC_EnableIRQ(EXTI9_5_IRQn);
	__NVIC_EnableIRQ(USART1_IRQn);

}
