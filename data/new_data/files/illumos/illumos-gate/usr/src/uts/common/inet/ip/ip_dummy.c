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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#include <sys/types.h>
#include <sys/conf.h>
#include <sys/modctl.h>
#include <inet/common.h>

/*
 * Dummy streams module that is used by ICMP, UDP, and TCP by setting
 * INETMODSTRTAB to dummymodinfo
 *
 * It's reason for existance is so that mibopen() that I_PUSH icmp, udp, and
 * tcp can continue to push modules with those names, even though all the
 * MIB information comes from IP.
 */

static int	dummy_wput(queue_t *, mblk_t *);
static int	dummy_modclose(queue_t *, int, cred_t *);
static int	dummy_modopen(queue_t *q, dev_t *devp, int flag,
		    int sflag, cred_t *credp);

/*
 * This is common code for the tcp, udp, and icmp streams module which is
 * an empty STREAMS module provided for compatibility for mibopen()
 * code which I_PUSH modules with those names.
 */
struct module_info dummy_mod_info = {
	5799, "dummymod", 1, INFPSZ, 65536, 1024
};


static struct qinit dummyrmodinit = {
	dummy_wput, NULL, dummy_modopen, dummy_modclose, NULL,
	&dummy_mod_info
};

static struct qinit dummywmodinit = {
	dummy_wput, NULL, NULL, NULL, NULL, &dummy_mod_info
};

struct streamtab dummymodinfo = {
	&dummyrmodinit, &dummywmodinit
};

/* ARGSUSED */
static int
dummy_wput(queue_t *q, mblk_t *m)
{
	putnext(q, m);
	return (0);
}

static int
dummy_modclose(queue_t *q, int flags __unused, cred_t *credp __unused)
{
	qprocsoff(q);
	return (0);
}

/* ARGSUSED */
static int
dummy_modopen(queue_t *q, dev_t *devp, int flag, int sflag, cred_t *credp)
{
	/* If the stream is already open, return immediately. */
	if (q->q_ptr != NULL)
		return (0);

	/* If this is not a push of dummy as a module, fail. */
	if (sflag != MODOPEN)
		return (EINVAL);

	qprocson(q);
	return (0);
}
