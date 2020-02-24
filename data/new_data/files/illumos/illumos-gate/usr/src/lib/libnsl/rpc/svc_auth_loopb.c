/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Copyright 2017 Joyent Inc
 * Use is subject to license terms.
 */

/*
 * Handles the loopback UNIX flavor authentication parameters on the
 * service side of rpc.
 */

#include "mt.h"
#include <stdio.h>
#include <rpc/rpc.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/debug.h>

/*
 * NOTE: this has to fit inside RQCRED_SIZE bytes. If you update this struct,
 * double-check it still fits.
 */
struct authlpbk_area {
	struct authsys_parms area_aup;
	char area_machname[MAX_MACHINE_NAME+1];
	gid_t area_gids[NGRPS_LOOPBACK];
};
CTASSERT(sizeof (struct authlpbk_area) <= RQCRED_SIZE);

/*
 * Loopback system (Unix) longhand authenticator
 */
enum auth_stat
__svcauth_loopback(struct svc_req *rqst, struct rpc_msg *msg)
{
	struct authsys_parms *aup;
	int32_t *buf;
	struct authlpbk_area *area;
	uint_t auth_len;
	uint_t str_len, gid_len;
	int i;

	/* LINTED pointer cast */
	area = (struct authlpbk_area *)rqst->rq_clntcred;
	aup = &area->area_aup;
	aup->aup_machname = area->area_machname;
	aup->aup_gids = area->area_gids;
	auth_len = msg->rm_call.cb_cred.oa_length;
	if (auth_len == 0)
		return (AUTH_BADCRED);

	/* LINTED pointer cast */
	buf = (int32_t *)msg->rm_call.cb_cred.oa_base;

	aup->aup_time = IXDR_GET_INT32(buf);
	str_len = IXDR_GET_U_INT32(buf);
	if (str_len > MAX_MACHINE_NAME)
		return (AUTH_BADCRED);
	if (str_len > auth_len)
		return (AUTH_BADCRED);
	(void) memcpy(aup->aup_machname, buf, str_len);
	aup->aup_machname[str_len] = 0;
	str_len = RNDUP(str_len);
	buf += str_len / sizeof (int);
	aup->aup_uid = IXDR_GET_INT32(buf);
	aup->aup_gid = IXDR_GET_INT32(buf);
	gid_len = IXDR_GET_U_INT32(buf);
	if (gid_len > NGRPS_LOOPBACK)
		return (AUTH_BADCRED);
	/*
	 * five is the smallest unix credentials structure -
	 * timestamp, hostname len (0), uid, gid, and gids len (0).
	 */
	if ((5 + gid_len) * BYTES_PER_XDR_UNIT + str_len > auth_len)
		return (AUTH_BADCRED);

	aup->aup_len = gid_len;
	for (i = 0; i < gid_len; i++) {
		aup->aup_gids[i] = (gid_t)IXDR_GET_INT32(buf);
	}

	rqst->rq_xprt->xp_verf.oa_flavor = AUTH_NULL;
	rqst->rq_xprt->xp_verf.oa_length = 0;

	return (AUTH_OK);
}
