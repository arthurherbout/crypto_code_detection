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
 * Copyright (c) 2006, 2010, Oracle and/or its affiliates. All rights reserved.
 */

#include <limits.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <alloca.h>
#include <libnvpair.h>
#include <fm/topo_mod.h>
#include <sys/fm/protocol.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/objfs.h>
#include <sys/modctl.h>
#include <libelf.h>
#include <gelf.h>

#include <topo_method.h>
#include <topo_subr.h>
#include <pkg.h>

#define	BUFLEN	(2 * PATH_MAX)

static int pkg_enum(topo_mod_t *, tnode_t *, const char *, topo_instance_t,
    topo_instance_t, void *, void *);
static void pkg_release(topo_mod_t *, tnode_t *);
static int pkg_fmri_create_meth(topo_mod_t *, tnode_t *, topo_version_t,
    nvlist_t *, nvlist_t **);
static int pkg_fmri_nvl2str(topo_mod_t *, tnode_t *, topo_version_t,
    nvlist_t *, nvlist_t **);

static const topo_method_t pkg_methods[] = {
	{ TOPO_METH_FMRI, TOPO_METH_FMRI_DESC, TOPO_METH_FMRI_VERSION,
	    TOPO_STABILITY_INTERNAL, pkg_fmri_create_meth },
	{ TOPO_METH_NVL2STR, TOPO_METH_NVL2STR_DESC, TOPO_METH_NVL2STR_VERSION,
	    TOPO_STABILITY_INTERNAL, pkg_fmri_nvl2str },
	{ NULL }
};

static const topo_modops_t pkg_ops =
	{ pkg_enum, pkg_release };
static const topo_modinfo_t pkg_info =
	{ "pkg", FM_FMRI_SCHEME_PKG, PKG_VERSION, &pkg_ops };

int
pkg_init(topo_mod_t *mod, topo_version_t version)
{
	if (getenv("TOPOPKGDEBUG"))
		topo_mod_setdebug(mod);
	topo_mod_dprintf(mod, "initializing pkg builtin\n");

	if (version != PKG_VERSION)
		return (topo_mod_seterrno(mod, EMOD_VER_NEW));

	if (topo_mod_register(mod, &pkg_info, TOPO_VERSION) != 0) {
		topo_mod_dprintf(mod, "failed to register pkg_info: "
		    "%s\n", topo_mod_errmsg(mod));
		return (-1);
	}

	return (0);
}

void
pkg_fini(topo_mod_t *mod)
{
	topo_mod_unregister(mod);
}

/*ARGSUSED*/
static int
pkg_enum(topo_mod_t *mod, tnode_t *pnode, const char *name,
    topo_instance_t min, topo_instance_t max, void *notused1, void *notused2)
{
	/*
	 * Methods are registered, but there is no enumeration.  Should
	 * enumeration be added be sure to cater for global vs non-global
	 * zones.
	 */
	(void) topo_method_register(mod, pnode, pkg_methods);
	return (0);
}

static void
pkg_release(topo_mod_t *mod, tnode_t *node)
{
	topo_method_unregister_all(mod, node);
}

static int
read_thru(topo_mod_t *mp, FILE *fp, const char *substr)
{
	char *tmpbuf = alloca(2 * MAXPATHLEN);
	int notfound = 1;

	while (fgets(tmpbuf, 2 * MAXPATHLEN, fp) != NULL) {
		if (substr == NULL)
			topo_mod_dprintf(mp, "%s", tmpbuf);
		else if (strstr(tmpbuf, substr) != NULL) {
			notfound = 0;
			break;
		}
	}
	return (notfound);
}

static nvlist_t *
construct_fru_fmri(topo_mod_t *mp, const char *pkgname, FILE *fp)
{
	nvlist_t *f = NULL;
	char *tmpbuf = alloca(BUFLEN);
	char *pkgdir = NULL;
	char *pkgver = NULL;
	char *token;
	int e;

	while (fgets(tmpbuf, BUFLEN, fp) != NULL) {
		if (strstr(tmpbuf, "VERSION:") != NULL) {
			token = strtok(tmpbuf, ":");
			token = strtok(NULL, ": \t\n");
			pkgver = topo_mod_strdup(mp, token);
		} else if (strstr(tmpbuf, "BASEDIR:") != NULL) {
			token = strtok(tmpbuf, ":");
			token = strtok(NULL, ": \t\n");
			pkgdir = topo_mod_strdup(mp, token);
		}
	}

	if (pkgdir == NULL || pkgver == NULL) {
		(void) topo_mod_seterrno(mp, EMOD_METHOD_INVAL);
		goto fmrileave;
	}

	if (topo_mod_nvalloc(mp, &f, NV_UNIQUE_NAME) != 0) {
		(void) topo_mod_seterrno(mp, EMOD_FMRI_NVL);
		goto fmrileave;
	}

	e = nvlist_add_string(f, FM_FMRI_SCHEME, FM_FMRI_SCHEME_PKG);
	e |= nvlist_add_uint8(f, FM_VERSION, FM_PKG_SCHEME_VERSION);
	e |= nvlist_add_string(f, FM_FMRI_PKG_BASEDIR, pkgdir);
	e |= nvlist_add_string(f, FM_FMRI_PKG_INST, pkgname);
	e |= nvlist_add_string(f, FM_FMRI_PKG_VERSION, pkgver);
	if (e == 0)
		goto fmrileave;

	topo_mod_dprintf(mp, "construction of pkg nvl failed");
	(void) topo_mod_seterrno(mp, EMOD_FMRI_NVL);
	nvlist_free(f);
	f = NULL;

fmrileave:
	if (pkgdir != NULL)
		topo_mod_strfree(mp, pkgdir);
	if (pkgver != NULL)
		topo_mod_strfree(mp, pkgver);

	return (f);
}

#define	PKGINFO_CMD	"LC_MESSAGES= /usr/bin/pkginfo -l %s 2>/dev/null"
#define	PKGCHK_CMD	"LC_MESSAGES= /usr/sbin/pkgchk -lp %s 2>/dev/null"
#define	PKG_KEYPHRASE	"Referenced by the following packages:"

static nvlist_t *
pkg_fmri_create(topo_mod_t *mp, const char *path)
{
	static char tmpbuf[BUFLEN];
	char *findpkgname;
	char *pkgname = NULL;
	FILE *pcout;
	nvlist_t *out = NULL;

	(void) snprintf(tmpbuf, BUFLEN, PKGCHK_CMD, path);
	topo_mod_dprintf(mp, "popen of %s\n", tmpbuf);
	pcout = popen(tmpbuf, "r");
	if (read_thru(mp, pcout, PKG_KEYPHRASE)) {
		(void) pclose(pcout);
		goto pfc_bail;
	}
	(void) fgets(tmpbuf, BUFLEN, pcout);
	(void) pclose(pcout);
	topo_mod_dprintf(mp, "%s", tmpbuf);

	if ((findpkgname = strtok(tmpbuf, " 	\n")) == NULL)
		goto pfc_bail;
	pkgname = topo_mod_strdup(mp, findpkgname);

	(void) snprintf(tmpbuf, BUFLEN, PKGINFO_CMD, pkgname);
	topo_mod_dprintf(mp, "popen of %s\n", tmpbuf);
	pcout = popen(tmpbuf, "r");
	out = construct_fru_fmri(mp, pkgname, pcout);
	(void) pclose(pcout);

pfc_bail:
	if (pkgname != NULL)
		topo_mod_strfree(mp, pkgname);
	return (out);
}

/*ARGSUSED*/
static int
pkg_fmri_create_meth(topo_mod_t *mp, tnode_t *node, topo_version_t version,
    nvlist_t *in, nvlist_t **out)
{
	nvlist_t *args = NULL;
	char *path;

	if (version > TOPO_METH_FMRI_VERSION)
		return (topo_mod_seterrno(mp, EMOD_VER_NEW));

	if (nvlist_lookup_nvlist(in, TOPO_METH_FMRI_ARG_NVL, &args) != 0 ||
	    nvlist_lookup_string(args, "path", &path) != 0) {
		topo_mod_dprintf(mp, "no path string in method argument\n");
		return (topo_mod_seterrno(mp, EMOD_METHOD_INVAL));
	}

	if ((*out = pkg_fmri_create(mp, path)) == NULL)
		return (-1);
	return (0);
}

static ssize_t
fmri_nvl2str(nvlist_t *nvl, char *buf, size_t buflen)
{
	nvlist_t *anvl = NULL;
	nvpair_t *apair;
	uint8_t version;
	ssize_t size = 0;
	char *pkgname = NULL, *aname, *aval;
	int err;

	if (nvlist_lookup_uint8(nvl, FM_VERSION, &version) != 0 ||
	    version > FM_PKG_SCHEME_VERSION)
		return (-1);

	/* Get authority, if present */
	err = nvlist_lookup_nvlist(nvl, FM_FMRI_AUTHORITY, &anvl);
	if (err != 0 && err != ENOENT)
		return (-1);

	/*
	 *  For brevity, we only include the pkgname and any authority
	 *  info present in the FMRI in our output string.  The FMRI
	 *  also has data on the package directory and version.
	 */
	err = nvlist_lookup_string(nvl, FM_FMRI_PKG_INST, &pkgname);
	if (err != 0 || pkgname == NULL)
		return (-1);

	/* pkg:// */
	topo_fmristr_build(&size, buf, buflen, FM_FMRI_SCHEME_PKG, NULL, "://");

	/* authority, if any */
	if (anvl != NULL) {
		for (apair = nvlist_next_nvpair(anvl, NULL);
		    apair != NULL; apair = nvlist_next_nvpair(anvl, apair)) {
			if (nvpair_type(apair) != DATA_TYPE_STRING ||
			    nvpair_value_string(apair, &aval) != 0)
				continue;
			aname = nvpair_name(apair);
			topo_fmristr_build(&size, buf, buflen, ":", NULL, NULL);
			topo_fmristr_build(&size, buf, buflen, "=",
			    aname, aval);
		}
	}

	/* pkg-name part */
	topo_fmristr_build(&size, buf, buflen, pkgname, "/", NULL);

	return (size);
}

/*ARGSUSED*/
static int
pkg_fmri_nvl2str(topo_mod_t *mod, tnode_t *node, topo_version_t version,
    nvlist_t *nvl, nvlist_t **out)
{
	ssize_t len;
	char *name = NULL;
	nvlist_t *fmristr;

	if (version > TOPO_METH_NVL2STR_VERSION)
		return (topo_mod_seterrno(mod, EMOD_VER_NEW));

	if ((len = fmri_nvl2str(nvl, NULL, 0)) == 0 ||
	    (name = topo_mod_alloc(mod, len + 1)) == NULL ||
	    fmri_nvl2str(nvl, name, len + 1) == 0) {
		if (name != NULL)
			topo_mod_free(mod, name, len + 1);
		return (topo_mod_seterrno(mod, EMOD_FMRI_NVL));
	}

	if (topo_mod_nvalloc(mod, &fmristr, NV_UNIQUE_NAME) != 0)
		return (topo_mod_seterrno(mod, EMOD_FMRI_NVL));
	if (nvlist_add_string(fmristr, "fmri-string", name) != 0) {
		topo_mod_free(mod, name, len + 1);
		nvlist_free(fmristr);
		return (topo_mod_seterrno(mod, EMOD_FMRI_NVL));
	}
	topo_mod_free(mod, name, len + 1);
	*out = fmristr;

	return (0);
}
