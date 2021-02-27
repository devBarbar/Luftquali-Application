/*****************************************************************************
  \file stdioAdapter.c

  \brief  Serial port manager for various connection type.

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).


  History:
    16/01/13 S. Dmitriev - Created
******************************************************************************/
/******************************************************************************
                   Includes section
******************************************************************************/
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdioAdapter.h>
#include <halTaskManager.h>
#include <appTimer.h>

/******************************************************************************
                   Define(s) section
******************************************************************************/
#define SHOT_INSTANT 1

/***********************************************************************************
                          Local variables section
***********************************************************************************/
static HAL_UsartDescriptor_t *stdioDescr = NULL;
static HAL_AppTimer_t txDone;

/*******************************************************************************
                      Implementation section
*******************************************************************************/
/**************************************************************************//**
\brief Init stdio adapter

\param[in]
  descriptor - pointer to HAL_UsartDescriptor_t structure

\returns positive usart descriptor on success or -1 in cases: \n
    - the channel was already opened. \n
    - pointer to descriptor is equal to NULL. \n
******************************************************************************/
int HAL_OpenStdio(HAL_UsartDescriptor_t *descriptor)
{
  if (NULL == descriptor)
    return -1;
  if (NULL != stdioDescr)
    return -1;

  _setmode(_fileno(stdout), _O_BINARY);

  stdioDescr = descriptor;

  txDone.interval = SHOT_INSTANT;
  txDone.mode = TIMER_ONE_SHOT_MODE;
  txDone.callback = descriptor->txCallback;

  if (NULL == descriptor->rxBuffer)
    descriptor->rxBufferLength = 0;
  if (NULL == descriptor->txBuffer)
    descriptor->txBufferLength = 0;

  descriptor->service.rxPointOfRead = 0;
  descriptor->service.rxPointOfWrite = 0;
  return 1;
}

/*************************************************************************//**
\brief Releases the usart channel and pins, if hardware flow control was used.

\ingroup hal_usart

\param[in]
  descriptor - pointer to HAL_UsartDescriptor_t structure

\return
 -1 - bad descriptor or channel is already closed; \n
  0 - success.
*****************************************************************************/
int HAL_CloseStdio(HAL_UsartDescriptor_t *descriptor)
{
  if (NULL == descriptor)
    return -1;
  if (descriptor != stdioDescr)
    return -1;

  stdioDescr = NULL;

  return 0;
}

/*************************************************************************//**
\brief Reads a number of bytes from stdio and places them to the buffer.

\ingroup hal_usart

\param[in]
  descriptor - pointer to HAL_UsartDescriptor_t structure;

\param[in]
  buffer - pointer to the application buffer;

\param[in]
  length - number of bytes to be placed to the buffer;

\return
  -1 - bad descriptor, or bad number of bytes to read; \n
  Number of bytes placed to the buffer - success.
*****************************************************************************/
int HAL_ReadStdin(HAL_UsartDescriptor_t *descriptor, uint8_t *buffer, uint16_t length)
{
  uint16_t wasRead = 0;

  if (NULL == descriptor)
    return -1;
  if (stdioDescr != descriptor)
    return -1;
  if (!buffer || !length)
    return -1;

  while(!feof(stdin) && (wasRead < length))
  {
    buffer[wasRead] = fgetc(stdin);
    wasRead++;
  }

  return wasRead;
}

/**************************************************************************//**
\brief Writes a number of bytes to stdout.

\param[in]
  descriptor - pointer to HAL_UsartDescriptor_t structure;

\param[in]
  buffer - pointer to the application data buffer;

\param[in]
  length - number of bytes to transfer;

\return
  -1 - bad descriptor; \n
   Number of bytes placed to the buffer - success.
******************************************************************************/
int HAL_WriteStdout(HAL_UsartDescriptor_t *descriptor, uint8_t *buffer, uint16_t length)
{
  uint8_t *por = buffer;
  uint16_t wasWrite = 0;

  if (NULL == descriptor)
    return -1;
  if (stdioDescr != descriptor)
    return -1;
  if (!buffer || !length)
    return -1;

  while ((!ferror(stdout)) && (wasWrite < length))
  {
    fputc(*por++, stdout);
    wasWrite++;
  }
  fflush(stdout);

  HAL_StartAppTimer(&txDone);

  return wasWrite;
}