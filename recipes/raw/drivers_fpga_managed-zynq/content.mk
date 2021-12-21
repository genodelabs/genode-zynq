content: drivers.config devices.config

drivers.config devices.config:
	cp $(REP_DIR)/recipes/raw/drivers_fpga_managed-zynq/$@ $@
