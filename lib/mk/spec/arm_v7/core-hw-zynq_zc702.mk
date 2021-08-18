#
# \brief  Build config for Genodes core process
# \author Johannes Schlatow
# \date   2016-05-03
#

# add include paths
REP_INC_DIR += src/core/board/zynq_zc702

NR_OF_CPUS = 2

# include less specific configuration
include $(call select_from_repositories,lib/mk/spec/arm_v7/core-hw-zynq.inc)
