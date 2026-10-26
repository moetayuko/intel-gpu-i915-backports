/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_PWM_H
#define _BACKPORT_DSM_PWM_H

#include_next <linux/pwm.h>

#define pwm_apply_might_sleep pwm_apply_state

#endif
