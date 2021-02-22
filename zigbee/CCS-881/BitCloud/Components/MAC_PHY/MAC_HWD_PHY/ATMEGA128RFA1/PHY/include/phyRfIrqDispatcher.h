/*************************************************************************//**
  \file phyRfIrqDispatcher.h

  \brief RF IRQs handlers prototypes

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
  History:
    11.08.09 A. Taradov - Created.
*****************************************************************************/

#ifndef _PHYRFIRQDISPATCHER_H
#define _PHYRFIRQDISPATCHER_H

#include "machwdData.h"

/******************************************************************************
                    Define(s) section
******************************************************************************/
#define TX_FRAME_HANDLER_MAX_FRAME_RETRIES 4
#define RX_SAFE_MODE_CHANGE_TIME           2

/******************************************************************************
                        Prototypes section.
******************************************************************************/

/******************************************************************************
  Enables the irq.
******************************************************************************/
void phyRestoreRfIrqMask(uint8_t mask);

/******************************************************************************
  Disables the irq.
******************************************************************************/
uint8_t phyClearRfIrqMask(void);

/**************************************************************************//**
\brief External getter for tx data status
\return Data status
******************************************************************************/
MACHWD_DataStatus_t phyGetDataStatus(void);

#endif /* _PHYRFIRQDISPATCHER_H */

// eof phyRfIrqDispatcher.h
