TARGET  := zynq_pin
SRC_CC  := main.cc
INC_DIR += $(PRG_DIR)
INC_DIR += $(REP_DIR)/src/driver/pin/include
LIBS    += base

vpath main.cc $(REP_DIR)/src/driver/pin
