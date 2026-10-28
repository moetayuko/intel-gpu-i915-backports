/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef _BACKPORT_DSM_AUXILIARY_BUS_H
#define _BACKPORT_DSM_AUXILIARY_BUS_H

#if IS_ENABLED(CPTCFG_AUXILIARY_BUS_BACKPORT)
#include <linux/auxiliary_bus-backport.h>
#else
#include_next <linux/auxiliary_bus.h>
#endif

#endif
