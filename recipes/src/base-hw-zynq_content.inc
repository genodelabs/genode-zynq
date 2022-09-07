SRC_BOOTSTRAP = board/zynq_platform.cc

# supplemental lib/mk/ not captured by base-hw_content.inc
CONTENT += $(addprefix lib/mk/spec/arm_v7/,core-hw-zynq.inc bootstrap-hw-zynq.inc)

include $(GENODE_DIR)/repos/base-hw/recipes/src/base-hw_content.inc
