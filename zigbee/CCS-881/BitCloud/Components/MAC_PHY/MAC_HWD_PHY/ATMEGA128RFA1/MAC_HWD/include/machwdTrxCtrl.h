/******************************************************************************
  \file machwdTrxCtrl.h

  \brief Prototypes of TRX control module.

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
  History:
    18/06/07 A. Mandychev - Created.
******************************************************************************/

#ifndef _MACHWDTRXCTRL_H
#define _MACHWDTRXCTRL_H

/******************************************************************************
                        Includes section.
******************************************************************************/
#include <sysTypes.h>
#include <machwd.h>
#include <machwdManager.h>
#include <machwdHandlerCtrl.h>
#include <phySetTrxState.h>

/******************************************************************************
                    Types section
******************************************************************************/
typedef enum
{
  MACHWD_TX_ON_TRX_STATE = PHY_TX_ARET_ON_TRX_STATE,
  MACHWD_RX_ON_TRX_STATE = PHY_RX_AACK_ON_TRX_STATE,
  MACHWD_OFF_TRX_STATE   = PHY_OFF_TRX_STATE
} MachwdTrxState_t;

/******************************************************************************
                        External variables.
******************************************************************************/
// Do not use it in othere files.
extern MachwdHandlerId_t __sourceHandler;

/******************************************************************************
                        Prototypes section.
******************************************************************************/
/******************************************************************************
  TBD.
******************************************************************************/
MachwdHandlerResult_t machwdSendTrxCmd(MACHWD_TrxCmd_t cmd, MachwdHandlerId_t source);

/******************************************************************************
  TBD.
******************************************************************************/
MachwdTrxState_t machwdConvertTrxCmdToTrxState(MACHWD_TrxCmd_t cmd);

/******************************************************************************
  TBD.
******************************************************************************/
MACHWD_TrxCmd_t machwdConvertTrxStateToTrxCmd(MachwdTrxState_t state);

/******************************************************************************
  TBD.
******************************************************************************/
PHY_TrxCmd_t machwdGetPhyTrxCmd(MACHWD_TrxCmd_t cmd);

/******************************************************************************
                 Inline static functions prototypes section.
******************************************************************************/
/******************************************************************************
  TBD.
******************************************************************************/
// concurrency (asynchronous) process
INLINE void PHY_SetTrxStateConf(void)
{
  machwdSetHandlerId(__sourceHandler);
  MACHWD_PostTask();
}

/******************************************************************************
  TBD.
******************************************************************************/
INLINE MachwdTrxState_t machwdGetTrxState(void)
{
  return (MachwdTrxState_t) phyGetTrxState();
}

#endif /* _MACHWDTRXCTRL_H */

// eof machwdTrxCtrl.h
