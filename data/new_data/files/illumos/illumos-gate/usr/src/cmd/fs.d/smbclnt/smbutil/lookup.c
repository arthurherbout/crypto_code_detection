/*
 * Copyright (c) 2000, Boris Popov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by Boris Popov.
 * 4. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: lookup.c,v 1.1.1.1 2001/06/09 00:28:13 zarzycki Exp $
 */

/*
 * Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#include <sys/param.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include <sysexits.h>
#include <libintl.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <cflib.h>

#include <netsmb/netbios.h>
#include <netsmb/smb_lib.h>
#include <netsmb/nb_lib.h>

#include "common.h"


int
cmd_lookup(int argc, char *argv[])
{
	struct nb_ctx *ctx;
	struct sockaddr *sap;
	char *hostname;
	int error, opt;

	if (argc < 2)
		lookup_usage();
	error = nb_ctx_create(&ctx);
	if (error) {
		smb_error(gettext("unable to create nbcontext"), error);
		exit(1);
	}
	if (smb_open_rcfile(NULL) == 0) {
		if (nb_ctx_readrcsection(NULL, ctx, "default", 0) != 0)
			exit(1);
		smb_close_rcfile();
	}
	if ((ctx->nb_flags & NBCF_NS_ENABLE) == 0) {
		fprintf(stderr,
		    gettext("nbns_enable=false, cannot do lookup\n"));
		exit(1);
	}
	while ((opt = getopt(argc, argv, "w:")) != EOF) {
		switch (opt) {
		case 'w':
			nb_ctx_setns(ctx, optarg);
			break;
		default:
			lookup_usage();
			/*NOTREACHED*/
		}
	}
	if (optind >= argc)
		lookup_usage();
	if (nb_ctx_resolve(ctx) != 0)
		exit(1);
	hostname = argv[argc - 1];
	error = nbns_resolvename(hostname, ctx, &sap);
	if (error) {
		smb_error(gettext("unable to resolve %s"), error, hostname);
		exit(1);
	}
	printf(gettext("Got response from %s\n"),
	    inet_ntoa(ctx->nb_lastns.sin_addr));
	printf(gettext("IP address of %s: %s\n"), hostname,
	    inet_ntoa(((struct sockaddr_in *)sap)->sin_addr));
	return (0);
}


void
lookup_usage(void)
{
	printf(gettext("usage: smbutil lookup [-w host] name\n"));
	exit(1);
}
