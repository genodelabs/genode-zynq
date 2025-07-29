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


#include <linux/blkdev.h>

int bd_prepare_to_claim(struct block_device * bdev,void * holder,
                        const struct blk_holder_ops *hops)
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


#include <linux/blkdev.h>

struct file file_dummy;

struct file * bdev_file_open_by_dev(dev_t dev,blk_mode_t mode,void * holder,const struct blk_holder_ops * hops)
{
	return &file_dummy;
}


#include <linux/file.h>

void fput(struct file * file)
{
	if (file != &file_dummy)
		lx_emul_trace_and_stop(__func__);
}
