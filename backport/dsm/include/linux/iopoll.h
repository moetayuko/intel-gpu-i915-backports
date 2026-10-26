/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_IOPOLL_H
#define _BACKPORT_DSM_IOPOLL_H

#include_next <linux/iopoll.h>

#define poll_timeout_us_atomic(op, cond, delay_us, timeout_us, \
			       delay_before_op) \
({ \
	u64 __timeout_us = (timeout_us); \
	s64 __left_ns = __timeout_us * NSEC_PER_USEC; \
	unsigned long __delay_us = (delay_us); \
	u64 __delay_ns = __delay_us * NSEC_PER_USEC; \
	int ___ret; \
	if ((delay_before_op) && __delay_us) { \
		udelay(__delay_us); \
		if (__timeout_us) \
			__left_ns -= __delay_ns; \
	} \
	for (;;) { \
		bool __expired = __timeout_us && __left_ns < 0; \
		/* guarantee 'op' and 'cond' are evaluated after timeout expired */ \
		barrier(); \
		op; \
		if (cond) { \
			___ret = 0; \
			break; \
		} \
		if (__expired) { \
			___ret = -ETIMEDOUT; \
			break; \
		} \
		if (__delay_us) { \
			udelay(__delay_us); \
			if (__timeout_us) \
				__left_ns -= __delay_ns; \
		} \
		cpu_relax(); \
		if (__timeout_us) \
			__left_ns--; \
	} \
	___ret; \
})

#endif
