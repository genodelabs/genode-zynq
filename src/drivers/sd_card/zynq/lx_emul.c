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


#include <linux/random.h>
#include <lx_emul/random.h>

u32 __get_random_u32_below(u32 ceil)
{
	/**
	 * Returns a random number from the half-open interval [0, ceil)
	 * with uniform distribution.
	 *
	 * The idea here is to split [0, 2^32) into #ceil bins. By dividing a random
	 * number from the 32-bit interval, we can determine into which bin the number
	 * fell.
	 */

	/* determine divisor to determine bin number by dividing 2^32 by ceil */
	u32 div = 0x100000000ULL / ceil;

	/**
	 * In case the above division has a remainder, we will end up with an
	 * additional (but smaller) bin at the end of the 32-bit interval. We'll
	 * discard the result if the number fell into this bin and repeat.
	 */
	u32 result = ceil;
	while (result >= ceil)
		result = lx_emul_random_gen_u32() / div;

	return result;
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
