



/* Slim MAX7219 Device Driver */

/* Prevent Multiple inclusions */
#ifndef _H_D_MAX7219_
#define _H_D_MAX7219_



#include "mHal_SPI.h"


#define MX_SPI SPI2

#define D_MAX7219_DATA_Msk 0X00FF
#define D_MAX7219_DATA_Pos 0UL
#define D_MAX7219_ADDR_Msk 0x0F00
#define D_MAX7219_ADDR_Pos 8UL


#define D_MAX7219_DIG0_ADDR  0X01
#define D_MAX7219_DIG1_ADDR  0X02
#define D_MAX7219_DIG2_ADDR  0X03
#define D_MAX7219_DIG3_ADDR  0X04
#define D_MAX7219_DIG4_ADDR  0X05
#define D_MAX7219_DIG5_ADDR  0X06
#define D_MAX7219_DIG6_ADDR  0X07
#define D_MAX7219_DIG7_ADDR  0X08
#define D_MAX7219_DMODE_ADDR 0X09
#define D_MAX7219_INTS_ADDR  0X0A
#define D_MAX7219_SCAN_ADDR  0X0B
#define D_MAX7219_SHUTD_ADDR 0X0C
#define D_MAX7219_DISPT_ADDR 0X0F



#define D_MAX7219_SHUTDOWN_CMD 0X00
#define D_MAX7219_WAKEUP_CMD   0X01


#define D_MAX7219_DMOD_SEG0_Pos 0UL
#define D_MAX7219_DMOD_SEG0_Msk ( 1UL << D_MAX7219_DMOD_SEG0_Pos )
#define D_MAX7219_DMOD_SEG1_Pos 1UL
#define D_MAX7219_DMOD_SEG1_Msk ( 1UL << D_MAX7219_DMOD_SEG1_Pos )
#define D_MAX7219_DMOD_SEG2_Pos 2UL
#define D_MAX7219_DMOD_SEG2_Msk ( 1UL << D_MAX7219_DMOD_SEG2_Pos )
#define D_MAX7219_DMOD_SEG3_Pos 3UL
#define D_MAX7219_DMOD_SEG3_Msk ( 1UL << D_MAX7219_DMOD_SEG3_Pos )
#define D_MAX7219_DMOD_SEG4_Pos 4UL
#define D_MAX7219_DMOD_SEG4_Msk ( 1UL << D_MAX7219_DMOD_SEG4_Pos )
#define D_MAX7219_DMOD_SEG5_Pos 5UL
#define D_MAX7219_DMOD_SEG5_Msk ( 1UL << D_MAX7219_DMOD_SEG5_Pos )
#define D_MAX7219_DMOD_SEG6_Pos 6UL
#define D_MAX7219_DMOD_SEG6_Msk ( 1UL << D_MAX7219_DMOD_SEG6_Pos )
#define D_MAX7219_DMOD_SEG7_Pos 7UL
#define D_MAX7219_DMOD_SEG7_Msk ( 1UL << D_MAX7219_DMOD_SEG7_Pos )


#define D_MAX7219_CODE_D0_Pos 0UL
#define D_MAX7219_CODE_D0_Msk ( 1UL << D_MAX7219_CODE_D0_Pos )
#define D_MAX7219_CODE_D1_Pos 1UL
#define D_MAX7219_CODE_D1_Msk ( 1UL << D_MAX7219_CODE_D1_Pos )
#define D_MAX7219_CODE_D2_Pos 2UL
#define D_MAX7219_CODE_D2_Msk ( 1UL << D_MAX7219_CODE_D2_Pos )
#define D_MAX7219_CODE_D3_Pos 3UL
#define D_MAX7219_CODE_D3_Msk ( 1UL << D_MAX7219_CODE_D3_Pos )
#define D_MAX7219_CODE_D4_Pos 4UL
#define D_MAX7219_CODE_D4_Msk ( 1UL << D_MAX7219_CODE_D4_Pos )
#define D_MAX7219_CODE_D5_Pos 5UL
#define D_MAX7219_CODE_D5_Msk ( 1UL << D_MAX7219_CODE_D5_Pos )
#define D_MAX7219_CODE_D6_Pos 6UL
#define D_MAX7219_CODE_D6_Msk ( 1UL << D_MAX7219_CODE_D6_Pos )
#define D_MAX7219_CODE_DP_Pos 7UL
#define D_MAX7219_CODE_DP_Msk ( 1UL << D_MAX7219_CODE_DP_Pos )


#define D_MAX7219_SCANL_Pos 0UL
#define D_MAX7219_SCANL_Msk ( 0B111 << D_MAX7219_SCANL_Pos )



typedef struct {

	uint8_t Code_B ;
	uint8_t Intensity ;
	uint8_t Scan_Limit ;
	uint8_t d[8] ;



} D_MAX7219_Config_Struct ;







uint16_t D_MAX7219_ShutDown( void );
uint16_t D_MAX7219_SendCMD( uint8_t addr , uint8_t data );
uint16_t D_MAX7219_WakeUp( void );
void D_MAX7219_Test ( void );
void D_MAX7219_Normal ( void );
void D_MAX7219_Intensity( uint8_t pwm_val );
void D_MAX7219_ScanLimit( uint8_t Limit_val );
void D_MAX7219_WriteNum ( uint32_t Val , uint8_t dp_val );
void D_MAX7219_WriteDigits ( uint8_t* Digits  );
void D_MAX7219_Init ( D_MAX7219_Config_Struct * cfgptr );













#endif




