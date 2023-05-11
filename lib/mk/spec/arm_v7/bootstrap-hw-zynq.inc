REQUIRES = hw

SRC_CC  += bootstrap/board/zynq_platform.cc
SRC_CC  += bootstrap/spec/arm/cortex_a9_mmu.cc
SRC_CC  += bootstrap/spec/arm/cpu.cc
SRC_CC  += bootstrap/spec/arm/gicv2.cc

vpath bootstrap/% $(REP_DIR)/src

include $(call select_from_repositories,lib/mk/spec/arm_v7/bootstrap-hw.inc)
