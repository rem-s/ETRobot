mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS += balancer.o balancer_param.o line_trace.o sensor_process.o bluetooth.o measure_position.o lookup_gate.o

APPL_CXXOBJS += 

SRCLANG := c

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

APPL_DIR += $(mkfile_path)unit

INCLUDES += \
	-I$(mkfile_path)unit
