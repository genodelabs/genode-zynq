/*
 * \brief  Dummy definitions of Linux Kernel functions
 * \author Automatically generated file - do no edit
 * \date   2023-03-28
 */

#include <lx_emul.h>


#include <linux/ratelimit_types.h>

int ___ratelimit(struct ratelimit_state * rs,const char * func)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/cpumask.h>

struct cpumask __cpu_active_mask;


#include <linux/cpuhotplug.h>

int __cpuhp_state_remove_instance(enum cpuhp_state state,struct hlist_node * node,bool invoke)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/serial_core.h>

const struct earlycon_id __earlycon_table[] = {};


#include <linux/serial_core.h>

const struct earlycon_id __earlycon_table_end[] = {};


#include <linux/blkdev.h>

int __invalidate_device(struct block_device * bdev,bool kill_dirty)
{
	lx_emul_trace_and_stop(__func__);
}


#include <asm-generic/percpu.h>

unsigned long __per_cpu_offset[NR_CPUS] = {};


#include <linux/printk.h>

void __printk_safe_enter(void)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/printk.h>

void __printk_safe_exit(void)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/sched/task.h>

void __put_task_struct(struct task_struct * tsk)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/fs.h>

void __remove_inode_hash(struct inode * inode)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/mm.h>

void __show_mem(unsigned int filter,nodemask_t * nodemask,int max_zone_idx)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/vmalloc.h>

void * __vmalloc_node_range(unsigned long size,unsigned long align,unsigned long start,unsigned long end,gfp_t gfp_mask,pgprot_t prot,unsigned long vm_flags,int node,const void * caller)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/kstrtox.h>

int _kstrtoul(const char * s,unsigned int base,unsigned long * res)
{
	lx_emul_trace_and_stop(__func__);
}


extern noinline unsigned int _parse_integer(const char * s,unsigned int base,unsigned long long * p);
noinline unsigned int _parse_integer(const char * s,unsigned int base,unsigned long long * p)
{
	lx_emul_trace_and_stop(__func__);
}


extern noinline const char * _parse_integer_fixup_radix(const char * s,unsigned int * base);
noinline const char * _parse_integer_fixup_radix(const char * s,unsigned int * base)
{
	lx_emul_trace_and_stop(__func__);
}


extern noinline unsigned int _parse_integer_limit(const char * s,unsigned int base,unsigned long long * p,size_t max_chars);
noinline unsigned int _parse_integer_limit(const char * s,unsigned int base,unsigned long long * p,size_t max_chars)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/printk.h>

int _printk_deferred(const char * fmt,...)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/random.h>

void __init add_bootloader_randomness(const void * buf,size_t len)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/kobject.h>

int add_uevent_var(struct kobj_uevent_env * env,const char * format,...)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/amba/bus.h>

int amba_device_add(struct amba_device * dev,struct resource * parent)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/amba/bus.h>

struct amba_device * amba_device_alloc(const char * name,resource_size_t base,size_t size)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/amba/bus.h>

void amba_device_put(struct amba_device * dev)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/dma-map-ops.h>

void arch_teardown_dma_ops(struct device * dev)
{
	lx_emul_trace_and_stop(__func__);
}


extern void arch_trigger_cpumask_backtrace(const cpumask_t * mask,bool exclude_self);
void arch_trigger_cpumask_backtrace(const cpumask_t * mask,bool exclude_self)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/badblocks.h>

ssize_t badblocks_show(struct badblocks * bb,char * page,int unack)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/badblocks.h>

ssize_t badblocks_store(struct badblocks * bb,const char * page,size_t len,int unack)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/backing-dev.h>

void bdi_put(struct backing_dev_info * bdi)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/backing-dev.h>

void bdi_unregister(struct backing_dev_info * bdi)
{
	lx_emul_trace_and_stop(__func__);
}


extern void blk_drop_partitions(struct gendisk * disk);
void blk_drop_partitions(struct gendisk * disk)
{
	lx_emul_trace_and_stop(__func__);
}


extern void blk_free_queue_stats(struct blk_queue_stats * stats);
void blk_free_queue_stats(struct blk_queue_stats * stats)
{
	lx_emul_trace_and_stop(__func__);
}


extern void blk_mq_sysfs_deinit(struct request_queue * q);
void blk_mq_sysfs_deinit(struct request_queue * q)
{
	lx_emul_trace_and_stop(__func__);
}


extern void blk_stat_add(struct request * rq,u64 now);
void blk_stat_add(struct request * rq,u64 now)
{
	lx_emul_trace_and_stop(__func__);
}


extern void blk_stat_free_callback(struct blk_stat_callback * cb);
void blk_stat_free_callback(struct blk_stat_callback * cb)
{
	lx_emul_trace_and_stop(__func__);
}


extern bool blk_stats_alloc_enable(struct request_queue * q);
bool blk_stats_alloc_enable(struct request_queue * q)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/init.h>

char __initdata boot_command_line[] = {};


#include <linux/kernel.h>

void bust_spinlocks(int yes)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/cdev.h>

int cdev_device_add(struct cdev * cdev,struct device * dev)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/cdev.h>

void cdev_device_del(struct cdev * cdev,struct device * dev)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/cdev.h>

void cdev_init(struct cdev * cdev,const struct file_operations * fops)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/srcu.h>

void cleanup_srcu_struct(struct srcu_struct * ssp)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/clk-provider.h>

const char * clk_hw_get_name(const struct clk_hw * hw)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/console.h>

void console_flush_on_panic(enum con_flush_mode mode)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/console.h>

void console_unblank(void)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/printk.h>

void console_verbose(void)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/arch_topology.h>

struct cpu_topology cpu_topology[NR_CPUS] = {};


#include <linux/sched/topology.h>

bool cpus_share_cache(int this_cpu,int that_cpu)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/context_tracking_irq.h>

void ct_irq_enter_irqson(void)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/context_tracking_irq.h>

void ct_irq_exit_irqson(void)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/clk-provider.h>

struct clk * devm_clk_register(struct device * dev,struct clk_hw * hw)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/math64.h>

u64 div64_u64(u64 dividend,u64 divisor)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/math64.h>

u64 div64_u64_rem(u64 dividend,u64 divisor,u64 * remainder)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/math64.h>

s64 div_s64_rem(s64 dividend,s32 divisor,s32 * remainder)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/dma-map-ops.h>

bool dma_default_coherent;


#include <linux/dma-mapping.h>

unsigned long dma_get_merge_boundary(struct device * dev)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/dma-mapping.h>

size_t dma_max_mapping_size(struct device * dev)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/dma-mapping.h>

void dma_sync_sg_for_cpu(struct device * dev,struct scatterlist * sg,int nelems,enum dma_data_direction dir)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/dma-mapping.h>

void * dmam_alloc_attrs(struct device * dev,size_t size,dma_addr_t * dma_handle,gfp_t gfp,unsigned long attrs)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/dma-mapping.h>

void dmam_free_coherent(struct device * dev,size_t size,void * vaddr,dma_addr_t dma_handle)
{
	lx_emul_trace_and_stop(__func__);
}


#include <asm-generic/softirq_stack.h>

void do_softirq_own_stack(void)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/printk.h>

asmlinkage __visible void dump_stack(void)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/reboot.h>

void emergency_restart(void)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/rcuwait.h>

void finish_rcuwait(struct rcuwait * w)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/kobject.h>

struct kobject *firmware_kobj;


extern void flush_dcache_page(struct page * page);
void flush_dcache_page(struct page * page)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/blkdev.h>

int fsync_bdev(struct block_device * bdev)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/uuid.h>

const u8 guid_index[16] = {};


#include <linux/init.h>

bool initcall_debug;


#include <linux/ioport.h>

int insert_resource(struct resource * parent,struct resource * new)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/sched.h>

void __sched io_schedule(void)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/sched.h>

void io_schedule_finish(int token)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/sched.h>

int io_schedule_prepare(void)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/sched.h>

long __sched io_schedule_timeout(long timeout)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/ioport.h>

struct resource iomem_resource;


#include <linux/ioport.h>

struct resource ioport_resource;


#include <asm-generic/logic_io.h>

void __iomem * ioremap(resource_size_t res_cookie,size_t size)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/fs.h>

void iput(struct inode * inode)
{
	lx_emul_trace_and_stop(__func__);
}


extern bool irq_wait_for_poll(struct irq_desc * desc);
bool irq_wait_for_poll(struct irq_desc * desc)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/irq_work.h>

void irq_work_tick(void)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/property.h>

bool is_software_node(const struct fwnode_handle * fwnode)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/mm.h>

bool is_vmalloc_addr(const void * x)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/sched.h>

void kick_process(struct task_struct * p)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/slab.h>

void * kmalloc_large_node(size_t size,gfp_t flags,int node)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/slab.h>

void kmem_dump_obj(void * object)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/slab.h>

bool kmem_valid_obj(void * object)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/kmsg_dump.h>

void kmsg_dump(enum kmsg_dump_reason reason)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/kobject.h>

int kobject_synth_uevent(struct kobject * kobj,const char * buf,size_t count)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/kobject.h>

int kobject_uevent_env(struct kobject * kobj,enum kobject_action action,char * envp_ext[])
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/kstrtox.h>

noinline int kstrtobool(const char * s,bool * res)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/kstrtox.h>

noinline int kstrtoint(const char * s,unsigned int base,int * res)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/kstrtox.h>

noinline int kstrtoll(const char * s,unsigned int base,long long * res)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/writeback.h>

void laptop_io_completion(struct backing_dev_info * info)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/writeback.h>

int laptop_mode;


#include <linux/logic_pio.h>

unsigned long logic_pio_trans_hwaddr(struct fwnode_handle * fwnode,resource_size_t addr,resource_size_t size)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/delay.h>

unsigned long lpj_fine;


#include <linux/memblock.h>

struct memblock memblock;


#include <linux/memblock.h>

int __init_memblock memblock_add(phys_addr_t base,phys_addr_t size)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/memblock.h>

void __init memblock_cap_memory_range(phys_addr_t base,phys_addr_t size)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/memblock.h>

int __init_memblock memblock_clear_nomap(phys_addr_t base,phys_addr_t size)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/memblock.h>

bool __init_memblock memblock_is_region_reserved(phys_addr_t base,phys_addr_t size)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/memblock.h>

int __init_memblock memblock_mark_hotplug(phys_addr_t base,phys_addr_t size)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/memblock.h>

int __init_memblock memblock_mark_nomap(phys_addr_t base,phys_addr_t size)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/memblock.h>

bool __init_memblock memblock_overlaps_region(struct memblock_type * type,phys_addr_t base,phys_addr_t size)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/memblock.h>

phys_addr_t __init memblock_phys_alloc_range(phys_addr_t size,phys_addr_t align,phys_addr_t start,phys_addr_t end)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/memblock.h>

int __init_memblock memblock_phys_free(phys_addr_t base,phys_addr_t size)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/memblock.h>

int __init_memblock memblock_reserve(phys_addr_t base,phys_addr_t size)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/irq.h>

struct irq_chip no_irq_chip;


#include <linux/fs.h>

int nonseekable_open(struct inode * inode,struct file * filp)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/irq.h>

void note_interrupt(struct irq_desc * desc,irqreturn_t action_ret)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/clk-provider.h>

int of_clk_add_provider(struct device_node * np,struct clk * (* clk_src_get)(struct of_phandle_args * clkspec,void * data),void * data)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/clk-provider.h>

void of_clk_del_provider(struct device_node * np)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/clk-provider.h>

struct clk * of_clk_src_simple_get(struct of_phandle_args * clkspec,void * data)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/serial_core.h>

int __init of_setup_earlycon(const struct earlycon_id * match,unsigned long node,const char * options)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/reboot.h>

enum reboot_mode panic_reboot_mode;


#include <linux/reboot.h>

enum reboot_mode reboot_mode;


#include <linux/ioport.h>

int release_resource(struct resource * old)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/mm.h>

int set_page_dirty_lock(struct page * page)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/sched/debug.h>

void show_state_filter(unsigned int state_filter)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/siphash.h>

u64 siphash_1u32(const u32 first,const siphash_key_t * key)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/smp.h>

void smp_call_function_many(const struct cpumask * mask,smp_call_func_t func,void * info,bool wait)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/smp.h>

int smp_call_function_single_async(int cpu,struct __call_single_data * csd)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/smp.h>

void smp_send_stop(void)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/jump_label.h>

bool static_key_initialized;


#include <linux/printk.h>

int suppress_printk;


#include <linux/rcupdate.h>

void synchronize_rcu(void)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/mfd/syscon.h>

struct regmap * syscon_node_to_regmap(struct device_node * np)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/clockchips.h>

void tick_broadcast(const struct cpumask * mask)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/fs.h>

void unregister_chrdev_region(dev_t from,unsigned count)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/reboot.h>

int unregister_restart_handler(struct notifier_block * nb)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/uuid.h>

const u8 uuid_index[16] = {};


#include <linux/vmalloc.h>

void vfree(const void * addr)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/vmalloc.h>

bool vmalloc_dump_obj(void * object)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/sched/wake_q.h>

void wake_q_add_safe(struct wake_q_head * head,struct task_struct * task)
{
	lx_emul_trace_and_stop(__func__);
}

