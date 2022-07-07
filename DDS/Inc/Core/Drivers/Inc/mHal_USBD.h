/*
 * mHal_USBD.h
 *
 *  Created on: Jun 12, 2022
 *      Author: Garmoosh
 */

#ifndef CORE_DRIVERS_INC_MHAL_USBD_H_
#define CORE_DRIVERS_INC_MHAL_USBD_H_

#include "mHal.h"




#define USB_OTG_DEVICE ((USB_OTG_DeviceTypeDef*)((void*)USB_OTG_DEVICE_BASE + USB_OTG_FS_PERIPH_BASE ))
#define USB_OTG_FIFO   ((uint32_t*)((void*)USB_OTG_FIFO_BASE + USB_OTG_FS_PERIPH_BASE ))
#define USB_OTG_OUTEP  ((USB_OTG_OUTEndpointTypeDef*)((void*)USB_OTG_OUT_ENDPOINT_BASE + USB_OTG_FS_PERIPH_BASE ))
#define USB_OTG_INEP   ((USB_OTG_OUTEndpointTypeDef*)((void*)USB_OTG_IN_ENDPOINT_BASE + USB_OTG_FS_PERIPH_BASE ))
typedef enum {

	MHAL_USBD_MOD_DEVICE = 0UL,
	MHAL_USBD_MOD_HOST   = 1UL


} mHal_USBD_FS_MOD;



typedef enum {


	MHAL_USBD_TXFE_LVL_HALF  = 0UL ,
	MHAL_USBD_TXFE_LVL_EMPTY = 1UL


} mHal_USBD_TXFE_LVL ;

typedef enum {

	MHALL_USBD_KEEP_PACKET_STALL    = 0UL ,
	MHALL_USBD_DISCARD_PACKET_STALL = 1UL ,


} mHal_USBD_NZ_STATUS_OUT_HSHK ;


typedef enum {

	MHAL_USBD_EOF_INTV_80 = 0B00 ,
	MHAL_USBD_EOF_INTV_85 = 0B01 ,
	MHAL_USBD_EOF_INTV_90 = 0B10 ,
	MHAL_USBD_EOF_INTV_95 = 0B11 ,

} mHal_USBD_EOF_INTERVAL ;

typedef enum {

	MHAL_USBD_TEST_DISABLED    = 0B000 ,
	MHAL_USBD_TEST_JMODE       = 0B001 ,
	MHAL_USBD_TEST_KMODE       = 0B010 ,
	MHAL_USBD_TEST_SE0_NAKMODE = 0B011 ,
	MHAL_USBD_TEST_PACKETMODE  = 0B100 ,
	MHAL_USBD_TEST_FORCEMODE   = 0B101 ,


} mHal_USBD_Test_Mode ;


typedef enum {

	MHAL_USBD_EP_CTRL = 0B00 ,
	MHAL_USBD_EP_ISOC = 0B01 ,
	MHAL_USBD_EP_BULK = 0B10 ,
	MHAL_USBD_EP_INT  = 0B11 ,

} mHal_USBD_EP_Type ;

typedef enum {

	MHAL_USBD_EP_DIR_IN  = 0B00 ,
	MHAL_USBD_EP_DIR_OUT = 0B01 ,


} mHal_USBD_EP_Dir ;


typedef struct {

	uint32_t EP_Size ;                   /* EP Size in Bytes */
	uint32_t EP_Num  ;                   /* EP Number */
	mHal_USBD_EP_Type EP_Type ;          /* EP Type */
	mHal_USBD_EP_Dir EP_Dir ;            /* EP Direction , in or out */
	uint32_t start_addr     ;            /* RAM Start Address for FIFO */
	uint32_t max_pckt_sz    ;            /* Max Number of Packets */
	uint32_t transfer_sz    ;			 /* Transfer Size in packets */
	uint32_t transfer_szb   ;            /* Transfer size in bytes , = transfer_sz * max_packet_sz*/
	uint32_t fifo_depth     ;            /* FIFO Depth for EP */






} mHal_USBD_EP_Config_Struct ;


typedef struct {

	/* Core Configuration */
	flip_switch HNP_sw ;               /* HNP CAPABILITY ( DEVICE TO HOST OR VICE VERSA SWITCHING ) */
	flip_switch SRP_sw ;               /* SRP CAPABILITY */
	uint8_t  FS_Timeout_Cal ;          /* USB FULL SPEED TIME OUT CALIBRATION VALUE */
	uint8_t  USB_turnaround_time ;	   /* USB TURN AROUND TIME */
	mHal_USBD_TXFE_LVL txfe_lvl ;      /* TXFIFO EMPTY INTERRUPT LEVEL */
	flip_switch gint_sw ;              /* Global Interrupt Mask Switch to the Application */
	flip_switch fdevice_mode ;         /* Forces Device Mode */
	flip_switch fhost_mode   ;         /* "X" Forces Host Mode */
	uint8_t D_Speed         ;          /* Device mode speed , possible value is only 0B11 */
	/* Device Configuration */
	/*DCFG*/
	mHal_USBD_NZ_STATUS_OUT_HSHK nz_status_hshk ;   /* Configures Device Handshake Response to NON ZERO Length Packet received During OUT Transaction in the Status Stage */
	mHal_USBD_EOF_INTERVAL eof_intv ;               /* End of Frame interrupt at 80% ->> 95% of the frame interval */
	/*DCTL*/
	mHal_USBD_Test_Mode test_mode ;
	/* DIEPMSK , Specifies what interrupts can IN endpoints generate */

	flip_switch IEP_Nak ;                       /* NAK Mask */
	flip_switch IEPE_Nak ;                      /* Effective NAK Mask */
	flip_switch IEP_mismatch ;                  /* IN Token Received With EP Mismatch */
	flip_switch IEP_TxEmpty ;                   /* IN Token Received with TX Fifo Empty */
	flip_switch IEP_Timeout ;                   /* Timeout Condition */
	flip_switch IEP_Disabled ;                  /* End Point Disabled */
	flip_switch IEP_TransferComplete ;          /* Transfer Complete */

	/* DOEPMSK , Specifies what interrupts can IN endpoints generate */

	flip_switch OEP_Nak ;                       /* NAK Mask */
	flip_switch OEP_Babble ;                    /* Babble Error Mask */
	flip_switch OEP_PacketError ;               /* OUT packet Error Mask */
	flip_switch OEP_Ctrl_Status ;               /* Status Phase for Control Transfer Received */
	flip_switch OEP_OTR_Disabled ;              /* Out Token Received while EP is disabled */
	flip_switch OEP_SetupDone     ;             /* Setup Phase Done Mask */
	flip_switch OEP_Disabled ;                  /* End Point Disabled */
	flip_switch OEP_TransferComplete ;          /* Transfer Complete */


	/* DAINTMSK  : Controls Which EP Has DIEPMSK or DOEPMSK Enabled for them */

	uint16_t IEP_M ;
	uint16_t OEP_M ;

	/* DVBUSDIS  */

	uint32_t vbus_dis_t ;

	/* VBUSPULSE  , pulse time in phy clocks */

	uint32_t vbus_pulse_t ;

	/* DIEPEMPMSK  Controls which IN End Point has TX FIFO Empty Interrupt Enabled on it */

	uint16_t IEP_TX_EMP_Msk ;














} mHal_USBD_Config_Struct ;


















#endif /* CORE_DRIVERS_INC_MHAL_USBD_H_ */
