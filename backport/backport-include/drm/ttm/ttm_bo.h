#ifndef __BACKPORT_DRM_TTM_BO_H__
#define __BACKPORT_DRM_TTM_BO_H__

#include_next <drm/ttm/ttm_bo.h>

#ifdef BPM_TTM_BO_FINI_PRESENT
#define ttm_bo_put ttm_bo_fini
#endif

void ttm_bo_set_bulk_move(struct ttm_buffer_object *bo,
                          struct ttm_lru_bulk_move *bulk);

#endif /* __BACKPORT_DRM_TTM_BO_H__ */
