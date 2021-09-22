#
# \brief  Build config for Genodes core process
# \author Johannes Schlatow
# \date   2021-09-21
#

# add include paths
REP_INC_DIR += src/core/board/zynq_usrp_e31x

NR_OF_CPUS = 2

# include less specific configuration
include $(call select_from_repositories,lib/mk/spec/arm_v7/core-hw-zynq.inc)
