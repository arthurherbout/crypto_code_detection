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
 * Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#include <sys/types.h>
#include <sys/processor.h>
#include <fm/fmd_fmri.h>
#include <fm/libtopo.h>

#include <strings.h>
#include <errno.h>
#include <kstat.h>


/*
 * The scheme plugin for cpu FMRIs.
 */

ssize_t
fmd_fmri_nvl2str(nvlist_t *nvl, char *buf, size_t buflen)
{
	int err;
	ssize_t len;
	topo_hdl_t *thp;
	char *str;

	if ((thp = fmd_fmri_topo_hold(TOPO_VERSION)) == NULL)
		return (fmd_fmri_set_errno(EINVAL));
	if (topo_fmri_nvl2str(thp, nvl, &str, &err) != 0) {
		fmd_fmri_topo_rele(thp);
		return (fmd_fmri_set_errno(EINVAL));
	}
	if (buf != NULL)
		len = snprintf(buf, buflen, "%s", str);
	else
		len = strlen(str);
	topo_hdl_strfree(thp, str);
	fmd_fmri_topo_rele(thp);
	return (len);
}

/*
 * Determine if a cpuid is present.
 */
/*ARGSUSED*/
static int
cpu_cpuid_present(uint32_t cpuid)
{
#ifdef	sparc
	/*
	 * For SPARC, use kstats to see if the cpuid is present.
	 * Note that this may need to change for sun4v.
	 */
	kstat_ctl_t *kc;
	kstat_t *ksp = NULL;
	if ((kc = kstat_open()) == NULL)
		return (-1); /* errno is set for us */
	ksp = kstat_lookup(kc, "cpu_info", cpuid, NULL);
	(void) kstat_close(kc);
	return ((ksp == NULL) ? 0 : 1);
#else	/* sparc */
	/*
	 * For x64, just return true.
	 */
	return (1);
#endif	/* sparc */
}

static int
cpu_get_serialid_kstat(uint32_t cpuid, uint64_t *serialidp)
{
	kstat_named_t *kn;
	kstat_ctl_t *kc;
	kstat_t *ksp;
	int i;

	if ((kc = kstat_open()) == NULL)
		return (-1); /* errno is set for us */

	if ((ksp = kstat_lookup(kc, "cpu_info", cpuid, NULL)) == NULL) {
		(void) kstat_close(kc);
		return (fmd_fmri_set_errno(ENOENT));
	}

	if (kstat_read(kc, ksp, NULL) == -1) {
		int oserr = errno;
		(void) kstat_close(kc);
		return (fmd_fmri_set_errno(oserr));
	}

	for (kn = ksp->ks_data, i = 0; i < ksp->ks_ndata; i++, kn++) {
		if (strcmp(kn->name, "device_ID") == 0) {
			*serialidp = kn->value.ui64;
			(void) kstat_close(kc);
			return (0);
		}
	}

	(void) kstat_close(kc);
	return (fmd_fmri_set_errno(ENOENT));
}

static int
cpu_get_serialid_V1(uint32_t cpuid, char *serbuf, size_t len)
{
	int err;
	uint64_t serial = 0;

	err = cpu_get_serialid_kstat(cpuid, &serial);

	(void) snprintf(serbuf, len, "%llX", (u_longlong_t)serial);
	return (err);
}

static int
cpu_get_serialid_V0(uint32_t cpuid, uint64_t *serialidp)
{
	return (cpu_get_serialid_kstat(cpuid, serialidp));
}

int
fmd_fmri_expand(nvlist_t *nvl)
{
	uint8_t version;
	uint32_t cpuid;
	uint64_t serialid;
	char *serstr, serbuf[21]; /* sizeof (UINT64_MAX) + '\0' */
	int rc, err;
	topo_hdl_t *thp;

	if (nvlist_lookup_uint8(nvl, FM_VERSION, &version) != 0 ||
	    nvlist_lookup_uint32(nvl, FM_FMRI_CPU_ID, &cpuid) != 0)
		return (fmd_fmri_set_errno(EINVAL));

	/*
	 * If the cpu-scheme topology exports this method expand(), invoke it.
	 */
	if ((thp = fmd_fmri_topo_hold(TOPO_VERSION)) == NULL)
		return (fmd_fmri_set_errno(EINVAL));

	rc = topo_fmri_expand(thp, nvl, &err);
	fmd_fmri_topo_rele(thp);
	if (err != ETOPO_METHOD_NOTSUP)
		return (rc);

	if (version == CPU_SCHEME_VERSION0) {
		if ((rc = nvlist_lookup_uint64(nvl, FM_FMRI_CPU_SERIAL_ID,
		    &serialid)) != 0) {
			if (rc != ENOENT)
				return (fmd_fmri_set_errno(rc));

			if (cpu_get_serialid_V0(cpuid, &serialid) != 0)
				return (-1); /* errno is set for us */

			if ((rc = nvlist_add_uint64(nvl, FM_FMRI_CPU_SERIAL_ID,
			    serialid)) != 0)
				return (fmd_fmri_set_errno(rc));
		}
	} else if (version == CPU_SCHEME_VERSION1) {
		if ((rc = nvlist_lookup_string(nvl, FM_FMRI_CPU_SERIAL_ID,
		    &serstr)) != 0) {
			if (rc != ENOENT)
				return (fmd_fmri_set_errno(rc));

			if (cpu_get_serialid_V1(cpuid, serbuf, 21) != 0)
				return (0); /* Serial number is optional */

			if ((rc = nvlist_add_string(nvl, FM_FMRI_CPU_SERIAL_ID,
			    serbuf)) != 0)
				return (fmd_fmri_set_errno(rc));
		}
	} else {
		return (fmd_fmri_set_errno(EINVAL));
	}

	return (0);
}

int
fmd_fmri_present(nvlist_t *nvl)
{
	int rc, err;
	uint8_t version;
	uint32_t cpuid;
	uint64_t nvlserid, curserid;
	char *nvlserstr, curserbuf[21]; /* sizeof (UINT64_MAX) + '\0' */
	topo_hdl_t *thp;

	if (nvlist_lookup_uint8(nvl, FM_VERSION, &version) != 0 ||
	    nvlist_lookup_uint32(nvl, FM_FMRI_CPU_ID, &cpuid) != 0)
		return (fmd_fmri_set_errno(EINVAL));

	/*
	 * If the cpu-scheme topology exports this method present(), invoke it.
	 */
	if ((thp = fmd_fmri_topo_hold(TOPO_VERSION)) == NULL)
		return (fmd_fmri_set_errno(EINVAL));
	rc = topo_fmri_present(thp, nvl, &err);
	fmd_fmri_topo_rele(thp);
	if (err != ETOPO_METHOD_NOTSUP)
		return (rc);

	if (version == CPU_SCHEME_VERSION0) {
		if (nvlist_lookup_uint64(nvl, FM_FMRI_CPU_SERIAL_ID,
		    &nvlserid) != 0)
			return (fmd_fmri_set_errno(EINVAL));
		if (cpu_get_serialid_V0(cpuid, &curserid) != 0)
			return (errno == ENOENT ? 0 : -1);

		return (curserid == nvlserid);

	} else if (version == CPU_SCHEME_VERSION1) {
		if ((rc = nvlist_lookup_string(nvl, FM_FMRI_CPU_SERIAL_ID,
		    &nvlserstr)) != 0)
			if (rc != ENOENT)
				return (fmd_fmri_set_errno(EINVAL));

		/*
		 * If serial id is not available, just check if the cpuid
		 * is present.
		 */
		if (cpu_get_serialid_V1(cpuid, curserbuf, 21) != 0)
			return (cpu_cpuid_present(cpuid));

		return (strcmp(curserbuf, nvlserstr) == 0 ? 1 : 0);

	} else {
		return (fmd_fmri_set_errno(EINVAL));
	}
}

int
fmd_fmri_replaced(nvlist_t *nvl)
{
	int rc, err = 0;
	uint8_t version;
	uint32_t cpuid;
	uint64_t nvlserid, curserid;
	char *nvlserstr, curserbuf[21]; /* sizeof (UINT64_MAX) + '\0' */
	topo_hdl_t *thp;

	if (nvlist_lookup_uint8(nvl, FM_VERSION, &version) != 0 ||
	    nvlist_lookup_uint32(nvl, FM_FMRI_CPU_ID, &cpuid) != 0)
		return (fmd_fmri_set_errno(EINVAL));

	/*
	 * If the cpu-scheme topology exports this method replaced(), invoke it.
	 */
	if ((thp = fmd_fmri_topo_hold(TOPO_VERSION)) == NULL)
		return (fmd_fmri_set_errno(EINVAL));
	rc = topo_fmri_replaced(thp, nvl, &err);
	fmd_fmri_topo_rele(thp);
	if (err != ETOPO_METHOD_NOTSUP)
		return (rc);

	if (version == CPU_SCHEME_VERSION0) {
		if (nvlist_lookup_uint64(nvl, FM_FMRI_CPU_SERIAL_ID,
		    &nvlserid) != 0)
			return (fmd_fmri_set_errno(EINVAL));
		if (cpu_get_serialid_V0(cpuid, &curserid) != 0)
			return (errno == ENOENT ?
			    FMD_OBJ_STATE_NOT_PRESENT : -1);

		return (curserid == nvlserid ? FMD_OBJ_STATE_STILL_PRESENT :
		    FMD_OBJ_STATE_REPLACED);

	} else if (version == CPU_SCHEME_VERSION1) {
		if ((rc = nvlist_lookup_string(nvl, FM_FMRI_CPU_SERIAL_ID,
		    &nvlserstr)) != 0)
			if (rc != ENOENT)
				return (fmd_fmri_set_errno(EINVAL));

		/*
		 * If serial id is not available, just check if the cpuid
		 * is present.
		 */
		if (cpu_get_serialid_V1(cpuid, curserbuf, 21) != 0)
			if (cpu_cpuid_present(cpuid))
				return (FMD_OBJ_STATE_UNKNOWN);
			else
				return (FMD_OBJ_STATE_NOT_PRESENT);

		return (strcmp(curserbuf, nvlserstr) == 0 ?
		    FMD_OBJ_STATE_STILL_PRESENT : FMD_OBJ_STATE_REPLACED);

	} else {
		return (fmd_fmri_set_errno(EINVAL));
	}
}

int
fmd_fmri_unusable(nvlist_t *nvl)
{
	int rc, err = 0;
	uint8_t version;
	uint32_t cpuid;
	topo_hdl_t *thp;

	if (nvlist_lookup_uint8(nvl, FM_VERSION, &version) != 0 ||
	    version > FM_CPU_SCHEME_VERSION ||
	    nvlist_lookup_uint32(nvl, FM_FMRI_CPU_ID, &cpuid) != 0)
		return (fmd_fmri_set_errno(EINVAL));

	/*
	 * If the cpu-scheme topology exports this method unusable(), invoke it.
	 */
	if ((thp = fmd_fmri_topo_hold(TOPO_VERSION)) == NULL)
		return (fmd_fmri_set_errno(EINVAL));
	rc = topo_fmri_unusable(thp, nvl, &err);
	fmd_fmri_topo_rele(thp);
	if (err != ETOPO_METHOD_NOTSUP)
		return (rc);

	return (p_online(cpuid, P_STATUS) == P_FAULTED);
}
int
fmd_fmri_contains(nvlist_t *er, nvlist_t *ee)
{
	int ret1, ret2;
	char *erserstr, *eeserstr;
	uint8_t  erversion, eeversion;
	uint64_t erserint, eeserint;
	uint32_t erval, eeval;

	if (nvlist_lookup_uint32(er, FM_FMRI_CPU_ID, &erval) != 0)
		return (0);
	if (nvlist_lookup_uint32(ee, FM_FMRI_CPU_ID, &eeval) != 0)
		return (0);
	if (erval != eeval)
		return (0);

	if (nvlist_lookup_uint8(er, FM_VERSION, &erversion) != 0)
		return (0);

	if (nvlist_lookup_uint8(ee, FM_VERSION, &eeversion) != 0)
		return (0);

	if (erversion != eeversion)
		return (0);

	if (erversion == CPU_SCHEME_VERSION0) {
		if (nvlist_lookup_uint64(er, FM_FMRI_CPU_SERIAL_ID,
		    &erserint) != 0)
			return (0);
		if (nvlist_lookup_uint64(ee, FM_FMRI_CPU_SERIAL_ID,
		    &eeserint) != 0)
			return (0);
		if (erserint != eeserint)
			return (0);
	} else if (erversion == CPU_SCHEME_VERSION1) {
		/* Serial ID is an optional element */
		ret1 = nvlist_lookup_string(er, FM_FMRI_CPU_SERIAL_ID,
		    &erserstr);
		ret2 = nvlist_lookup_string(ee, FM_FMRI_CPU_SERIAL_ID,
		    &eeserstr);
		if (ret1 != ret2)
			return (0);
		if (ret1 == ENOENT)
			/*
			 * Serial IDs not found in both container, containee
			 */
			return (1);
		if (ret1 != 0)
			return (0);
		/*
		 * Found Serial Ids in both container and containee.
		 * Check if they are same.
		 */
		if (strlen(erserstr) != strlen(eeserstr))
			return (0);
		if (strcmp(erserstr, eeserstr) != 0)
			return (0);
	}
	return (1);
}
