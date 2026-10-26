/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __BACKPORT_LINUX_SLAB_H
#define __BACKPORT_LINUX_SLAB_H

#include_next <linux/slab.h>
#include <linux/module.h>
#include <linux/overflow.h>

#ifdef BPM_KREALLOC_ARRAY_NOT_PRESENT
static inline void *krealloc_array(void *p, size_t n, size_t size, gfp_t flags)
{
	size_t bytes;

	if (check_mul_overflow(n, size, &bytes))
		return NULL;
	return krealloc(p, bytes, flags);
}
#endif

#ifdef BPM_KVREALLOC_OLDSIZE_PARAM_PRESENT
#ifndef BPM_KVREALLOC_NOPROF_PRESENT

#define __bp_kvrealloc_3(_p, _newsize, _flags) \
	kvrealloc(_p, ksize(_p), _newsize, _flags)
#define __bp_kvrealloc_4(_p, _oldsize, _newsize, _flags) \
	kvrealloc(_p, _oldsize, _newsize, _flags)
#define __bp_kvrealloc_pick(_1, _2, _3, _4, _pick, ...) _pick

#ifndef kvrealloc
#define kvrealloc(...) \
	__bp_kvrealloc_pick(__VA_ARGS__, __bp_kvrealloc_4, __bp_kvrealloc_3) \
	(__VA_ARGS__)
#endif

#endif
#endif

/* Common helper macros for slab allocation compatibility */
#ifndef __default_gfp
#define __default_gfp(a,b,...) b
#endif

#ifndef default_gfp
#define default_gfp(...) __default_gfp(,##__VA_ARGS__,GFP_KERNEL)
#endif

#if defined(BPM_KMALLOC_OBJ_NOT_PRESENT) || defined(BPM_KMALLOC_OBJS_NOT_PRESENT) || \
    defined(BPM_KZALLOC_OBJ_NOT_PRESENT) || defined(BPM_KZALLOC_OBJS_NOT_PRESENT) || \
    defined(BPM_KVZALLOC_OBJS_NOT_PRESENT) || defined(BPM_KVMALLOC_OBJS_NOT_PRESENT)
#ifndef __alloc_objs
#define __alloc_objs(KMALLOC, GFP, TYPE, COUNT)				\
({									\
	const size_t __obj_size = size_mul(sizeof(TYPE), COUNT);	\
	(TYPE *)KMALLOC(__obj_size, GFP);				\
})
#endif /* __alloc_objs */
#endif

#ifdef BPM_KMALLOC_OBJ_NOT_PRESENT
/**
 * kmalloc_obj - Allocate memory for a single object
 * @VAR_OR_TYPE: Variable or type to allocate
 * @...: optional GFP flags (defaults to GFP_KERNEL if not provided)
 */
#define kmalloc_obj(VAR_OR_TYPE, ...) \
	__alloc_objs(kmalloc, default_gfp(__VA_ARGS__), typeof(VAR_OR_TYPE), 1)
#endif /* BPM_KMALLOC_OBJ_NOT_PRESENT */

#ifdef BPM_KMALLOC_OBJS_NOT_PRESENT
/**
 * kmalloc_objs - Allocate memory for multiple objects
 * @VAR_OR_TYPE: Variable or type to allocate
 * @COUNT: number of objects to allocate
 * @...: optional GFP flags (defaults to GFP_KERNEL if not provided)
 */
#define kmalloc_objs(VAR_OR_TYPE, COUNT, ...) \
	__alloc_objs(kmalloc, default_gfp(__VA_ARGS__), typeof(VAR_OR_TYPE), COUNT)
#endif /* BPM_KMALLOC_OBJS_NOT_PRESENT */

#ifdef BPM_KZALLOC_OBJ_NOT_PRESENT
/**
 * kzalloc_obj - Allocate and zero memory for a single object
 * @P: pointer to the type of object to allocate
 * @...: optional GFP flags (defaults to GFP_KERNEL if not provided)
 *
 */
#define kzalloc_obj(P, ...) \
	__alloc_objs(kzalloc, default_gfp(__VA_ARGS__), typeof(P), 1)
#endif /* BPM_KZALLOC_OBJ_NOT_PRESENT */

#ifdef BPM_KZALLOC_OBJS_NOT_PRESENT
/**
 * kzalloc_objs - Allocate and zero memory for multiple objects
 * @P: pointer to the type of object to allocate
 * @COUNT: number of objects to allocate
 * @...: optional GFP flags (defaults to GFP_KERNEL if not provided)
 *
 */
#define kzalloc_objs(P, COUNT, ...) \
	__alloc_objs(kzalloc, default_gfp(__VA_ARGS__), typeof(P), COUNT)
#endif /* BPM_KZALLOC_OBJS_NOT_PRESENT */

#ifdef BPM_KVZALLOC_OBJS_NOT_PRESENT
/**
 * kvzalloc_objs - Allocate and zero memory for multiple objects
 * @P: pointer to the type of object to allocate
 * @COUNT: number of objects to allocate
 * @...: optional GFP flags (defaults to GFP_KERNEL if not provided)
 *
 * This macro provides compatibility for kernels where kvzalloc_objs
 * was introduced. Uses kvzalloc which tries kmalloc first, 
 * falls back to vmalloc.
 */
#define kvzalloc_objs(P, COUNT, ...) \
	__alloc_objs(kvzalloc, default_gfp(__VA_ARGS__), typeof(P), COUNT)
#endif /* BPM_KVZALLOC_OBJS_NOT_PRESENT */

#ifdef BPM_KVMALLOC_OBJS_NOT_PRESENT
/**
 * kvmalloc_objs - Allocate memory for multiple objects (vmalloc fallback)
 * @P: pointer to the type of object to allocate
 * @COUNT: number of objects to allocate
 * @...: optional GFP flags (defaults to GFP_KERNEL if not provided)
 *
 * This macro provides compatibility for kernels where kvmalloc_objs
 * was introduced. Uses kvmalloc which tries kmalloc first (no zeroing),
 * falls back to vmalloc.
 */
#define kvmalloc_objs(P, COUNT, ...) \
	__alloc_objs(kvmalloc, default_gfp(__VA_ARGS__), typeof(P), COUNT)
#endif /* BPM_KVMALLOC_OBJS_NOT_PRESENT */

#ifdef BPM_KZALLOC_FLEX_NOT_PRESENT
#if __has_builtin(__builtin_counted_by_ref) && \
    !defined(CONFIG_CC_HAS_BROKEN_COUNTED_BY_REF)
/**
 * __flex_counter() - Get pointer to counter member for the given
 *                    flexible array, if it was annotated with __counted_by()
 * @FAM: Pointer to flexible array member of an addressable struct instance
 *
 * For example, with:
 *
 *	struct foo {
 *		int counter;
 *		short array[] __counted_by(counter);
 *	} *p;
 *
 * __flex_counter(p->array) will resolve to &p->counter.
 *
 * Note that Clang may not allow this to be assigned to a separate
 * variable; it must be used directly.
 *
 * If p->array is unannotated, this returns (void *)NULL.
 */
#define __flex_counter(FAM)	__builtin_counted_by_ref(FAM)
#else
#define __flex_counter(FAM)	((void *)NULL)
#endif

/**
 * __set_flex_counter() - Set the counter associated with the given flexible
 *                        array member that has been annoated by __counted_by()
 * @FAM: Instance of flexible array member within a given struct.
 * @COUNT: Value to store to the __counted_by annotated @FAM_PTR's counter.
 *
 * This is a no-op if no annotation exists. Count needs to be checked with
 * overflows_flex_counter_type() before using this function.
 */
#define __set_flex_counter(FAM, COUNT)				\
({								\
	*_Generic(__flex_counter(FAM),				\
		  void *:  &(size_t){ 0 },			\
		  default: __flex_counter(FAM)) = (COUNT);	\
})

#ifndef __alloc_flex
#define __alloc_flex(KMALLOC, GFP, TYPE, FAM, COUNT)			\
({									\
	const size_t __count = (COUNT);					\
	const size_t __obj_size = struct_size_t(TYPE, FAM, __count);	\
	TYPE *__obj_ptr = KMALLOC(__obj_size, GFP);			\
	if (__obj_ptr)							\
		__set_flex_counter(__obj_ptr->FAM, __count);		\
	__obj_ptr;							\
})
#endif /* __alloc_flex */

/**
 * kzalloc_flex - Allocate a single instance of the given flexible structure
 * @VAR_OR_TYPE: Variable or type to allocate (with its flex array).
 * @FAM: The name of the flexible array member of the structure.
 * @COUNT: How many flexible array member elements are desired.
 * @...: optional GFP flags (defaults to GFP_KERNEL if not provided)
 *
 * Returns: newly allocated pointer to @VAR_OR_TYPE on success, NULL on
 * failure. If @FAM has been annotated with __counted_by(), the allocation
 * will immediately fail if @COUNT is larger than what the type of the
 * struct's counter variable can represent.
 */
#define kzalloc_flex(P, FAM, COUNT, ...) \
	__alloc_flex(kzalloc, default_gfp(__VA_ARGS__), typeof(P), FAM, COUNT)
#endif /* BPM_KZALLOC_FLEX_NOT_PRESENT */

#endif /* _BACKPORT_LINUX_SLAB_H */
