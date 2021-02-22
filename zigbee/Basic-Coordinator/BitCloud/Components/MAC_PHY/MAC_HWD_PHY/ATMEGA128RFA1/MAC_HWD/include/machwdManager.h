/******************************************************************************
  \file machwdManager.h

  \brief Manager module prototypes and types.

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
    History:
      28/05/07 A. Mandychev - Created.
******************************************************************************/

#ifndef _MACHWDMANAGER_H
#define _MACHWDMANAGER_H

/******************************************************************************
                        Includes section.
******************************************************************************/
#include <atomic.h>
#include <sysTaskManager.h>
#include <macDbg.h>
#include <halDiagnostic.h>
/******************************************************************************
                        Definitions section.
******************************************************************************/

/******************************************************************************
                        External variables.
******************************************************************************/
// Do not use it in othere files.
extern bool __wasPostTask;

/******************************************************************************
                        Prototypes section.
******************************************************************************/
/******************************************************************************
 TBD.
******************************************************************************/
void MAC_PHY_HWD_TaskHandler(void);

/******************************************************************************
\brief MAC_PHY_HWD handler that is called Force Run Task
******************************************************************************/
void MAC_PHY_HWD_ForceTaskHandler(void);

/******************************************************************************
                        Inline static functions prototypes section.
******************************************************************************/
/******************************************************************************
 TBD.
******************************************************************************/
INLINE void MACHWD_PostTask(void)
{
  ATOMIC_SECTION_ENTER
  if (!__wasPostTask)
  {
    SYS_PostTask(MAC_PHY_HWD_TASK_ID);
    __wasPostTask = true;
  }
  ATOMIC_SECTION_LEAVE
}

#endif /* _MACHWDMANAGER_H */

// eof machwdManager.h
