/*
 * mHalI2C.h
 *
 *  Created on: Jun 18, 2022
 *      Author: Garmoosh
 */

#ifndef CORE_DRIVERS_INC_MHAL_I2C_H_
#define CORE_DRIVERS_INC_MHAL_I2C_H_

#include "mHal.h"

#define MHAL_I2C_SM_MAX_TR 1000UL
#define MHAL_I2C_FM_MAX_TR 300UL

#define MHAL_I2C_10B_HEADER (uint8_t)0B11110000




#define MHAL_I2C_SM_GET_CCR( SCL_FREQ , PERPH_CLK ) (PERPH_CLK/(2*SCL_FREQ))
#define MHAL_I2C_FM_GET_CCR( SCL_FREQ , PERPH_CLK , DUTY_MODE ) ( PERPH_CLK/(DUTY_MODE*SCL_FREQ))





//#define MHAL_I2C_INTWCALLBACKS


typedef enum {
	MHAL_I2C_MASTER_TRANSMITTER = 0 ,
	MHAL_I2C_MASTER_RECEIVER    = 1 ,
} mHal_I2C_Master_Mode ;

typedef enum {

	MHAL_I2C_DF_OFF    = 0B0000 ,
	MHAL_I2C_DF_1TP    = 0B0001 ,
	MHAL_I2C_DF_2TP    = 0B0010 ,
	MHAL_I2C_DF_3TP    = 0B0011 ,
	MHAL_I2C_DF_4TP    = 0B0100 ,
	MHAL_I2C_DF_5TP    = 0B0101 ,
	MHAL_I2C_DF_6TP    = 0B0110 ,
	MHAL_I2C_DF_7TP    = 0B0111 ,
	MHAL_I2C_DF_8TP    = 0B1000 ,
	MHAL_I2C_DF_9TP    = 0B1001 ,
	MHAL_I2C_DF_10TP   = 0B1010 ,
	MHAL_I2C_DF_11TP   = 0B1011 ,
	MHAL_I2C_DF_12TP   = 0B1100 ,
	MHAL_I2C_DF_13TP   = 0B1101 ,
	MHAL_I2C_DF_14TP   = 0B1110 ,
	MHAL_I2C_DF_15TP   = 0B1111 ,



} mHal_I2C_DF ;


typedef enum {

	MHAL_I2C_SM = 0B0 ,
	MHAl_I2C_FM = 0B1 ,


} mHal_I2C_Mode ;

typedef enum {

	MHAL_I2C_I2CBUS = 0B0 ,
	MHAL_I2C_SMBUS  = 0B1 ,

} mHal_I2C_BUSM ;

typedef enum {

	mHal_I2C_FM_DS_1B2  = 0B0 ,
	mHal_I2C_FM_DS_16B9 = 0B1 ,

} mHal_I2C_FMDS ;


typedef enum {

	MHAL_I2C_SMBUS_DEVICE = 0B0 ,
	MHAL_I2C_SMBUS_HOST   = 0B1 ,

} MHAL_I2C_SMBUS_TYPE ;

typedef enum {

	MHAL_I2C_PARAM_NAKED = 0B00000000 ,
	MHAL_I2C_PARAM_START = 0B00000001 ,
	MHAL_I2C_PARAM_STOP  = 0B00000010 ,
	MHAL_I2C_PARAM_ADDR  = 0B00000100 ,


} mHal_I2C_Params ;

typedef struct {

	mHal_I2C_Mode mode             ; /* i2c mode , standard mode (SM) or Fast Mode (FM) */
	mHal_I2C_BUSM bus_mode         ; /* I2C BUS OR SMBUS */
	MHAL_I2C_SMBUS_TYPE smbus_type ; /* SMBUS Type only if bus is selected to be smbus */
	uint16_t ccr                   ; /* Determines SCL Period */
	mHal_I2C_FMDS fm_duty          ; /* FM mode duty cycle tlow / thigh */
	uint8_t tp_freq                ; /* Peripheral Frequency 2<= fph <=50 */
	uint16_t oaddr1                ; /* own address  1 */
	uint8_t oaddr2                 ; /* own address 2 if dual addressing is enabled */
	flip_switch addr10bit          ; /* ten bit address enable */
	flip_switch dual_addren        ; /* dual address enable */
	uint16_t tr                     ; /* Rise time */
	flip_switch analog_f_en        ; /* Analog Filter Enable */
	mHal_I2C_DF digita_f           ; /* Digital Filter 0b0000 = Disabled , else it filters up to value * Peripheral clock */
	/* Interrupts */
	flip_switch err_int_en         ; /* Error Interrupt Enable */
	flip_switch event_int_en       ; /* Event Interrupt Enable */
	flip_switch buff_int_en        ; /* Buffer interrupt Enable */
	flip_switch dma_rq_en          ; /* DMA Requests enabled */
	/* Control Switches */
	flip_switch gcall_en           ; /* General Call enable (responds to general call on the bus */
    flip_switch pec_en             ; /* error check packet enable */
    flip_switch arp_en             ; /* SMBUS Related  */
    flip_switch clk_stretch_en     ; /* Enable Clock Stretching */



} mHal_I2C_Config_Struct ;










 extern mHal_I2C_Config_Struct mHal_I2C_SM_Struct ;



 int32_t mHal_I2C_Master_Begin( I2C_TypeDef * i2c  , uint8_t* data , int32_t len , uint16_t addr  , mHal_I2C_Master_Mode i2c_mm , mHal_I2C_Params tparams );
int32_t mHal_I2C_Fill ( mHal_I2C_Config_Struct* i2c_s ,  I2C_TypeDef* i2c );
void mHal_I2C_Stop_Gen ( I2C_TypeDef * i2c );
void mHal_I2C_Start_Gen( I2C_TypeDef * i2c  );
void mHal_I2C_Reset( I2C_TypeDef* i2c );
void mHal_I2C_Errata_WA( I2C_TypeDef * i2c  , uint8_t scl , uint8_t sda , GPIO_TypeDef * gpio );
void mHal_I2C_Enable (I2C_TypeDef * i2c );
void mHal_I2C_Disable (I2C_TypeDef * i2c );



















#endif /* CORE_DRIVERS_INC_MHAL_I2C_H_ */
