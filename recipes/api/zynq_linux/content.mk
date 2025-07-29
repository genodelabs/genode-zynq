#
# Content hosted in the dde_linux repository
#

MIRRORED_FROM_DDE_LINUX := src/lib/lx_emul \
                           src/lib/lx_kit \
                           src/include/lx_emul \
                           src/include/lx_user \
                           src/include/spec/arm_v7/lx_kit \
                           src/include/lx_kit \
                           lib/import/import-lx_emul_common.inc

content: $(MIRRORED_FROM_DDE_LINUX)
$(MIRRORED_FROM_DDE_LINUX):
	mkdir -p $(dir $@); cp -r $(GENODE_DIR)/repos/dde_linux/$@ $(dir $@)


#
# Content hosted in the zynq repository
#

MIRRORED_FROM_REP_DIR := lib/mk/spec/arm_v7/zynq_linux_generated.mk \
                         lib/mk/spec/arm_v7/zynq_lx_emul.mk \
                         src/zynq_linux/common.inc \
                         lib/import/import-zynq_lx_emul.mk \
                         src/include/lx_emul/initcall_order.h \
                         src/lib/lx_emul/zynq

content: $(MIRRORED_FROM_REP_DIR)
$(MIRRORED_FROM_REP_DIR):
	$(mirror_from_rep_dir)


#
# Content from the Linux source tree
#

PORT_DIR := $(call port_dir,$(GENODE_DIR)/repos/dde_linux/ports/linux)
LX_REL_DIR := src/linux
LX_ABS_DIR := $(addsuffix /$(LX_REL_DIR),$(PORT_DIR))

# ingredients needed for creating a Linux build directory / generated headers
LX_FILES += Kbuild \
            Makefile \
            arch/arm/Makefile \
            arch/arm/boot/dts \
            arch/arm/configs \
            arch/arm/include/asm/Kbuild \
            arch/arm/include/asm/archrandom.h \
            arch/arm/include/asm/futex.h \
            arch/arm/include/asm/pgalloc.h \
            arch/arm/include/asm/simd.h \
            arch/arm/include/asm/vermagic.h \
            arch/arm/include/asm/xor.h \
            arch/arm/include/uapi/asm/Kbuild \
            arch/arm/include/uapi/asm/statfs.h \
            arch/arm/tools/Makefile \
            arch/arm/tools/gen-mach-types \
            arch/arm/tools/mach-types \
            arch/arm/tools/syscall.tbl \
            arch/arm/tools/syscallnr.sh \
            include/asm-generic/Kbuild \
            include/asm-generic/cfi.h \
            include/asm-generic/early_ioremap.h \
            include/asm-generic/flat.h \
            include/asm-generic/local64.h \
            include/asm-generic/module.lds.h \
            include/asm-generic/parport.h \
            include/asm-generic/runtime-const.h \
            include/asm-generic/serial.h \
            include/asm-generic/vga.h \
            include/asm-generic/video.h \
            include/linux/compiler-version.h \
            include/linux/kbuild.h \
            include/linux/license.h \
            include/uapi/Kbuild \
            include/uapi/asm-generic/Kbuild \
            include/uapi/asm-generic/msgbuf.h \
            kernel/configs/tiny-base.config \
            kernel/configs/tiny.config \
            scripts/Kbuild.include \
            scripts/Makefile \
            scripts/Makefile.asm-headers \
            scripts/Makefile.build \
            scripts/Makefile.compiler \
            scripts/Makefile.defconf \
            scripts/Makefile.extrawarn \
            scripts/Makefile.host \
            scripts/Makefile.lib \
            scripts/as-version.sh \
            scripts/basic/Makefile \
            scripts/basic/fixdep.c \
            scripts/cc-version.sh \
            scripts/checksyscalls.sh \
            scripts/config \
            scripts/dtc \
            scripts/include/array_size.h \
            scripts/include/hash.h \
            scripts/include/hashtable.h \
            scripts/include/list.h \
            scripts/include/list_types.h \
            scripts/include/xalloc.h \
            scripts/kconfig/merge_config.sh \
            scripts/kconfig/preprocess.h \
            scripts/ld-version.sh \
            scripts/min-tool-version.sh \
            scripts/mkcompile_h \
            scripts/mod \
            scripts/pahole-version.sh \
            scripts/remove-stale-files \
            scripts/rustc-llvm-version.sh \
            scripts/rustc-version.sh \
            scripts/setlocalversion \
            scripts/sorttable.c \
            scripts/sorttable.h \
            scripts/subarch.include \
            scripts/syscall.tbl \
            scripts/syscallhdr.sh \
            scripts/syscalltbl.sh \
            tools/include/tools

# prevent warning with 'grep' when building api archive
LX_FILES += arch/x86/entry/syscalls/syscall_32.tbl

LX_SCRIPTS_KCONFIG_FILES := $(notdir $(wildcard $(LX_ABS_DIR)/scripts/kconfig/*.c)) \
                            $(notdir $(wildcard $(LX_ABS_DIR)/scripts/kconfig/*.h)) \
                            Makefile lexer.l parser.y
LX_FILES += $(addprefix scripts/kconfig/,$(LX_SCRIPTS_KCONFIG_FILES)) \

LX_FILES += $(shell cd $(LX_ABS_DIR); find -name "Kconfig*" -printf "%P\n")

# needed for generated/asm-offsets.h
LX_FILES += kernel/bounds.c \
            kernel/time/timeconst.bc \
            arch/arm/kernel/asm-offsets.c \
            arch/arm/kernel/signal.h \
            arch/arm/include/asm/current.h \
            arch/arm/include/asm/fixmap.h \
            arch/arm/include/asm/glue-df.h \
            arch/arm/include/asm/glue-pf.h \
            arch/arm/include/asm/hardware/cache-l2x0.h \
            arch/arm/include/asm/irqflags.h \
            arch/arm/include/asm/kasan_def.h \
            arch/arm/include/asm/kexec-internal.h \
            arch/arm/include/asm/mach/arch.h \
            arch/arm/include/asm/memory.h \
            arch/arm/include/asm/mpu.h \
            arch/arm/include/asm/opcodes-sec.h \
            arch/arm/include/asm/page.h \
            arch/arm/include/asm/pgtable-2level-hwdef.h \
            arch/arm/include/asm/pgtable-2level-types.h \
            arch/arm/include/asm/pgtable-2level.h \
            arch/arm/include/asm/pgtable-hwdef.h \
            arch/arm/include/asm/pgtable.h \
            arch/arm/include/asm/procinfo.h \
            arch/arm/include/asm/smp_plat.h \
            arch/arm/include/asm/spinlock.h \
            arch/arm/include/asm/suspend.h \
            arch/arm/include/asm/tlbflush.h \
            arch/arm/include/asm/ucontext.h \
            include/uapi/linux/arm_sdei.h \
            include/acpi \
            include/asm-generic/current.h \
            include/asm-generic/memory_model.h \
            include/asm-generic/qspinlock.h \
            include/asm-generic/fixmap.h \
            include/asm-generic/pgtable_uffd.h \
            include/asm-generic/tlbflush.h \
            include/linux/arm_sdei.h \
            include/linux/arm-smccc.h \
            include/linux/cper.h \
            include/linux/page_table_check.h \
            include/linux/pgtable.h

# needed for gen_crc32table
LX_FILES += lib/gen_crc32table.c \
            lib/crc32.c

content: src/linux/include/linux/kvm_host.h
src/linux/include/linux/kvm_host.h: # cut dependencies from kvm via dummy header
	mkdir -p $(dir $@)
	touch $@

# prevent build of vdso as side effect of the linux-build directory preparation
content: src/linux/arch/arm/kernel/vdso/Makefile
src/linux/arch/arm/kernel/vdso/Makefile:
	mkdir -p $(dir $@)
	echo "default:" > $@
	echo "include/generated/vdso-offsets.h:" >> $@
	echo "arch/arm/kernel/vdso/vdso.so:" >> $@

# add content listed in the repository's source.list or dep.list files
LX_FILE_LISTS := $(shell find -H $(REP_DIR) -name dep.list -or -name source.list)
LX_FILES += $(shell cat $(LX_FILE_LISTS))
LX_FILES := $(sort $(LX_FILES))
MIRRORED_FROM_PORT_DIR += $(addprefix $(LX_REL_DIR)/,$(LX_FILES))

content: $(MIRRORED_FROM_PORT_DIR)
$(MIRRORED_FROM_PORT_DIR):
	mkdir -p $(dir $@)
	cp -r $(addprefix $(PORT_DIR)/,$@) $@

content: LICENSE
LICENSE:
	cp $(PORT_DIR)/src/linux/COPYING $@
