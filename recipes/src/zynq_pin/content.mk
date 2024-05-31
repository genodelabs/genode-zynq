SRC_DIR = src/driver/pin/zynq src/driver/pin/include
include $(GENODE_DIR)/repos/base/recipes/src/content.inc

content: src/driver/pin/main.cc

src/driver/pin/main.cc:
	mkdir -p $(dir $@)
	cp $(REP_DIR)/$@ $(dir $@)
