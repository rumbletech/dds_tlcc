/*
 * mHal_USBD.c
 *
 *  Created on: Jun 12, 2022
 *      Author: Garmoosh
 */


#include "../Inc/mHal_USBD.h"


static inline mHal_USBD_FS_MOD mHal_USBD_GetMode( void )
{
	return (mHal_USBD_FS_MOD)( USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_CMOD_Msk ) ;
}


static inline void mHal_USBD_SetAddress ( uint32_t daddr ){

	uint32_t temp = USB_OTG_DEVICE->DCFG ;
	temp&= ~( USB_OTG_DCFG_DAD_Msk );
	temp|= ( (daddr << USB_OTG_DCFG_DAD_Pos ) & USB_OTG_DCFG_DAD_Msk );
	USB_OTG_DEVICE->DCFG = temp ;

}


/*Global NAK on IN,OUT Set,clear Functions */

static inline void mHal_Clear_Out_Nak ( void )
{
	USB_OTG_DEVICE->DCTL |= ( 1 << USB_OTG_DCTL_CGONAK_Pos ) ;
}

static inline void mHal_Set_Out_Nak ( void )
{
	if ( !(USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_BOUTNAKEFF_Msk))
		USB_OTG_DEVICE->DCTL |= ( 1 << USB_OTG_DCTL_SGONAK_Pos ) ;
}

static inline void mHal_Clear_IN_Nak ( void )
{
	USB_OTG_DEVICE->DCTL |= ( 1 << USB_OTG_DCTL_CGINAK_Pos ) ;
}

static inline void mHal_Set_IN_Nak ( void )
{
	if ( !(USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_GINAKEFF_Msk))
		USB_OTG_DEVICE->DCTL |= ( 1 << USB_OTG_DCTL_SGINAK_Pos ) ;
}


static inline int32_t mHal_Get_OUT_Nak_Status ( void )
{
	return ( USB_OTG_DEVICE->DCTL & USB_OTG_DCTL_GONSTS_Msk );
}
static inline int32_t mHal_Get_IN_Nak_Status ( void )
{
	return ( USB_OTG_DEVICE->DCTL & USB_OTG_DCTL_GINSTS_Msk );
}



static inline void mHal_Soft_Disconnect ( void )
{
	USB_OTG_DEVICE->DCTL |= ( 1 << USB_OTG_DCTL_SDIS_Pos );
	/* Wait For a Few ms Here depending on device state */
	//wait  1ms

	/*end*/
}

/* Status Information */


static inline int32_t mHal_Get_FrameN( void )
{
	return  ( (USB_OTG_DEVICE->DSTS & USB_OTG_DSTS_FNSOF_Msk ) >> USB_OTG_DSTS_FNSOF_Pos ) ;
}


static inline int32_t mHal_Get_Susp_Status ( void )
{
	return ( USB_OTG_DEVICE->DSTS & USB_OTG_DSTS_SUSPSTS_Msk ) ;
}

static inline int32_t mHal_Get_Enum_Speed ( void )
{
	return ( USB_OTG_DEVICE->DSTS &  USB_OTG_DSTS_ENUMSPD_Msk ) ;
}

static inline int32_t mHal_Get_EERR ( void )
{
	return ( USB_OTG_DEVICE->DSTS &  USB_OTG_DSTS_EERR_Msk ) ;

}



//int32_t mHal_EP_Resize ( )


int32_t mHal_USBD_Fill ( mHal_USBD_Config_Struct * usbdptr )
{





	return 1 ;
}



