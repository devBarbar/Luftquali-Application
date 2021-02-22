/**************************************************************************//**
  \file app.c

  \brief Basis-Anwendung.

  \author Markus Krauﬂe

******************************************************************************/


#include <appTimer.h>
#include <zdo.h>
#include <app.h>
#include <sysTaskManager.h>
#include <usartManager.h>
#include <bspLeds.h>

HAL_AppTimer_t receiveTimerLed;

static AppState_t appstate = APP_INIT;
APS_DataReq_t dataReq;
					  //1 234567 890123456 7890 1234
uint8_t uart_output[]= "{\"panid\":0xxxxx,\"UID\":0xxxxx,\"role\":\"Co\"}\n\r";

static uint8_t deviceType;


static ZDO_StartNetworkReq_t networkParams;
static SimpleDescriptor_t simpleDescriptor;
static APS_RegisterEndpointReq_t endPoint;

static void ZDO_StartNetworkConf(ZDO_StartNetworkConf_t *confirmInfo);
static void initEndpoint(void);
static void receiveTimerLedFired(void);
static void initTimer(void);

void APS_DataInd(APS_DataInd_t *indData);

static void initEndpoint(void){
	simpleDescriptor.AppDeviceId = 1;
	simpleDescriptor.AppProfileId = 1;
	simpleDescriptor.endpoint = 1;
	simpleDescriptor.AppDeviceVersion = 1;
	endPoint.simpleDescriptor= &simpleDescriptor;
	endPoint.APS_DataInd = APS_DataInd;
	APS_RegisterEndpointReq(&endPoint);
}

static void initTimer(void){
	receiveTimerLed.interval= 500;
	receiveTimerLed.mode= TIMER_ONE_SHOT_MODE;
	receiveTimerLed.callback=receiveTimerLedFired;
}

static void receiveTimerLedFired(void){
	BSP_OffLed(LED_RED);
}

void APS_DataInd(APS_DataInd_t *indData){
	BSP_OnLed(LED_RED);
	HAL_StartAppTimer(&receiveTimerLed);
	
	appWriteDataToUsart(indData->asdu,indData->asduLength);
	appWriteDataToUsart((uint8_t*)"\r\n",2);
}

void APL_TaskHandler(void){
	switch(appstate){
		
		case APP_INIT:
		
			appInitUsartManager();
			initTimer();
			BSP_OpenLeds();
			appstate=APP_STARTJOIN_NETWORK;
			SYS_PostTask(APL_TASK_ID);
		break;
		
		case APP_STARTJOIN_NETWORK:
		
			networkParams.ZDO_StartNetworkConf = ZDO_StartNetworkConf;
			ZDO_StartNetworkReq(&networkParams);
			appstate=APP_INIT_ENDPOINT;
		break;
		
		case APP_INIT_ENDPOINT:
		
			initEndpoint();
			appstate=APP_NOTHING;
			SYS_PostTask(APL_TASK_ID);
			
		break;
		
		case APP_NOTHING:
		break;
		}
	}
void ZDO_StartNetworkConf(ZDO_StartNetworkConf_t *confirmInfo){
	
	if (ZDO_SUCCESS_STATUS == confirmInfo->status){
		
		CS_ReadParameter(CS_DEVICE_TYPE_ID,&deviceType);
		
		if(deviceType==DEV_TYPE_COORDINATOR){
			BSP_OnLed(LED_RED); //Red Green is Coordinator
			BSP_OnLed(LED_GREEN);
			
			uint16_to_hexstr(uart_output, sizeof(uart_output), CS_NWK_PANID, 11);
			uint16_to_hexstr(uart_output, sizeof(uart_output), CS_UID,24);
			appWriteDataToUsart(uart_output, sizeof(uart_output));
		}

	}
	else{
		
	appWriteDataToUsart((uint8_t*)"Error\r\n",sizeof("Error\r\n")-1);
	}
	
	SYS_PostTask(APL_TASK_ID);
}

/*******************************************************************************
  \brief The function is called by the stack to notify the application about 
  various network-related events. See detailed description in API Reference.
  
  Mandatory function: must be present in any application.

  \param[in] nwkParams - contains notification type and additional data varying
             an event
  \return none
*******************************************************************************/
void ZDO_MgmtNwkUpdateNotf(ZDO_MgmtNwkUpdateNotf_t *nwkParams)
{
  nwkParams = nwkParams;  // Unused parameter warning prevention
}

/*******************************************************************************
  \brief The function is called by the stack when the node wakes up by timer.
  
  When the device starts after hardware reset the stack posts an application
  task (via SYS_PostTask()) once, giving control to the application, while
  upon wake up the stack only calls this indication function. So, to provide 
  control to the application on wake up, change the application state and post
  an application task via SYS_PostTask(APL_TASK_ID) from this function.

  Mandatory function: must be present in any application.
  
  \return none
*******************************************************************************/
void ZDO_WakeUpInd(void)
{
}

#ifdef _BINDING_
/***********************************************************************************
  \brief The function is called by the stack to notify the application that a 
  binding request has been received from a remote node.
  
  Mandatory function: must be present in any application.

  \param[in] bindInd - information about the bound device
  \return none
 ***********************************************************************************/
void ZDO_BindIndication(ZDO_BindInd_t *bindInd)
{
  (void)bindInd;
}

/***********************************************************************************
  \brief The function is called by the stack to notify the application that a 
  binding request has been received from a remote node.

  Mandatory function: must be present in any application.
  
  \param[in] unbindInd - information about the unbound device
  \return none
 ***********************************************************************************/
void ZDO_UnbindIndication(ZDO_UnbindInd_t *unbindInd)
{
  (void)unbindInd;
}
#endif //_BINDING_

/**********************************************************************//**
  \brief The entry point of the program. This function should not be
  changed by the user without necessity and must always include an
  invocation of the SYS_SysInit() function and an infinite loop with
  SYS_RunTask() function called on each step.

  \return none
**************************************************************************/
int main(void)
{
  //Initialization of the System Environment
  SYS_SysInit();

  //The infinite loop maintaing task management
  for(;;)
  {
    //Each time this function is called, the task
    //scheduler processes the next task posted by one
    //of the BitCloud components or the application
    SYS_RunTask();
  }
}

//eof app.c
