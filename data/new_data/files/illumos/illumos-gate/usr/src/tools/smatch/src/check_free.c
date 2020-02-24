/*
 * Copyright (C) 2010 Dan Carpenter.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see http://www.gnu.org/copyleft/gpl.txt
 */

/*
 * check_memory() is getting too big and messy.
 *
 */

#include <string.h>
#include "smatch.h"
#include "smatch_slist.h"
#include "smatch_extra.h"

static int my_id;

STATE(freed);
STATE(ok);

static void ok_to_use(struct sm_state *sm, struct expression *mod_expr)
{
	if (sm->state != &ok)
		set_state(my_id, sm->name, sm->sym, &ok);
}

static void pre_merge_hook(struct sm_state *cur, struct sm_state *other)
{
	if (is_impossible_path())
		set_state(my_id, cur->name, cur->sym, &ok);
}

static int is_freed(struct expression *expr)
{
	struct sm_state *sm;

	sm = get_sm_state_expr(my_id, expr);
	if (sm && slist_has_state(sm->possible, &freed))
		return 1;
	return 0;
}

static void match_symbol(struct expression *expr)
{
	struct expression *parent;
	char *name;

	if (is_impossible_path())
		return;
	if (__in_fake_parameter_assign)
		return;

	parent = expr_get_parent_expr(expr);
	while (parent && parent->type == EXPR_PREOP && parent->op == '(')
		parent = expr_get_parent_expr(parent);
	if (parent && parent->type == EXPR_PREOP && parent->op == '&')
		return;

	if (!is_freed(expr))
		return;
	name = expr_to_var(expr);
	sm_warning("'%s' was already freed.", name);
	free_string(name);
}

static void match_dereferences(struct expression *expr)
{
	char *name;

	if (__in_fake_parameter_assign)
		return;

	if (expr->type != EXPR_PREOP)
		return;

	if (is_impossible_path())
		return;

	expr = strip_expr(expr->unop);
	if (!is_freed(expr))
		return;
	name = expr_to_var(expr);
	sm_error("dereferencing freed memory '%s'", name);
	set_state_expr(my_id, expr, &ok);
	free_string(name);
}

static int ignored_params[16];

static void set_ignored_params(struct expression *call)
{
	struct expression *arg;
	const char *p;
	int i;

	memset(&ignored_params, 0, sizeof(ignored_params));

	i = -1;
	FOR_EACH_PTR(call->args, arg) {
		i++;
		if (arg->type != EXPR_STRING)
			continue;
		goto found;
	} END_FOR_EACH_PTR(arg);

	return;

found:
	i++;
	p = arg->string->data;
	while ((p = strchr(p, '%'))) {
		if (i >= ARRAY_SIZE(ignored_params))
			return;
		p++;
		if (*p == '%') {
			p++;
			continue;
		}
		if (*p == '.')
			p++;
		if (*p == '*')
			i++;
		if (*p == 'p')
			ignored_params[i] = 1;
		i++;
	}
}

static int is_free_func(struct expression *fn)
{
	char *name;
	int ret = 0;

	name = expr_to_str(fn);
	if (!name)
		return 0;
	if (strstr(name, "free"))
		ret = 1;
	free_string(name);

	return ret;
}

static void match_call(struct expression *expr)
{
	struct expression *arg;
	char *name;
	int i;

	if (is_impossible_path())
		return;

	set_ignored_params(expr);

	i = -1;
	FOR_EACH_PTR(expr->args, arg) {
		i++;
		if (!is_pointer(arg))
			continue;
		if (!is_freed(arg))
			continue;
		if (ignored_params[i])
			continue;

		name = expr_to_var(arg);
		if (is_free_func(expr->fn))
			sm_error("double free of '%s'", name);
		else
			sm_warning("passing freed memory '%s'", name);
		set_state_expr(my_id, arg, &ok);
		free_string(name);
	} END_FOR_EACH_PTR(arg);
}

static void match_return(struct expression *expr)
{
	char *name;

	if (is_impossible_path())
		return;
	if (__in_fake_parameter_assign)
		return;

	if (!expr)
		return;
	if (!is_freed(expr))
		return;

	name = expr_to_var(expr);
	sm_warning("returning freed memory '%s'", name);
	set_state_expr(my_id, expr, &ok);
	free_string(name);
}

static void match_free(const char *fn, struct expression *expr, void *param)
{
	struct expression *arg;

	if (is_impossible_path())
		return;

	arg = get_argument_from_call_expr(expr->args, PTR_INT(param));
	if (!arg)
		return;
	if (is_freed(arg)) {
		char *name = expr_to_var(arg);

		sm_error("double free of '%s'", name);
		free_string(name);
	}
	set_state_expr(my_id, arg, &freed);
}

static void set_param_freed(struct expression *call, struct expression *arg, char *key, char *unused)
{
	struct symbol *sym;
	char *name;

	name = get_variable_from_key(arg, key, &sym);
	if (!name || !sym)
		goto free;

	set_state(my_id, name, sym, &freed);
free:
	free_string(name);
}

int parent_is_free_var_sym(const char *name, struct symbol *sym)
{
	char buf[256];
	char *start;
	char *end;
	struct smatch_state *state;

	if (option_project == PROJ_KERNEL)
		return parent_is_free_var_sym_strict(name, sym);

	strncpy(buf, name, sizeof(buf) - 1);
	buf[sizeof(buf) - 1] = '\0';

	start = &buf[0];
	while ((*start == '&'))
		start++;

	while ((end = strrchr(start, '-'))) {
		*end = '\0';
		state = __get_state(my_id, start, sym);
		if (state == &freed)
			return 1;
	}
	return 0;
}

int parent_is_free(struct expression *expr)
{
	struct symbol *sym;
	char *var;
	int ret = 0;

	expr = strip_expr(expr);
	var = expr_to_var_sym(expr, &sym);
	if (!var || !sym)
		goto free;
	ret = parent_is_free_var_sym(var, sym);
free:
	free_string(var);
	return ret;
}

void check_free(int id)
{
	my_id = id;

	if (option_project == PROJ_KERNEL) {
		/* The kernel use check_free_strict.c */
		return;
	}

	add_function_hook("free", &match_free, INT_PTR(0));

	if (option_spammy)
		add_hook(&match_symbol, SYM_HOOK);
	add_hook(&match_dereferences, DEREF_HOOK);
	add_hook(&match_call, FUNCTION_CALL_HOOK);
	add_hook(&match_return, RETURN_HOOK);

	add_modification_hook(my_id, &ok_to_use);
	select_return_implies_hook(PARAM_FREED, &set_param_freed);
	add_pre_merge_hook(my_id, &pre_merge_hook);
}
