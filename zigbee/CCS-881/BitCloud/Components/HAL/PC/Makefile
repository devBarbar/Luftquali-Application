###########################################################################//##
# \file Makefile
#
# \brief Main makefile for pc platform
#
# \author
#   Atmel Corporation: http://www.atmel.com \n
#   Support email: avr@atmel.com
#
# Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
# Licensed under Atmel's Limited License Agreement (BitCloudTM).
#
# \internal
#  History:
#    09.08.12 N. Fomin - Created
#  Last change:
#    $Id: Makefile 20611 2012-03-23 09:31:24Z arazinkov $
################################################################################

HAL_PATH = ..
include $(HAL_PATH)/Makerules

###
LIBDIR = $(STACK_LIB_PATH)
LIB = $(LIBDIR)/lib$(HAL_LIB).a
ifeq ($(HAL), WIN)
  BUILDDIR = $(HAL_PATH)/pc/windows
endif

##### PATHS FLAGS OF INCLUDES #########
CFLAGS += -I$(HAL_PATH)/include
CFLAGS += -I$(HAL_HWD_COMMON_PATH)/include
CFLAGS += -I$(HAL_MAC_API_PATH)/include
CFLAGS += -I$(SE_PATH)/include

###### LIB ##########
common_hwd =  halAppClock
common_hwd += halAtomic
common_hwd += halSleep
common_hwd += halUsart
common_hwd += halInit

hwi =  appTimer
hwi += halTaskManager
hwi += timer
hwi += usart
hwi += resetReason
hwi += stdioAdapter

objects_hwd = $(addsuffix .o,$(addprefix $(HAL_HWD_COMMON_PATH)/objs/,$(common_hwd)))
sources_hwd = $(addsuffix .c,$(addprefix $(HAL_HWD_COMMON_PATH)/src/,$(common_hwd)))
objects_hwi = $(addsuffix .o,$(addprefix $(BUILDDIR)/../common/objs/,$(hwi)))
sources_hwi = $(addsuffix .c,$(addprefix $(BUILDDIR)/../common/src/,$(hwi)))


###### TARGETS ################
all: component_label $(LIB)

component_label:
	@echo 
	@echo ----------------------------------------------------
	@echo HAL library creation.
	@echo ----------------------------------------------------

################ common part ##############################
$(HAL_HWD_COMMON_PATH)/objs/%.o: $(HAL_HWD_COMMON_PATH)/src/%.c
	$(CC_MSG)
	$(Q)$(CC) $(CFLAGS) $^ -o $@
################ common part ##############################

################ hwi part ###################################
$(BUILDDIR)/../common/objs/%.o: $(BUILDDIR)/../common/src/%.c
	$(CC_MSG)
	$(Q)$(CC) $(CFLAGS) $^ -o $@
################ hwi part ###################################

################
$(LIB): $(objects_hwd) $(objects_hwi)
	$(AR_MSG)
	$(Q)$(AR) $(AR_KEYS) $(LIB) $(objects_hwd) $(objects_hwi)
	$(SIZE_MSG)
	$(Q)$(SHOW_SIZE) -td $(LIB)		

################
clean:
	@echo 
	@echo ----------------------------------------------------
	@echo HAL component cleaning is started...
	$(Q)rm -f $(objects_hwd) $(objects_hwi) $(LIB)
	$(Q)rm -f $(wildcard $(HAL_PATH)/lib/*.a) $(wildcard $(HAL_PATH)/lib/*.o) $(wildcard $(HAL_PATH)/list/*.*)
	@echo HAL component cleaning is done!
	@echo ----------------------------------------------------

