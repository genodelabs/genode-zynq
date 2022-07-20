/*
 * \brief  Dummy definitions of Linux Kernel functions - handled manually
 * \author Johannes Schlatow
 * \date   2022-04-04
 */

/*
 * Copyright (C) 2022 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

#include <lx_emul.h>

#include <../block/blk.h>

struct kobj_type blk_queue_ktype;


#include <linux/pinctrl/devinfo.h>

int pinctrl_bind_pins(struct device * dev)
{
	lx_emul_trace(__func__);
	return 0;
}


#include <linux/dma-map-ops.h>

void arch_setup_dma_ops(struct device * dev,u64 dma_base,u64 size,const struct iommu_ops * iommu,bool coherent)
{
	lx_emul_trace(__func__);
}


#include <linux/clk/clk-conf.h>

int of_clk_set_defaults(struct device_node * node,bool clk_supplier)
{
	lx_emul_trace(__func__);
	return 0;
}


extern void arm_heavy_mb(void);
void arm_heavy_mb(void)
{
	/* XXX linux issues an L2 cache sync here to drain the store buffer */
	lx_emul_trace(__func__);
}


#include <linux/dma-mapping.h>

int dma_supported(struct device * dev,u64 mask)
{
	return 1;
}


#include <linux/sched.h>

void sched_set_fifo(struct task_struct * p)
{
	lx_emul_trace(__func__);
}


#include <linux/clk.h>

int clk_set_phase(struct clk * clk,int degrees)
{
	/* XXX double-check that the correct phase is already set */
	lx_emul_trace(__func__);
	return 0;
}


#include <linux/pinctrl/devinfo.h>

int pinctrl_init_done(struct device * dev)
{
	lx_emul_trace(__func__);
	return 0;
}


#include <linux/random.h>

void add_interrupt_randomness(int irq,int irq_flags)
{
	lx_emul_trace(__func__);
}


#include <linux/prandom.h>

u32 prandom_u32(void)
{
	lx_emul_trace(__func__);
	return (u32) 0xdeadbeef;
}


#include <linux/backing-dev.h>

struct backing_dev_info * bdi_alloc(int node_id)
{
	struct backing_dev_info * ret = kzalloc(sizeof(struct backing_dev_info), GFP_KERNEL);
	return ret;
}


int bdi_register(struct backing_dev_info * bdi,const char * fmt,...)
{
	lx_emul_trace(__func__);
	return 0;
}


void bdi_set_owner(struct backing_dev_info * bdi,struct device * owner)
{
	lx_emul_trace(__func__);
}


#include <../block/blk-stat.h>
struct blk_queue_stats { int dummy; };

struct blk_queue_stats * blk_alloc_queue_stats(void)
{
	static struct blk_queue_stats ret;
	lx_emul_trace(__func__);
	return &ret;
}


extern struct blk_stat_callback * blk_stat_alloc_callback(void (* timer_fn)(struct blk_stat_callback *),int (* bucket_fn)(const struct request *),unsigned int buckets,void * data);
struct blk_stat_callback * blk_stat_alloc_callback(void (* timer_fn)(struct blk_stat_callback *),int (* bucket_fn)(const struct request *),unsigned int buckets,void * data)
{
	static struct blk_stat_callback ret;
	lx_emul_trace(__func__);
	return &ret;
}


extern void blk_mq_sysfs_init(struct request_queue * q);
void blk_mq_sysfs_init(struct request_queue * q)
{
	lx_emul_trace(__func__);
}


extern void blk_mq_hctx_kobj_init(struct blk_mq_hw_ctx * hctx);
void blk_mq_hctx_kobj_init(struct blk_mq_hw_ctx * hctx)
{
	lx_emul_trace(__func__);
}


#include <linux/cpuhotplug.h>

int __cpuhp_state_add_instance(enum cpuhp_state state,struct hlist_node * node,bool invoke)
{
	lx_emul_trace(__func__);
	return 0;
}


#include <linux/genhd.h>

void rand_initialize_disk(struct gendisk * disk)
{
	lx_emul_trace(__func__);
}


#include <linux/blkdev.h>

void blkdev_put(struct block_device * bdev,fmode_t mode)
{
	lx_emul_trace(__func__);
}


#include <linux/wait_bit.h>

void __init wait_bit_init(void)
{
	lx_emul_trace(__func__);
}
