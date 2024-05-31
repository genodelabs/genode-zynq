/*
 * \brief  Driver-specific emulation of Linux Kernel functions
 * \author Johannes Schlatow
 * \date   2022-04-08
 */

/*
 * Copyright (C) 2022 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

#include <linux/fs.h>

int alloc_chrdev_region(dev_t * dev,unsigned baseminor,unsigned count,const char * name)
{
	static dev_t counter = 0;
	*dev = counter++;
	return 0;
}


#include <linux/mm.h>
#include <../mm/slab.h>

void * kmem_cache_alloc_lru(struct kmem_cache * cachep,struct list_lru * lru,gfp_t flags)
{
	return kmalloc(cachep->size, flags);
}


#include <linux/blkdev.h>

int bd_prepare_to_claim(struct block_device * bdev,void * holder)
{
	struct block_device *whole = bdev_whole(bdev);
	whole->bd_claiming = holder;

	return 0;
}


#include <linux/blkdev.h>

void bd_abort_claiming(struct block_device * bdev,void * holder)
{
	struct block_device *whole = bdev_whole(bdev);
	whole->bd_claiming = NULL;
}


#include <linux/ioprio.h>

int __get_task_ioprio(struct task_struct * p)
{
	return IOPRIO_DEFAULT;
}
