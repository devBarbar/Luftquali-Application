/*****************************************************************************
  \file i2controller.c

  \brief

  \author
    Pascal Schieferstein
******************************************************************************/
#include <i2cPacket.h>

/*******************************************************************************
	Description: Opens the i2c Bus and sets the register we want to read from
				 and writes additional bytes after it when needed
	
	Parameters: callback - a pointer to the callback function being called
						   when the operation is completed
				data	 - an array with the byte(s) to be written to the i2c
						   bus.
				length   - the amount of bytes to be written
				*descriptor - a pointer to an I2c Descriptor
	Returns:	-1 when open failed, -2 when writing failed or -3 when booth
				failed and 0 when all success
  *****************************************************************************/
int8_t set_register( void(*callback)(_Bool) , uint8_t data [], uint8_t length, 
	HAL_I2cDescriptor_t *descriptor){
	int8_t return_value = 0;
	
	descriptor -> f = callback;
	descriptor -> data = data;
	descriptor -> length = length;
	
	if (-1 == HAL_OpenI2cPacket(descriptor)){
		return_value-=1; //sets the return value to -1 when open fails
	}
	if (-1 == HAL_WriteI2cPacket(descriptor)){
		return_value-=2; /*sets the return value to -2 when only the write
							fails, and to -3 when open and write fails*/
	}
	return return_value;
}
/*******************************************************************************
	Description: Reads the currently set register from the i2c bus
  
	Parameters: callback - a pointer to the callback function being called
						   when the operation is completed
				data	 - an array where the read bytes should be saved to
				length	 - the amount of bytes to be read.
				*descriptor - a pointer to the I2c Descriptor which schould be
						   read from.
	Returns:	-1 when failed and 0 when success
 ******************************************************************************/
int8_t read_register( void(*callback)(_Bool) , uint8_t data [], uint8_t length, 
	HAL_I2cDescriptor_t *descriptor){
	int8_t return_value = 0;
	
	descriptor -> f = callback;
	descriptor -> data = data;
	descriptor -> length = length;
	
	if (-1 == HAL_ReadI2cPacket(descriptor)){
		return_value-=1; /*sets the return value to -2 when only the write
							fails, and to -3 when open and write fails*/
	}
	return return_value;
}
/******************************************************************************
	Description: closes the i2c bus and sets the next state for 
  
	Parameters: nextstate - the next state for the finite state machine
				*descriptor - the I2c Descriptor to close the bus.
	Returns: -1 when close failed and 0 when success
 *****************************************************************************/
int8_t close_I2C(AppState_t nextstate, AppState_t *appstate, HAL_I2cDescriptor_t *descriptor){
	int8_t return_value = 0;
	
	if (-1 == HAL_CloseI2cPacket(descriptor)){
		return_value = -1;
	}
	*appstate = nextstate;
	SYS_PostTask(APL_TASK_ID);
	return return_value;
}