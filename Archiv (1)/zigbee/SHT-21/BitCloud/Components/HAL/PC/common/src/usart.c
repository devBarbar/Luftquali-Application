/*****************************************************************************
  \file pccomAdapter.c

  \brief  Serial port manager for various connection type.

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).


  History:
******************************************************************************/
/******************************************************************************
                   Includes section
******************************************************************************/
#include <stdio.h>
#include <usart.h>
#include <halTaskManager.h>

/******************************************************************************
                   Define(s) section
******************************************************************************/
// Reserve space for 4 packets
#define MAX_PACKET_SIZE 254
#define MAX_RX_BUF_SIZE (MAX_PACKET_SIZE * 4)

// Start of frame marker
#define SOF 0x2A

#define HANDLERS_GET(A, I) memcpy_P(A, &halUsartHandlers[I], sizeof(HalUsartTask_t))

/******************************************************************************
                   Types section
******************************************************************************/
typedef volatile uint8_t HalUsartTaskBitMask_t;
typedef void (* HalUsartTask_t)(void);

/******************************************************************************
                   Prototype(s) section
******************************************************************************/
static void halUsartTaskUsartTxc(void);
static void halUsartTaskUsartRxc(void);

/***********************************************************************************
                          Local variables section
***********************************************************************************/
static HAL_UsartDescriptor_t *usartDescr = NULL;
static volatile HalUsartTaskBitMask_t halUsartTaskBitMask = 0;
static PROGMEM_DECLARE(HalUsartTask_t halUsartHandlers[HAL_USART_TASKS_NUMBER]) =
{
  halUsartTaskUsartTxc,
  halUsartTaskUsartRxc
};

/*******************************************************************************
                      Implementation section
*******************************************************************************/
/**************************************************************************//**
\brief Opens USART interface

\param[in]
  descriptor - pointer to HAL_UsartDescriptor_t structure

\returns positive usart descriptor on success or -1 in cases: \n
    - the channel was already opened. \n
    - pointer to descriptor is equal to NULL. \n
******************************************************************************/
int HAL_OpenUsart(HAL_UsartDescriptor_t *descriptor)
{
  if (NULL == descriptor)
    return -1;
  if (NULL != usartDescr)
    return -1;

  halOpenUsart(descriptor->baudrate, descriptor->dataLength, descriptor->stopbits, descriptor->parity, descriptor->tty);

  usartDescr = descriptor;

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
int HAL_CloseUsart(HAL_UsartDescriptor_t *descriptor)
{
  if (NULL == descriptor)
    return -1;
  if (descriptor != usartDescr)
    return -1;

  usartDescr = NULL;

  return 0;
}

/*************************************************************************//**
\brief Reads a number of bytes from usart and places them to the buffer.

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
int HAL_ReadUsart(HAL_UsartDescriptor_t *descriptor, uint8_t *buffer, uint16_t length)
{
  uint16_t           wasRead = 0;
  uint16_t           poW;
  uint16_t           poR;
  HalUsartService_t *halUsartControl;

  if (NULL == descriptor)
    return -1;
  if (usartDescr != descriptor)
    return -1;
  if (!buffer || !length)
    return -1;

  halUsartControl = &usartDescr->service;
  ATOMIC_SECTION_ENTER
    poW = halUsartControl->rxPointOfWrite;
    poR = halUsartControl->rxPointOfRead;
  ATOMIC_SECTION_LEAVE

  while ((poR != poW) && (wasRead < length))
  {
    buffer[wasRead] = descriptor->rxBuffer[poR];
    if (++poR == descriptor->rxBufferLength)
      poR = 0;
    wasRead++;
  }

  ATOMIC_SECTION_ENTER
    halUsartControl->rxPointOfRead = poR;
    halUsartControl->rxBytesInBuffer -= wasRead;
  ATOMIC_SECTION_LEAVE

  return wasRead;
}

/**************************************************************************//**
\brief Writes a number of bytes to a usart channel.
txCallback function will be used to notify when the transmission is finished.
If hardware flow control is used for transmitting then RTS and DTR pins will
be tested during transmission.

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
int HAL_WriteUsart(HAL_UsartDescriptor_t *descriptor, uint8_t *buffer, uint16_t length)
{
  if (NULL == descriptor)
    return -1;
  if (usartDescr != descriptor)
    return -1;
  if (!buffer || !length)
    return -1;

  halWriteUsartData(buffer, length);

  return length;
}

/**************************************************************************//**
\brief Posts specific USART task

\param[in] taskId - unique identifier of the task to be posted
******************************************************************************/
void halPostUsartTask(HalUsartTaskId_t taskId)
{
  halUsartTaskBitMask |= (HalUsartTaskBitMask_t)1 << taskId;
  halPostTask(HAL_TASK_USART);
}

/**************************************************************************//**
\brief  HAL USART task. Exact action depends on USART internal task.
******************************************************************************/
void halUsartHandler(void)
{
  HalUsartTask_t         handler;
  HalUsartTaskBitMask_t  mask = 1;
  HalUsartTaskId_t       index = 0;

  for ( ; index < HAL_USART_TASKS_NUMBER; index++, mask <<= 1)
  {
    if (halUsartTaskBitMask & mask)
    {
      ATOMIC_SECTION_ENTER
      halUsartTaskBitMask ^= mask;
      ATOMIC_SECTION_LEAVE
      HANDLERS_GET(&handler, index);
      handler();
    }
  }
}

/**************************************************************************//**
\brief Transmit complete handler for usart
******************************************************************************/
static void halUsartTaskUsartTxc(void)
{
  if(usartDescr->txCallback)
    usartDescr->txCallback();
}

/**************************************************************************//**
\brief Transmit complete handler for usart
******************************************************************************/
static void halUsartTaskUsartRxc(void)
{
  uint16_t number;

  ATOMIC_SECTION_ENTER
  number = usartDescr->service.rxBytesInBuffer;
  ATOMIC_SECTION_LEAVE

  if (number)
    if(usartDescr->rxCallback)
      usartDescr->rxCallback(number);
}

/**************************************************************************//**
\brief Puts received bytes to the cyclic buffer

\param[in] data   - data to put;
\param[in] length - channel number
******************************************************************************/
void halUsartRxBufferFiller(uint8_t *data, uint16_t length)
{
  uint16_t powOld;
  uint16_t cntOld;
  uint16_t i = 0;
  HalUsartService_t *halUsartControl;

ATOMIC_SECTION_ENTER
  halUsartControl = &usartDescr->service;
  if (NULL != usartDescr->rxBuffer)
  {
    powOld = halUsartControl->rxPointOfWrite;
    cntOld = halUsartControl->rxBytesInBuffer;
    for (; i < length; i++)
    {
      usartDescr->rxBuffer[halUsartControl->rxPointOfWrite++] = data[i];
      halUsartControl->rxBytesInBuffer++;
      if (halUsartControl->rxPointOfWrite == usartDescr->rxBufferLength)
        halUsartControl->rxPointOfWrite = 0;
      if (halUsartControl->rxPointOfWrite == halUsartControl->rxPointOfRead)
      { // Buffer is full.
        halUsartControl->rxPointOfWrite = powOld;
        halUsartControl->rxBytesInBuffer = cntOld;
        break;
      } // Buffer is full.
    }
  }
ATOMIC_SECTION_LEAVE
}

/**************************************************************************//**
\brief Forbids the host to transmit data.
Only USART_CHANNEL_1 can be used for hardware flow control for avr.

\ingroup hal_usart

\param[in]
  descriptor - pointer to HAL_UsartDescriptor_t structure;

\return
  -1 - bad descriptor, bad usart, or unsupported mode; \n
   0 - success.
******************************************************************************/
int HAL_OnUsartCts(HAL_UsartDescriptor_t *descriptor)
{
  (void)descriptor;
  return -1;
}

/**************************************************************************//**
\brief Allows the host to transmit data.
Only USART_CHANNEL_1 can be used for hardware flow control for avr.

\ingroup hal_usart

\param[in]
  descriptor - pointer to HAL_UsartDescriptor_t structure;

\return
  -1 - bad descriptor, bad usart, or unsupported mode; \n
   0 - success.
******************************************************************************/
int HAL_OffUsartCts(HAL_UsartDescriptor_t *descriptor)
{
  (void)descriptor;
  return -1;
}

/**************************************************************************//**
\brief Reads RTS pin state.
Only USART_CHANNEL_1 can be used for hardware flow control for avr.

\ingroup hal_usart

\param[in]
  descriptor - pointer to HAL_UsartDescriptor_t structure;

\return
  -1 - bad descriptor, bad usart, or unsupported mode; \n
   0 - RTS is low level; \n
   1 - RTS is high level;
******************************************************************************/
int HAL_ReadUsartRts(HAL_UsartDescriptor_t *descriptor)
{
  (void)descriptor;
  return -1;
}

/**************************************************************************//**
\brief Reads DTR pin state.
Only USART_CHANNEL_1 can be used for hardware flow control for avr.

\ingroup hal_usart

\param[in]
  descriptor - pointer to HAL_UsartDescriptor_t structure;

\return
  -1 - bad descriptor, bad usart, or unsupported mode; \n
   0 - DTR is low level; \n
   1 - DTR is high level;
******************************************************************************/
int HAL_ReadUsartDtr(HAL_UsartDescriptor_t *descriptor)
{
  (void)descriptor;
  return -1;
}

/**************************************************************************//**
\brief Checks the status of tx buffer.

\ingroup hal_usart

\param[in]
  descriptor - pointer to HAL_UsartDescriptor_t structure;

\return
  -1 - bad descriptor, no tx buffer; \n
   1 - tx buffer is empty; \n
   0 - tx buffer is not empty;
******************************************************************************/
int HAL_IsTxEmpty(HAL_UsartDescriptor_t *descriptor)
{
  (void)descriptor;
  return -1;
}

/**************************************************************************//**
\brief Holds execution of all tasks except related to reqiured channel.

\param[in] channel - channel to accept tasks for
******************************************************************************/
void HAL_HoldOnOthersUsartTasks(UsartChannel_t channel)
{
  (void)channel;
}

/**************************************************************************//**
\brief Accepts execution of previously holded tasks.
******************************************************************************/
void HAL_ReleaseAllHeldUsartTasks(void)
{}

//eof pccomAdapter.c
