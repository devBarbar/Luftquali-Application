/**************************************************************************//**
  \file appTimer.c

  \brief
    Implementation of appTimer.

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
    History:
    21.05.12 N. Fomin - Created.
******************************************************************************/
/******************************************************************************
                    Includes section
******************************************************************************/
#include <stdio.h>
#include <appTimer.h>
#include <halDbg.h>
#include <sysAssert.h>

/******************************************************************************
                    Global prototypes section
******************************************************************************/
void simulateAppTimer(void);

/******************************************************************************
                    Prototypes section
******************************************************************************/
static bool isTimerAlreadyStarted(HAL_AppTimer_t *appTimer);

/******************************************************************************
                   External global variables section
******************************************************************************/
extern uint8_t halAppTimeOvfw;

/******************************************************************************
                    Local variables
******************************************************************************/
static HAL_AppTimer_t *halAppTimerHead = NULL;

/******************************************************************************
                    Implementation section
******************************************************************************/
/******************************************************************************
Interrupt handler of appTimer clock.
******************************************************************************/
void halAppTimerHandler(void)
{
  uint32_t sysTime;

  // search for expired timers and call their callbacks
  while (halAppTimerHead
      && ((sysTime = halGetTimeOfAppTimer()) - halAppTimerHead->service.sysTimeLabel) >= halAppTimerHead->interval)
  {
    HAL_AppTimer_t *p = halAppTimerHead;
    halRemoveTimer(&halAppTimerHead, NULL, p);
    if (TIMER_REPEAT_MODE == p->mode)
    {
      p->service.sysTimeLabel = sysTime;
      halAddTimer(&halAppTimerHead, p, sysTime);
    }

    SYS_E_ASSERT_FATAL(p->callback, APPTIMER_HANDLER_0);
    p->callback();
  }
}

/**************************************************************************//**
\brief Starts a timer

\param[in] appTimer - pointer to a timer

\returns -1 if pointer is NULL, 0 otherwise
******************************************************************************/
int HAL_StartAppTimer(HAL_AppTimer_t *appTimer)
{
  uint32_t sysTime;

  if (!appTimer)
    return -1;

  if (true == isTimerAlreadyStarted(appTimer))
    return 0;

  sysTime = halGetTimeOfAppTimer();
  appTimer->service.next = NULL;
  appTimer->service.sysTimeLabel = sysTime;
  halAddTimer((Timer_t**)(&halAppTimerHead), (Timer_t*)appTimer, sysTime);
  return 0;
}

/**************************************************************************//**
\brief Stop a timer

\param[in] appTimer - pointer to a timer

\returns -1 if a timer was not started, 0 otherwise
******************************************************************************/
int HAL_StopAppTimer(HAL_AppTimer_t *appTimer)
{
  Timer_t *prev = 0;
  Timer_t **t = &appTimer;

  if (!appTimer)
    return -1;
  if (halAppTimerHead != *t)
  {
    if (!(prev = halFindPrevTimer((Timer_t**)(&halAppTimerHead), appTimer)))
      return -1;  // This timer is not in the list
  }
  halRemoveTimer((Timer_t**)(&halAppTimerHead), prev, appTimer);
  return 0;
}

/**************************************************************************//**
\brief Checks if timer is already started

\param[in] appTimer - pointer to a timer

\returns  true - timer specified already started and presents in the system timers queue
          false - timer is't started yet
******************************************************************************/
static bool isTimerAlreadyStarted(HAL_AppTimer_t *appTimer)
{
  bool result = false;
  Timer_t *p; // p is bottom of list
  p = halAppTimerHead;

  while (NULL != p)
  {
    if (p == appTimer)
    {
      SYS_E_ASSERT_ERROR(false, APPTIMER_MISTAKE);
      return true;
    }
    p = (Timer_t *)p->service.next;
  }
  return result;
}

/**************************************************************************//**
\brief Gets system time

\return time since power up in milliseconds(8 bytes)
******************************************************************************/
BcTime_t HAL_GetSystemTime(void)
{
  BcTime_t sysTime = 0ull;

  sysTime = halGetTimeOfAppTimer();
  sysTime |= ((BcTime_t)halAppTimeOvfw << 32);

  return sysTime;
}
//eof appTimer.c
