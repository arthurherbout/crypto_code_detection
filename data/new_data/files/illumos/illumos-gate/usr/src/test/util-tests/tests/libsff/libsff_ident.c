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
 * Copyright (c) 2017, Joyent, Inc.
 */

/*
 * Print all SFF Identifier values
 */

#include <stdio.h>
#include <errno.h>
#include <strings.h>
#include <err.h>
#include <libsff.h>

/*
 * Pick up private sff header file with offsets from lib/libsff.
 */
#include "sff.h"

int
main(void)
{
	uint_t i;
	uint8_t buf[256];

	bzero(buf, sizeof (buf));
	for (i = 0; i < UINT8_MAX; i++) {
		int ret;
		nvlist_t *nvl;
		char *val;

		buf[SFF_8472_IDENTIFIER] = i;
		if ((ret = libsff_parse(buf, sizeof (buf), 0xa0, &nvl)) != 0) {
			errx(1, "TEST FAILED: failed to parse identifier "
			    "%d: %s\n", i, strerror(ret));
		}

		if ((ret = nvlist_lookup_string(nvl, LIBSFF_KEY_IDENTIFIER,
		    &val)) != 0) {
			errx(1, "TEST FAILED: failed to find key %s with "
			    "value %d: %s", LIBSFF_KEY_IDENTIFIER, i,
			    strerror(ret));
		}

		(void) puts(val);
		nvlist_free(nvl);
	}
	return (0);
}
