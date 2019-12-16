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
 * Copyright 2014 Nexenta Systems, Inc.  All rights reserved.
 * Copyright 2019 Joyent, Inc.
 */

#include <sys/types.h>
#include <sys/conf.h>
#include <sys/ddi.h>
#include <sys/modctl.h>
#include <sys/cred.h>
#include <sys/disp.h>
#include <sys/ioccom.h>
#include <sys/policy.h>
#include <sys/cmn_err.h>
#include <smbsrv/smb_kproto.h>
#include <smbsrv/smb_ioctl.h>

#ifdef	_FAKE_KERNEL
#error	"See libfksmbsrv"
#endif	/* _FAKE_KERNEL */

static int smb_drv_open(dev_t *, int, int, cred_t *);
static int smb_drv_close(dev_t, int, int, cred_t *);
static int smb_drv_ioctl(dev_t, int, intptr_t, int, cred_t *, int *);
static int smb_drv_attach(dev_info_t *, ddi_attach_cmd_t);
static int smb_drv_detach(dev_info_t *, ddi_detach_cmd_t);
static int smb_drv_getinfo(dev_info_t *, ddi_info_cmd_t, void *, void **);

/*
 * *****************************************************************************
 * ****************************** Global Variables *****************************
 * *****************************************************************************
 *
 * These variables can only be changed through the /etc/system file.
 */

/*
 * Maximum buffer size for NT: configurable based on the client environment.
 * IR104720 Experiments with Windows 2000 indicate that we achieve better
 * SmbWriteX performance with a buffer size of 64KB instead of the 37KB used
 * with Windows NT4.0. Previous experiments with NT4.0 resulted in directory
 * listing problems so this buffer size is configurable based on the end-user
 * environment. When in doubt use 37KB.
 */
int	smb_maxbufsize = SMB_NT_MAXBUF;
int	smb_flush_required = 1;
int	smb_dirsymlink_enable = 1;
int	smb_sign_debug = 0;
int	smb_shortnames = 1;
uint_t	smb_audit_flags =
#ifdef	DEBUG
    SMB_AUDIT_NODE;
#else
    0;
#endif

int smb_allow_advisory_locks = 0;	/* See smb_vops.c */

/*
 * Maximum number of simultaneous authentication, share mapping, pipe open
 * requests to be processed.
 */
int	smb_ssetup_threshold = SMB_AUTHSVC_MAXTHREAD;
int	smb_tcon_threshold = 1024;
int	smb_opipe_threshold = 1024;

/*
 * Number of milliseconds that a request will be stalled if it comes in after
 * the maximum number of inflight operations are being proccessed.
 */
int	smb_ssetup_timeout = (30 * 1000);
int	smb_tcon_timeout = (30 * 1000);
int	smb_opipe_timeout = (30 * 1000);

/*
 * Thread priorities used in smbsrv.  Our threads spend most of their time
 * blocked on various conditions.  However, if the system gets heavy load,
 * the scheduler has to choose an order to run these.  We want the order:
 * (a) timers, (b) notifications, (c) workers, (d) receivers (and etc.)
 * where notifications are oplock and change notify work.  Aside from this
 * relative ordering, smbsrv threads should run with a priority close to
 * that of normal user-space threads (thus minclsyspri below), just like
 * NFS and other "file service" kinds of processing.
 */
int smbsrv_base_pri	= MINCLSYSPRI;
int smbsrv_listen_pri	= MINCLSYSPRI;
int smbsrv_receive_pri	= MINCLSYSPRI;
int smbsrv_worker_pri	= MINCLSYSPRI + 1;
int smbsrv_notify_pri	= MINCLSYSPRI + 2;
int smbsrv_timer_pri	= MINCLSYSPRI + 5;


/*
 * *****************************************************************************
 * ********************** Static Variables / Module Linkage ********************
 * *****************************************************************************
 */

static struct cb_ops cbops = {
	smb_drv_open,		/* cb_open */
	smb_drv_close,		/* cb_close */
	nodev,			/* cb_strategy */
	nodev,			/* cb_print */
	nodev,			/* cb_dump */
	nodev,			/* cb_read */
	nodev,			/* cb_write */
	smb_drv_ioctl,		/* cb_ioctl */
	nodev,			/* cb_devmap */
	nodev,			/* cb_mmap */
	nodev,			/* cb_segmap */
	nochpoll,		/* cb_chpoll */
	ddi_prop_op,		/* cb_prop_op */
	NULL,			/* cb_streamtab */
	D_MP,			/* cb_flag */
	CB_REV,			/* cb_rev */
	nodev,			/* cb_aread */
	nodev,			/* cb_awrite */
};

static struct dev_ops devops = {
	DEVO_REV,		/* devo_rev */
	0,			/* devo_refcnt */
	smb_drv_getinfo,	/* devo_getinfo */
	nulldev,		/* devo_identify */
	nulldev,		/* devo_probe */
	smb_drv_attach,		/* devo_attach */
	smb_drv_detach,		/* devo_detach */
	nodev,			/* devo_reset */
	&cbops,			/* devo_cb_ops */
	NULL,			/* devo_bus_ops */
	NULL,			/* devo_power */
	ddi_quiesce_not_needed,		/* devo_quiesce */
};

static struct modldrv modldrv = {
	&mod_driverops,					/* drv_modops */
	"CIFS Server Protocol",				/* drv_linkinfo */
	&devops,
};

static struct modlinkage modlinkage = {
	MODREV_1,	/* revision of the module, must be: MODREV_1	*/
	&modldrv,	/* ptr to linkage structures			*/
	NULL,
};

static dev_info_t *smb_drv_dip = NULL;

/*
 * ****************************************************************************
 *				    Module Interface
 * ****************************************************************************
 */

int
_init(void)
{
	int rc;

	if ((rc = smb_server_g_init()) != 0) {
		return (rc);
	}

	if ((rc = mod_install(&modlinkage)) != 0) {
		smb_server_g_fini();
	}

	return (rc);
}

int
_info(struct modinfo *modinfop)
{
	return (mod_info(&modlinkage, modinfop));
}

int
_fini(void)
{
	int	rc;

	if (smb_server_get_count() != 0)
		return (EBUSY);

	if ((rc = mod_remove(&modlinkage)) == 0) {
		smb_server_g_fini();
	}

	return (rc);
}

/*
 * ****************************************************************************
 *				Pseudo Device Entry Points
 * ****************************************************************************
 */
/* ARGSUSED */
static int
smb_drv_open(dev_t *devp, int flag, int otyp, cred_t *cr)
{
	zoneid_t zid;

	/*
	 * Check caller's privileges.
	 */
	if (secpolicy_smb(cr) != 0)
		return (EPERM);

	/*
	 * We need a unique minor per zone otherwise an smbd in any other
	 * zone will keep this minor open and we won't get a close call.
	 * The zone ID is good enough as a minor number.
	 */
	zid = crgetzoneid(cr);
	if (zid < 0)
		return (ENODEV);
	*devp = makedevice(getmajor(*devp), zid);

	/*
	 * Start SMB service state machine
	 */
	return (smb_server_create());
}

/* ARGSUSED */
static int
smb_drv_close(dev_t dev, int flag, int otyp, cred_t *credp)
{
	smb_server_t	*sv;
	int		rc;

	rc = smb_server_lookup(&sv);
	if (rc == 0)
		rc = smb_server_delete(sv);

	return (rc);
}

/* ARGSUSED */
static int
smb_drv_ioctl(dev_t drv, int cmd, intptr_t argp, int flags, cred_t *cred,
    int *retval)
{
	smb_ioc_t	*ioc;
	smb_ioc_header_t ioc_hdr;
	uint32_t	crc;
	boolean_t	copyout = B_FALSE;
	int		rc = 0;
	size_t		alloclen;

	if (ddi_copyin((void *)argp, &ioc_hdr, sizeof (ioc_hdr), flags))
		return (EFAULT);

	/*
	 * Check version and length.
	 *
	 * Note that some ioctls (i.e. SMB_IOC_SVCENUM) have payload
	 * data after the ioctl struct, in which case they specify a
	 * length much larger than sizeof smb_ioc_t.  The theoretical
	 * largest ioctl data is therefore the size of the union plus
	 * the max size of the payload (which is SMB_IOC_DATA_SIZE).
	 */
	if (ioc_hdr.version != SMB_IOC_VERSION ||
	    ioc_hdr.len < sizeof (ioc_hdr) ||
	    ioc_hdr.len > (sizeof (*ioc) + SMB_IOC_DATA_SIZE))
		return (EINVAL);

	crc = ioc_hdr.crc;
	ioc_hdr.crc = 0;
	if (smb_crc_gen((uint8_t *)&ioc_hdr, sizeof (ioc_hdr)) != crc)
		return (EINVAL);

	/*
	 * Note that smb_ioc_t is a union, and callers set ioc_hdr.len
	 * to the size of the actual union arm.  If some caller were to
	 * set that size too small, we could end up passing under-sized
	 * memory to one of the type-specific handler functions.  Avoid
	 * that problem by allocating at least the size of the union,
	 * (zeroed out) and then copy in the caller specified length.
	 */
	alloclen = MAX(ioc_hdr.len, sizeof (*ioc));
	ioc = kmem_zalloc(alloclen, KM_SLEEP);
	if (ddi_copyin((void *)argp, ioc, ioc_hdr.len, flags)) {
		kmem_free(ioc, alloclen);
		return (EFAULT);
	}

	/* Don't allow the request size to change mid-ioctl */
	if (ioc_hdr.len != ioc->ioc_hdr.len) {
		kmem_free(ioc, alloclen);
		return (EINVAL);
	}

	switch (cmd) {
	case SMB_IOC_CONFIG:
		rc = smb_server_configure(&ioc->ioc_cfg);
		break;
	case SMB_IOC_START:
		rc = smb_server_start(&ioc->ioc_start);
		break;
	case SMB_IOC_STOP:
		rc = smb_server_stop();
		break;
	case SMB_IOC_EVENT:
		rc = smb_server_notify_event(&ioc->ioc_event);
		break;
	case SMB_IOC_GMTOFF:
		rc = smb_server_set_gmtoff(&ioc->ioc_gmt);
		break;
	case SMB_IOC_SHARE:
		rc = smb_kshare_export_list(&ioc->ioc_share);
		break;
	case SMB_IOC_UNSHARE:
		rc = smb_kshare_unexport_list(&ioc->ioc_share);
		break;
	case SMB_IOC_SHAREINFO:
		rc = smb_kshare_info(&ioc->ioc_shareinfo);
		copyout = B_TRUE;
		break;
	case SMB_IOC_NUMOPEN:
		rc = smb_server_numopen(&ioc->ioc_opennum);
		copyout = B_TRUE;
		break;
	case SMB_IOC_SVCENUM:
		rc = smb_server_enum(&ioc->ioc_svcenum);
		copyout = B_TRUE;
		break;
	case SMB_IOC_SESSION_CLOSE:
		rc = smb_server_session_close(&ioc->ioc_session);
		break;
	case SMB_IOC_FILE_CLOSE:
		rc = smb_server_file_close(&ioc->ioc_fileid);
		break;
	case SMB_IOC_SPOOLDOC:
		rc = smb_server_spooldoc(&ioc->ioc_spooldoc);
		copyout = B_TRUE;
		break;
	default:
		rc = ENOTTY;
		break;
	}
	if ((rc == 0) && copyout) {
		if (ddi_copyout(ioc, (void *)argp, ioc_hdr.len, flags))
			rc = EFAULT;
	}
	kmem_free(ioc, alloclen);
	return (rc);
}

/*
 * ****************************************************************************
 *				Pseudo Device Operations
 * ****************************************************************************
 */
static int
smb_drv_attach(dev_info_t *dip, ddi_attach_cmd_t cmd)
{
	if (cmd == DDI_ATTACH) {
		/* we only allow instance 0 to attach */
		if (ddi_get_instance(dip) == 0) {
			/* create the minor node */
			if (ddi_create_minor_node(dip, "smbsrv", S_IFCHR, 0,
			    DDI_PSEUDO, 0) == DDI_SUCCESS) {
				smb_drv_dip = dip;
				return (DDI_SUCCESS);
			} else {
				cmn_err(CE_WARN, "smb_drv_attach:"
				    " failed creating minor node");
			}
		}
	}
	return (DDI_FAILURE);
}

static int
smb_drv_detach(dev_info_t *dip, ddi_detach_cmd_t cmd)
{
	if (cmd == DDI_DETACH) {
		ASSERT(dip == smb_drv_dip);
		ddi_remove_minor_node(dip, NULL);
		smb_drv_dip = NULL;
		return (DDI_SUCCESS);
	}
	return (DDI_FAILURE);
}

/* ARGSUSED */
static int
smb_drv_getinfo(dev_info_t *dip, ddi_info_cmd_t cmd, void *arg, void **result)
{
	ulong_t instance = getminor((dev_t)arg);

	switch (cmd) {
	case DDI_INFO_DEVT2DEVINFO:
		*result = smb_drv_dip;
		return (DDI_SUCCESS);

	case DDI_INFO_DEVT2INSTANCE:
		*result = (void *)instance;
		return (DDI_SUCCESS);

	default:
		break;
	}

	return (DDI_FAILURE);
}
