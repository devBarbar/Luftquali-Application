/******************************************************************************
  \file phySetTrxState.h

  \brief Radio transceiver state control functions prototype.

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
    History:
      26/05/07 A. Mandychev - Created.
******************************************************************************/

#ifndef _PHYSETTRXSTATE_H
#define _PHYSETTRXSTATE_H

/******************************************************************************
                        Includes section.
******************************************************************************/
#include <atmega128rfa1.h>
#include <halMacIsr.h>
#include <phyCommon.h>
#include <phyStateMachine.h>
#include <phyMemAccess.h>

/******************************************************************************
                        Prototypes section.
******************************************************************************/
/******************************************************************************
  Performs activities to switch to the necessery state.
  Parameters: cmd - command to switch.
  Returns: status of operation. PHY_BUSY_RX_REQ_STATUS,
           PHY_SUCCESS_REQ_STATUS, PHY_IN_PROGRESS_REQ_STATUS are all possible.
******************************************************************************/
PHY_ReqStatus_t PHY_SetTrxStateReq(PHY_TrxCmd_t cmd);

/******************************************************************************
                 Inline static functions prototypes section.
******************************************************************************/
/******************************************************************************
  Gets current TRX state.
  Parameters: none.
  Returns: current TRX state.
******************************************************************************/
INLINE PhyTrxState_t phyGetTrxState(void)
{
  return *phyMemPhyTrxState();
}

/******************************************************************************
  Set TRX OFF state.
******************************************************************************/
INLINE void phyResetTrxState(void)
{
  *phyMemPhyTrxState() = PHY_OFF_TRX_STATE;
}

/******************************************************************************
  RTimer event handler. It's used for state polling after receive procedure.
  To detect that acknowledgement was already sent.
  Parameters:
    none.
  Returns:
    none.
******************************************************************************/
INLINE void phyPollTrxStateHandler(void)
{
  if (TRX_STATUS_PLL_ON == TRX_STATUS_REG_s.trxStatus)
  {
#if defined(_FFD_) && defined(_CONTROL_FRAME_PENDING_BIT_)
    CSMA_SEED_1_REG_s.aackSetPd = 1;
#endif
    // To switch back to RX_AACK_ON after receive procedure.
    HAL_StopRtimer();
    TRX_STATE_REG = TRX_CMD_RX_AACK_ON;
    phySetState(PHY_IDLE_STATE);
  }
}

/******************************************************************************
  \brief RTimer event handler. It's used to check if radio is in ready to send state.
******************************************************************************/
INLINE void phyPollRTSTrxStateHandler(void)
{
  if (TRX_STATUS_RX_AACK_ON == TRX_STATUS_REG_s.trxStatus)
  {
    HAL_StopRtimer();
    MACHWD_PostTask();
  }
}

/******************************************************************************
  Converts command to switch to the state to switch.
  Parameters: cmd - command to convert.
  Returns: state originated from cmd.
******************************************************************************/
INLINE PhyTrxState_t phyConvertTrxCmdToTrxState(PHY_TrxCmd_t cmd)
{
  PhyTrxState_t state = (PhyTrxState_t) cmd;

  if (cmd == PHY_FORCE_TRX_OFF_CMD)
    state = PHY_OFF_TRX_STATE;
  return state;
}

#endif /* _PHYSETTRXSTATE_H */

// eof phySetTrxState.h
