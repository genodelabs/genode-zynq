DRIVER := zynq_sd_card
TARGET := $(DRIVER)_drv

REQUIRES := arm_v7a
LIBS = base zynq_lx_emul jitterentropy

SRC_CC += main.cc
SRC_CC += lx_emul/shared_dma_buffer.cc
SRC_CC += lx_emul/random.cc
SRC_C  += block.c

SRC_CC  += genode_c_api/block.cc
GENODE_C_API_SRC_DIR := $(call select_from_repositories,src/lib/genode_c_api)
vpath genode_c_api/block.cc $(dir $(GENODE_C_API_SRC_DIR))

SRC_C  += lx_emul/zynq/common_dummies.c
vpath lx_emul/zynq/common_dummies.c $(REP_DIR)/src/lib
