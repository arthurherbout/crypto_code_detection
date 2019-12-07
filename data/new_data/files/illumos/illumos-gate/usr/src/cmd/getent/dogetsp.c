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
 * Copyright (c) 2018 Peter Tribble.
 * Copyright (c) 2014 Gary Mills
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
 * Copyright 2012 Nexenta Systems, Inc. All rights reserved.
 * Use is subject to license terms.
 */

#include <stdio.h>
#include <shadow.h>
#include <stdlib.h>
#include <errno.h>
#include "getent.h"

/*
 * getspnam - get entries from shadow database
 */
int
dogetsp(const char **list)
{
	struct spwd *sp;
	int rc = EXC_SUCCESS;


	if (list == NULL || *list == NULL) {
		setspent();
		while ((sp = getspent()) != NULL)
			(void) putspent(sp, stdout);
		endspent();
	} else {
		for (; *list != NULL; list++) {
			sp = getspnam(*list);
			if (sp == NULL)
				rc = EXC_NAME_NOT_FOUND;
			else
				(void) putspent(sp, stdout);
		}
	}

	return (rc);
}
