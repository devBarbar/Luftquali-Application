/**************************************************************************//**
  \file app.h

  \brief Usart-Anwendung Headerdatei.

  \author
    Pascal Schieferstein, Aylin Erdem

******************************************************************************/

#ifndef _APP_H
#define _APP_H

#define APP_SENDE_INTERVAL 10000
//#define LM73_DEVICE_ADDRESS 0x4D

BEGIN_PACK
/* Depending on security settings, the user data 
 * size is different.
 */
typedef struct _AppMessage_t{
	uint8_t header[APS_ASDU_OFFSET]; //APS header
	
	#if CS_ZDO_SECURITY_STATUS == 0
		uint8_t data[95];
	
	#elif CS_ZDO_SECURITY_STATUS == 1
		uint8_t data[77];
	
	#elif CS_ZDO_SECURITY_STATUS == 2
		uint8_t data[59];
	
	#elif CS_ZDO_SECURITY_STATUS == 3
		uint8_t data[59];
	
	#else
		uint8_t data[6]; //fallback size
	#endif
	
	uint8_t footer[APS_AFFIX_LENGTH - APS_ASDU_OFFSET]; // Footer
} PACK AppMessage_t;
END_PACK

typedef enum{
	APP_INIT,
	
	APP_STATUS_VERTIFY_SETUP,
	APP_STATUS_VERTIFY_READ,
	APP_STATUS_DONE,
	
	APP_ERROR_SETUP,
	APP_ERROR_READ,
	APP_ERROR_DONE,
	
	APP_SW_RESET_WRITE_REG,
	APP_RESET_DONE,
	
	APP_START_MODE,
	APP_START_DONE,
	
	APP_MESSURE_MODE_SETUP,
	APP_MESSURE_MODE_SETUP_DONE,
	
	APP_ALG_RESULT_DATA_SETUP,
	APP_ALG_READ_DATA,
	
	
	APP_OUTPUT,
	
	APP_NOTHING
} AppState_t;
#endif
// eof app.h