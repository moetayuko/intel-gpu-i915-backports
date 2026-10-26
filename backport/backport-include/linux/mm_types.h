#ifndef _BACKPORT_LINUX_MM_TYPES_H
#define _BACKPORT_LINUX_MM_TYPES_H

#include_next <linux/mm_types.h>

#ifndef EMPTY_VMA_FLAGS
  #ifdef BPM_VMA_FLAGS_T_NOT_PRESENT
    #define EMPTY_VMA_FLAGS 0
  #else
    #define EMPTY_VMA_FLAGS ((vma_flags_t){ })
  #endif
#endif /* EMPTY_VMA_FLAGS */

#endif /* _BACKPORT_LINUX_MM_TYPES_H */
