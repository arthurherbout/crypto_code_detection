/*
 * spinconsole.c
 *
 * Author: Maksym Sobolyev <sobomax@sippysoft.com>
 * Copyright (c) 2009 Sippy Software, Inc.
 * All rights reserved.
 *
 * Subject to the following obligations and disclaimer of warranty, use and
 * redistribution of this software, in source or object code forms, with or
 * without modifications are expressly permitted by Whistle Communications;
 * provided, however, that:
 * 1. Any and all reproductions of the source or object code must include the
 *    copyright notice above and the following disclaimer of warranties; and
 * 2. No rights are granted, in any manner or form, to use Whistle
 *    Communications, Inc. trademarks, including the mark "WHISTLE
 *    COMMUNICATIONS" on advertising, endorsements, or otherwise except as
 *    such appears in the above copyright notice or in the software.
 *
 * THIS SOFTWARE IS BEING PROVIDED BY WHISTLE COMMUNICATIONS "AS IS", AND
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, WHISTLE COMMUNICATIONS MAKES NO
 * REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED, REGARDING THIS SOFTWARE,
 * INCLUDING WITHOUT LIMITATION, ANY AND ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
 * WHISTLE COMMUNICATIONS DOES NOT WARRANT, GUARANTEE, OR MAKE ANY
 * REPRESENTATIONS REGARDING THE USE OF, OR THE RESULTS OF THE USE OF THIS
 * SOFTWARE IN TERMS OF ITS CORRECTNESS, ACCURACY, RELIABILITY OR OTHERWISE.
 * IN NO EVENT SHALL WHISTLE COMMUNICATIONS BE LIABLE FOR ANY DAMAGES
 * RESULTING FROM OR ARISING OUT OF ANY USE OF THIS SOFTWARE, INCLUDING
 * WITHOUT LIMITATION, ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * PUNITIVE, OR CONSEQUENTIAL DAMAGES, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES, LOSS OF USE, DATA OR PROFITS, HOWEVER CAUSED AND UNDER ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF WHISTLE COMMUNICATIONS IS ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>

#include <stand.h>
#include <bootstrap.h>


static void	spinc_probe(struct console *cp);
static int	spinc_init(struct console *cp, int arg);
static void	spinc_putchar(struct console *cp, int c);
static int	spinc_getchar(struct console *cp);
static int	spinc_ischar(struct console *cp);
static void	spinc_devinfo(struct console *cp);

struct console spinconsole = {
	.c_name = "spin",
	.c_desc = "spin port",
	.c_flags = 0,
	.c_probe = spinc_probe,
	.c_init = spinc_init,
	.c_out = spinc_putchar,
	.c_in = spinc_getchar,
	.c_ready = spinc_ischar,
	.c_ioctl = NULL,
	.c_devinfo = spinc_devinfo,
	.c_private = NULL
};

static void
spinc_devinfo(struct console *cp __unused)
{
	printf("\tsoftware device");
}

static void
spinc_probe(struct console *cp)
{
	int i;
	struct console *parent;

	if (cp->c_private == NULL) {
		for (i = 0; consoles[i] != NULL; i++)
			if (strcmp(consoles[i]->c_name, "text") == 0)
				break;
		cp->c_private = consoles[i];
	}

	parent = cp->c_private;
	if (parent != NULL)
		parent->c_probe(cp);
}

static int
spinc_init(struct console *cp, int arg)
{
	struct console *parent;

	parent = cp->c_private;
	if (parent != NULL)
		return (parent->c_init(cp, arg));
	else
		return (0);
}

static void
spinc_putchar(struct console *cp, int c __unused)
{
	static unsigned tw_chars = 0x5C2D2F7C;    /* "\-/|" */
	static time_t lasttime = 0;
	struct console *parent;
	time_t now;

	now = time(NULL);
	if (now < (lasttime + 1))
		return;
	lasttime = now;
	parent = cp->c_private;
	if (parent == NULL)
		return;

	parent->c_out(parent, (char)tw_chars);
	parent->c_out(parent, '\b');
	tw_chars = (tw_chars >> 8) | ((tw_chars & (unsigned long)0xFF) << 24);
}

static int
spinc_getchar(struct console *cp __unused)
{

	return (-1);
}

static int
spinc_ischar(struct console *cp __unused)
{

	return (0);
}
