/******************************************************************************
  \file phyEd.h

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
    History:
      26/05/07 A. Mandychev - Created.
******************************************************************************/

#ifndef _PHYED_H
#define _PHYED_H

/******************************************************************************
                        Includes section.
******************************************************************************/
#include <phy.h>
#include <atmega128rfa1.h>
#include <phyCommon.h>
#include <machwdEdHandler.h>

/******************************************************************************
                        Prototypes section.
******************************************************************************/
/******************************************************************************
  Energy detect request
******************************************************************************/
PHY_ReqStatus_t PHY_EdReq(void);

/******************************************************************************
                 Inline static functions prototypes section.
******************************************************************************/
/******************************************************************************
  Energy detect handler
******************************************************************************/
INLINE void PHY_EdHandler(void)
{
  PHY_EdConf((PHY_EnergyLevel_t)PHY_ED_LEVEL_REG);
}

#endif /* _PHYED_H */

// eof phyEd.h
