/******************************************************************************
  \file phyRxFrame.h

  \brief Frame receive routines.

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
    History:
      26/05/07 ALuzhetsky - Created.
******************************************************************************/

#ifndef _PHYRXFRAME_H
#define _PHYRXFRAME_H

/******************************************************************************
                        Includes section.
******************************************************************************/
#include <atmega128rfa1.h>
#include <phyStateMachine.h>
#include <phy.h>
#include <machwdRxBuffer.h>
#include <machwdRxFrameHandler.h>
#include <halDiagnostic.h>
#include <phyStateMachine.h>
#include <phySetTrxState.h>

/******************************************************************************
                        Definitions section.
******************************************************************************/
#define ADDITIONAL_FIELDS_SIZE 4  // Frame length + LQI + RSSI + internal flags

/******************************************************************************
                        Prototypes section.
******************************************************************************/
/******************************************************************************
  \brief Finish frame receive operations.
  \param[in] type - the type of target buffer.
******************************************************************************/
static inline void phyDownloadFrame(FrameRxBufferType_t type, uint8_t *frame, uint8_t length)
{
  uint8_t i;

  // Save frame length.
  frame[0] = length;

  // Upload raw frame (PSDU) and LQI.
  for (i = 0; i < length + PHY_LQI_FIELD_SIZE; i++)
    frame[i + PHY_FRAME_LENGTH_FIELD_SIZE] = TRX_FRAME_BUFFER(i);

  // The calculation of LQI will be performed later
  // *phyMemRssi() contains value from PHY_ED_LEVEL register. So *phyMemRssi() is equivalent of ED level.
  // frame[length + PHY_FRAME_LENGTH_FIELD_SIZE] =
  // (uint8_t) getLqi(frame[length + PHY_FRAME_LENGTH_FIELD_SIZE], *phyMemRssi());

  // Save RSSI.
  frame[length + PHY_FRAME_LENGTH_FIELD_SIZE + PHY_LQI_FIELD_SIZE] = (uint8_t) (*phyMemRssi() + ATMEGA128RFA1_RSSI_BASE_VAL);

#if defined(_FFD_) && defined(_CONTROL_FRAME_PENDING_BIT_)
  // Save extra flags to the last byte
  frame[length + PHY_FRAME_LENGTH_FIELD_SIZE + PHY_LQI_FIELD_SIZE + PHY_RSSI_FIELD_SIZE] = *phyMemAckedWithoutPendBit() ?
                                                                                            MAC_FLAG_ACKED_WO_FRAME_PENDING_BIT
                                                                                            : 0;
#endif
  type = type;
}

#endif /* _PHYRXFRAME_H */

// eof phyRxFrame.h
