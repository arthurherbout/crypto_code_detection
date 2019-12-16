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
#pragma ident	"%Z%%M%	%I%	%E% SMI" 

/*
 * Copyright (c) 1986 Sun Microsystems Inc.
 */

/*
 * Put out a C declaration which initializes "epoch" to the time ("tv_sec"
 * portion) when this program was run.
 */

#include <stdio.h>
#include <sys/time.h>

int	gettimeofday();
void	perror();
void	exit();

/*ARGSUSED*/
int
main(argc, argv)
	int argc;
	char **argv;
{
	struct timeval now;

	if (gettimeofday(&now, (struct timezone *)NULL) < 0) {
		perror("mkepoch: gettimeofday failed");
		exit(1);
	}

	if (printf("static long epoch = %ld;\n", now.tv_sec) == EOF) {
		perror("mkepoch: can't write output");
		exit(1);
	}

	return (0);
}
