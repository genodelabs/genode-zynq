SRC_DIR = src/drivers/pin/zynq src/drivers/pin/include
include $(GENODE_DIR)/repos/base/recipes/src/content.inc

content: src/drivers/pin/main.cc

src/drivers/pin/main.cc:
	mkdir -p $(dir $@)
	cp $(REP_DIR)/$@ $(dir $@)
