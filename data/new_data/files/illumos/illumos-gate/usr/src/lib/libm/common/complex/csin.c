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

#pragma weak __csin = csin

/* INDENT OFF */
/*
 * dcomplex csin(dcomplex z);
 *
 * If z = x+iy, then since csin(iz) = i*csinh(z),  we have
 *
 * csin(z)	= csin((-1)*(-z)) = csin(i*i*(-z))
 *		= i*csinh(i*(-z)) = i*csinh(i*(-x-yi))
 *		= i*csinh(y-ix)
 *		= -Im(csinh(y-ix))+i*Re(csinh(y-ix))
 */
/* INDENT ON */

#include "libm.h"
#include "complex_wrapper.h"

dcomplex
csin(dcomplex z) {
	double x, y;
	dcomplex ans, ct;

	x = D_RE(z);
	y = D_IM(z);
	D_RE(z) = y;
	D_IM(z) = -x;
	ct = csinh(z);
	D_RE(ans) = -D_IM(ct);
	D_IM(ans) = D_RE(ct);
	return (ans);
}
