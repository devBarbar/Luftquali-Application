/******************************************************************************
  \file phyTxFrame.h

  \brief Frame transmission routines.

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
    History:
      04/06/07 ALuzhetsky - Created.
******************************************************************************/

#ifndef _PHYTXFRAME_H
#define _PHYTXFRAME_H

/******************************************************************************
                        Includes section.
******************************************************************************/
#include <sysTypes.h>
#include <atmega128rfa1.h>
#include <phyCommon.h>
#include <phyDataStatus.h>
#include <phySetTrxState.h>
#include <macenvPib.h>
#include <phyMemAccess.h>

/******************************************************************************
                    Types section
******************************************************************************/

/******************************************************************************
                        External variables.
******************************************************************************/
extern uint8_t machwdFrameRetries;

/******************************************************************************
                        Prototypes section.
******************************************************************************/
/******************************************************************************
  Performs data transmission.
  Parameters: frame - pointer to the data to be sent.
  Returns:    PHY_IN_PROGRESS_REQ_STATUS.
******************************************************************************/
PHY_ReqStatus_t PHY_DataReq(uint8_t *frame);

#endif /* _PHYTXFRAME_H */

// eof phyTxFrame.h
