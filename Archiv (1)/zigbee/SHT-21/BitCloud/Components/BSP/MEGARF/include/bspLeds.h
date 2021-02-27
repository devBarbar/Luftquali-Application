/***************************************************************************//**
\file  bspLeds.h

\brief Declaration of leds defines.

\author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

\internal
  History:
    26.08.09 A. Taradov - Created
*******************************************************************************/

#ifndef _BSPLEDS_H
#define _BSPLEDS_H

/******************************************************************************
                   Includes section
******************************************************************************/
// \cond
#include <gpio.h>
#include <leds.h>
// \endcond

/******************************************************************************
                   Define(s) section
******************************************************************************/

#define halInitFirstLed()       GPIO_B7_make_out()
#define halUnInitFirstLed()     GPIO_B7_make_in()
#define halOnFirstLed()         GPIO_B7_set()
#define halOffFirstLed()        GPIO_B7_clr()
#define halReadFirstLed()       GPIO_B7_read()
#define halToggleFirstLed()     GPIO_B7_toggle()
// Second LED
#define halInitSecondLed()      GPIO_B6_make_out()
#define halUnInitSecondLed()    GPIO_B6_make_in()
#define halOnSecondLed()        GPIO_B6_set()
#define halOffSecondLed()       GPIO_B6_clr()
#define halReadSecondLed()      GPIO_B6_read()
#define halToggleSecondLed()    GPIO_B6_toggle()
// Third LED
#define halInitThirdLed()       GPIO_B4_make_out()
#define halUnInitThirdLed()     GPIO_B4_make_in()
#define halOnThirdLed()         GPIO_B4_set()
#define halOffThirdLed()        GPIO_B4_clr()
#define halReadThirdLed()       GPIO_B4_read()
#define halToggleThirdLed()     GPIO_B4_toggle()
#endif /*_BSPLEDS_H*/
// eof bspLeds.h
