/*
 * \brief  Common dummy definitions of Linux Kernel functions - handled manually
 * \author Johannes Schlatow
 * \date   2022-04-05
 */

/*
 * Copyright (C) 2022 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */


#include <lx_emul.h>

#include <asm-generic/sections.h>

char __start_rodata[] = {};
char __end_rodata[]   = {};


#include <linux/srcu.h>

void synchronize_srcu(struct srcu_struct * ssp)
{
	lx_emul_trace(__func__);
}


#include <linux/prandom.h>

unsigned long net_rand_noise;


#include <linux/tracepoint-defs.h>

const struct trace_print_flags vmaflag_names[]  = { {0,NULL}};


#include <linux/tracepoint-defs.h>

const struct trace_print_flags pageflag_names[]  = { {0,NULL}};


#include <linux/tracepoint-defs.h>

const struct trace_print_flags gfpflag_names[]  = { {0,NULL}};


#include <linux/of_reserved_mem.h>
#include <linux/mod_devicetable.h>

const struct of_device_id __reservedmem_of_table[] = {};


#include <asm/uaccess.h>

unsigned long arm_copy_from_user(void *to, const void *from, unsigned long n)
{
	lx_emul_trace_and_stop(__func__);
}


#include <asm/uaccess.h>

unsigned long arm_copy_to_user(void *to, const void *from, unsigned long n)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/cpuhotplug.h>

int __cpuhp_setup_state(enum cpuhp_state state,const char * name,bool invoke,int (* startup)(unsigned int cpu),int (* teardown)(unsigned int cpu),bool multi_instance)
{
	lx_emul_trace(__func__);
	return 0;
}


#include <linux/sched/signal.h>

void ignore_signals(struct task_struct * t)
{
	lx_emul_trace(__func__);
}


#include <linux/sched/loadavg.h>

void calc_global_load(void)
{
	lx_emul_trace(__func__);
}


#include <linux/kernel_stat.h>

void account_process_tick(struct task_struct * p,int user_tick)
{
	lx_emul_trace(__func__);
}


#include <linux/rcupdate.h>

void rcu_sched_clock_irq(int user)
{
	lx_emul_trace(__func__);
}


#include <linux/kobject.h>

int kobject_uevent(struct kobject * kobj,enum kobject_action action)
{
	lx_emul_trace(__func__);
	return 0;
}


#include <linux/property.h>

int software_node_notify(struct device * dev,unsigned long action)
{
	lx_emul_trace(__func__);
	return 0;
}


#include <linux/random.h>

void get_random_bytes(void * buf,int nbytes)
{
	lx_emul_trace(__func__);
}


#include <linux/random.h>

int __must_check get_random_bytes_arch(void * buf,int nbytes)
{
	lx_emul_trace(__func__);
	return 0;
}


#include <linux/random.h>

int add_random_ready_callback(struct random_ready_callback * rdy)
{
	lx_emul_trace(__func__);
	return 0;
}


#include <linux/logic_pio.h>

struct logic_pio_hwaddr * find_io_range_by_fwnode(struct fwnode_handle * fwnode)
{
	lx_emul_trace(__func__);
	return NULL;
}


#include <linux/syscore_ops.h>

void register_syscore_ops(struct syscore_ops * ops)
{
	lx_emul_trace(__func__);
}


#include <linux/kernel.h>

bool parse_option_str(const char *str, const char *option)
{
	lx_emul_trace(__func__);
	return false;
}


/* asm symbols */

void __div0(void)
{
	lx_emul_trace_and_stop(__func__);
}
