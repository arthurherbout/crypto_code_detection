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
 * Copyright 2015 Nexenta Systems, Inc.  All rights reserved.
 * Copyright 2019 Joyent, Inc.
 */

#include <aes/aes_impl.h>
#include <stdio.h>

#include "cryptotest.h"
#include "aes_cmac.h"

static size_t updatelens[] = {
	1, AES_BLOCK_LEN, AES_BLOCK_LEN + 1, 2*AES_BLOCK_LEN,
	CTEST_UPDATELEN_WHOLE, CTEST_UPDATELEN_END
};

int
main(void)
{
	int errs = 0;
	int i;
	uint8_t N[AES_BLOCK_LEN];
	cryptotest_t args = {
		.in = M,
		.out = N,
		.outlen = sizeof (N),
		.key = keytest,
		.keylen = sizeof (keytest),
		.mechname = SUN_CKM_AES_CMAC,
		.updatelens = updatelens
	};

	for (i = 0; i < sizeof (RES) / sizeof (RES[0]); i++) {
		args.inlen = DATALEN[i];
		errs += run_test(&args, RES[i], AES_BLOCK_LEN, MAC_FG);
		(void) fprintf(stderr, "----------\n");
	}
	if (errs != 0)
		(void) fprintf(stderr, "%d tests failed\n", errs);

	return (errs);
}
