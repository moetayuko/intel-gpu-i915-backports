/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_DMA_FENCE_CHAIN_H
#define _BACKPORT_DSM_DMA_FENCE_CHAIN_H

#include_next <linux/dma-fence-chain.h>
#include <linux/slab.h>

#define dma_fence_chain_alloc() \
	((struct dma_fence_chain *)kmalloc(sizeof(struct dma_fence_chain), GFP_KERNEL))

static inline void dma_fence_chain_free(struct dma_fence_chain *chain) { kfree(chain); }

static inline struct dma_fence *dma_fence_chain_contained(struct dma_fence *fence)
{
 struct dma_fence_chain *chain = to_dma_fence_chain(fence);
 return chain ? chain->fence : fence;
}

#endif
