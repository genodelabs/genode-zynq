MIRRORED_FROM_REP_DIR := src/driver/sd_card/zynq

content: $(MIRRORED_FROM_REP_DIR)
$(MIRRORED_FROM_REP_DIR):
	$(mirror_from_rep_dir)

PORT_DIR := $(call port_dir,$(GENODE_DIR)/repos/dde_linux/ports/legacy_linux)

content: LICENSE
LICENSE:
	cp $(PORT_DIR)/src/linux/COPYING $@
