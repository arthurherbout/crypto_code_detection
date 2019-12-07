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
 * Copyright (c) 2007, 2010, Oracle and/or its affiliates. All rights reserved.
 * Copyright 2018 Nexenta Systems, Inc.  All rights reserved.
 */

/*
 * SMB server interface to idmap
 * (smb_idmap_get..., smb_idmap_batch_...)
 *
 * There are three implementations of this interface.
 * This is the libsmb version of these routines.  See also:
 * $SRC/uts/common/fs/smbsrv/smb_idmap.c
 * $SRC/lib/smbsrv/libfksmbsrv/common/fksmb_idmap.c
 *
 * There are enough differences (relative to the code size)
 * that it's more trouble than it's worth to merge them.
 *
 * This one differs from the others in that it:
 *	calls idmap interfaces (libidmap)
 *	domain SIDs returned are allocated
 */

#include <syslog.h>
#include <strings.h>
#include <smbsrv/libsmb.h>

static int smb_idmap_batch_binsid(smb_idmap_batch_t *sib);

/*
 * Report an idmap error.
 */
void
smb_idmap_check(const char *s, idmap_stat stat)
{
	if (stat != IDMAP_SUCCESS) {
		if (s == NULL)
			s = "smb_idmap_check";

		syslog(LOG_ERR, "%s: %s", s, idmap_stat2string(stat));
	}
}

/*
 * smb_idmap_getsid
 *
 * Tries to get a mapping for the given uid/gid
 * Allocates ->sim_domsid
 */
idmap_stat
smb_idmap_getsid(uid_t id, int idtype, smb_sid_t **sid)
{
	smb_idmap_batch_t sib;
	idmap_stat stat;

	stat = smb_idmap_batch_create(&sib, 1, SMB_IDMAP_ID2SID);
	if (stat != IDMAP_SUCCESS)
		return (stat);

	stat = smb_idmap_batch_getsid(sib.sib_idmaph, &sib.sib_maps[0],
	    id, idtype);

	if (stat != IDMAP_SUCCESS) {
		smb_idmap_batch_destroy(&sib);
		return (stat);
	}

	stat = smb_idmap_batch_getmappings(&sib);

	if (stat != IDMAP_SUCCESS) {
		smb_idmap_batch_destroy(&sib);
		return (stat);
	}

	*sid = smb_sid_dup(sib.sib_maps[0].sim_sid);

	smb_idmap_batch_destroy(&sib);

	return (IDMAP_SUCCESS);
}

/*
 * smb_idmap_getid
 *
 * Tries to get a mapping for the given SID
 */
idmap_stat
smb_idmap_getid(smb_sid_t *sid, uid_t *id, int *id_type)
{
	smb_idmap_batch_t sib;
	smb_idmap_t *sim;
	idmap_stat stat;

	stat = smb_idmap_batch_create(&sib, 1, SMB_IDMAP_SID2ID);
	if (stat != IDMAP_SUCCESS)
		return (stat);

	sim = &sib.sib_maps[0];
	sim->sim_id = id;
	stat = smb_idmap_batch_getid(sib.sib_idmaph, sim, sid, *id_type);
	if (stat != IDMAP_SUCCESS) {
		smb_idmap_batch_destroy(&sib);
		return (stat);
	}

	stat = smb_idmap_batch_getmappings(&sib);

	if (stat != IDMAP_SUCCESS) {
		smb_idmap_batch_destroy(&sib);
		return (stat);
	}

	*id_type = sim->sim_idtype;
	smb_idmap_batch_destroy(&sib);

	return (IDMAP_SUCCESS);
}

/*
 * smb_idmap_batch_create
 *
 * Creates and initializes the context for batch ID mapping.
 */
idmap_stat
smb_idmap_batch_create(smb_idmap_batch_t *sib, uint16_t nmap, int flags)
{
	idmap_stat	stat;

	if (!sib)
		return (IDMAP_ERR_ARG);

	bzero(sib, sizeof (smb_idmap_batch_t));
	stat = idmap_get_create(&sib->sib_idmaph);

	if (stat != IDMAP_SUCCESS) {
		smb_idmap_check("idmap_get_create", stat);
		return (stat);
	}

	sib->sib_flags = flags;
	sib->sib_nmap = nmap;
	sib->sib_size = nmap * sizeof (smb_idmap_t);
	sib->sib_maps = malloc(sib->sib_size);
	if (!sib->sib_maps)
		return (IDMAP_ERR_MEMORY);

	bzero(sib->sib_maps, sib->sib_size);
	return (IDMAP_SUCCESS);
}

/*
 * smb_idmap_batch_destroy
 *
 * Frees the batch ID mapping context.
 */
void
smb_idmap_batch_destroy(smb_idmap_batch_t *sib)
{
	int i;

	if (sib == NULL)
		return;

	if (sib->sib_idmaph) {
		idmap_get_destroy(sib->sib_idmaph);
		sib->sib_idmaph = NULL;
	}

	if (sib->sib_maps == NULL)
		return;

	if (sib->sib_flags & SMB_IDMAP_ID2SID) {
		/*
		 * SIDs are allocated only when mapping
		 * UID/GID to SIDs
		 */
		for (i = 0; i < sib->sib_nmap; i++) {
			smb_sid_free(sib->sib_maps[i].sim_sid);
			free(sib->sib_maps[i].sim_domsid);
		}
	} else if (sib->sib_flags & SMB_IDMAP_SID2ID) {
		/*
		 * SID prefixes are allocated only when mapping
		 * SIDs to UID/GID
		 */
		for (i = 0; i < sib->sib_nmap; i++) {
			free(sib->sib_maps[i].sim_domsid);
		}
	}

	if (sib->sib_size && sib->sib_maps) {
		free(sib->sib_maps);
		sib->sib_maps = NULL;
	}
}

/*
 * smb_idmap_batch_getid
 *
 * Queue a request to map the given SID to a UID or GID.
 *
 * sim->sim_id should point to variable that's supposed to
 * hold the returned UID/GID. This needs to be setup by caller
 * of this function.
 * If requested ID type is known, it's passed as 'idtype',
 * if it's unknown it'll be returned in sim->sim_idtype.
 */
idmap_stat
smb_idmap_batch_getid(idmap_get_handle_t *idmaph, smb_idmap_t *sim,
    smb_sid_t *sid, int idtype)
{
	char sidstr[SMB_SID_STRSZ];
	idmap_stat stat;
	int flag = 0;

	if (idmaph == NULL || sim == NULL || sid == NULL)
		return (IDMAP_ERR_ARG);

	smb_sid_tostr(sid, sidstr);
	if (smb_sid_splitstr(sidstr, &sim->sim_rid) != 0)
		return (IDMAP_ERR_SID);
	/* Note: Free sim_domsid in smb_idmap_batch_destroy */
	sim->sim_domsid = strdup(sidstr);
	sim->sim_idtype = idtype;

	switch (idtype) {
	case SMB_IDMAP_USER:
		stat = idmap_get_uidbysid(idmaph, sim->sim_domsid,
		    sim->sim_rid, flag, sim->sim_id, &sim->sim_stat);
		smb_idmap_check("idmap_get_uidbysid", stat);
		break;

	case SMB_IDMAP_GROUP:
		stat = idmap_get_gidbysid(idmaph, sim->sim_domsid,
		    sim->sim_rid, flag, sim->sim_id, &sim->sim_stat);
		smb_idmap_check("idmap_get_gidbysid", stat);
		break;

	case SMB_IDMAP_UNKNOWN:
		stat = idmap_get_pidbysid(idmaph, sim->sim_domsid,
		    sim->sim_rid, flag, sim->sim_id, &sim->sim_idtype,
		    &sim->sim_stat);
		smb_idmap_check("idmap_get_pidbysid", stat);
		break;

	default:
		stat = IDMAP_ERR_ARG;
		break;
	}

	return (stat);
}

/*
 * smb_idmap_batch_getsid
 *
 * Queue a request to map the given UID/GID to a SID.
 *
 * sim->sim_domsid and sim->sim_rid will contain the mapping
 * result upon successful process of the batched request.
 * Stash the type for error reporting (caller saves the ID).
 *
 * NB: sim_domsid allocated by strdup, here or in libidmap
 */
idmap_stat
smb_idmap_batch_getsid(idmap_get_handle_t *idmaph, smb_idmap_t *sim,
    uid_t id, int idtype)
{
	idmap_stat stat;
	int flag = 0;

	if (!idmaph || !sim)
		return (IDMAP_ERR_ARG);

	sim->sim_idtype = idtype;
	switch (idtype) {
	case SMB_IDMAP_USER:
		stat = idmap_get_sidbyuid(idmaph, id, flag,
		    &sim->sim_domsid, &sim->sim_rid, &sim->sim_stat);
		smb_idmap_check("idmap_get_sidbyuid", stat);
		break;

	case SMB_IDMAP_GROUP:
		stat = idmap_get_sidbygid(idmaph, id, flag,
		    &sim->sim_domsid, &sim->sim_rid, &sim->sim_stat);
		smb_idmap_check("idmap_get_sidbygid", stat);
		break;

	case SMB_IDMAP_OWNERAT:
		/* Current Owner S-1-5-32-766 */
		sim->sim_domsid = strdup(NT_BUILTIN_DOMAIN_SIDSTR);
		sim->sim_rid = SECURITY_CURRENT_OWNER_RID;
		sim->sim_stat = IDMAP_SUCCESS;
		stat = IDMAP_SUCCESS;
		break;

	case SMB_IDMAP_GROUPAT:
		/* Current Group S-1-5-32-767 */
		sim->sim_domsid = strdup(NT_BUILTIN_DOMAIN_SIDSTR);
		sim->sim_rid = SECURITY_CURRENT_GROUP_RID;
		sim->sim_stat = IDMAP_SUCCESS;
		stat = IDMAP_SUCCESS;
		break;

	case SMB_IDMAP_EVERYONE:
		/* Everyone S-1-1-0 */
		sim->sim_domsid = strdup(NT_WORLD_AUTH_SIDSTR);
		sim->sim_rid = 0;
		sim->sim_stat = IDMAP_SUCCESS;
		stat = IDMAP_SUCCESS;
		break;

	default:
		return (IDMAP_ERR_ARG);
	}

	return (stat);
}

static void
smb_idmap_bgm_report(smb_idmap_batch_t *sib, smb_idmap_t *sim)
{

	if ((sib->sib_flags & SMB_IDMAP_ID2SID) != 0) {
		/*
		 * Note: The ID and type we asked idmap to map
		 * were saved in *sim_id and sim_idtype.
		 */
		uint_t id = (sim->sim_id == NULL) ?
		    0 : (uint_t)*sim->sim_id;
		syslog(LOG_ERR, "Can't get SID for "
		    "ID=%u type=%d, status=%d",
		    id, sim->sim_idtype, sim->sim_stat);
	}

	if ((sib->sib_flags & SMB_IDMAP_SID2ID) != 0) {
		syslog(LOG_ERR, "Can't get ID for SID %s-%u, status=%d",
		    sim->sim_domsid, sim->sim_rid, sim->sim_stat);
	}
}

/*
 * smb_idmap_batch_getmappings
 *
 * trigger ID mapping service to get the mappings for queued
 * requests.
 *
 * Checks the result of all the queued requests.
 */
idmap_stat
smb_idmap_batch_getmappings(smb_idmap_batch_t *sib)
{
	idmap_stat stat = IDMAP_SUCCESS;
	smb_idmap_t *sim;
	int i;

	if ((stat = idmap_get_mappings(sib->sib_idmaph)) != IDMAP_SUCCESS) {
		smb_idmap_check("idmap_get_mappings", stat);
		return (stat);
	}

	/*
	 * Check the status for all the queued requests
	 */
	for (i = 0, sim = sib->sib_maps; i < sib->sib_nmap; i++, sim++) {
		if (sim->sim_stat != IDMAP_SUCCESS) {
			smb_idmap_bgm_report(sib, sim);
			if ((sib->sib_flags & SMB_IDMAP_SKIP_ERRS) == 0) {
				return (sim->sim_stat);
			}
		}
	}

	if (smb_idmap_batch_binsid(sib) != 0)
		stat = IDMAP_ERR_OTHER;

	return (stat);
}

/*
 * smb_idmap_batch_binsid
 *
 * Convert sidrids to binary sids
 *
 * Returns 0 if successful and non-zero upon failure.
 */
static int
smb_idmap_batch_binsid(smb_idmap_batch_t *sib)
{
	smb_sid_t *sid;
	smb_idmap_t *sim;
	int i;

	if (sib->sib_flags & SMB_IDMAP_SID2ID)
		/* This operation is not required */
		return (0);

	sim = sib->sib_maps;
	for (i = 0; i < sib->sib_nmap; sim++, i++) {
		if (sim->sim_domsid == NULL)
			return (-1);

		sid = smb_sid_fromstr(sim->sim_domsid);
		if (sid == NULL)
			return (-1);

		sim->sim_sid = smb_sid_splice(sid, sim->sim_rid);
		smb_sid_free(sid);
	}

	return (0);
}
