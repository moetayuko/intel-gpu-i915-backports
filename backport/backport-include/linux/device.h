#ifndef __BACKPORT_DEVICE_H_
#define __BACKPORT_DEVICE_H_
#include_next <linux/device.h>

#include <linux/irq.h>

#ifndef DEVICE_ATTR_ADMIN_RW
#define DEVICE_ATTR_ADMIN_RW(_name) \
	struct device_attribute dev_attr_##_name = __ATTR_RW_MODE(_name, 0600)
#endif

#ifndef DEVICE_ATTR_ADMIN_RO
#define DEVICE_ATTR_ADMIN_RO(_name) \
	struct device_attribute dev_attr_##_name = __ATTR_RO_MODE(_name, 0400)
#endif

#ifdef BPM_CLASS_CREATE_NO_OWNER_NOT_PRESENT
#ifdef class_create
#undef class_create
#endif
/**
 * class_create - create a struct class structure
 * @owner: pointer to the module that is to "own" this struct class
 * @name: pointer to a string for the name of this class.
 *
 * This is used to create a struct class pointer that can then be used
 * in calls to device_create().
 *
 * Returns &struct class pointer on success, or ERR_PTR() on error.
 *
 * Note, the pointer created here is to be destroyed when finished by
 * making a call to class_destroy().
 */
#define class_create(name)				\
({							\
	static struct lock_class_key __key;		\
	__class_create(THIS_MODULE, name, &__key);	\
})
#endif

#ifndef PCI_DVSEC_HEADER1_LEN
#define PCI_DVSEC_HEADER1_LEN(x)   (((x) >> 20) & 0xfff)
#endif

#endif /* __BACKPORT_DEVICE_H_ */
