/******************************************************************************
  \file machwdTxFrameHandler.h

  \brief Prototypes of Tx Frame handler.

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
    History:
      09/06/07 A. Mandychev - Created.
******************************************************************************/

#ifndef _MACHWDTXFRAMEHANDLER_H
#define _MACHWDTXFRAMEHANDLER_H

/******************************************************************************
                    Includes section
******************************************************************************/
#include <sysTypes.h>
#include <phyDataStatus.h>
#include <machwdData.h>
#include <machwdManager.h>
#include <machwdHandlerCtrl.h>

/******************************************************************************
                    Types section
******************************************************************************/
typedef enum
{
  MACHWD_IDLE_SUBSTATE,
  MACHWD_SETTXONSTATE_SUBSTATE,
  MACHWD_TX_IN_PROGRESS_SUBSTATE
} TxFrameHandlerSubState_t;

/*****************************************************************************
                              External variables section
******************************************************************************/
extern TxFrameHandlerSubState_t machwdTxFrameHandlerSubState;
extern uint8_t machwdTxFrameHandlerRetryCounter;
extern uint16_t machwdTxFrameHandlerRandomSeed;

/******************************************************************************
                    Prototypes section
******************************************************************************/
/******************************************************************************
  TBD.
******************************************************************************/
MachwdHandlerResult_t machwdTxFrameReqHandler(MACHWD_DataReq_t *request);

/******************************************************************************
  TBD.
******************************************************************************/
MachwdHandlerResult_t machwdTxFrameTaskHandler(void);

/******************************************************************************
  TBD.
******************************************************************************/
void machwdResetTxFrameHandler(void);

/******************************************************************************
  TBD.
******************************************************************************/
void machwdTxRetryHandler(void);

#endif /* _MACHWDTXFRAMEHANDLER_H */

// eof machwdTxFrameHandler.h
