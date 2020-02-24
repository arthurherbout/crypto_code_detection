/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2011 Nexenta Systems, Inc.  All rights reserved.
 */
/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma weak __logb = logb
#pragma weak _logb = logb

#include "libm.h"
#include "xpg6.h"	/* __xpg6 */
#define	_C99SUSv3_logb	_C99SUSv3_logb_subnormal_is_like_ilogb

#if defined(__x86)
static const double two52 = 4503599627370496.0;
#else
/*
 * v: high part of a non-zero subnormal |x|; w: low part of |x|
 */
static int
ilogb_subnormal(unsigned v, unsigned w) {
	int r = -1022 - 52;

	if (v)
		r += 32;
	else
		v = w;
	if (v & 0xffff0000)
		r += 16, v >>= 16;
	if (v & 0xff00)
		r += 8, v >>= 8;
	if (v & 0xf0)
		r += 4, v >>= 4;
	v <<= 1;
	return (r + ((0xffffaa50 >> v) & 0x3));
}
#endif	/* defined(__x86) */

double
logb(double x) {
	int *px = (int *) &x, k = px[HIWORD] & ~0x80000000;

	if (k < 0x00100000) {
		if ((px[LOWORD] | k) == 0)
			return (_SVID_libm_err(x, x, 45));
		else if ((__xpg6 & _C99SUSv3_logb) != 0) {
#if defined(__x86)
			x *= two52;
			return ((double) (((px[HIWORD] & 0x7ff00000) >> 20)
				- 1075));
#else
			return ((double) ilogb_subnormal(k, px[LOWORD]));
#endif
		} else
			return (-1022.0);
	} else if (k < 0x7ff00000)
		return ((double) ((k >> 20) - 1023));
	else
		return (x * x);
}
