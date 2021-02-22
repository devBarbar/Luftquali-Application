/**************************************************************************//**
  \file callback.c

  \brief a unified function to manage all callback function in one place.

  \author Pascal Schieferstein

******************************************************************************/
static void fsm_callback(AppState_t nextstate, AppState_t *appstate){
	*appstate = nextstate;
	SYS_PostTask(APL_TASK_ID);
}
