/*
 * This file and its contents are supplied under the terms of the
 * Common Development and Distribution License ("CDDL"), version 1.0.
 * You may only use this file in accordance with the terms of version
 * 1.0 of the CDDL.
 *
 * A full copy of the text of the CDDL should have accompanied this
 * source.  A copy of the CDDL is also available via the Internet at
 * http://www.illumos.org/license/CDDL.
 */

/*
 * Copyright (c) 2018 Joyent, Inc.
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/random.h>

ssize_t
getrandom(void *buf, size_t len, unsigned int flags)
{
	return (syscall(SYS_getrandom, buf, len, flags));
}
