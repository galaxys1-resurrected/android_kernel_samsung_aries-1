/*
 * Copyright (C)  2017, Cswl Coldwind <cswl1337@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/memblock.h>
#include <linux/mm.h>

#include <mach/media.h>
#include <plat/media.h>

#include "dynbigmem.h"

static struct s5p_media_device *media_devs;

/* sysfs interface */
static ssize_t enable_show(struct kobject *kobj,
        struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", dynbigmem_control);
}


static ssize_t enable_store(struct kobject *kobj, struct kobj_attribute *attr,
        const char *buf, size_t count)
{
    int input;
    struct s5p_media_device *mdev;
    int nr_mdevs;
    int i;
    int ret;

    sscanf(buf, "%du", &input);
    dynbigmem_control = input;
        media_devs=aries_get_media_devs(&nr_mdevs);
        if (!media_devs) {
        printk(KERN_INFO "dynbigmem: unable to get media_devs");
        return count;
     }

     printk(KERN_INFO "dynbigmem:  nr_mdevs=%d, mdevs=0x%p \n", nr_mdevs,  media_devs);
            for (i = 0; i < nr_mdevs; i++) {
        mdev = &media_devs[i];
        // Skip FIMD LCD driver and small JPEG
        if (!(strcmp(mdev->name, "fimd")) || !(strcmp(mdev->name, "jpeg") )) {
            continue;
        }

    printk(KERN_INFO "dynbigmem: %lu previous reserve:"
            "for %s at 0x%08x, %d-bank\n",
            (unsigned long) mdev->memsize, mdev->name, mdev->paddr,
            mdev->bank);

        switch (dynbigmem_control) {
        case nocam: 
            ret = memblock_free(mdev->paddr, mdev->memsize);
            ret = memblock_add(mdev->paddr, mdev->memsize);
            mdev->memsize=0;
            printk(KERN_INFO "dynbigmem: %s free: ret=%d\n",mdev->name, ret);
            break;
        }
    }
    return count;
}

static struct kobj_attribute enable_attribute =
    __ATTR(enable, 0666, enable_show, enable_store);

static struct attribute *attrs[] = {
    &enable_attribute.attr,
    NULL,
};

static struct attribute_group attr_group = {
    .attrs = attrs,
};

static struct kobject *enable_kobj;

static int __init dynbigmem_init(void)
{
    int retval;

    enable_kobj = kobject_create_and_add("dynbigmem", kernel_kobj);
    if (!enable_kobj) {
        return -ENOMEM;
    }
    retval = sysfs_create_group(enable_kobj, &attr_group);
    if (retval)
        kobject_put(enable_kobj);
    return retval;
}

MODULE_AUTHOR("Cswl Coldwind <cswl1337@gmail.com>");
MODULE_DESCRIPTION("Dynamic bigmem for aries");
MODULE_LICENSE("GPL");

module_init(dynbigmem_init);
