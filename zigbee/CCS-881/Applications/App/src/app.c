/**************************************************************************//**
  \file app.c

  \brief Read HW-ID.

  \author Pascal Schieferstein, Aylin Erdem

******************************************************************************/


#include <appTimer.h>
#include <zdo.h>
#include <app.h>
#include <sysTaskManager.h>
#include <usartManager.h>
#include <usart.h>
#include <i2cPacket.h>
#include <bspLeds.h>
#include "../atmelStudio_projects/src/i2controller.c"#include "../atmelStudio_projects/src/callback.c"static HAL_AppTimer_t readTimer;static HAL_AppTimer_t sw_reset_timer;static HAL_AppTimer_t start_timer;static HAL_AppTimer_t warm_up_timer;static AppState_t appstate;static AppState_t nextstate;static ZDO_StartNetworkReq_t networkParams;
static SimpleDescriptor_t simpleDescriptor;
static AppMessage_t transmitData;static APS_RegisterEndpointReq_t endPoint;	   APS_DataReq_t dataReq;	   static uint8_t deviceType;uint8_t data[8];uint8_t STATUS[] = {0x00};uint8_t ERROR[] = {0xE0};uint8_t SET_RESET_REG[] = {0xFF};uint8_t SW_RESET_MODE[] = {0xFF, 0x11, 0xE5, 0x72, 0x8A};uint8_t MEAS_MODE_SETUP[] = {0x01, 0x20};uint8_t ALG_RESULT_DATA_MODE[] = {0x02};uint8_t START_MODE[] = {0xF4};	uint8_t output_CO2[]="CO2: 0000\r\n";
uint8_t output_TVOC[]="TVOC: 0000\r\n";
					   //			|		   |           |          |           |           |          |           |         |
					   //1 234567 890123456 7890 123456789 01234 5678901 234567 8901234 5678 90123456 7890 12345678 9012345678901234 5 6
uint8_t uart_output[] = "{\"Panid\":0xxxxx,\"NWK\":0xxxxx,\"eCO2\":xxxx,\"eTVOC\":xxxx}\n\r";
uint8_t errorString[] = "ERROR: 0xxx\r\n";

uint32_t _eCO2;
uint32_t _TVOC;bool warm_up = false;bool login_notification = true;
static void init_timer(void);
static void init_sw_reset_timer(void);
static void read_timer_fired(void);
static void warm_up_timer_fired(void);
static void callback(void);

static void ZDO_StartNetworkConf(ZDO_StartNetworkConf_t *confirmInfo);
static void initEndpoint(void);
static void APS_DataConf(APS_DataConf_t *confInfo);
static void initTransmitData(void);
static void initNetwork(void);

void APS_DataInd(APS_DataInd_t *indData);

/**************************************************************************/
static void callback(){
	fsm_callback(nextstate, &appstate);
}
static void warm_up_timer_fired(){
	/*this function is triggered once after the 20 minute warmup periode*/
	warm_up = true;
}
/**************************************************************************/
static void init_timer(){
	readTimer.interval = APP_SENDE_INTERVAL; // Timer interval
	readTimer.mode = TIMER_REPEAT_MODE; // Timer-Mode
	readTimer.callback = read_timer_fired; // Callback function
	HAL_StartAppTimer(&readTimer); // Start readTimer
}
//-------------------------------------------------------------------------------------------------
static void init_sw_reset_timer(){
	sw_reset_timer.interval = 20; //technically 20 schould be good enough but arduino does 100
	sw_reset_timer.mode = TIMER_ONE_SHOT_MODE;		// Timer-Mode
	sw_reset_timer.callback = callback;	// Callback function
	HAL_StartAppTimer(&sw_reset_timer);				// Start sendeTimer
}
//-------------------------------------------------------------------------------------------------
static void init_start_timer(void){
	start_timer.interval = 100; //the aduino has 100ms delay too
	start_timer.mode = TIMER_ONE_SHOT_MODE;
	start_timer.callback = callback;
	HAL_StartAppTimer(&start_timer);
	
	appstate = APP_NOTHING;
}
//-------------------------------------------------------------------------------------------------
static void init_warm_up_timer(void){
	warm_up_timer.interval = 1200000; // at 20 minute timer, where the sensor is warming up
	warm_up_timer.mode = TIMER_ONE_SHOT_MODE;
	warm_up_timer.callback = warm_up_timer_fired;
	HAL_StartAppTimer(&warm_up_timer);
}
/**************************************************************************/
// Repeat timer needs own callback
static void read_timer_fired(){	
	appstate=APP_ALG_RESULT_DATA_SETUP;
	SYS_PostTask(APL_TASK_ID);
}
/**************************************************************************/
static void initEndpoint(void){
	simpleDescriptor.AppDeviceId = 1;
	simpleDescriptor.AppProfileId = 1;
	simpleDescriptor.endpoint = 1;
	simpleDescriptor.AppDeviceVersion = 1;
	endPoint.simpleDescriptor= &simpleDescriptor;
	endPoint.APS_DataInd = APS_DataInd;
	APS_RegisterEndpointReq(&endPoint);
}
//-------------------------------------------------------------------------------------------------
/* This is the Output of the send Data to the USART */
void APS_DataInd(APS_DataInd_t *indData){
	
	appWriteDataToUsart(indData->asdu,indData->asduLength);
	appWriteDataToUsart((uint8_t*)"\r\n",2);
}
//-------------------------------------------------------------------------------------------------
static void initTransmitData(void){
	dataReq.profileId=1;
	dataReq.dstAddrMode =APS_SHORT_ADDRESS;
	dataReq.dstAddress.shortAddress= CPU_TO_LE16(0);
	dataReq.dstEndpoint =1;
	dataReq.asdu=transmitData.data;
	dataReq.asduLength=sizeof(transmitData.data);
	dataReq.srcEndpoint = 1;
	dataReq.APS_DataConf=APS_DataConf;
}
//-------------------------------------------------------------------------------------------------
static void APS_DataConf(APS_DataConf_t *confInfo){
	if (confInfo->status == APS_SUCCESS_STATUS){
		appstate=APP_NOTHING;
		SYS_PostTask(APL_TASK_ID);
	}
}
//-------------------------------------------------------------------------------------------------
void ZDO_StartNetworkConf(ZDO_StartNetworkConf_t *confirmInfo){
	if (ZDO_SUCCESS_STATUS == confirmInfo -> status){
		CS_ReadParameter(CS_DEVICE_TYPE_ID, &deviceType);
		
		if(deviceType == DEV_TYPE_COORDINATOR){
			appWriteDataToUsart((uint8_t*)"Coordinator\r\n", sizeof("Coordinator\r\n")-1);
			BSP_OnLed(LED_RED); //Red Green is Coordinator
			BSP_OnLed(LED_GREEN);
		}
		else if (deviceType == DEV_TYPE_ROUTER){
			appWriteDataToUsart((uint8_t*)"Router\r\n", sizeof("Router\r\n")-1);
			BSP_OnLed(LED_RED); // ONLY RED is router
		}
		else if (deviceType == DEV_TYPE_ENDDEVICE){
			appWriteDataToUsart((uint8_t*)"End Device\r\n", sizeof("End Device\r\n")-1);
			BSP_OnLed(LED_GREEN); // Only is End Device
		}
		else{
			appWriteDataToUsart((uint8_t*)"ERROR No Device set\r\n", sizeof("ERROR No Device set\r\n")-1);
		}
		
	}
	else{
		appWriteDataToUsart((uint8_t*)"NETWORK ERROR\r\n",sizeof("NETWORK ERROR\r\n")-1);
		
	}
}
//-------------------------------------------------------------------------------------------------
static void initNetwork(){
	networkParams.ZDO_StartNetworkConf = ZDO_StartNetworkConf;
	ZDO_StartNetworkReq(&networkParams);
	initEndpoint();
	initTransmitData();
}
/**************************************************************************/
static void read_alg_data_cb(void){
	_eCO2 = ((uint16_t)data[0] << 8) | ((uint16_t)data[1]);
	_TVOC = ((uint16_t)data[2] << 8) | ((uint16_t)data[3]);
		
	CS_ReadParameter(CS_NWK_PANID_ID, &deviceType);
	uint16_to_hexstr(uart_output, sizeof(uart_output), deviceType,11);// writes the 4 hex numbers from POS 11
	CS_ReadParameter(CS_NWK_ADDR_ID, &deviceType);
	uint16_to_hexstr(uart_output, sizeof(uart_output), deviceType,24); // writes the CSUID from POS 24
	uint32_to_str(uart_output, sizeof(uart_output),_eCO2,36,4); // writes the eCO2 value into the string
	uint32_to_str(uart_output, sizeof(uart_output),_TVOC,49,4); // writes eTVOC value into String
	
	appstate = APP_OUTPUT;
	SYS_PostTask(APL_TASK_ID);
}
/**************************************************************************/
static HAL_I2cDescriptor_t i2cdescriptor={
	.tty = TWI_CHANNEL_0,
	.clockRate = I2C_CLOCK_RATE_62,
	.id = 0x5A,
	.lengthAddr = HAL_NO_INTERNAL_ADDRESS
};
/**************************************************************************/
void APL_TaskHandler(void){
	
	switch(appstate){
		case APP_INIT:
			initNetwork();
			appInitUsartManager();
			BSP_OpenLeds();
			
				appstate = APP_SW_RESET_WRITE_REG;
				SYS_PostTask(APL_TASK_ID);
		break;
		/****************************************************************************************************************************************************/
		case APP_SW_RESET_WRITE_REG:
			{
				nextstate = APP_RESET_DONE; // setting next state for unified callback
			
				int8_t return_value = set_register(init_sw_reset_timer, SW_RESET_MODE, 5, &i2cdescriptor);
				if (return_value != 0){
					appWriteDataToUsart((uint8_t*)"DEBUG: ERROR in set_register SW\r\n", sizeof("DEBUG: ERROR in set_register SW\r\n")-1);
				}
				appstate = APP_NOTHING;
			}
		break;
		//----------------------------------------------------------------------------------------------------------------------------------------------------
		case APP_RESET_DONE:
			{
				nextstate = APP_STATUS_VERTIFY_SETUP; // setting next state for unified callback
				
				int8_t return_value = close_I2C(nextstate, &appstate, &i2cdescriptor);
				if (return_value != 0){
					appWriteDataToUsart((uint8_t*)"DEBUG: ERROR in close SW\r\n", sizeof("DEBUG: ERROR in close SW\r\n")-1);
				}
			}
		break;
		/****************************************************************************************************************************************************/
		case APP_STATUS_VERTIFY_SETUP:
			{
				nextstate = APP_STATUS_VERTIFY_READ; // setting next state for unified callback
			
				int8_t return_value = set_register(callback, STATUS, 1, &i2cdescriptor);
				if (return_value != 0){
					appWriteDataToUsart((uint8_t*)"DEBUG: ERROR in set_register Status\r\n", sizeof("DEBUG: ERROR in set_register Status\r\n")-1);
				}
				appstate = APP_NOTHING;
			}
		break;
		//----------------------------------------------------------------------------------------------------------------------------------------------------
		case APP_STATUS_VERTIFY_READ:
			{
				nextstate = APP_STATUS_DONE; // setting next state for unified callback
			
				int8_t return_value = read_register(callback, data, 1, &i2cdescriptor);
				if (return_value != 0){
					appWriteDataToUsart((uint8_t*)"DEBUG: ERROR in set_register Status\r\n", sizeof("DEBUG: ERROR in set_register Status\r\n")-1);
				}
				appstate = APP_NOTHING;
			}
		break;
		//----------------------------------------------------------------------------------------------------------------------------------------------------
		case APP_STATUS_DONE:
			{	
				int8_t return_value = 0;
			
				if (data[0] != 0x10) {
				
					nextstate = APP_ERROR_SETUP; // setting next state for unified callback
				
					return_value = close_I2C(nextstate, &appstate, &i2cdescriptor);
				
					appWriteDataToUsart((uint8_t*)"DEBUG: ERROR detected\r\n", sizeof("DEBUG: EXIT ERROR detected\r\n")-1);
				
					uint8_to_hexstr(errorString, sizeof(errorString),data[0],9);
					appWriteDataToUsart(errorString, sizeof(errorString));
				}
				else 
				{
				
					nextstate = APP_START_MODE; // setting next state for unified callback
				
					return_value = close_I2C(nextstate, &appstate, &i2cdescriptor);
				
				}	
				if (return_value != 0){
					appWriteDataToUsart((uint8_t*)"DEBUG: ERROR in close SW\r\n", sizeof("DEBUG: ERROR in close SW\r\n")-1);
				}
			}
		break;
		/****************************************************************************************************************************************************/
		case APP_ERROR_SETUP:
			{	
				nextstate = APP_ERROR_READ; // setting next state for unified callback
			
				int8_t return_value = set_register(callback, ERROR, 1, &i2cdescriptor);
				if (return_value != 0){
					appWriteDataToUsart((uint8_t*)"DEBUG: ERROR in set_register Status\r\n", sizeof("DEBUG: ERROR in set_register Status\r\n")-1);
				}
				appstate = APP_NOTHING;
			}
		break;
		//----------------------------------------------------------------------------------------------------------------------------------------------------
		case APP_ERROR_READ:
			{	
				nextstate = APP_ERROR_DONE; // setting next state for unified callback
				
				int8_t return_value = read_register(callback, data, 1, &i2cdescriptor);
				if (return_value != 0){
					appWriteDataToUsart((uint8_t*)"DEBUG: ERROR in set_register Status\r\n", sizeof("DEBUG: ERROR in set_register Status\r\n")-1);
				}
				appstate = APP_NOTHING;
			}
		break;
		//----------------------------------------------------------------------------------------------------------------------------------------------------
		case APP_ERROR_DONE:
			{	
				nextstate = APP_START_MODE; // setting next state for unified callback
				
				uint8_to_hexstr(errorString, sizeof(errorString),data[0],9);
				appWriteDataToUsart(errorString, sizeof(errorString));
			
				int8_t return_value = close_I2C(nextstate, &appstate, &i2cdescriptor);
				if (return_value != 0){
					appWriteDataToUsart((uint8_t*)"DEBUG: ERROR in close SW\r\n", sizeof("DEBUG: ERROR in close SW\r\n")-1);
				}
			}
		break;
		/****************************************************************************************************************************************************/
		case APP_START_MODE:
			{
				nextstate = APP_START_DONE; // setting next state for unified callback
				
				int8_t return_value = set_register(init_start_timer, START_MODE, 1, &i2cdescriptor);
				if (return_value != 0){
					appWriteDataToUsart((uint8_t*)"DEBUG: ERROR in set_register Startup\r\n", sizeof("DEBUG: ERROR in set_register Startup\r\n")-1);
				}
				appstate = APP_NOTHING;
			}
		break;
		//----------------------------------------------------------------------------------------------------------------------------------------------------
		case APP_START_DONE:
			{
				nextstate = APP_MESSURE_MODE_SETUP; // setting next state for unified callback
				
				int8_t return_value = close_I2C(nextstate, &appstate, &i2cdescriptor);
				if (return_value != 0){
					appWriteDataToUsart((uint8_t*)"DEBUG: ERROR in close SW\r\n", sizeof("DEBUG: ERROR in close SW\r\n")-1);
				}
			}
		break;			
		/****************************************************************************************************************************************************/
		case APP_MESSURE_MODE_SETUP:
			{
				nextstate = APP_MESSURE_MODE_SETUP_DONE; // setting next state for unified callback
				
				int8_t return_value = set_register(callback, MEAS_MODE_SETUP, 2, &i2cdescriptor);
				if (return_value != 0){
					appWriteDataToUsart((uint8_t*)"DEBUG: ERROR in set_register MES\r\n", sizeof("DEBUG: ERROR in set_register MES\r\n")-1);
				}
				appstate = APP_NOTHING;
			}
		break;
		//----------------------------------------------------------------------------------------------------------------------------------------------------
		case APP_MESSURE_MODE_SETUP_DONE:
			{
				nextstate = APP_NOTHING; // setting next state for unified callback
				
				int8_t return_value = close_I2C(nextstate, &appstate, &i2cdescriptor);
				if (return_value != 0){
					appWriteDataToUsart((uint8_t*)"DEBUG: ERROR in close MES\r\n", sizeof("DEBUG: ERROR in close MES\r\n")-1);
				}
				nextstate = APP_ALG_RESULT_DATA_SETUP; // Nextstate for timer
				
				init_timer();
				init_warm_up_timer(); //setting a 20 minute timer where the sensor is warming up before any data is beeing send
				appstate = APP_NOTHING;
			}
		break;
		/****************************************************************************************************************************************************/
		case APP_ALG_RESULT_DATA_SETUP:
			{
				nextstate = APP_ALG_READ_DATA; // setting next state for unified callback
				
				int8_t return_value =  set_register(callback, ALG_RESULT_DATA_MODE, 1, &i2cdescriptor);
				if (return_value != 0){
					appWriteDataToUsart((uint8_t*)"DEBUG: ERROR in set_register APP_ALG_RESULT_DATA_SETUP\r\n", sizeof("DEBUG: ERROR in set_register APP_ALG_RESULT_DATA_SETUP\r\n")-1);
				}
				appstate = APP_NOTHING;
			}
		break;
		//----------------------------------------------------------------------------------------------------------------------------------------------------
		case APP_ALG_READ_DATA:
		{
			//NO unified callback support
			int8_t return_value =  read_register(read_alg_data_cb, data, 8, &i2cdescriptor);
			
			if (return_value != 0){
				appWriteDataToUsart((uint8_t*)"DEBUG: ERROR in set_register APP_ALG_READ_DATA\r\n", sizeof("DEBUG: ERROR in set_register APP_ALG_READ_DATA\r\n")-1);
			}
			appstate = APP_NOTHING;
		}
		break;
		/****************************************************************************************************************************************************/
		case APP_OUTPUT:
			{	
				nextstate = APP_NOTHING; // setting next state for unified callback
				
				int8_t return_value = close_I2C(nextstate, &appstate, &i2cdescriptor);
				if (return_value != 0){
					appWriteDataToUsart((uint8_t*)"DEBUG: ERROR in close MES\r\n", sizeof("DEBUG: ERROR in close MES\r\n")-1);
				}
				#if CS_DEVICE_TYPE == DEV_TYPE_COORDINATOR
					appWriteDataToUsart(uart_output, sizeof(uart_output));
					appstate=APP_NOTHING;
					
				#else
					if (warm_up){ //when sensor is warmed up only then send data to coordinator
						for(uint32_t i = 0; i < sizeof(uart_output); i++){
							transmitData.data[i] = uart_output[i];
						}
						APS_DataReq(&dataReq);
					}
					else if(!warm_up && login_notification){// when sensor is not warmed up, and is the login was not yet performed
						login_notification = false; //we only need to login once
						uint8_t login[] = "{\"Panid\":0xxxxx,\"NWK\":0xxxxx}\n\r";
						
						CS_ReadParameter(CS_NWK_PANID_ID, &deviceType);
						uint16_to_hexstr(login, sizeof(login), deviceType,11);// writes the 4 hex numbers from POS 11
						
						CS_ReadParameter(CS_NWK_ADDR_ID, &deviceType);
						uint16_to_hexstr(login, sizeof(login), deviceType,24); // writes the Network address from POS 24	
						
						for(uint32_t i = 0; i < sizeof(login); i++){
							transmitData.data[i] = login[i];
						}
						APS_DataReq(&dataReq);
					}
						
				#endif
			}
		break;
		/****************************************************************************************************************************************************/
		case APP_NOTHING:
			//appWriteDataToUsart((uint8_t*)"DEBUG: Nothing\r\n", sizeof("DEBUG: Nothing\r\n")-1);
		break;
	}
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