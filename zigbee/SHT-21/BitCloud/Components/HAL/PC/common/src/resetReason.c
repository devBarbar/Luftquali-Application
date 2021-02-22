/**************************************************************************//**
  \file  resetReason.c

  \brief Fake implementation of the reset reason interface for PC.

  \author
      Atmel Corporation: http://www.atmel.com \n
      Support email: avr@atmel.com

    Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
    Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
    History:
      5/12/07 D. Kolmakov - Created
 ******************************************************************************/
/******************************************************************************
 *   WARNING: CHANGING THIS FILE MAY AFFECT CORE FUNCTIONALITY OF THE STACK.  *
 *   EXPERT USERS SHOULD PROCEED WITH CAUTION.                                *
 ******************************************************************************/

/******************************************************************************
                   Includes section
******************************************************************************/
#include <resetReason.h>
#include <sysTypes.h>

/******************************************************************************
                   Implementations section
******************************************************************************/
/******************************************************************************
Returns the reset reason.
Parameters:
  none.
Returns:
  The reason of reset.
******************************************************************************/
HAL_ResetReason_t HAL_ReadResetReason(void)
{
  return FAKE_RESET_REASON;
}
//eof resetReason.c
