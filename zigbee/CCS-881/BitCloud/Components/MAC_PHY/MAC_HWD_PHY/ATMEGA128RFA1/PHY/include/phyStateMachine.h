/******************************************************************************
  \file phyStateMachine.h

  \brief PHY sublayer state control functions prototypes.

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
    History:
      30/05/07 A. Mandychev - Created.
******************************************************************************/

#ifndef _PHYSTATEMACHINE_H
#define _PHYSTATEMACHINE_H

/******************************************************************************
                        Includes section.
******************************************************************************/
#include <sysTypes.h>
#include <machwdManager.h>
#include <phyMemAccess.h>

/******************************************************************************
                        External variables.
******************************************************************************/
// Do not use it in othere files.
extern bool __wasRequest;

/******************************************************************************
                 Inline static functions prototypes section.
******************************************************************************/
/******************************************************************************
  TBD
******************************************************************************/
INLINE void phySetState(PhyState_t state)
{
  if ((state == PHY_IDLE_STATE) && __wasRequest)
  {
    MACHWD_PostTask();
    __wasRequest = false;
  }
  *phyMemPhyState() = state;
}

/******************************************************************************
  TBD
******************************************************************************/
INLINE PhyState_t phyGetState(void)
{
  if (*phyMemPhyState() == PHY_BUSY_RX_STATE) __wasRequest = true;
  return *phyMemPhyState();
}

/******************************************************************************
  TODO
******************************************************************************/
INLINE void phyResetState(void)
{
  *phyMemPhyState() = PHY_IDLE_STATE;
}

#endif /* _PHYSTATEMACHINE_H */

// eof phyStateMachine.h
