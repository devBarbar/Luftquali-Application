/**************************************************************************//**
  \file app.h

  \brief Usart-Anwendung Headerdatei.

  \author
    Markus Krauﬂe

******************************************************************************/

#ifndef _APP_H
#define _APP_H

#define APP_SENDE_INTERVAL    1000

BEGIN_PACK
typedef struct _AppMessage_t{
	uint8_t header[APS_ASDU_OFFSET]; //APS header
	uint8_t data[6];
	uint8_t footer[APS_AFFIX_LENGTH - APS_ASDU_OFFSET]; // Footer
} PACK AppMessage_t;
END_PACK

typedef enum{
	APP_INIT,
	APP_STARTJOIN_NETWORK,
	APP_INIT_ENDPOINT,
	APP_NOTHING
} AppState_t;
#endif
// eof app.h