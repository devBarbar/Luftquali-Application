/**************************************************************************//**
  \file  timer.c

  \brief Functions to manipulate by timers list.

  \author
      Atmel Corporation: http://www.atmel.com \n
      Support email: avr@atmel.com

    Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
    Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
    History:
      2/08/12 N. Fomin - Created
 ******************************************************************************/

/******************************************************************************
                   Includes section
******************************************************************************/
#include <bcTimer.h>

/******************************************************************************
                   Implementations section
******************************************************************************/
/**************************************************************************//**
\brief Adds timer to the timer's list

\param[in] head - pointer to a pointer to the head of the timers list;
\param[in] timer - pointer to a timer that must be added to the list;
\param[in] sysTime - current time
******************************************************************************/
void halAddTimer(Timer_t **head, Timer_t *newTimer, uint32_t sysTime)
{
  if (!*head)
  {
    *head = newTimer;
    return;
  }

  Timer_t *it, *prev = NULL;
  for (it = *head; it; it = it->service.next)
  {
    uint64_t remain;
    if (it->service.sysTimeLabel <= sysTime)
      remain = (uint64_t)(it->service.sysTimeLabel) + it->interval - sysTime;
    else
      remain = (uint64_t)it->interval - (~it->service.sysTimeLabel + 1) - sysTime;
    if ((remain <= UINT32_MAX) && (remain >= newTimer->interval))
      break;
    prev = it;
  }
  if (it == *head)
  {
    newTimer->service.next = *head;
    *head = newTimer;
  }
  else
  {
    prev->service.next = newTimer;
    newTimer->service.next = it;
  }
}

/**************************************************************************//**
\brief Removes timer from the timers list

\param[in] head - pointer to a pointer to the head of the timers list;
\param[in] prev - pointer to a timer that is previous to timer "p"
\param[in] p - pointer to a timer to be removed from the list

\returns pointer to next timer cell or
  pointer to the head of the timer list
******************************************************************************/
Timer_t* halRemoveTimer(Timer_t **head, Timer_t *prev, Timer_t *p)
{
  Timer_t *t;
  if (p == *head)
  {// removing first element of list
    t = p->service.next;
    p->service.next = 0;
    *head = t;
    return *head;
  }
  else
  {
    prev->service.next = p->service.next;
    p->service.next = 0;
    return prev->service.next;
  }
}

/**************************************************************************//**
\brief The search of the timer that is previous to timer "p" in the timers list

\param[in] head - pointer to a pointer to the head of the timers list;
\param[in] p - pointer to a timer

\returns pointer to a searched timer
******************************************************************************/
Timer_t *halFindPrevTimer(Timer_t **head,  Timer_t *p)
{
  Timer_t *t = *head;

  for (; t ;)
  {
    if (t->service.next == p)
      return t;
    t = t->service.next;
  }
  return NULL;
}

//eof timer.c
