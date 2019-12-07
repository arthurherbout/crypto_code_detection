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
 * Copyright (c) 2009, 2010, Oracle and/or its affiliates. All rights reserved.
 * Copyright 2018 Nexenta Systems, Inc.  All rights reserved.
 */

/*
 * Volume Copy Shadow Services (VSS) provides a way for users to
 * restore/recover deleted files/directories.
 * For the server to support VSS for Microsoft clients, there is
 * two basic functions that need to be implemented.
 * The first is to intercept the NT_TRANSACT_IOCTL command with
 * the function code of FSCTL_SRV_ENUMERATE_SNAPSHOTS (0x00144064).
 * This is to report the count or the count and list of snapshots
 * for that share.
 * The second function need to trap commands with the
 * SMB_FLAGS2_REPARSE_PATH bit set in the smb header.  This bit
 * means that there is a @GMT token in path that needs to be
 * processed.  The @GMT token means to process this command, but
 * in the snapshot.
 */

#include <smbsrv/smb_kproto.h>
#include <smbsrv/string.h>
#include <smb/winioctl.h>
#include <smbsrv/smb_door.h>

/* Size of the token on the wire due to encoding */
#define	SMB_VSS_GMT_NET_SIZE(sr) (smb_ascii_or_unicode_null_len(sr) * \
    SMB_VSS_GMT_SIZE)

#define	SMB_VSS_COUNT_SIZE 16

static boolean_t smb_vss_is_gmttoken(const char *);
static const char *smb_vss_find_gmttoken(const char *);
static uint32_t smb_vss_encode_gmttokens(smb_request_t *, smb_fsctl_t *,
    int32_t, smb_gmttoken_response_t *);
static void smb_vss_remove_first_token_from_path(char *);

static uint32_t smb_vss_get_count(smb_tree_t *, char *);
static void smb_vss_map_gmttoken(smb_tree_t *, char *, char *, time_t, char *);
static void smb_vss_get_snapshots(smb_tree_t *, char *,
    uint32_t, smb_gmttoken_response_t *);
static void smb_vss_get_snapshots_free(smb_gmttoken_response_t *);
static int smb_vss_lookup_node(smb_request_t *sr, smb_node_t *, vnode_t *,
    char *, smb_node_t *, smb_node_t **);

/*
 * This is to respond to the nt_transact_ioctl to either respond with the
 * number of snapshots, or to respond with the list.  It needs to be sorted
 * before the reply.  If the the max data bytes to return is
 * SMB_VSS_COUNT_SIZE, then all that is requested is the count, otherwise
 * return the count and the list of @GMT tokens (one token for each
 * snapshot).
 */
uint32_t
smb_vss_enum_snapshots(smb_request_t *sr, smb_fsctl_t *fsctl)
{
	uint32_t count = 0;
	char *root_path;
	uint32_t status = NT_STATUS_SUCCESS;
	smb_gmttoken_response_t snaps;

	ASSERT(sr->fid_ofile);
	ASSERT(sr->fid_ofile->f_node);

	if (fsctl->MaxOutputResp < SMB_VSS_COUNT_SIZE)
		return (NT_STATUS_INVALID_PARAMETER);

	/*
	 * smbd will find the root of the lowest filesystem from mntpath of a
	 * file by comparing it agaisnt mnttab, repeatedly removing components
	 * until one matches.
	 */
	root_path  = kmem_zalloc(MAXPATHLEN, KM_SLEEP);
	if (smb_node_getmntpath(sr->fid_ofile->f_node, root_path,
	    MAXPATHLEN) != 0)
		return (NT_STATUS_INVALID_PARAMETER);

	if (fsctl->MaxOutputResp == SMB_VSS_COUNT_SIZE) {
		count = smb_vss_get_count(sr->tid_tree, root_path);
		if (smb_mbc_encodef(fsctl->out_mbc, "lllw", count, 0,
		    (count * SMB_VSS_GMT_NET_SIZE(sr) +
		    smb_ascii_or_unicode_null_len(sr)), 0) != 0) {
			status = NT_STATUS_INVALID_PARAMETER;
		}
	} else {
		count = fsctl->MaxOutputResp / SMB_VSS_GMT_NET_SIZE(sr);

		smb_vss_get_snapshots(sr->tid_tree, root_path,
		    count, &snaps);

		status = smb_vss_encode_gmttokens(sr, fsctl, count, &snaps);

		smb_vss_get_snapshots_free(&snaps);
	}

	kmem_free(root_path, MAXPATHLEN);
	return (status);
}

/*
 * sr - the request info, used to find root of dataset,
 *      unicode or ascii, where the share is rooted in the
 *      dataset
 * cur_node - where in the share for the command
 * vss_cur_node - returned value for the snapshot version
 *                of the cur_node
 * gmttoken - if SMB1, the gmttoken to be used to find the snapshot.
 *            Otherwise, NULL.
 *
 * This routine is the processing for handling the
 * SMB_FLAGS2_REPARSE_PATH bit being set in the smb header.
 *
 * By using the cur_node passed in, a new node is found or
 * created that is the same place in the directory tree, but
 * in the snapshot.
 */
int
smb_vss_lookup_nodes(smb_request_t *sr, smb_node_t *cur_node,
    smb_node_t **vss_cur_node, char *gmttoken)
{
	smb_arg_open_t	*op = &sr->arg.open;
	char		*snapname, *path;
	vnode_t		*fsrootvp = NULL;
	time_t		toktime;
	int		err = 0;

	if (sr->tid_tree == NULL)
		return (ESTALE);

	if (gmttoken != NULL) {
		toktime = 0;
	} else {
		/* SMB2 and later */
		toktime = op->timewarp.tv_sec;
	}

	path = smb_srm_alloc(sr, MAXPATHLEN);
	snapname = smb_srm_alloc(sr, MAXPATHLEN);

	err = smb_node_getmntpath(cur_node, path, MAXPATHLEN);
	if (err != 0)
		return (err);

	/*
	 * Find the corresponding snapshot name.  If snapname is
	 * empty after the map call, no such snapshot was found.
	 */
	*snapname = '\0';
	smb_vss_map_gmttoken(sr->tid_tree, path, gmttoken, toktime,
	    snapname);
	if (*snapname == '\0')
		return (ENOENT);

	/* find snapshot nodes */
	err = VFS_ROOT(cur_node->vp->v_vfsp, &fsrootvp);
	if (err != 0)
		return (err);

	/* find snapshot node corresponding to cur_node */
	err = smb_vss_lookup_node(sr, cur_node, fsrootvp,
	    snapname, cur_node, vss_cur_node);

	VN_RELE(fsrootvp);

	return (err);
}

/*
 * Find snapshot node corresponding to 'node', and return it in
 * 'vss_node', as follows:
 * - find the path from fsrootvp to node, appending it to the
 *   the snapshot path
 * - lookup the vnode and smb_node (vss_node).
 */
static int
smb_vss_lookup_node(smb_request_t *sr, smb_node_t *node, vnode_t *fsrootvp,
    char *snapname, smb_node_t *dnode, smb_node_t **vss_node)
{
	char *p, *path;
	int err, len;
	vnode_t *vp = NULL;

	*vss_node = NULL;

	path = kmem_alloc(MAXPATHLEN, KM_SLEEP);
	(void) snprintf(path, MAXPATHLEN, ".zfs/snapshot/%s/", snapname);
	len = strlen(path);
	p = path + len;

	err = smb_node_getpath(node, fsrootvp, p, MAXPATHLEN - len);
	if (err == 0) {
		vp = smb_lookuppathvptovp(sr, path, fsrootvp, fsrootvp);
		if (vp) {
			*vss_node = smb_node_lookup(sr, NULL, zone_kcred(),
			    vp, snapname, dnode, NULL);
			VN_RELE(vp);
		}
	}

	kmem_free(path, MAXPATHLEN);

	if (*vss_node != NULL)
		return (0);

	return (err ? err : ENOENT);
}


static boolean_t
smb_vss_is_gmttoken(const char *s)
{
	char *t = "@GMT-NNNN.NN.NN-NN.NN.NN";
	const char *str;
	char *template;

	template = t;
	str = s;

	while (*template) {
		if (*template == 'N') {
			if (!smb_isdigit(*str))
				return (B_FALSE);
		} else if (*template != *str) {
			return (B_FALSE);
		}

		template++;
		str++;
	}

	/* Make sure it is JUST the @GMT token */
	if ((*str == '\0') || (*str == '/'))
		return (B_TRUE);

	return (B_FALSE);
}

static const char *
smb_vss_find_gmttoken(const char *path)
{
	const char *p;

	p = path;

	while (*p) {
		if (*p == '@' && smb_vss_is_gmttoken(p))
			return (p);
		p++;
	}
	return (NULL);
}

static uint32_t
smb_vss_encode_gmttokens(smb_request_t *sr, smb_fsctl_t *fsctl,
    int32_t count, smb_gmttoken_response_t *snap_data)
{
	uint32_t i;
	uint32_t returned_count;
	uint32_t num_gmttokens;
	char **gmttokens;
	uint32_t status = NT_STATUS_SUCCESS;
	uint32_t data_size;

	returned_count = snap_data->gtr_count;
	num_gmttokens = snap_data->gtr_gmttokens.gtr_gmttokens_len;
	gmttokens = snap_data->gtr_gmttokens.gtr_gmttokens_val;

	if (returned_count > count)
		status = NT_STATUS_BUFFER_TOO_SMALL;

	data_size = returned_count * SMB_VSS_GMT_NET_SIZE(sr) +
	    smb_ascii_or_unicode_null_len(sr);

	if (smb_mbc_encodef(fsctl->out_mbc, "lll", returned_count,
	    num_gmttokens, data_size) != 0)
		return (NT_STATUS_INVALID_PARAMETER);

	if (status == NT_STATUS_SUCCESS) {
		for (i = 0; i < num_gmttokens; i++) {
			if (smb_mbc_encodef(fsctl->out_mbc, "%u", sr,
			    *gmttokens) != 0)
				status = NT_STATUS_INVALID_PARAMETER;
			gmttokens++;
		}
	}

	return (status);
}

/* This removes the first @GMT from the path */
static void
smb_vss_remove_first_token_from_path(char *path)
{
	boolean_t found;
	char *src, *dest;

	src = path;
	dest = path;

	found = B_FALSE;

	while (*src != '\0') {
		if (!found && smb_vss_is_gmttoken(src)) {
			src += SMB_VSS_GMT_SIZE - 1;
			if (*src == '/')
				src += 1;
			found = B_TRUE;
			continue;
		}
		*dest = *src;
		src++;
		dest++;
	}
	*dest = *src;
}

/*
 * This returns the number of snapshots for the dataset
 * of the path provided.
 */
static uint32_t
smb_vss_get_count(smb_tree_t *tree, char *resource_path)
{
	uint32_t	count = 0;
	int		rc;
	smb_string_t	path;

	path.buf = resource_path;

	rc = smb_kdoor_upcall(tree->t_server, SMB_DR_VSS_GET_COUNT,
	    &path, smb_string_xdr, &count, xdr_uint32_t);

	if (rc != 0)
		count = 0;

	return (count);
}

/*
 * This takes a path for the root of the dataset and gets the counts of
 * snapshots for that dataset and the list of @GMT tokens (one for each
 * snapshot) up to the count provided.
 *
 * Call smb_vss_get_snapshots_free after to free up the data.
 */
static void
smb_vss_get_snapshots(smb_tree_t *tree, char *resource_path,
    uint32_t count, smb_gmttoken_response_t *gmttokens)
{
	smb_gmttoken_query_t	request;

	request.gtq_count = count;
	request.gtq_path = resource_path;
	bzero(gmttokens, sizeof (smb_gmttoken_response_t));

	(void) smb_kdoor_upcall(tree->t_server, SMB_DR_VSS_GET_SNAPSHOTS,
	    &request, smb_gmttoken_query_xdr,
	    gmttokens, smb_gmttoken_response_xdr);
}

static void
smb_vss_get_snapshots_free(smb_gmttoken_response_t *reply)
{
	xdr_free(smb_gmttoken_response_xdr, (char *)reply);
}

/*
 * Returns the snapshot name for the @GMT token provided for the dataset
 * of the path.  If the snapshot cannot be found, a string with a NULL
 * is returned.
 */
static void
smb_vss_map_gmttoken(smb_tree_t *tree, char *path, char *gmttoken,
    time_t toktime, char *snapname)
{
	smb_gmttoken_snapname_t	request;
	smb_string_t		result;

	bzero(&result, sizeof (smb_string_t));
	result.buf = snapname;

	request.gts_path = path;
	request.gts_gmttoken = gmttoken;
	request.gts_toktime = toktime;

	(void) smb_kdoor_upcall(tree->t_server, SMB_DR_VSS_MAP_GMTTOKEN,
	    &request, smb_gmttoken_snapname_xdr,
	    &result, smb_string_xdr);
}

int
smb_vss_extract_gmttoken(char *buf, char *gmttoken)
{
	const char *p;

	/* get gmttoken from buf */
	if ((p = smb_vss_find_gmttoken(buf)) == NULL)
		return (ENOENT);

	bcopy(p, gmttoken, SMB_VSS_GMT_SIZE);
	gmttoken[SMB_VSS_GMT_SIZE - 1] = '\0';

	smb_vss_remove_first_token_from_path(buf);

	return (0);
}
