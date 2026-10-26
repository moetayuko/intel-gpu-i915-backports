#ifndef __BACKPORT_LINUX_DMA_RESV_H
#define __BACKPORT_LINUX_DMA_RESV_H

#include_next <linux/dma-resv.h>

#ifdef BPM_DMA_RESV_SHARED_LIST_NOT_PRESENT
#define dma_resv_shared_list dma_resv_get_list
#endif

#ifdef BPM_DMA_RESV_USAGE_NOT_PRESENT
enum dma_resv_usage {
	DMA_RESV_USAGE_KERNEL,
	DMA_RESV_USAGE_WRITE,
	DMA_RESV_USAGE_READ,
	DMA_RESV_USAGE_BOOKKEEP,
};

static inline enum dma_resv_usage dma_resv_usage_rw(bool write)
{
	return write ? DMA_RESV_USAGE_READ : DMA_RESV_USAGE_WRITE;
}

/* Tagged-pointer dma_resv fence list: low bits of each fence pointer encode its usage. */
#define DMA_RESV_LIST_MASK 0x3

#define dma_resv_iter			LINUX_BACKPORT(dma_resv_iter)
struct dma_resv_iter {
	struct dma_resv *obj;
	enum dma_resv_usage usage;
	struct dma_fence *fence;
	enum dma_resv_usage fence_usage;
	unsigned int index;
	struct dma_resv_list *fences;
	unsigned int num_fences;
	bool is_restarted;
};

#define dma_resv_iter_begin		LINUX_BACKPORT(dma_resv_iter_begin)
static inline void dma_resv_iter_begin(struct dma_resv_iter *cursor,
					struct dma_resv *obj,
					enum dma_resv_usage usage)
{
	cursor->obj = obj;
	cursor->usage = usage;
	cursor->fence = NULL;
}

#define dma_resv_iter_end		LINUX_BACKPORT(dma_resv_iter_end)
static inline void dma_resv_iter_end(struct dma_resv_iter *cursor)
{
	dma_fence_put(cursor->fence);
}

#define dma_resv_iter_usage		LINUX_BACKPORT(dma_resv_iter_usage)
static inline enum dma_resv_usage
dma_resv_iter_usage(struct dma_resv_iter *cursor)
{
	return cursor->fence_usage;
}

#define dma_resv_iter_is_restarted	LINUX_BACKPORT(dma_resv_iter_is_restarted)
static inline bool
dma_resv_iter_is_restarted(struct dma_resv_iter *cursor)
{
	return cursor->is_restarted;
}

#define dma_resv_iter_first		LINUX_BACKPORT(dma_resv_iter_first)
struct dma_fence *dma_resv_iter_first(struct dma_resv_iter *cursor);

#define dma_resv_iter_next		LINUX_BACKPORT(dma_resv_iter_next)
struct dma_fence *dma_resv_iter_next(struct dma_resv_iter *cursor);

#define dma_resv_iter_first_unlocked	LINUX_BACKPORT(dma_resv_iter_first_unlocked)
struct dma_fence *dma_resv_iter_first_unlocked(struct dma_resv_iter *cursor);

#define dma_resv_iter_next_unlocked	LINUX_BACKPORT(dma_resv_iter_next_unlocked)
struct dma_fence *dma_resv_iter_next_unlocked(struct dma_resv_iter *cursor);

#ifdef BPM_DMA_RESV_FOR_EACH_FENCE_UNLOCKED_NOT_PRESENT
#define dma_resv_for_each_fence_unlocked(cursor, fence)		\
	for ((fence) = dma_resv_iter_first_unlocked((cursor));		\
	     (fence);							\
	     (fence) = dma_resv_iter_next_unlocked((cursor)))
#endif

#define dma_resv_wait_timeout		LINUX_BACKPORT(dma_resv_wait_timeout)
long dma_resv_wait_timeout(struct dma_resv *obj, enum dma_resv_usage usage,
			   bool intr, unsigned long timeout);

#define dma_resv_test_signaled		LINUX_BACKPORT(dma_resv_test_signaled)
bool dma_resv_test_signaled(struct dma_resv *obj, enum dma_resv_usage usage);

#define dma_resv_set_deadline		LINUX_BACKPORT(dma_resv_set_deadline)
void dma_resv_set_deadline(struct dma_resv *obj, enum dma_resv_usage usage,
			   ktime_t deadline);

#define dma_resv_copy_fences		LINUX_BACKPORT(dma_resv_copy_fences)
int dma_resv_copy_fences(struct dma_resv *dst, struct dma_resv *src);

#define dma_resv_add_fence		LINUX_BACKPORT(dma_resv_add_fence)
void dma_resv_add_fence(struct dma_resv *obj, struct dma_fence *fence,
			enum dma_resv_usage usage);

#define dma_resv_get_fences		LINUX_BACKPORT(dma_resv_get_fences)
int dma_resv_get_fences(struct dma_resv *obj, enum dma_resv_usage usage,
			unsigned int *num_fences, struct dma_fence ***fences);

#define dma_resv_get_singleton		LINUX_BACKPORT(dma_resv_get_singleton)
int dma_resv_get_singleton(struct dma_resv *obj, enum dma_resv_usage usage,
			   struct dma_fence **fence);

#define dma_resv_reserve_fences		LINUX_BACKPORT(dma_resv_reserve_fences)
int dma_resv_reserve_fences(struct dma_resv *obj, unsigned int num_fences);

#define dma_resv_replace_fences		LINUX_BACKPORT(dma_resv_replace_fences)
void dma_resv_replace_fences(struct dma_resv *obj, uint64_t context,
			     struct dma_fence *replacement,
			     enum dma_resv_usage usage);

/* Intercept teardown so tag bits are stripped before generic code frees the fence list. */
#define dma_resv_fini			LINUX_BACKPORT(dma_resv_fini)
void dma_resv_fini(struct dma_resv *obj);

/* No native equivalent: strips the usage tag bits before generic code touches the fence list. */
void dma_resv_recover_ptr(struct dma_resv *obj);
#else
static inline void dma_resv_recover_ptr(struct dma_resv *obj) { }
#endif

#ifdef BPM_DMA_RESV_FOR_EACH_FENCE_NOT_PRESENT
#define dma_resv_for_each_fence(cursor, obj, usage, fence) \
	for (dma_resv_iter_begin((cursor), (obj), (usage)), \
	     (fence) = dma_resv_iter_first((cursor)); \
	     (fence); \
	     (fence) = dma_resv_iter_next((cursor)))
#endif

#endif
