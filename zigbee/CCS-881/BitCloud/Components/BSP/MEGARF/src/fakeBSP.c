/***************************************************************************//**
\file  fakeBSP.c

\brief Implementation of fake board-specific periphery.

\author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

\internal
  History:
    05/05/11 A. Malkin - Created
*******************************************************************************/

/******************************************************************************
                   Includes section
******************************************************************************/
#include <sysTypes.h>
#include <sysAssert.h>
#include <bspTaskManager.h>
#include <bspDbg.h>

/******************************************************************************
                   Local variables section
******************************************************************************/
static void(* readTemperatureDataCallback)(bool error, int16_t data);
static void(* readLightDataCallback)(bool error, int16_t data);
static void(* readBatteryDataCallback)(int16_t data);

/******************************************************************************
                   Implementations section
******************************************************************************/
#if BSP_SUPPORT == BOARD_FAKE
/******************************************************************************
                             Leds
******************************************************************************/
/**************************************************************************//**
\brief Opens leds module to use.

\return
    operation state
******************************************************************************/
result_t BSP_OpenLeds(void)
{
  return BC_SUCCESS;
}

/**************************************************************************//**
\brief Closes leds module.

\return
    operation state
******************************************************************************/
result_t BSP_CloseLeds(void)
{
  return BC_SUCCESS;
}

/**************************************************************************//**
\brief Turns on the LED.

\param[in]
    id - number of led
******************************************************************************/
void BSP_OnLed(uint8_t id)
{
  (void)id;
}

/**************************************************************************//**
\brief Turns off the LED.

\param[in]
      id - number of led
******************************************************************************/
void BSP_OffLed(uint8_t id)
{
  (void)id;
}

/**************************************************************************//**
\brief Changes the LED state to opposite.

\param[in]
      id - number of led
******************************************************************************/
void BSP_ToggleLed(uint8_t id)
{
  (void)id;
}

/******************************************************************************
 Returns number which was read from external eeprom.
 Parameters:
   id - UID buffer pointer.
 Returns:
   0 - if unique ID has been found without error;
  -1 - if there are some erros during UID discovery.
******************************************************************************/
int BSP_ReadUid(uint64_t *id)
{
  (void)id;
  return 0;
}
#endif

#if BSP_SUPPORT == BOARD_FAKE || BSP_SUPPORT == BOARD_RCB_KEY_REMOTE
/******************************************************************************
                             Buttons
******************************************************************************/
/**************************************************************************//**
\brief Registers handlers for button events.

\param[in]
    pressed - the handler to process pressing the button
\param[in]
    released - the handler to process releasing the button
\param[in]
    bn - button number.

\return
  BC_SUCCESS - always.
******************************************************************************/
result_t BSP_OpenButtons(void (*pressed)(uint8_t bn), void (*released)(uint8_t bn))
{
  (void)pressed;
  (void)released;

  return BC_SUCCESS;
}

/**************************************************************************//**
\brief Cancel buttons handlers.

\return
  BC_SUCCESS - always.
******************************************************************************/
result_t BSP_CloseButtons(void)
{
  return BC_SUCCESS;
}

/**************************************************************************//**
\brief Reads state of buttons.

\return
    Current buttons state in a binary way. \n
    Bit 0 defines state of the button 1, \n
    bit 1 defines state of the button 2.
******************************************************************************/
uint8_t BSP_ReadButtonsState(void)
{
  return 0;
}

#endif

#if BSP_SUPPORT == BOARD_FAKE
/**************************************************************************//**
\brief Initialize LEDs.
******************************************************************************/
void BSP_InitLeds(void)
{
}

/**************************************************************************//**
\brief Set brightness level
\param[in] level - new brightness level
******************************************************************************/
void BSP_SetLedBrightnessLevel(uint8_t level)
{
  (void)level;
}
#endif

#if BSP_SUPPORT == BOARD_FAKE || BSP_SUPPORT != BOARD_RCB_KEY_REMOTE


/******************************************************************************
                             Leds Extention
******************************************************************************/
/**************************************************************************//**
\brief Set the color using Hue and Saturation as parameters
\param[in] level - a new brightness level
******************************************************************************/
void BSP_SetLedColorHS(uint16_t hue, uint8_t saturation)
{
  (void)hue;
  (void)saturation;
}

/**************************************************************************//**
\brief Set the color using X and Y as parameters
\param[in] level - a new brightness level
******************************************************************************/
void BSP_SetLedColorXY(uint16_t x, uint16_t y)
{
  (void)x;
  (void)y;
}

/******************************************************************************
                             Lcd Extention
******************************************************************************/
/**************************************************************************//**
\brief Clear screen
******************************************************************************/
void BSP_LcdCls(void)
{
}

/**************************************************************************//**
\brief Print a formatted string on the LCD
******************************************************************************/
void BSP_LcdPrint(uint8_t x, uint8_t y, const char *format, ...)
{
  (void)x;
  (void)y;
  (void)format;
}

/**************************************************************************//**
\brief Fill the LCD screen (using internal test command)
******************************************************************************/
void BSP_LcdFill(bool fill)
{
  (void)fill;
}

/**************************************************************************//**
\brief Initialize the LCD
******************************************************************************/
void BSP_LcdInit(void)
{
}
#endif // BSP_SUPPORT == BOARD_FAKE

/******************************************************************************
                             Sensors
******************************************************************************/
/***************************************************************************//**
\brief Opens temperature sensor.

\return
  BC_SUCCESS - always.
*******************************************************************************/
result_t BSP_OpenTemperatureSensor(void)
{
  return BC_SUCCESS;
}

/***************************************************************************//**
\brief Closes the temperature sensor.

\return
  BC_SUCCESS - always.
*******************************************************************************/
result_t BSP_CloseTemperatureSensor(void)
{
  return BC_SUCCESS;
}

/**************************************************************************//**
\brief Reads data from the temperature sensor.
\param[in]
    result - the result of the requested operation.
             true - operation finished successfully, false - some error has
             occured.
\param[in]
    data - sensor data.

\return
  BC_SUCCESS - always.
******************************************************************************/
result_t BSP_ReadTemperatureData(void (*f)(bool result, int16_t data))
{
  readTemperatureDataCallback = f;

  bspPostTask(BSP_TEMPERATURE);

  return BC_SUCCESS;
}

/**************************************************************************//**
\brief BSP task handler for temperature sensor.
******************************************************************************/
void bspTemperatureSensorHandler(void)
{
  SYS_E_ASSERT_FATAL(readTemperatureDataCallback, BSPSENSORS_TEMPERATURENULLCALLBACK_0);
  readTemperatureDataCallback(true, 0);
}

/***************************************************************************//**
\brief Opens the light sensor.

\return
  BC_SUCCESS - always.
*******************************************************************************/
result_t BSP_OpenLightSensor(void)
{
  return BC_SUCCESS;
}

/***************************************************************************//**
\brief Closes the light sensor.

\return
  BC_SUCCESS - always.
*******************************************************************************/
result_t BSP_CloseLightSensor(void)
{
  return BC_SUCCESS;
}

/**************************************************************************//**
\brief Reads data from the light sensor.
\param[in]
    result - the result of the requested operation.
             true - operation finished successfully, false - some error has
             occured.
\param[in]
    data - sensor data.

\return
  BC_SUCCESS - always.
******************************************************************************/
result_t BSP_ReadLightData(void (*f)(bool result, int16_t data))
{
  readLightDataCallback = f;

  bspPostTask(BSP_LIGHT);

  return BC_SUCCESS;
}

/**************************************************************************//**
\brief BSP task handler for light sensor.
******************************************************************************/
void bspLightSensorHandler(void)
{
  SYS_E_ASSERT_FATAL(readLightDataCallback, BSPSENSORS_LIGHTNULLCALLBACK_0);
  readLightDataCallback(true, 0);
}

/***************************************************************************//**
\brief Opens the battery sensor.

\return
  BC_SUCCESS - always.
*******************************************************************************/
result_t BSP_OpenBatterySensor(void)
{
  return BC_SUCCESS;
}

/***************************************************************************//**
\brief Closes the battery sensor.

\return
  BC_SUCCESS - always.
*******************************************************************************/
result_t BSP_CloseBatterySensor(void)
{
  return BC_SUCCESS;
}

/**************************************************************************//**
\brief Reads data from battery sensor.

\param[in]
    data - sensor data.

\return
  BC_SUCCESS - always.
******************************************************************************/
result_t BSP_ReadBatteryData(void (*f)(int16_t data))
{
  readBatteryDataCallback = f;

  bspPostTask(BSP_BATTERY);

  return BC_SUCCESS;
}

/**************************************************************************//**
\brief BSP task handler for battery sensor.
******************************************************************************/
void bspEmptyBatteryHandler(void)
{
  SYS_E_ASSERT_FATAL(readBatteryDataCallback, BSPSENSORS_BATTERYNULLCALLBACK_0);
  readBatteryDataCallback(0);
}

/******************************************************************************
                             Sliders
******************************************************************************/
/**************************************************************************//**
\brief Reads the sliders.

\return
   state of 3 on-board DIP-switches.User can uses SLIDER0, SLIDER1, SLIDER2
   defines to test state. Value 1 indicates that slider is on.
******************************************************************************/
uint8_t BSP_ReadSliders(void)
{
  return 0;
}

// eof fakeBSP.c
