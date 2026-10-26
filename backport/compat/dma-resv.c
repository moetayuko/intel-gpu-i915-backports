// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2012-2014 Canonical Ltd (Maarten Lankhorst)
 *
 * Based on bo.c which bears the following copyright notice,
 * but is dual licensed:
 *
 * Copyright (c) 2006-2009 VMware, Inc., Palo Alto, CA., USA
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/
/*
 * Authors: Thomas Hellstrom <thellstrom-at-vmware-dot-com>
 */

#include <linux/dma-resv.h>
#include <linux/dma-fence-array.h>
#include <linux/export.h>
#include <linux/mm.h>
#include <linux/sched/mm.h>
#include <linux/mmu_notifier.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/overflow.h>
#include <drm/drm_gem.h>

#ifdef BPM_DMA_RESV_USAGE_NOT_PRESENT

static void dma_resv_list_entry_tagged(struct dma_resv_list *list,
					unsigned int index,
					struct dma_fence **fence,
					enum dma_resv_usage *usage)
{
	unsigned long tmp;

	/* Locked callers pass a fully serialized list; unlocked callers hold rcu_read_lock(). */
	tmp = (unsigned long)rcu_dereference_check(list->shared[index], 1);
	*fence = (struct dma_fence *)(tmp & ~DMA_RESV_LIST_MASK);
	if (usage)
		*usage = (enum dma_resv_usage)(tmp & DMA_RESV_LIST_MASK);
}

static void dma_resv_list_set_tagged(struct dma_resv_list *list,
				      unsigned int index,
				      struct dma_fence *fence,
				      enum dma_resv_usage usage)
{
	unsigned long tmp = ((unsigned long)fence) | (unsigned long)usage;

	RCU_INIT_POINTER(list->shared[index], (struct dma_fence *)tmp);
}

void dma_resv_recover_ptr(struct dma_resv *obj)
{
	struct dma_resv_list *list;
	int i;

	list = rcu_dereference_protected(obj->fence, 1);
	if (!list)
		return;

	for (i = 0; i < list->shared_count; ++i) {
		unsigned long tmp = (unsigned long)list->shared[i];

		list->shared[i] = (struct dma_fence *)(tmp & ~DMA_RESV_LIST_MASK);
	}
}
EXPORT_SYMBOL(dma_resv_recover_ptr);

void dma_resv_add_fence(struct dma_resv *obj, struct dma_fence *fence,
			enum dma_resv_usage usage)
{
	struct dma_resv_list *list;
	struct dma_fence *old;
	unsigned int i, count;
	int err;

	dma_resv_assert_held(obj);

	if (WARN_ON(!fence))
		return;

	list = dma_resv_shared_list(obj);
	count = list ? list->shared_count : 0;

	/* Reuse an already-signaled slot, or an older fence from the same
	 * context, instead of growing the list without bound.
	 */
	for (i = 0; i < count; ++i) {
		enum dma_resv_usage old_usage;

		dma_resv_list_entry_tagged(list, i, &old, &old_usage);
		/* dma_fence_is_later_or_same() isn't available on this baseline. */
		if ((old->context == fence->context && old_usage >= usage &&
		     (fence == old || dma_fence_is_later(fence, old))) ||
		    dma_fence_is_signaled(old)) {
			dma_fence_get(fence);
			write_seqcount_begin(&obj->seq);
			dma_resv_list_set_tagged(list, i, fence, usage);
			write_seqcount_end(&obj->seq);
			dma_fence_put(old);
			return;
		}
	}

	if (!list || count >= list->shared_max) {
		err = dma_resv_reserve_fences(obj, 1);
		if (err)
			return;
		list = dma_resv_shared_list(obj);
	}

	if (WARN_ON(!list))
		return;

	dma_fence_get(fence);

	write_seqcount_begin(&obj->seq);
	dma_resv_list_set_tagged(list, count, fence, usage);
	/* Publish tagged slot before extending shared_count for lockless readers. */
	smp_wmb();
	WRITE_ONCE(list->shared_count, count + 1);
	write_seqcount_end(&obj->seq);
}
EXPORT_SYMBOL(dma_resv_add_fence);

/* Restart the unlocked iteration by initializing the cursor object. */
static void dma_resv_iter_restart_unlocked(struct dma_resv_iter *cursor)
{
	cursor->index = 0;
	cursor->num_fences = 0;
	cursor->fences = dma_resv_shared_list(cursor->obj);
	if (cursor->fences)
		cursor->num_fences = READ_ONCE(cursor->fences->shared_count);
	cursor->is_restarted = true;
}

/* Walk to the next not signaled fence and grab a reference to it. */
static void dma_resv_iter_walk_unlocked(struct dma_resv_iter *cursor)
{
	if (!cursor->fences)
		return;

	do {
		struct dma_fence *raw;

		/* Drop the reference from the previous round. */
		dma_fence_put(cursor->fence);

		if (cursor->index >= cursor->num_fences) {
			cursor->fence = NULL;
			break;
		}

		dma_resv_list_entry_tagged(cursor->fences, cursor->index++,
					    &raw, &cursor->fence_usage);
		cursor->fence = dma_fence_get_rcu(raw);
		if (!cursor->fence) {
			dma_resv_iter_restart_unlocked(cursor);
			continue;
		}

		if (!dma_fence_is_signaled(cursor->fence) &&
		    cursor->usage >= cursor->fence_usage)
			break;
	} while (true);
}

struct dma_fence *dma_resv_iter_first_unlocked(struct dma_resv_iter *cursor)
{
	rcu_read_lock();
	do {
		dma_resv_iter_restart_unlocked(cursor);
		dma_resv_iter_walk_unlocked(cursor);
	} while (dma_resv_shared_list(cursor->obj) != cursor->fences);
	rcu_read_unlock();

	return cursor->fence;
}
EXPORT_SYMBOL(dma_resv_iter_first_unlocked);

struct dma_fence *dma_resv_iter_next_unlocked(struct dma_resv_iter *cursor)
{
	bool restart;

	rcu_read_lock();
	cursor->is_restarted = false;
	restart = dma_resv_shared_list(cursor->obj) != cursor->fences;
	do {
		if (restart)
			dma_resv_iter_restart_unlocked(cursor);
		dma_resv_iter_walk_unlocked(cursor);
		restart = true;
	} while (dma_resv_shared_list(cursor->obj) != cursor->fences);
	rcu_read_unlock();

	return cursor->fence;
}
EXPORT_SYMBOL(dma_resv_iter_next_unlocked);

struct dma_fence *dma_resv_iter_first(struct dma_resv_iter *cursor)
{
	struct dma_fence *fence;

	dma_resv_assert_held(cursor->obj);

	cursor->index = 0;
	cursor->fences = dma_resv_shared_list(cursor->obj);
	cursor->num_fences = cursor->fences ? cursor->fences->shared_count : 0;

	fence = dma_resv_iter_next(cursor);
	cursor->is_restarted = true;
	return fence;
}
EXPORT_SYMBOL_GPL(dma_resv_iter_first);

struct dma_fence *dma_resv_iter_next(struct dma_resv_iter *cursor)
{
	struct dma_fence *fence;

	dma_resv_assert_held(cursor->obj);

	cursor->is_restarted = false;

	do {
		if (!cursor->fences || cursor->index >= cursor->num_fences)
			return NULL;

		dma_resv_list_entry_tagged(cursor->fences, cursor->index++,
					    &fence, &cursor->fence_usage);
	} while (cursor->fence_usage > cursor->usage);

	return fence;
}
EXPORT_SYMBOL_GPL(dma_resv_iter_next);

/*
 * dma_resv_init/fini, dma_resv_lock* and the exclusive/shared fence
 * primitives are all provided by the running kernel already; only the
 * usage-tagged fence list and the iterator built on top of it are missing
 * on this baseline, so that's all that gets backported here.
 */

/**
 * dma_resv_get_fences - Get an object's fences
 * fences without update side lock held
 * @obj: the reservation object
 * @usage: controls which fences to include, see enum dma_resv_usage.
 * @num_fences: the number of fences returned
 * @fences: the array of fence ptrs returned (array is krealloc'd to the
 * required size, and must be freed by caller)
 *
 * Retrieve all fences from the reservation object.
 * Returns either zero or -ENOMEM.
 */
int dma_resv_get_fences(struct dma_resv *obj, enum dma_resv_usage usage,
			unsigned int *num_fences, struct dma_fence ***fences)
{
	struct dma_resv_iter cursor;
	struct dma_fence *fence;
	struct dma_fence **array = NULL;
	unsigned int count = 0, max = 0;

	*num_fences = 0;
	*fences = NULL;

	dma_resv_iter_begin(&cursor, obj, usage);
	dma_resv_for_each_fence_unlocked(&cursor, fence) {

		if (dma_resv_iter_is_restarted(&cursor)) {
			while (count)
				dma_fence_put(array[--count]);
		}

		if (count == max) {
			unsigned int new_max = max ? max * 2 : 4;
			struct dma_fence **new_array;

			new_array = krealloc_array(array, new_max,
						    sizeof(*array), GFP_KERNEL);
			if (!new_array) {
				dma_resv_iter_end(&cursor);
				while (count--)
					dma_fence_put(array[count]);
				kfree(array);
				return -ENOMEM;
			}
			array = new_array;
			max = new_max;
		}

		array[count++] = dma_fence_get(fence);
	}
	dma_resv_iter_end(&cursor);

	*num_fences = count;
	*fences = array;
	return 0;
}
EXPORT_SYMBOL_GPL(dma_resv_get_fences);

/**
 * dma_resv_get_singleton - Get a single fence for all the fences
 * @obj: the reservation object
 * @usage: controls which fences to include, see enum dma_resv_usage.
 * @fence: the resulting fence
 *
 * Get a single fence representing all the fences inside the resv object.
 * Returns either 0 for success or -ENOMEM.
 */
int dma_resv_get_singleton(struct dma_resv *obj, enum dma_resv_usage usage,
			    struct dma_fence **fence)
{
	struct dma_fence_array *array;
	struct dma_fence **fences;
	unsigned int count;
	int r;

	r = dma_resv_get_fences(obj, usage, &count, &fences);
	if (r)
		return r;

	if (count == 0) {
		*fence = NULL;
		return 0;
	}

	if (count == 1) {
		*fence = fences[0];
		kfree(fences);
		return 0;
	}

	array = dma_fence_array_create(count, fences,
					dma_fence_context_alloc(1), 1, false);
	if (!array) {
		while (count--)
			dma_fence_put(fences[count]);
		kfree(fences);
		return -ENOMEM;
	}

	*fence = &array->base;
	return 0;
}
EXPORT_SYMBOL_GPL(dma_resv_get_singleton);

void dma_resv_replace_fences(struct dma_resv *obj, uint64_t context,
			      struct dma_fence *replacement,
			      enum dma_resv_usage usage)
{
	struct dma_resv_list *list;
	unsigned int i;

	dma_resv_assert_held(obj);

	list = dma_resv_shared_list(obj);
	for (i = 0; list && i < READ_ONCE(list->shared_count); ++i) {
		struct dma_fence *old;
		enum dma_resv_usage old_usage;

		dma_resv_list_entry_tagged(list, i, &old, &old_usage);
		if (old->context != context)
			continue;
		if (!replacement)
			continue;

		write_seqcount_begin(&obj->seq);
		dma_resv_list_set_tagged(list, i, dma_fence_get(replacement), usage);
		write_seqcount_end(&obj->seq);
		dma_fence_put(old);
	}
}
EXPORT_SYMBOL(dma_resv_replace_fences);

long dma_resv_wait_timeout(struct dma_resv *obj, enum dma_resv_usage usage,
			    bool intr, unsigned long timeout)
{
	long ret = timeout ? timeout : 1;
	struct dma_resv_iter cursor;
	struct dma_fence *fence;

	dma_resv_iter_begin(&cursor, obj, usage);
	dma_resv_for_each_fence_unlocked(&cursor, fence) {
		ret = dma_fence_wait_timeout(fence, intr, timeout);
		if (ret <= 0)
			break;

		if (timeout)
			timeout = ret;
	}
	dma_resv_iter_end(&cursor);

	return ret;
}
EXPORT_SYMBOL_GPL(dma_resv_wait_timeout);

bool dma_resv_test_signaled(struct dma_resv *obj, enum dma_resv_usage usage)
{
	struct dma_resv_iter cursor;
	struct dma_fence *fence;

	dma_resv_iter_begin(&cursor, obj, usage);
	dma_resv_for_each_fence_unlocked(&cursor, fence) {
		dma_resv_iter_end(&cursor);
		return false;
	}
	dma_resv_iter_end(&cursor);

	return true;
}
EXPORT_SYMBOL_GPL(dma_resv_test_signaled);

void dma_resv_set_deadline(struct dma_resv *obj, enum dma_resv_usage usage,
			    ktime_t deadline)
{
	struct dma_resv_iter cursor;
	struct dma_fence *fence;

	dma_resv_iter_begin(&cursor, obj, usage);
	dma_resv_for_each_fence_unlocked(&cursor, fence)
		dma_fence_set_deadline(fence, deadline);
	dma_resv_iter_end(&cursor);
}
EXPORT_SYMBOL_GPL(dma_resv_set_deadline);

int dma_resv_reserve_fences(struct dma_resv *obj, unsigned int num_fences)
{
	struct dma_resv_list *old, *new;
	unsigned int i, j, k, max;

	dma_resv_assert_held(obj);

	if (WARN_ON(!num_fences))
		return -EINVAL;

	old = dma_resv_shared_list(obj);
	if (old && old->shared_max) {
		if ((old->shared_count + num_fences) <= old->shared_max)
			return 0;
		max = max(old->shared_count + num_fences, old->shared_max * 2);
	} else {
		max = max(4ul, roundup_pow_of_two(num_fences));
	}

	new = kmalloc(struct_size(new, shared, max), GFP_KERNEL);
	if (!new)
		return -ENOMEM;

	new->shared_max = (ksize(new) - offsetof(typeof(*new), shared)) /
		sizeof(*new->shared);

	/*
	 * No need to bump fence refcounts: rcu_read access requires
	 * kref_get_unless_zero(), and the references from the old list
	 * carry over to the new one. Entries are copied by fence identity
	 * and usage, never as raw tagged pointers, so the new list is built
	 * fully tagged and swapped in atomically for unlocked readers.
	 */
	for (i = 0, j = 0, k = new->shared_max;
	     i < (old ? old->shared_count : 0); ++i) {
		enum dma_resv_usage usage;
		struct dma_fence *fence;

		dma_resv_list_entry_tagged(old, i, &fence, &usage);
		if (dma_fence_is_signaled(fence))
			RCU_INIT_POINTER(new->shared[--k], fence);
		else
			dma_resv_list_set_tagged(new, j++, fence, usage);
	}
	new->shared_count = j;

	rcu_assign_pointer(obj->fence, new);

	if (!old)
		return 0;

	/* Drop the references to the signaled fences */
	for (i = k; i < new->shared_max; ++i) {
		struct dma_fence *fence;

		fence = rcu_dereference_protected(new->shared[i], dma_resv_held(obj));
		dma_fence_put(fence);
	}
	kfree_rcu(old, rcu);

	return 0;
}
EXPORT_SYMBOL_GPL(dma_resv_reserve_fences);

int dma_resv_copy_fences(struct dma_resv *dst, struct dma_resv *src)
{
	struct dma_resv_iter cursor;
	struct dma_fence *f;
	int ret;

	dma_resv_iter_begin(&cursor, src, DMA_RESV_USAGE_BOOKKEEP);
	for (f = dma_resv_iter_first_unlocked(&cursor); f;
	     f = dma_resv_iter_next_unlocked(&cursor)) {
		ret = dma_resv_reserve_fences(dst, 1);
		if (ret) {
			dma_resv_iter_end(&cursor);
			return ret;
		}
		dma_resv_add_fence(dst, f, dma_resv_iter_usage(&cursor));
	}
	dma_resv_iter_end(&cursor);
	return 0;
}
EXPORT_SYMBOL_GPL(dma_resv_copy_fences);

void dma_resv_fini(struct dma_resv *obj)
{
	dma_resv_recover_ptr(obj);
	/* Undef the redirect so this calls the kernel's real dma_resv_fini(), not itself. */
#undef dma_resv_fini
	dma_resv_fini(obj);
#define dma_resv_fini LINUX_BACKPORT(dma_resv_fini)
}
EXPORT_SYMBOL(dma_resv_fini);

#ifndef CPTCFG_DRM_BACKPORT
void drm_gem_object_release(struct drm_gem_object *obj)
{
	dma_resv_recover_ptr(&obj->_resv);
	/* Undef the redirect so this calls the kernel's real drm_gem_object_release(), not itself. */
#undef drm_gem_object_release
	drm_gem_object_release(obj);
#define drm_gem_object_release LINUX_BACKPORT(drm_gem_object_release)
}
EXPORT_SYMBOL(drm_gem_object_release);
#endif

#if IS_ENABLED(CONFIG_LOCKDEP)
static int __init dma_resv_lockdep(void)
{
	struct mm_struct *mm = mm_alloc();
	struct ww_acquire_ctx ctx;
	struct dma_resv obj;
	struct address_space mapping;
	int ret;

	if (!mm)
		return -ENOMEM;

	dma_resv_init(&obj);
	address_space_init_once(&mapping);

	mmap_read_lock(mm);
	ww_acquire_init(&ctx, &reservation_ww_class);
	ret = dma_resv_lock(&obj, &ctx);
	if (ret) {
		/* Only EDEADLK from the error injection is possible here */
		WARN_ON(ret != -EDEADLK);
		dma_resv_lock_slow(&obj, &ctx);
	}
	fs_reclaim_acquire(GFP_KERNEL);
	/* for unmap_mapping_range on trylocked buffer objects in shrinkers */
	i_mmap_lock_write(&mapping);
	i_mmap_unlock_write(&mapping);
#ifdef CONFIG_MMU_NOTIFIER
	lock_map_acquire(&__mmu_notifier_invalidate_range_start_map);
	__dma_fence_might_wait();
	lock_map_release(&__mmu_notifier_invalidate_range_start_map);
#else
	__dma_fence_might_wait();
#endif
	fs_reclaim_release(GFP_KERNEL);
	ww_mutex_unlock(&obj.lock);
	ww_acquire_fini(&ctx);
	mmap_read_unlock(mm);

	mmput(mm);

	return 0;
}
subsys_initcall(dma_resv_lockdep);
#endif

#endif /* BPM_DMA_RESV_USAGE_NOT_PRESENT */
