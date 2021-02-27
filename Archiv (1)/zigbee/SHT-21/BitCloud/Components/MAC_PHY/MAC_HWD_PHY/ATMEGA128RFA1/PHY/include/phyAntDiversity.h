/**************************************************************************//**
  \file phyAntDiversity.h

  \brief Prototype of antenna diversity functions.

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
    History:
      11/11/10 A. Malkin - Created.
******************************************************************************/
#ifndef _PHYANTDIVERSITY_H
#define _PHYANTDIVERSITY_H

/******************************************************************************
                    Includes section
******************************************************************************/
#include <sysTypes.h>
#include <phyCommon.h>

/******************************************************************************
                    Define(s) section
******************************************************************************/
// ANT_EXT_SW_EN values of ANT_DIV_REG
#define ANT_EXT_SW_ENABLED   0x04
#define ANT_EXT_SW_DISABLED  0x00

// ANT_CTRL values of ANT_DIV_REG
#define ANT_CTRL_DEFAULT    0x03
#define ANT_CTRL_ANTENNA_0  0x02
#define ANT_CTRL_ANTENNA_1  0x01

// RX_CTRL_REG values
#define PDT_THRES_ANT_DIV_ENABLED   0x03
#define PDT_THRES_DEFAULT           0x07

/******************************************************************************
                    Prototypes section
******************************************************************************/
/**************************************************************************//**
\brief  Enable antenna diversity feature.
******************************************************************************/
void phyAntennaDiversityInit(void);

/**************************************************************************//**
\brief  Enable antenna diversity in the receive state of the radio transceiver.
******************************************************************************/
void phyRxAntennaDiversity(void);

/**************************************************************************//**
\brief  Enable antenna diversity in the transmit state of the radio transceiver.
******************************************************************************/
void phyTxAntennaDiversity(void);

/**************************************************************************//**
\brief  Turn on pin 9 (DIG1) and pin 10 (DIG2) to enable antenna select.
******************************************************************************/
void phyAntennaSwitcherOn(void);

/**************************************************************************//**
\brief  Turn off pin 9 (DIG1) and pin 10 (DIG2) to disable antenna select.
******************************************************************************/
void phyAntennaSwitcherOff(void);

/**************************************************************************//**
\brief  Store the Antenna used for recent Tx
******************************************************************************/
void phyStoreAntennaUsedForTx(void);

/**************************************************************************//**
\brief  Read the antenna used for last transmission and switch to other antenna 
******************************************************************************/
void phySwitchLastTxAntenna(void);

/**************************************************************************//**
\brief  Allow antenna diversity for next Transmission
******************************************************************************/
void phyAllowAntDivForTx(void);

/**************************************************************************//**
\brief  read the antenna used for last transmission
******************************************************************************/
uint8_t PHY_ReadLastAntennaForTx(void);

/**************************************************************************//**
\brief  Select antenna 1/2 for next transmission or reception.
******************************************************************************/
int8_t PHY_SelectAntenna(uint8_t antenna);

#endif /* _PHYANTDIVERSITY_H */

// eof phyAntDiversity.h
