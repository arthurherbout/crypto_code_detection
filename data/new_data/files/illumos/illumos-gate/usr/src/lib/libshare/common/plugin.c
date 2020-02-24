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
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libshare.h>
#include "libshare_impl.h"
#include <dlfcn.h>
#include <link.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libintl.h>
#include <sys/systeminfo.h>
#include <thread.h>
#include <synch.h>

#define	MAXISALEN	257	/* based on sysinfo(2) man page */

/*
 * protocol plugin interface
 *
 * finds plugins and makes them accessible. This is only "used" by
 * libshare.so.
 */

struct sa_proto_plugin *sap_proto_list;

static struct sa_proto_handle sa_proto_handle;

void proto_plugin_fini();

/*
 * Returns true if name is "." or "..", otherwise returns false.
 */
static boolean_t
proto_is_dot_or_dotdot(const char *name)
{
	if (*name != '.')
		return (B_FALSE);

	if ((name[1] == '\0') || (name[1] == '.' && name[2] == '\0'))
		return (B_TRUE);

	return (B_FALSE);
}

/*
 * proto_plugin_init()
 *
 * Initialize the protocol specific plugin modules.
 *
 * Walk /usr/lib/fs/\* for libshare_*.so modules, for example,
 * /usr/lib/fs/nfs/libshare_nfs.so. A protocol specific directory
 * would have modules with names of the form libshare_<proto>.so.
 * For each protocol found, initialize it and add it to the internal
 * list of protocols. These are used for protocol specific operations.
 */

int
proto_plugin_init()
{
	struct sa_proto_plugin *proto;
	int num_protos = 0;
	struct sa_plugin_ops *plugin_ops;
	void *dlhandle;
	DIR *dir;
	struct dirent *dent;
	int ret = SA_OK;
	struct stat st;
	char isa[MAXISALEN];

#if defined(_LP64)
	if (sysinfo(SI_ARCHITECTURE_64, isa, MAXISALEN) == -1)
		isa[0] = '\0';
#else
	isa[0] = '\0';
#endif

	if ((dir = opendir(SA_LIB_DIR)) == NULL)
		return (SA_OK);

	while ((dent = readdir(dir)) != NULL) {
		char path[MAXPATHLEN];

		if (proto_is_dot_or_dotdot(dent->d_name))
			continue;

		(void) snprintf(path, MAXPATHLEN,
		    "%s/%s/%s/libshare_%s.so.1", SA_LIB_DIR,
		    dent->d_name, isa, dent->d_name);

		/*
		 * If file doesn't exist, don't try to map it
		 */
		if (stat(path, &st) < 0)
			continue;

		if ((dlhandle = dlopen(path, RTLD_FIRST|RTLD_LAZY)) == NULL) {
			(void) fprintf(stderr, dgettext(TEXT_DOMAIN,
			    "Error in plugin for protocol %s: %s\n"),
			    dent->d_name, dlerror());
			continue;
		}

		plugin_ops = (struct sa_plugin_ops *)
		    dlsym(dlhandle, "sa_plugin_ops");
		if (plugin_ops == NULL) {
			(void) fprintf(stderr, dgettext(TEXT_DOMAIN,
			    "Error in plugin ops for protocol %s: %s\n"),
			    dent->d_name, dlerror());
			(void) dlclose(dlhandle);
			continue;
		}

		proto = (struct sa_proto_plugin *)
		    calloc(1, sizeof (struct sa_proto_plugin));
		if (proto == NULL) {
			(void) dlclose(dlhandle);
			ret = SA_NO_MEMORY;
			continue;
		}

		proto->plugin_ops = plugin_ops;
		proto->plugin_handle = dlhandle;
		num_protos++;
		proto->plugin_next = sap_proto_list;
		sap_proto_list = proto;
	}

	(void) closedir(dir);

	if (num_protos != 0) {
		sa_proto_handle.sa_proto =
		    (char **)calloc(num_protos, sizeof (char *));
		sa_proto_handle.sa_ops =
		    (struct sa_plugin_ops **)calloc(num_protos,
		    sizeof (struct sa_plugin_ops *));
		if (sa_proto_handle.sa_proto != NULL &&
		    sa_proto_handle.sa_ops != NULL) {
			int i;
			struct sa_proto_plugin *tmp;

			for (i = 0, tmp = sap_proto_list;
			    i < num_protos && tmp != NULL;
			    tmp = tmp->plugin_next) {
				int err;
				err = SA_OK;
				if (tmp->plugin_ops->sa_init != NULL)
					err = tmp->plugin_ops->sa_init();
				if (err == SA_OK) {
					/*
					 * Only include if the init
					 * succeeded or was NULL
					 */
					sa_proto_handle.sa_num_proto++;
					sa_proto_handle.sa_ops[i] =
					    tmp->plugin_ops;
					sa_proto_handle.sa_proto[i] =
					    tmp->plugin_ops->sa_protocol;
					i++;
				}
			}
		} else {
			ret = SA_NO_MEMORY;
		}
	}

	/*
	 * There was an error, so cleanup prior to return of failure.
	 */
	if (ret != SA_OK)
		proto_plugin_fini();

	return (ret);
}

/*
 * proto_plugin_fini()
 *
 * Uninitialize all the plugin modules.
 */

void
proto_plugin_fini()
{
	struct sa_proto_plugin *p;

	/*
	 * Protocols may call this framework during _fini
	 * (the smbfs plugin is known to do this) so do
	 * two passes: 1st call _fini; 2nd free, dlclose.
	 */
	for (p = sap_proto_list; p != NULL; p = p->plugin_next)
		p->plugin_ops->sa_fini();

	while ((p = sap_proto_list) != NULL) {
		sap_proto_list = p->plugin_next;

		if (p->plugin_handle != NULL)
			(void) dlclose(p->plugin_handle);
		free(p);
	}
	if (sa_proto_handle.sa_ops != NULL) {
		free(sa_proto_handle.sa_ops);
		sa_proto_handle.sa_ops = NULL;
	}
	if (sa_proto_handle.sa_proto != NULL) {
		free(sa_proto_handle.sa_proto);
		sa_proto_handle.sa_proto = NULL;
	}
	sa_proto_handle.sa_num_proto = 0;
}

/*
 * find_protocol(proto)
 *
 * Search the plugin list for the specified protocol and return the
 * ops vector.  NULL if protocol is not defined.
 */

static struct sa_plugin_ops *
find_protocol(char *proto)
{
	int i;
	struct sa_plugin_ops *ops = NULL;
	extern mutex_t sa_global_lock;

	(void) mutex_lock(&sa_global_lock);
	if (proto != NULL) {
		for (i = 0; i < sa_proto_handle.sa_num_proto; i++) {
			if (strcmp(proto, sa_proto_handle.sa_proto[i]) == 0) {
				ops = sa_proto_handle.sa_ops[i];
				break;
			}
		}
	}
	(void) mutex_unlock(&sa_global_lock);
	return (ops);
}

/*
 * sa_proto_share(proto, share)
 *
 * Activate a share for the specified protocol.
 */

int
sa_proto_share(char *proto, sa_share_t share)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	int ret = SA_INVALID_PROTOCOL;

	if (ops != NULL && ops->sa_share != NULL)
		ret = ops->sa_share(share);
	return (ret);
}

/*
 * sa_proto_unshare(proto, share)
 *
 * Deactivate (unshare) the share for this protocol.
 */

int
sa_proto_unshare(sa_share_t share, char *proto, char *path)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	int ret = SA_INVALID_PROTOCOL;

	if (ops != NULL && ops->sa_unshare != NULL)
		ret = ops->sa_unshare(share, path);
	return (ret);
}

/*
 * sa_proto_share_resource(char *proto, sa_resource_t resource)
 *
 * For protocols that actually enable at the resource level, do the
 * protocol specific resource enable. If it doesn't, return an error.
 * Note that the resource functions are optional so can return
 * SA_NOT_SUPPORTED.
 */

int
sa_proto_share_resource(char *proto, sa_resource_t resource)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	int ret = SA_INVALID_PROTOCOL;

	if (ops != NULL) {
		if (ops->sa_enable_resource != NULL)
			ret = ops->sa_enable_resource(resource);
		else
			ret = SA_NOT_SUPPORTED;
	}
	return (ret);
}

/*
 * sa_proto_unshare_resource(char *proto, sa_resource_t resource)
 *
 * For protocols that actually disable at the resource level, do the
 * protocol specific resource disable. If it doesn't, return an error.
 */

int
sa_proto_unshare_resource(char *proto, sa_resource_t resource)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	int ret = SA_INVALID_PROTOCOL;

	if (ops != NULL) {
		if (ops->sa_disable_resource != NULL)
			ret = ops->sa_disable_resource(resource);
		else
			ret = SA_NOT_SUPPORTED;
	}
	return (ret);
}

/*
 * sa_proto_valid_prop(handle, proto, prop, opt)
 *
 * Check to see if the specified prop is valid for this protocol.
 */

int
sa_proto_valid_prop(sa_handle_t handle, char *proto, sa_property_t prop,
    sa_optionset_t opt)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	int ret = 0;

	if (ops != NULL && ops->sa_valid_prop != NULL)
		ret = ops->sa_valid_prop(handle, prop, opt);
	return (ret);
}

/*
 * sa_proto_valid_space(proto, space)
 *
 * Check if space is valid optionspace for proto.
 * Protocols that don't implement this don't support spaces.
 */
int
sa_proto_valid_space(char *proto, char *token)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	int ret = 0;

	if (ops != NULL && ops->sa_valid_space != NULL)
		ret = ops->sa_valid_space(token);
	return (ret);
}

/*
 * sa_proto_space_alias(proto, space)
 *
 * If the name for space is an alias, return its proper name.  This is
 * used to translate "default" values into proper form.
 */
char *
sa_proto_space_alias(char *proto, char *space)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	char *ret = space;

	if (ops != NULL && ops->sa_space_alias != NULL)
		ret = ops->sa_space_alias(space);
	return (ret);
}

/*
 * sa_proto_security_prop(proto, token)
 *
 * Check to see if the property name in token is a valid named
 * optionset property.
 */

int
sa_proto_security_prop(char *proto, char *token)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	int ret = 0;

	if (ops != NULL && ops->sa_security_prop != NULL)
		ret = ops->sa_security_prop(token);
	return (ret);
}

/*
 * sa_proto_legacy_opts(proto, grouup, options)
 *
 * Have the protocol specific parser parse the options string and add
 * an appropriate optionset to group.
 */

int
sa_proto_legacy_opts(char *proto, sa_group_t group, char *options)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	int ret = SA_INVALID_PROTOCOL;

	if (ops != NULL && ops->sa_legacy_opts != NULL)
		ret = ops->sa_legacy_opts(group, options);
	return (ret);
}

/*
 * sa_proto_legacy_format(proto, group, hier)
 *
 * Return a legacy format string representing either the group's
 * properties or the groups hierarchical properties.
 */

char *
sa_proto_legacy_format(char *proto, sa_group_t group, int hier)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	char *ret = NULL;

	if (ops != NULL && ops->sa_legacy_format != NULL)
		ret = ops->sa_legacy_format(group, hier);
	return (ret);
}

void
sa_format_free(char *str)
{
	free(str);
}

/*
 * sharectl related API functions
 */

/*
 * sa_proto_get_properties(proto)
 *
 * Return the set of properties that are specific to the
 * protocol. These are usually in /etc/dfs/<proto> and related files,
 * but only the protocol module knows which ones for sure.
 */

sa_protocol_properties_t
sa_proto_get_properties(char *proto)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	sa_protocol_properties_t props = NULL;

	if (ops != NULL && ops->sa_get_proto_set != NULL)
		props = ops->sa_get_proto_set();
	return (props);
}

/*
 * sa_proto_set_property(proto, prop)
 *
 * Update the protocol specific property.
 */

int
sa_proto_set_property(char *proto, sa_property_t prop)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	int ret = SA_OK;

	if (ops != NULL && ops->sa_set_proto_prop != NULL)
		ret = ops->sa_set_proto_prop(prop);
	return (ret);
}

/*
 * sa_valid_protocol(proto)
 *
 * Check to see if the protocol specified is defined by a
 * plugin. Returns true (1) or false (0)
 */

int
sa_valid_protocol(char *proto)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	return (ops != NULL);
}

/*
 * Return the current operational status of the protocol
 */

char *
sa_get_protocol_status(char *proto)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	char *ret = NULL;
	if (ops != NULL && ops->sa_get_proto_status != NULL)
		ret = ops->sa_get_proto_status(proto);
	return (ret);
}

/*
 * sa_proto_update_legacy(proto, share)
 *
 * Update the protocol specific legacy files if necessary for the
 * specified share.
 */

int
sa_proto_update_legacy(char *proto, sa_share_t share)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	int ret = SA_NOT_IMPLEMENTED;

	if (ops != NULL) {
		if (ops->sa_update_legacy != NULL)
			ret = ops->sa_update_legacy(share);
	}
	return (ret);
}

/*
 * sa_delete_legacy(proto, share)
 *
 * Remove the specified share from the protocol specific legacy files.
 */

int
sa_proto_delete_legacy(char *proto, sa_share_t share)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	int ret = SA_NOT_IMPLEMENTED;

	if (ops != NULL) {
		if (ops->sa_delete_legacy != NULL)
			ret = ops->sa_delete_legacy(share);
	} else {
		if (proto != NULL)
			ret = SA_NOT_IMPLEMENTED;
		else
			ret = SA_INVALID_PROTOCOL;
	}
	return (ret);
}

/*
 * sa_proto_delete_section(proto, section)
 *
 * Remove the specified section from the protocol specific legacy files,
 * if supported.
 */

int
sa_proto_delete_section(char *proto, char *section)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	int ret = SA_OK;

	if (ops != NULL) {
		if (ops->sa_delete_proto_section != NULL)
			ret = ops->sa_delete_proto_section(section);
	} else {
		if (proto != NULL)
			ret = SA_NOT_IMPLEMENTED;
		else
			ret = SA_INVALID_PROTOCOL;
	}
	return (ret);
}

/*
 * sa_proto_change_notify(share, char *protocol)
 *
 * Notify the protocol that a change has been made to the share
 */

int
sa_proto_change_notify(sa_share_t share, char *proto)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	int ret = SA_NOT_IMPLEMENTED;

	if (ops != NULL) {
		if (ops->sa_change_notify != NULL)
			ret = ops->sa_change_notify(share);
	} else	if (proto == NULL) {

			ret = SA_INVALID_PROTOCOL;
	}
	return (ret);
}

/*
 * sa_proto_notify_resource(resource, char *protocol)
 *
 * Notify the protocol that a change has been made to the share
 */

int
sa_proto_notify_resource(sa_resource_t resource, char *proto)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	int ret = SA_NOT_IMPLEMENTED;

	if (ops != NULL) {
		if (ops->sa_notify_resource != NULL)
			ret = ops->sa_notify_resource(resource);
	} else if (proto == NULL) {
			ret = SA_INVALID_PROTOCOL;
	}
	return (ret);
}

/*
 * sa_proto_get_featureset(protocol)
 *
 * Get bitmask of defined features of the protocol. These are
 * primarily things like SA_FEATURE_RESOURCE (shares are by resource
 * name rather than path) and other operational features that affect
 * behavior.
 */

uint64_t
sa_proto_get_featureset(char *proto)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	uint64_t ret = 0;

	if (ops != NULL) {
		if (ops->sa_features != NULL)
			ret = ops->sa_features();
	}
	/* if not implemented, zero is valid */
	return (ret);
}

/*
 * sa_proto_get_transients(sa_handle_t)
 *
 * Called to get any protocol specific transient shares.  NFS doesn't
 * use this since the info is in sharetab which is processed as a
 * common transient store.
 *
 * The protocol plugin should verify that the share isn't in the
 * repository and then add it as a transient.
 *
 * Not having an entry is not a problem. It returns 0 in that case.
 */

int
sa_proto_get_transients(sa_handle_t handle, char *proto)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	int ret = 0;

	if (ops != NULL) {
		if (ops->sa_get_transient_shares != NULL)
			ret = ops->sa_get_transient_shares(handle);
	}
	return (ret);
}

/*
 * sa_proto_rename_resource(sa_handle_t, proto, sa_resource_t, newname)
 *
 * Protocols may need to know when a resource has changed names in
 * order to notify clients. This must be done "before" the name in the
 * resource has been changed. Not being implemented is not a problem.
 */

int
sa_proto_rename_resource(sa_handle_t handle, char *proto,
    sa_resource_t resource, char *newname)
{
	struct sa_plugin_ops *ops = find_protocol(proto);
	int ret = SA_OK;

	if (ops != NULL) {
		if (ops->sa_rename_resource != NULL)
			ret = ops->sa_rename_resource(handle, resource,
			    newname);
	}
	return (ret);
}
