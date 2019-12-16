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
 */

/*
 * LC_COLLATE database generation routines for localedef.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/avl.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>
#include <widec.h>
#include <limits.h>
#include "localedef.h"
#include "parser.tab.h"
#include "collatefile.h"

/*
 * Design notes.
 *
 * It will be extremely helpful to the reader if they have access to
 * the localedef and locale file format specifications available.
 * Latest versions of these are available from www.opengroup.org.
 *
 * The design for the collation code is a bit complex.  The goal is a
 * single collation database as described in collate.h (in
 * libc/port/locale).  However, there are some other tidbits:
 *
 * a) The substitution entries are now a directly indexable array.  A
 * priority elsewhere in the table is taken as an index into the
 * substitution table if it has a high bit (COLLATE_SUBST_PRIORITY)
 * set.  (The bit is cleared and the result is the index into the
 * table.
 *
 * b) We eliminate duplicate entries into the substitution table.
 * This saves a lot of space.
 *
 * c) The priorities for each level are "compressed", so that each
 * sorting level has consecutively numbered priorities starting at 1.
 * (O is reserved for the ignore priority.)  This means sort levels
 * which only have a few distinct priorities can represent the
 * priority level in fewer bits, which makes the strxfrm output
 * smaller.
 *
 * d) We record the total number of priorities so that strxfrm can
 * figure out how many bytes to expand a numeric priority into.
 *
 * e) For the UNDEFINED pass (the last pass), we record the maximum
 * number of bits needed to uniquely prioritize these entries, so that
 * the last pass can also use smaller strxfrm output when possible.
 *
 * f) Priorities with the sign bit set are verboten.  This works out
 * because no active character set needs that bit to carry significant
 * information once the character is in wide form.
 *
 * To process the entire data to make the database, we actually run
 * multiple passes over the data.
 *
 * The first pass, which is done at parse time, identifies elements,
 * substitutions, and such, and records them in priority order.  As
 * some priorities can refer to other priorities, using forward
 * references, we use a table of references indicating whether the
 * priority's value has been resolved, or whether it is still a
 * reference.
 *
 * The second pass walks over all the items in priority order, noting
 * that they are used directly, and not just an indirect reference.
 * This is done by creating a "weight" structure for the item.  The
 * weights are stashed in an AVL tree sorted by relative "priority".
 *
 * The third pass walks over all the weight structures, in priority
 * order, and assigns a new monotonically increasing (per sort level)
 * weight value to them.  These are the values that will actually be
 * written to the file.
 *
 * The fourth pass just writes the data out.
 */

/*
 * In order to resolve the priorities, we create a table of priorities.
 * Entries in the table can be in one of three states.
 *
 * UNKNOWN is for newly allocated entries, and indicates that nothing
 * is known about the priority.  (For example, when new entries are created
 * for collating-symbols, this is the value assigned for them until the
 * collating symbol's order has been determined.
 *
 * RESOLVED is used for an entry where the priority indicates the final
 * numeric weight.
 *
 * REFER is used for entries that reference other entries.  Typically
 * this is used for forward references.  A collating-symbol can never
 * have this value.
 *
 * The "pass" field is used during final resolution to aid in detection
 * of referencing loops.  (For example <A> depends on <B>, but <B> has its
 * priority dependent on <A>.)
 */
typedef enum {
	UNKNOWN,	/* priority is totally unknown */
	RESOLVED,	/* priority value fully resolved */
	REFER		/* priority is a reference (index) */
} res_t;

typedef struct weight {
	int32_t		pri;
	int		opt;
	avl_node_t	avl;
} weight_t;

typedef struct priority {
	res_t		res;
	int32_t		pri;
	int		pass;
	int		lineno;
} collpri_t;

#define	NUM_WT	collinfo.directive_count

/*
 * These are the abstract collating symbols, which are just a symbolic
 * way to reference a priority.
 */
struct collsym {
	char		*name;
	int32_t		ref;
	avl_node_t	avl;
};

/*
 * These are also abstract collating symbols, but we allow them to have
 * different priorities at different levels.
 */
typedef struct collundef {
	char		*name;
	int32_t		ref[COLL_WEIGHTS_MAX];
	avl_node_t	avl;
} collundef_t;

/*
 * These are called "chains" in libc.  This records the fact that two
 * more characters should be treated as a single collating entity when
 * they appear together.  For example, in Spanish <C><h> gets collated
 * as a character between <C> and <D>.
 */
struct collelem {
	char		*symbol;
	wchar_t		*expand;
	int32_t		ref[COLL_WEIGHTS_MAX];
	avl_node_t	avl_bysymbol;
	avl_node_t	avl_byexpand;
};

/*
 * Individual characters have a sequence of weights as well.
 */
typedef struct collchar {
	wchar_t		wc;
	int32_t		ref[COLL_WEIGHTS_MAX];
	avl_node_t	avl;
} collchar_t;

/*
 * Substitution entries.  The key is itself a priority.  Note that
 * when we create one of these, we *automatically* wind up with a
 * fully resolved priority for the key, because creation of
 * substitutions creates a resolved priority at the same time.
 */
typedef struct {
	int32_t		key;
	int32_t		ref[COLLATE_STR_LEN];
	avl_node_t	avl;
	avl_node_t	avl_ref;
} subst_t;

static avl_tree_t	collsyms;
static avl_tree_t	collundefs;
static avl_tree_t	elem_by_symbol;
static avl_tree_t	elem_by_expand;
static avl_tree_t	collchars;
static avl_tree_t	substs[COLL_WEIGHTS_MAX];
static avl_tree_t	substs_ref[COLL_WEIGHTS_MAX];
static avl_tree_t	weights[COLL_WEIGHTS_MAX];
static int32_t		nweight[COLL_WEIGHTS_MAX];

/*
 * This is state tracking for the ellipsis token.  Note that we start
 * the initial values so that the ellipsis logic will think we got a
 * magic starting value of NUL.  It starts at minus one because the
 * starting point is exclusive -- i.e. the starting point is not
 * itself handled by the ellipsis code.
 */
static int currorder = EOF;
static int lastorder = EOF;
static collelem_t *currelem;
static collchar_t *currchar;
static collundef_t *currundef;
static wchar_t ellipsis_start = 0;
static int32_t ellipsis_weights[COLL_WEIGHTS_MAX];

/*
 * We keep a running tally of weights.
 */
static int nextpri = 1;
static int nextsubst[COLL_WEIGHTS_MAX] = { 0 };

/*
 * This array collects up the weights for each level.
 */
static int32_t order_weights[COLL_WEIGHTS_MAX];
static int curr_weight = 0;
static int32_t subst_weights[COLLATE_STR_LEN];
static int curr_subst = 0;

/*
 * Some initial priority values.
 */
static int32_t pri_undefined[COLL_WEIGHTS_MAX];
static int32_t pri_ignore;

static collate_info_t collinfo;

static collpri_t	*prilist = NULL;
static int		numpri = 0;
static int		maxpri = 0;

static void start_order(int);

static int32_t
new_pri(void)
{
	int i;

	if (numpri >= maxpri) {
		maxpri = maxpri ? maxpri * 2 : 1024;
		prilist = realloc(prilist, sizeof (collpri_t) * maxpri);
		if (prilist == NULL) {
			errf(_("out of memory"));
			return (-1);
		}
		for (i = numpri; i < maxpri; i++) {
			prilist[i].res = UNKNOWN;
			prilist[i].pri = 0;
			prilist[i].pass = 0;
		}
	}
	return (numpri++);
}

static collpri_t *
get_pri(int32_t ref)
{
	if ((ref < 0) || (ref > numpri)) {
		INTERR;
		return (NULL);
	}
	return (&prilist[ref]);
}

static void
set_pri(int32_t ref, int32_t v, res_t res)
{
	collpri_t	*pri;

	pri = get_pri(ref);

	if ((res == REFER) && ((v < 0) || (v >= numpri))) {
		INTERR;
	}

	/* Resolve self references */
	if ((res == REFER) && (ref == v)) {
		v = nextpri;
		res = RESOLVED;
	}

	if (pri->res != UNKNOWN) {
		warn(_("repeated item in order list (first on %d)"),
		    pri->lineno);
		return;
	}
	pri->lineno = lineno;
	pri->pri = v;
	pri->res = res;
}

static int32_t
resolve_pri(int32_t ref)
{
	collpri_t	*pri;
	static int32_t	pass = 0;

	pri = get_pri(ref);
	pass++;
	while (pri->res == REFER) {
		if (pri->pass == pass) {
			/* report a line with the circular symbol */
			lineno = pri->lineno;
			errf(_("circular reference in order list"));
			return (-1);
		}
		if ((pri->pri < 0) || (pri->pri >= numpri)) {
			INTERR;
			return (-1);
		}
		pri->pass = pass;
		pri = &prilist[pri->pri];
	}

	if (pri->res == UNKNOWN) {
		return (-1);
	}
	if (pri->res != RESOLVED)
		INTERR;

	return (pri->pri);
}

static int
weight_compare(const void *n1, const void *n2)
{
	int32_t	k1 = ((const weight_t *)n1)->pri;
	int32_t	k2 = ((const weight_t *)n2)->pri;

	return (k1 < k2 ? -1 : k1 > k2 ? 1 : 0);
}

static int
collsym_compare(const void *n1, const void *n2)
{
	const collsym_t *c1 = n1;
	const collsym_t *c2 = n2;
	int rv;

	rv = strcmp(c1->name, c2->name);
	return ((rv < 0) ? -1 : (rv > 0) ? 1 : 0);
}

static int
collundef_compare(const void *n1, const void *n2)
{
	const collundef_t *c1 = n1;
	const collundef_t *c2 = n2;
	int rv;

	rv = strcmp(c1->name, c2->name);
	return ((rv < 0) ? -1 : (rv > 0) ? 1 : 0);
}

static int
element_compare_symbol(const void *n1, const void *n2)
{
	const collelem_t *c1 = n1;
	const collelem_t *c2 = n2;
	int rv;

	rv = strcmp(c1->symbol, c2->symbol);
	return ((rv < 0) ? -1 : (rv > 0) ? 1 : 0);
}

static int
element_compare_expand(const void *n1, const void *n2)
{
	const collelem_t *c1 = n1;
	const collelem_t *c2 = n2;
	int rv;

	rv = wcscmp(c1->expand, c2->expand);
	return ((rv < 0) ? -1 : (rv > 0) ? 1 : 0);
}

static int
collchar_compare(const void *n1, const void *n2)
{
	wchar_t	k1 = ((const collchar_t *)n1)->wc;
	wchar_t	k2 = ((const collchar_t *)n2)->wc;

	return (k1 < k2 ? -1 : k1 > k2 ? 1 : 0);
}

static int
subst_compare(const void *n1, const void *n2)
{
	int32_t	k1 = ((const subst_t *)n1)->key;
	int32_t	k2 = ((const subst_t *)n2)->key;

	return (k1 < k2 ? -1 : k1 > k2 ? 1 : 0);
}

static int
subst_compare_ref(const void *n1, const void *n2)
{
	int32_t *c1 = ((subst_t *)n1)->ref;
	int32_t *c2 = ((subst_t *)n2)->ref;
	int rv;

	rv = wcscmp((wchar_t *)c1, (wchar_t *)c2);
	return ((rv < 0) ? -1 : (rv > 0) ? 1 : 0);
}

void
init_collate(void)
{
	int i;

	avl_create(&collsyms, collsym_compare, sizeof (collsym_t),
	    offsetof(collsym_t, avl));

	avl_create(&collundefs, collundef_compare, sizeof (collsym_t),
	    offsetof(collundef_t, avl));

	avl_create(&elem_by_symbol, element_compare_symbol, sizeof (collelem_t),
	    offsetof(collelem_t, avl_bysymbol));
	avl_create(&elem_by_expand, element_compare_expand, sizeof (collelem_t),
	    offsetof(collelem_t, avl_byexpand));

	avl_create(&collchars, collchar_compare, sizeof (collchar_t),
	    offsetof(collchar_t, avl));

	for (i = 0; i < COLL_WEIGHTS_MAX; i++) {
		avl_create(&substs[i], subst_compare, sizeof (subst_t),
		    offsetof(subst_t, avl));
		avl_create(&substs_ref[i], subst_compare_ref,
		    sizeof (subst_t), offsetof(subst_t, avl_ref));
		avl_create(&weights[i], weight_compare, sizeof (weight_t),
		    offsetof(weight_t, avl));
		nweight[i] = 1;
	}

	(void) memset(&collinfo, 0, sizeof (collinfo));

	/* allocate some initial priorities */
	pri_ignore = new_pri();

	set_pri(pri_ignore, 0, RESOLVED);

	for (i = 0; i < COLL_WEIGHTS_MAX; i++) {
		pri_undefined[i] = new_pri();

		/* we will override this later */
		set_pri(pri_undefined[i], COLLATE_MAX_PRIORITY, UNKNOWN);
	}
}

void
define_collsym(char *name)
{
	collsym_t	*sym;
	avl_index_t	where;

	if ((sym = calloc(1, sizeof (*sym))) == NULL) {
		errf(_("out of memory"));
		return;
	}
	sym->name = name;
	sym->ref = new_pri();

	if (avl_find(&collsyms, sym, &where) != NULL) {
		/*
		 * This should never happen because we are only called
		 * for undefined symbols.
		 */
		free(sym);
		INTERR;
		return;
	}
	avl_insert(&collsyms, sym, where);
}

collsym_t *
lookup_collsym(char *name)
{
	collsym_t	srch;

	srch.name = name;
	return (avl_find(&collsyms, &srch, NULL));
}

collelem_t *
lookup_collelem(char *symbol)
{
	collelem_t	srch;

	srch.symbol = symbol;
	return (avl_find(&elem_by_symbol, &srch, NULL));
}

static collundef_t *
get_collundef(char *name)
{
	collundef_t	srch;
	collundef_t	*ud;
	avl_index_t	where;
	int		i;

	srch.name = name;
	if ((ud = avl_find(&collundefs, &srch, &where)) == NULL) {
		if (((ud = calloc(1, sizeof (*ud))) == NULL) ||
		    ((ud->name = strdup(name)) == NULL)) {
			errf(_("out of memory"));
			free(ud);
			return (NULL);
		}
		for (i = 0; i < NUM_WT; i++) {
			ud->ref[i] = new_pri();
		}
		avl_insert(&collundefs, ud, where);
	}
	add_charmap_undefined(name);
	return (ud);
}

static collchar_t *
get_collchar(wchar_t wc, int create)
{
	collchar_t	srch;
	collchar_t	*cc;
	avl_index_t	where;
	int		i;

	srch.wc = wc;
	cc = avl_find(&collchars, &srch, &where);
	if ((cc == NULL) && create) {
		if ((cc = calloc(1, sizeof (*cc))) == NULL) {
			errf(_("out of memory"));
			return (NULL);
		}
		for (i = 0; i < NUM_WT; i++) {
			cc->ref[i] = new_pri();
		}
		cc->wc = wc;
		avl_insert(&collchars, cc, where);
	}
	return (cc);
}

void
end_order_collsym(collsym_t *sym)
{
	start_order(T_COLLSYM);
	/* update the weight */

	set_pri(sym->ref, nextpri, RESOLVED);
	nextpri++;
}

void
end_order(void)
{
	int		i;
	int32_t		pri;
	int32_t		ref;
	collpri_t	*p;

	/* advance the priority/weight */
	pri = nextpri;

	switch (currorder) {
	case T_CHAR:
		for (i = 0; i < NUM_WT; i++) {
			if (((ref = order_weights[i]) < 0) ||
			    ((p = get_pri(ref)) == NULL) ||
			    (p->pri == -1)) {
				/* unspecified weight is a self reference */
				set_pri(currchar->ref[i], pri, RESOLVED);
			} else {
				set_pri(currchar->ref[i], ref, REFER);
			}
			order_weights[i] = -1;
		}

		/* leave a cookie trail in case next symbol is ellipsis */
		ellipsis_start = currchar->wc + 1;
		currchar = NULL;
		break;

	case T_ELLIPSIS:
		/* save off the weights were we can find them */
		for (i = 0; i < NUM_WT; i++) {
			ellipsis_weights[i] = order_weights[i];
			order_weights[i] = -1;
		}
		break;

	case T_COLLELEM:
		if (currelem == NULL) {
			INTERR;
		} else {
			for (i = 0; i < NUM_WT; i++) {

				if (((ref = order_weights[i]) < 0) ||
				    ((p = get_pri(ref)) == NULL) ||
				    (p->pri == -1)) {
					set_pri(currelem->ref[i], pri,
					    RESOLVED);
				} else {
					set_pri(currelem->ref[i], ref, REFER);
				}
				order_weights[i] = -1;
			}
		}
		break;

	case T_UNDEFINED:
		for (i = 0; i < NUM_WT; i++) {
			if (((ref = order_weights[i]) < 0) ||
			    ((p = get_pri(ref)) == NULL) ||
			    (p->pri == -1)) {
				set_pri(pri_undefined[i], -1, RESOLVED);
			} else {
				set_pri(pri_undefined[i], ref, REFER);
			}
			order_weights[i] = -1;
		}
		break;

	case T_SYMBOL:
		for (i = 0; i < NUM_WT; i++) {
			if (((ref = order_weights[i]) < 0) ||
			    ((p = get_pri(ref)) == NULL) ||
			    (p->pri == -1)) {
				set_pri(currundef->ref[i], pri, RESOLVED);
			} else {
				set_pri(currundef->ref[i], ref, REFER);
			}
			order_weights[i] = -1;
		}
		break;

	default:
		INTERR;
	}

	nextpri++;
}

static void
start_order(int type)
{
	int	i;

	lastorder = currorder;
	currorder = type;

	/* this is used to protect ELLIPSIS processing */
	if ((lastorder == T_ELLIPSIS) && (type != T_CHAR)) {
		errf(_("character value expected"));
	}

	for (i = 0; i < COLL_WEIGHTS_MAX; i++) {
		order_weights[i] = -1;
	}
	curr_weight = 0;
}

void
start_order_undefined(void)
{
	start_order(T_UNDEFINED);
}

void
start_order_symbol(char *name)
{
	currundef = get_collundef(name);
	start_order(T_SYMBOL);
}

void
start_order_char(wchar_t wc)
{
	collchar_t	*cc;
	int32_t		ref;

	start_order(T_CHAR);

	/*
	 * If we last saw an ellipsis, then we need to close the range.
	 * Handle that here.  Note that we have to be careful because the
	 * items *inside* the range are treated exclusiveley to the items
	 * outside of the range.  The ends of the range can have quite
	 * different weights than the range members.
	 */
	if (lastorder == T_ELLIPSIS) {
		int		i;

		if (wc < ellipsis_start) {
			errf(_("malformed range!"));
			return;
		}
		while (ellipsis_start < wc) {
			/*
			 * pick all of the saved weights for the
			 * ellipsis.  note that -1 encodes for the
			 * ellipsis itself, which means to take the
			 * current relative priority.
			 */
			if ((cc = get_collchar(ellipsis_start, 1)) == NULL) {
				INTERR;
				return;
			}
			for (i = 0; i < NUM_WT; i++) {
				collpri_t *p;
				if (((ref = ellipsis_weights[i]) == -1) ||
				    ((p = get_pri(ref)) == NULL) ||
				    (p->pri == -1)) {
					set_pri(cc->ref[i], nextpri, RESOLVED);
				} else {
					set_pri(cc->ref[i], ref, REFER);
				}
				ellipsis_weights[i] = 0;
			}
			ellipsis_start++;
			nextpri++;
		}
	}

	currchar = get_collchar(wc, 1);
}

void
start_order_collelem(collelem_t *e)
{
	start_order(T_COLLELEM);
	currelem = e;
}

void
start_order_ellipsis(void)
{
	int	i;

	start_order(T_ELLIPSIS);

	if (lastorder != T_CHAR) {
		errf(_("illegal starting point for range"));
		return;
	}

	for (i = 0; i < NUM_WT; i++) {
		ellipsis_weights[i] = order_weights[i];
	}
}

void
define_collelem(char *name, wchar_t *wcs)
{
	collelem_t	*e;
	avl_index_t	where1;
	avl_index_t	where2;
	int		i;

	if (wcslen(wcs) >= COLLATE_STR_LEN) {
		errf(_("expanded collation element too long"));
		return;
	}

	if ((e = calloc(1, sizeof (*e))) == NULL) {
		errf(_("out of memory"));
		return;
	}
	e->expand = wcs;
	e->symbol = name;

	/*
	 * This is executed before the order statement, so we don't
	 * know how many priorities we *really* need.  We allocate one
	 * for each possible weight.  Not a big deal, as collating-elements
	 * prove to be quite rare.
	 */
	for (i = 0; i < COLL_WEIGHTS_MAX; i++) {
		e->ref[i] = new_pri();
	}

	/* A character sequence can only reduce to one element. */
	if ((avl_find(&elem_by_symbol, e, &where1) != NULL) ||
	    (avl_find(&elem_by_expand, e, &where2) != NULL)) {
		errf(_("duplicate collating element definition"));
		free(e);
		return;
	}
	avl_insert(&elem_by_symbol, e, where1);
	avl_insert(&elem_by_expand, e, where2);
}

void
add_order_bit(int kw)
{
	uint8_t bit = DIRECTIVE_UNDEF;

	switch (kw) {
	case T_FORWARD:
		bit = DIRECTIVE_FORWARD;
		break;
	case T_BACKWARD:
		bit = DIRECTIVE_BACKWARD;
		break;
	case T_POSITION:
		bit = DIRECTIVE_POSITION;
		break;
	default:
		INTERR;
		break;
	}
	collinfo.directive[collinfo.directive_count] |= bit;
}

void
add_order_directive(void)
{
	if (collinfo.directive_count >= COLL_WEIGHTS_MAX) {
		errf(_("too many directives (max %d)"), COLL_WEIGHTS_MAX);
	}
	collinfo.directive_count++;
}

static void
add_order_pri(int32_t ref)
{
	if (curr_weight >= NUM_WT) {
		errf(_("too many weights (max %d)"), NUM_WT);
		return;
	}
	order_weights[curr_weight] = ref;
	curr_weight++;
}

void
add_order_collsym(collsym_t *s)
{
	add_order_pri(s->ref);
}

void
add_order_char(wchar_t wc)
{
	collchar_t *cc;

	if ((cc = get_collchar(wc, 1)) == NULL) {
		INTERR;
		return;
	}

	add_order_pri(cc->ref[curr_weight]);
}

void
add_order_collelem(collelem_t *e)
{
	add_order_pri(e->ref[curr_weight]);
}

void
add_order_ignore(void)
{
	add_order_pri(pri_ignore);
}

void
add_order_symbol(char *sym)
{
	collundef_t *c;
	if ((c = get_collundef(sym)) == NULL) {
		INTERR;
		return;
	}
	add_order_pri(c->ref[curr_weight]);
}

void
add_order_ellipsis(void)
{
	/* special 0 value indicates self reference */
	add_order_pri(0);
}

void
add_order_subst(void)
{
	subst_t srch;
	subst_t	*s;
	avl_index_t where;
	int i;

	(void) memset(&srch, 0, sizeof (srch));
	for (i = 0; i < curr_subst; i++) {
		srch.ref[i] = subst_weights[i];
		subst_weights[i] = 0;
	}
	s = avl_find(&substs_ref[curr_weight], &srch, &where);

	if (s == NULL) {
		if ((s = calloc(1, sizeof (*s))) == NULL) {
			errf(_("out of memory"));
			return;
		}
		s->key = new_pri();

		/*
		 * We use a self reference for our key, but we set a
		 * high bit to indicate that this is a substitution
		 * reference.  This will expedite table lookups later,
		 * and prevent table lookups for situations that don't
		 * require it.  (In short, its a big win, because we
		 * can skip a lot of binary searching.)
		 */
		set_pri(s->key,
		    (nextsubst[curr_weight] | COLLATE_SUBST_PRIORITY),
		    RESOLVED);
		nextsubst[curr_weight] += 1;

		for (i = 0; i < curr_subst; i++) {
			s->ref[i] = srch.ref[i];
		}

		avl_insert(&substs_ref[curr_weight], s, where);

		if (avl_find(&substs[curr_weight], s, &where) != NULL) {
			INTERR;
			return;
		}
		avl_insert(&substs[curr_weight], s, where);
	}
	curr_subst = 0;


	/*
	 * We are using the current (unique) priority as a search key
	 * in the substitution table.
	 */
	add_order_pri(s->key);
}

static void
add_subst_pri(int32_t ref)
{
	if (curr_subst >= COLLATE_STR_LEN) {
		errf(_("substitution string is too long"));
		return;
	}
	subst_weights[curr_subst] = ref;
	curr_subst++;
}

void
add_subst_char(wchar_t wc)
{
	collchar_t *cc;


	if (((cc = get_collchar(wc, 1)) == NULL) ||
	    (cc->wc != wc)) {
		INTERR;
		return;
	}
	/* we take the weight for the character at that position */
	add_subst_pri(cc->ref[curr_weight]);
}

void
add_subst_collelem(collelem_t *e)
{
	add_subst_pri(e->ref[curr_weight]);
}

void
add_subst_collsym(collsym_t *s)
{
	add_subst_pri(s->ref);
}

void
add_subst_symbol(char *ptr)
{
	collundef_t *cu;

	if ((cu = get_collundef(ptr)) != NULL) {
		add_subst_pri(cu->ref[curr_weight]);
	}
}

void
add_weight(int32_t ref, int pass)
{
	weight_t srch;
	weight_t *w;
	avl_index_t where;

	srch.pri = resolve_pri(ref);

	/* No translation of ignores */
	if (srch.pri == 0)
		return;

	/* Substitution priorities are not weights */
	if (srch.pri & COLLATE_SUBST_PRIORITY)
		return;

	if (avl_find(&weights[pass], &srch, &where) != NULL)
		return;

	if ((w = calloc(1, sizeof (*w))) == NULL) {
		errf(_("out of memory"));
		return;
	}
	w->pri = srch.pri;
	avl_insert(&weights[pass], w, where);
}

void
add_weights(int32_t *refs)
{
	int i;
	for (i = 0; i < NUM_WT; i++) {
		add_weight(refs[i], i);
	}
}

int32_t
get_weight(int32_t ref, int pass)
{
	weight_t	srch;
	weight_t	*w;
	int32_t		pri;

	pri = resolve_pri(ref);
	if (pri & COLLATE_SUBST_PRIORITY) {
		return (pri);
	}
	if (pri <= 0) {
		return (pri);
	}
	srch.pri = pri;
	if ((w = avl_find(&weights[pass], &srch, NULL)) == NULL) {
		INTERR;
		return (-1);
	}
	return (w->opt);
}

void
dump_collate(void)
{
	FILE			*f;
	int			i, j, n;
	size_t			sz;
	int32_t			pri;
	collelem_t		*ce;
	collchar_t		*cc;
	subst_t			*sb;
	char			vers[COLLATE_STR_LEN];
	collate_char_t		chars[UCHAR_MAX + 1];
	collate_large_t		*large;
	collate_subst_t		*subst[COLL_WEIGHTS_MAX];
	collate_chain_t		*chain;

	/*
	 * We have to run through a preliminary pass to identify all the
	 * weights that we use for each sorting level.
	 */
	for (i = 0; i < NUM_WT; i++) {
		add_weight(pri_ignore, i);
	}
	for (i = 0; i < NUM_WT; i++) {
		for (sb = avl_first(&substs[i]); sb;
		    sb = AVL_NEXT(&substs[i], sb)) {
			for (j = 0; sb->ref[j]; j++) {
				add_weight(sb->ref[j], i);
			}
		}
	}
	for (ce = avl_first(&elem_by_expand);
	    ce != NULL;
	    ce = AVL_NEXT(&elem_by_expand, ce)) {
		add_weights(ce->ref);
	}
	for (cc = avl_first(&collchars); cc; cc = AVL_NEXT(&collchars, cc)) {
		add_weights(cc->ref);
	}

	/*
	 * Now we walk the entire set of weights, removing the gaps
	 * in the weights.  This gives us optimum usage.  The walk
	 * occurs in priority.
	 */
	for (i = 0; i < NUM_WT; i++) {
		weight_t *w;
		for (w = avl_first(&weights[i]); w;
		    w = AVL_NEXT(&weights[i], w)) {
			w->opt = nweight[i];
			nweight[i] += 1;
		}
	}

	(void) memset(&chars, 0, sizeof (chars));
	(void) memset(vers, 0, COLLATE_STR_LEN);
	(void) strlcpy(vers, COLLATE_VERSION, sizeof (vers));

	/*
	 * We need to make sure we arrange for the UNDEFINED field
	 * to show up.  Also, set the total weight counts.
	 */
	for (i = 0; i < NUM_WT; i++) {
		if (resolve_pri(pri_undefined[i]) == -1) {
			set_pri(pri_undefined[i], -1, RESOLVED);
			/* they collate at the end of everything else */
			collinfo.undef_pri[i] = COLLATE_MAX_PRIORITY;
		}
		collinfo.pri_count[i] = nweight[i];
	}

	collinfo.pri_count[NUM_WT] = max_wide();
	collinfo.undef_pri[NUM_WT] = COLLATE_MAX_PRIORITY;
	collinfo.directive[NUM_WT] = DIRECTIVE_UNDEFINED;

	/*
	 * Ordinary character priorities
	 */
	for (i = 0; i <= UCHAR_MAX; i++) {
		if ((cc = get_collchar(i, 0)) != NULL) {
			for (j = 0; j < NUM_WT; j++) {
				chars[i].pri[j] = get_weight(cc->ref[j], j);
			}
		} else {
			for (j = 0; j < NUM_WT; j++) {
				chars[i].pri[j] =
				    get_weight(pri_undefined[j], j);
			}
			/*
			 * Per POSIX, for undefined characters, we
			 * also have to add a last item, which is the
			 * character code.
			 */
			chars[i].pri[NUM_WT] = i;
		}
	}

	/*
	 * Substitution tables
	 */
	for (i = 0; i < NUM_WT; i++) {
		collate_subst_t *st = NULL;
		n = collinfo.subst_count[i] = avl_numnodes(&substs[i]);
		if ((st = calloc(n, sizeof (collate_subst_t))) == NULL) {
			errf(_("out of memory"));
			return;
		}
		n = 0;
		for (sb = avl_first(&substs[i]); sb;
		    sb = AVL_NEXT(&substs[i], sb)) {
			if ((st[n].key = resolve_pri(sb->key)) < 0) {
				/* by definition these resolve! */
				INTERR;
			}
			if (st[n].key != (n | COLLATE_SUBST_PRIORITY)) {
				INTERR;
			}
			for (j = 0; sb->ref[j]; j++) {
				st[n].pri[j] = get_weight(sb->ref[j], i);
			}
			n++;
		}
		if (n != collinfo.subst_count[i])
			INTERR;
		subst[i] = st;
	}


	/*
	 * Chains, i.e. collating elements
	 */
	collinfo.chain_count = avl_numnodes(&elem_by_expand);
	chain = calloc(collinfo.chain_count, sizeof (collate_chain_t));
	if (chain == NULL) {
		errf(_("out of memory"));
		return;
	}
	for (n = 0, ce = avl_first(&elem_by_expand);
	    ce != NULL;
	    ce = AVL_NEXT(&elem_by_expand, ce), n++) {
		(void) wsncpy(chain[n].str, ce->expand, COLLATE_STR_LEN);
		for (i = 0; i < NUM_WT; i++) {
			chain[n].pri[i] = get_weight(ce->ref[i], i);
		}
	}
	if (n != collinfo.chain_count)
		INTERR;

	/*
	 * Large (> UCHAR_MAX) character priorities
	 */
	large = calloc(avl_numnodes(&collchars), sizeof (collate_large_t));
	if (large == NULL) {
		errf(_("out of memory"));
		return;
	}

	i = 0;
	for (cc = avl_first(&collchars); cc; cc = AVL_NEXT(&collchars, cc)) {
		int	undef = 0;
		/* we already gathered those */
		if (cc->wc <= UCHAR_MAX)
			continue;
		for (j = 0; j < NUM_WT; j++) {
			if ((pri = get_weight(cc->ref[j], j)) < 0) {
				undef = 1;
			}
			if (undef && (pri >= 0)) {
				/* if undefined, then all priorities are */
				INTERR;
			} else {
				large[i].pri.pri[j] = pri;
			}
		}
		if (!undef) {
			large[i].val = cc->wc;
			collinfo.large_count = i++;
		}
	}

	if ((f = open_category()) == NULL) {
		return;
	}

	/* Time to write the entire data set out */

	if ((wr_category(vers, COLLATE_STR_LEN, f) < 0) ||
	    (wr_category(&collinfo, sizeof (collinfo), f) < 0) ||
	    (wr_category(&chars, sizeof (chars), f) < 0)) {
		delete_category(f);
		return;
	}

	for (i = 0; i < NUM_WT; i++) {
		sz =  sizeof (collate_subst_t) * collinfo.subst_count[i];
		if (wr_category(subst[i], sz, f) < 0) {
			delete_category(f);
			return;
		}
	}
	sz = sizeof (collate_chain_t) * collinfo.chain_count;
	if (wr_category(chain, sz, f) < 0) {
		delete_category(f);
		return;
	}
	sz = sizeof (collate_large_t) * collinfo.large_count;
	if (wr_category(large, sz, f) < 0) {
		delete_category(f);
		return;
	}

	close_category(f);
}
