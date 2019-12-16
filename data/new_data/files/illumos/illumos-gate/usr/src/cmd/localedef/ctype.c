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
 * Copyright 2017 Nexenta Systems, Inc.
 * Copyright 2012 Garrett D'Amore <garrett@damore.org>
 * Copyright 2013 DEY Storage Systems, Inc.
 */

/*
 * LC_CTYPE database generation routines for localedef.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/avl.h>
#include <wchar.h>
#include <ctype.h>
#include <wctype.h>
#include <unistd.h>
#include "_ctype.h"
#include "localedef.h"
#include "parser.tab.h"
#include "runefile.h"

static avl_tree_t	ctypes;

static wchar_t		last_ctype;

typedef struct ctype_node {
	wchar_t wc;
	int32_t	ctype;
	int32_t	toupper;
	int32_t	tolower;
	avl_node_t avl;
} ctype_node_t;

typedef struct width_node {
	wchar_t start;
	wchar_t end;
	int8_t width;
	avl_node_t avl;
} width_node_t;

static int
ctype_compare(const void *n1, const void *n2)
{
	const ctype_node_t *c1 = n1;
	const ctype_node_t *c2 = n2;

	return (c1->wc < c2->wc ? -1 : c1->wc > c2->wc ? 1 : 0);
}

void
init_ctype(void)
{
	avl_create(&ctypes, ctype_compare, sizeof (ctype_node_t),
	    offsetof(ctype_node_t, avl));
}


static void
add_ctype_impl(ctype_node_t *ctn)
{
	switch (last_kw) {
	case T_ISUPPER:
		ctn->ctype |= (_ISUPPER | _ISALPHA | _ISGRAPH | _ISPRINT);
		break;
	case T_ISLOWER:
		ctn->ctype |= (_ISLOWER | _ISALPHA | _ISGRAPH | _ISPRINT);
		break;
	case T_ISALPHA:
		ctn->ctype |= (_ISALPHA | _ISGRAPH | _ISPRINT);
		break;
	case T_ISDIGIT:
		ctn->ctype |= (_ISDIGIT | _ISGRAPH | _ISPRINT | _ISXDIGIT);
		break;
	case T_ISSPACE:
		ctn->ctype |= _ISSPACE;
		break;
	case T_ISCNTRL:
		ctn->ctype |= _ISCNTRL;
		break;
	case T_ISGRAPH:
		ctn->ctype |= (_ISGRAPH | _ISPRINT);
		break;
	case T_ISPRINT:
		ctn->ctype |= _ISPRINT;
		break;
	case T_ISPUNCT:
		ctn->ctype |= (_ISPUNCT | _ISGRAPH | _ISPRINT);
		break;
	case T_ISXDIGIT:
		ctn->ctype |= (_ISXDIGIT | _ISPRINT);
		break;
	case T_ISBLANK:
		ctn->ctype |= (_ISBLANK | _ISSPACE);
		break;
	case T_ISPHONOGRAM:
		ctn->ctype |= (_E1 | _ISPRINT | _ISGRAPH);
		break;
	case T_ISIDEOGRAM:
		ctn->ctype |= (_E2 | _ISPRINT | _ISGRAPH);
		break;
	case T_ISENGLISH:
		ctn->ctype |= (_E3 | _ISPRINT | _ISGRAPH);
		break;
	case T_ISNUMBER:
		ctn->ctype |= (_E4 | _ISPRINT | _ISGRAPH);
		break;
	case T_ISSPECIAL:
		ctn->ctype |= (_E5 | _ISPRINT | _ISGRAPH);
		break;
	case T_ISALNUM:
		/*
		 * We can't do anything with this.  The character
		 * should already be specified as a digit or alpha.
		 */
		break;
	default:
		errf(_("not a valid character class"));
	}
}

static ctype_node_t *
get_ctype(wchar_t wc)
{
	ctype_node_t	srch;
	ctype_node_t	*ctn;
	avl_index_t	where;

	srch.wc = wc;
	if ((ctn = avl_find(&ctypes, &srch, &where)) == NULL) {
		if ((ctn = calloc(1, sizeof (*ctn))) == NULL) {
			errf(_("out of memory"));
			return (NULL);
		}
		ctn->wc = wc;

		avl_insert(&ctypes, ctn, where);
	}
	return (ctn);
}

void
add_ctype(int val)
{
	ctype_node_t	*ctn;

	if ((ctn = get_ctype(val)) == NULL) {
		INTERR;
		return;
	}
	add_ctype_impl(ctn);
	last_ctype = ctn->wc;
}

void
add_ctype_range(wchar_t end)
{
	ctype_node_t	*ctn;
	wchar_t		cur;

	if (end < last_ctype) {
		errf(_("malformed character range (%u ... %u))"),
		    last_ctype, end);
		return;
	}
	for (cur = last_ctype + 1; cur <= end; cur++) {
		if ((ctn = get_ctype(cur)) == NULL) {
			INTERR;
			return;
		}
		add_ctype_impl(ctn);
	}
	last_ctype = end;

}

/*
 * A word about widths: if the width mask is specified, then libc
 * unconditionally honors it.  Otherwise, it assumes printable
 * characters have width 1, and non-printable characters have width
 * -1 (except for NULL which is special with with 0).  Hence, we have
 * no need to inject defaults here -- the "default" unset value of 0
 * indicates that libc should use its own logic in wcwidth as described.
 */
void
add_width(int wc, int width)
{
	ctype_node_t	*ctn;

	if ((ctn = get_ctype(wc)) == NULL) {
		INTERR;
		return;
	}
	ctn->ctype &= ~(_CTYPE_SWM);
	switch (width) {
	case 0:
		ctn->ctype |= _CTYPE_SW0;
		break;
	case 1:
		ctn->ctype |= _CTYPE_SW1;
		break;
	case 2:
		ctn->ctype |= _CTYPE_SW2;
		break;
	case 3:
		ctn->ctype |= _CTYPE_SW3;
		break;
	}
}

void
add_width_range(int start, int end, int width)
{
	for (; start <= end; start++) {
		add_width(start, width);
	}
}

void
add_caseconv(int val, int wc)
{
	ctype_node_t	*ctn;

	ctn = get_ctype(val);
	if (ctn == NULL) {
		INTERR;
		return;
	}

	switch (last_kw) {
	case T_TOUPPER:
		ctn->toupper = wc;
		break;
	case T_TOLOWER:
		ctn->tolower = wc;
		break;
	default:
		INTERR;
		break;
	}
}

void
dump_ctype(void)
{
	FILE		*f;
	_FileRuneLocale	rl;
	ctype_node_t	*ctn, *last_ct, *last_lo, *last_up;
	_FileRuneEntry	*ct = NULL;
	_FileRuneEntry	*lo = NULL;
	_FileRuneEntry	*up = NULL;
	wchar_t		wc;

	(void) memset(&rl, 0, sizeof (rl));
	last_ct = NULL;
	last_lo = NULL;
	last_up = NULL;

	if ((f = open_category()) == NULL)
		return;

	(void) memcpy(rl.magic, _FILE_RUNE_MAGIC_1, 8);
	(void) strlcpy(rl.encoding, get_wide_encoding(), sizeof (rl.encoding));

	/*
	 * Initialize the identity map.
	 */
	for (wc = 0; (unsigned)wc < _CACHED_RUNES; wc++) {
		rl.maplower[wc] = wc;
		rl.mapupper[wc] = wc;
	}

	for (ctn = avl_first(&ctypes); ctn; ctn = AVL_NEXT(&ctypes, ctn)) {
		int conflict = 0;


		wc = ctn->wc;

		/*
		 * POSIX requires certain portable characters have
		 * certain types.  Add them if they are missing.
		 */
		if ((wc >= 1) && (wc <= 127)) {
			if ((wc >= 'A') && (wc <= 'Z'))
				ctn->ctype |= _ISUPPER;
			if ((wc >= 'a') && (wc <= 'z'))
				ctn->ctype |= _ISLOWER;
			if ((wc >= '0') && (wc <= '9'))
				ctn->ctype |= _ISDIGIT;
			if (wc == ' ')
				ctn->ctype |= _ISPRINT;
			if (strchr(" \f\n\r\t\v", (char)wc) != NULL)
				ctn->ctype |= _ISSPACE;
			if (strchr("0123456789ABCDEFabcdef", (char)wc) != NULL)
				ctn->ctype |= _ISXDIGIT;
			if (strchr(" \t", (char)wc))
				ctn->ctype |= _ISBLANK;

			/*
			 * Technically these settings are only
			 * required for the C locale.  However, it
			 * turns out that because of the historical
			 * version of isprint(), we need them for all
			 * locales as well.  Note that these are not
			 * necessarily valid punctation characters in
			 * the current language, but ispunct() needs
			 * to return TRUE for them.
			 */
			if (strchr("!\"'#$%&()*+,-./:;<=>?@[\\]^_`{|}~",
			    (char)wc))
				ctn->ctype |= _ISPUNCT;
		}

		/*
		 * POSIX also requires that certain types imply
		 * others.  Add any inferred types here.
		 */
		if (ctn->ctype & (_ISUPPER |_ISLOWER))
			ctn->ctype |= _ISALPHA;
		if (ctn->ctype & _ISDIGIT)
			ctn->ctype |= _ISXDIGIT;
		if (ctn->ctype & _ISBLANK)
			ctn->ctype |= _ISSPACE;
		if (ctn->ctype & (_ISALPHA|_ISDIGIT|_ISXDIGIT))
			ctn->ctype |= _ISGRAPH;
		if (ctn->ctype & _ISGRAPH)
			ctn->ctype |= _ISPRINT;

		/*
		 * Finally, POSIX requires that certain combinations
		 * are invalid.  We don't flag this as a fatal error,
		 * but we will warn about.
		 */
		if ((ctn->ctype & _ISALPHA) &&
		    (ctn->ctype & (_ISPUNCT|_ISDIGIT)))
			conflict++;
		if ((ctn->ctype & _ISPUNCT) &&
		    (ctn->ctype & (_ISDIGIT|_ISALPHA|_ISXDIGIT)))
			conflict++;
		if ((ctn->ctype & _ISSPACE) && (ctn->ctype & _ISGRAPH))
			conflict++;
		if ((ctn->ctype & _ISCNTRL) && (ctn->ctype & _ISPRINT))
			conflict++;
		if ((wc == ' ') && (ctn->ctype & (_ISPUNCT|_ISGRAPH)))
			conflict++;

		if (conflict) {
			warn("conflicting classes for character 0x%x (%x)",
			    wc, ctn->ctype);
		}

		/*
		 * Handle the lower 256 characters using the simple
		 * optimization.  Note that if we have not defined the
		 * upper/lower case, then we identity map it.
		 */
		if ((unsigned)wc < _CACHED_RUNES) {
			rl.runetype[wc] = ctn->ctype;
			if (ctn->tolower)
				rl.maplower[wc] = ctn->tolower;
			if (ctn->toupper)
				rl.mapupper[wc] = ctn->toupper;
			continue;
		}

		if ((last_ct != NULL) && (last_ct->ctype == ctn->ctype) &&
		    (last_ct->wc + 1 == wc)) {
			ct[rl.runetype_ext_nranges-1].max = wc;
		} else {
			rl.runetype_ext_nranges++;
			ct = realloc(ct,
			    sizeof (*ct) * rl.runetype_ext_nranges);
			if (ct == NULL)
				goto fail;
			ct[rl.runetype_ext_nranges - 1].min = wc;
			ct[rl.runetype_ext_nranges - 1].max = wc;
			ct[rl.runetype_ext_nranges - 1].map = ctn->ctype;
		}
		last_ct = ctn;
		if (ctn->tolower == 0) {
			last_lo = NULL;
		} else if ((last_lo != NULL) &&
		    (last_lo->tolower + 1 == ctn->tolower)) {
			lo[rl.maplower_ext_nranges-1].max = wc;
			last_lo = ctn;
		} else {
			rl.maplower_ext_nranges++;
			lo = realloc(lo,
			    sizeof (*lo) * rl.maplower_ext_nranges);
			if (lo == NULL)
				goto fail;
			lo[rl.maplower_ext_nranges - 1].min = wc;
			lo[rl.maplower_ext_nranges - 1].max = wc;
			lo[rl.maplower_ext_nranges - 1].map = ctn->tolower;
			last_lo = ctn;
		}

		if (ctn->toupper == 0) {
			last_up = NULL;
		} else if ((last_up != NULL) &&
		    (last_up->toupper + 1 == ctn->toupper)) {
			up[rl.mapupper_ext_nranges-1].max = wc;
			last_up = ctn;
		} else {
			rl.mapupper_ext_nranges++;
			up = realloc(up,
			    sizeof (*up) * rl.mapupper_ext_nranges);
			if (up == NULL)
				goto fail;
			up[rl.mapupper_ext_nranges - 1].min = wc;
			up[rl.mapupper_ext_nranges - 1].max = wc;
			up[rl.mapupper_ext_nranges - 1].map = ctn->toupper;
			last_up = ctn;
		}
	}

	if ((wr_category(&rl, sizeof (rl), f) == 0) &&
	    (wr_category(ct, sizeof (*ct) * rl.runetype_ext_nranges, f) == 0) &&
	    (wr_category(lo, sizeof (*lo) * rl.maplower_ext_nranges, f) == 0) &&
	    (wr_category(up, sizeof (*up) * rl.mapupper_ext_nranges, f) == 0)) {
		close_category(f);
		goto out;
	}

fail:
	delete_category(f);
out:
	free(ct);
	free(lo);
	free(up);
}
