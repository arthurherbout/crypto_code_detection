/*
 * Copyright (C) 2007-2014 VMware, Inc. All rights reserved.
 *
 * The contents of this file are subject to the terms of the Common
 * Development and Distribution License (the "License") version 1.0
 * and no later version.  You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at
 *         http://www.opensource.org/licenses/cddl1.php
 *
 * See the License for the specific language governing permissions
 * and limitations under the License.
 */

/*
 * Copyright (c) 2012, 2016 by Delphix. All rights reserved.
 */

#include <vmxnet3.h>

/*
 * This driver is based on VMware's version 3227872, and contains additional
 * enhancements (see README.txt).
 */
#define	BUILD_NUMBER_NUMERIC	3227872

/*
 * If we run out of rxPool buffers, only allocate if the MTU is <= PAGESIZE
 * so that we don't have to incur the cost of allocating multiple contiguous
 * pages (very slow) in interrupt context.
 */
#define	VMXNET3_ALLOC_OK(dp)	((dp)->cur_mtu <= PAGESIZE)

/*
 * TODO:
 *    - Tx data ring
 *    - MAC_CAPAB_POLL support
 *    - Dynamic RX pool
 */

static int vmxnet3_getstat(void *, uint_t, uint64_t *);
static int vmxnet3_start(void *);
static void vmxnet3_stop(void *);
static int vmxnet3_setpromisc(void *, boolean_t);
static void vmxnet3_ioctl(void *arg, queue_t *wq, mblk_t *mp);
static int vmxnet3_multicst(void *, boolean_t, const uint8_t *);
static int vmxnet3_unicst(void *, const uint8_t *);
static boolean_t vmxnet3_getcapab(void *, mac_capab_t, void *);
static int vmxnet3_get_prop(void *, const char *, mac_prop_id_t, uint_t,
    void *);
static int vmxnet3_set_prop(void *, const char *, mac_prop_id_t, uint_t,
    const void *);
static void vmxnet3_prop_info(void *, const char *, mac_prop_id_t,
    mac_prop_info_handle_t);

int vmxnet3s_debug = 0;

/* MAC callbacks */
static mac_callbacks_t vmxnet3_mac_callbacks = {
	.mc_callbacks =	MC_GETCAPAB | MC_IOCTL | MC_SETPROP | MC_PROPINFO,
	.mc_getstat =	vmxnet3_getstat,
	.mc_start =	vmxnet3_start,
	.mc_stop =	vmxnet3_stop,
	.mc_setpromisc = vmxnet3_setpromisc,
	.mc_multicst =	vmxnet3_multicst,
	.mc_unicst =	vmxnet3_unicst,
	.mc_tx =	vmxnet3_tx,
	.mc_ioctl =	vmxnet3_ioctl,
	.mc_getcapab =	vmxnet3_getcapab,
	.mc_getprop =	vmxnet3_get_prop,
	.mc_setprop =	vmxnet3_set_prop,
	.mc_propinfo =	vmxnet3_prop_info
};

/* Tx DMA engine description */
static ddi_dma_attr_t vmxnet3_dma_attrs_tx = {
	.dma_attr_version =	DMA_ATTR_V0,
	.dma_attr_addr_lo =	0x0000000000000000ull,
	.dma_attr_addr_hi =	0xFFFFFFFFFFFFFFFFull,
	.dma_attr_count_max =	0xFFFFFFFFFFFFFFFFull,
	.dma_attr_align =	0x0000000000000001ull,
	.dma_attr_burstsizes =	0x0000000000000001ull,
	.dma_attr_minxfer =	0x00000001,
	.dma_attr_maxxfer =	0x000000000000FFFFull,
	.dma_attr_seg =		0xFFFFFFFFFFFFFFFFull,
	.dma_attr_sgllen =	-1,
	.dma_attr_granular =	0x00000001,
	.dma_attr_flags =	0
};

/* --- */

/*
 * Fetch the statistics of a vmxnet3 device.
 *
 * Returns:
 *	0 on success, non-zero on failure.
 */
static int
vmxnet3_getstat(void *data, uint_t stat, uint64_t *val)
{
	vmxnet3_softc_t *dp = data;
	UPT1_TxStats *txStats;
	UPT1_RxStats *rxStats;

	VMXNET3_DEBUG(dp, 3, "getstat(%u)\n", stat);

	if (!dp->devEnabled) {
		return (EBUSY);
	}

	txStats = &VMXNET3_TQDESC(dp)->stats;
	rxStats = &VMXNET3_RQDESC(dp)->stats;

	/*
	 * First touch the related register
	 */
	switch (stat) {
	case MAC_STAT_MULTIRCV:
	case MAC_STAT_BRDCSTRCV:
	case MAC_STAT_MULTIXMT:
	case MAC_STAT_BRDCSTXMT:
	case MAC_STAT_NORCVBUF:
	case MAC_STAT_IERRORS:
	case MAC_STAT_NOXMTBUF:
	case MAC_STAT_OERRORS:
	case MAC_STAT_RBYTES:
	case MAC_STAT_IPACKETS:
	case MAC_STAT_OBYTES:
	case MAC_STAT_OPACKETS:
		VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_CMD, VMXNET3_CMD_GET_STATS);
		break;
	case MAC_STAT_IFSPEED:
	case MAC_STAT_COLLISIONS:
	case ETHER_STAT_LINK_DUPLEX:
		/* nothing */
		break;
	default:
		return (ENOTSUP);
	}

	/*
	 * Then fetch the corresponding stat
	 */
	switch (stat) {
	case MAC_STAT_IFSPEED:
		*val = dp->linkSpeed;
		break;
	case MAC_STAT_MULTIRCV:
		*val = rxStats->mcastPktsRxOK;
		break;
	case MAC_STAT_BRDCSTRCV:
		*val = rxStats->bcastPktsRxOK;
		break;
	case MAC_STAT_MULTIXMT:
		*val = txStats->mcastPktsTxOK;
		break;
	case MAC_STAT_BRDCSTXMT:
		*val = txStats->bcastPktsTxOK;
		break;
	case MAC_STAT_NORCVBUF:
		*val = rxStats->pktsRxOutOfBuf + dp->rx_alloc_failed;
		break;
	case MAC_STAT_IERRORS:
		*val = rxStats->pktsRxError;
		break;
	case MAC_STAT_NOXMTBUF:
		*val = txStats->pktsTxDiscard + dp->tx_pullup_failed;
		break;
	case MAC_STAT_OERRORS:
		*val = txStats->pktsTxError + dp->tx_error;
		break;
	case MAC_STAT_COLLISIONS:
		*val = 0;
		break;
	case MAC_STAT_RBYTES:
		*val = rxStats->ucastBytesRxOK + rxStats->mcastBytesRxOK +
		    rxStats->bcastBytesRxOK;
		break;
	case MAC_STAT_IPACKETS:
		*val = rxStats->ucastPktsRxOK + rxStats->mcastPktsRxOK +
		    rxStats->bcastPktsRxOK;
		break;
	case MAC_STAT_OBYTES:
		*val = txStats->ucastBytesTxOK + txStats->mcastBytesTxOK +
		    txStats->bcastBytesTxOK;
		break;
	case MAC_STAT_OPACKETS:
		*val = txStats->ucastPktsTxOK + txStats->mcastPktsTxOK +
		    txStats->bcastPktsTxOK;
		break;
	case ETHER_STAT_LINK_DUPLEX:
		*val = LINK_DUPLEX_FULL;
		break;
	default:
		ASSERT(B_FALSE);
	}

	return (0);
}

/*
 * Allocate and initialize the shared data structures of a vmxnet3 device.
 *
 * Returns:
 *	0 on sucess, non-zero on failure.
 */
static int
vmxnet3_prepare_drivershared(vmxnet3_softc_t *dp)
{
	Vmxnet3_DriverShared *ds;
	size_t allocSize = sizeof (Vmxnet3_DriverShared);
	int err;

	if ((err = vmxnet3_alloc_dma_mem_1(dp, &dp->sharedData, allocSize,
	    B_TRUE)) != 0) {
		return (err);
	}
	ds = VMXNET3_DS(dp);
	(void) memset(ds, 0, allocSize);

	allocSize = sizeof (Vmxnet3_TxQueueDesc) + sizeof (Vmxnet3_RxQueueDesc);
	if ((err = vmxnet3_alloc_dma_mem_128(dp, &dp->queueDescs, allocSize,
	    B_TRUE)) != 0) {
		vmxnet3_free_dma_mem(&dp->sharedData);
		return (err);
	}
	(void) memset(dp->queueDescs.buf, 0, allocSize);

	ds->magic = VMXNET3_REV1_MAGIC;

	/* Take care of most of devRead */
	ds->devRead.misc.driverInfo.version = BUILD_NUMBER_NUMERIC;
#ifdef _LP64
	ds->devRead.misc.driverInfo.gos.gosBits = VMXNET3_GOS_BITS_64;
#else
	ds->devRead.misc.driverInfo.gos.gosBits = VMXNET3_GOS_BITS_32;
#endif
	ds->devRead.misc.driverInfo.gos.gosType = VMXNET3_GOS_TYPE_SOLARIS;
	ds->devRead.misc.driverInfo.gos.gosVer = 10;
	ds->devRead.misc.driverInfo.vmxnet3RevSpt = 1;
	ds->devRead.misc.driverInfo.uptVerSpt = 1;

	ds->devRead.misc.uptFeatures = UPT1_F_RXCSUM;
	ds->devRead.misc.mtu = dp->cur_mtu;

	/* XXX: ds->devRead.misc.maxNumRxSG */
	ds->devRead.misc.numTxQueues = 1;
	ds->devRead.misc.numRxQueues = 1;
	ds->devRead.misc.queueDescPA = dp->queueDescs.bufPA;
	ds->devRead.misc.queueDescLen = allocSize;

	/* TxQueue and RxQueue information is filled in other functions */
	ds->devRead.intrConf.autoMask = (dp->intrMaskMode == VMXNET3_IMM_AUTO);
	ds->devRead.intrConf.numIntrs = 1;
	/* XXX: ds->intr.modLevels */
	ds->devRead.intrConf.eventIntrIdx = 0;

	VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_DSAL,
	    VMXNET3_ADDR_LO(dp->sharedData.bufPA));
	VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_DSAH,
	    VMXNET3_ADDR_HI(dp->sharedData.bufPA));

	return (0);
}

/*
 * Destroy the shared data structures of a vmxnet3 device.
 */
static void
vmxnet3_destroy_drivershared(vmxnet3_softc_t *dp)
{
	VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_DSAL, 0);
	VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_DSAH, 0);

	vmxnet3_free_dma_mem(&dp->queueDescs);
	vmxnet3_free_dma_mem(&dp->sharedData);
}

/*
 * Allocate and initialize the command ring of a queue.
 *
 * Returns:
 *	0 on success, non-zero on error.
 */
static int
vmxnet3_alloc_cmdring(vmxnet3_softc_t *dp, vmxnet3_cmdring_t *cmdRing)
{
	size_t ringSize = cmdRing->size * sizeof (Vmxnet3_TxDesc);
	int err;

	if ((err = vmxnet3_alloc_dma_mem_512(dp, &cmdRing->dma, ringSize,
	    B_TRUE)) != 0) {
		return (err);
	}
	(void) memset(cmdRing->dma.buf, 0, ringSize);
	cmdRing->avail = cmdRing->size;
	cmdRing->next2fill = 0;
	cmdRing->gen = VMXNET3_INIT_GEN;

	return (0);
}

/*
 * Allocate and initialize the completion ring of a queue.
 *
 * Returns:
 *    DDI_SUCCESS or DDI_FAILURE.
 */
static int
vmxnet3_alloc_compring(vmxnet3_softc_t *dp, vmxnet3_compring_t *compRing)
{
	size_t ringSize = compRing->size * sizeof (Vmxnet3_TxCompDesc);

	if (vmxnet3_alloc_dma_mem_512(dp, &compRing->dma, ringSize,
	    B_TRUE) != DDI_SUCCESS) {
		return (DDI_FAILURE);
	}
	(void) memset(compRing->dma.buf, 0, ringSize);
	compRing->next2comp = 0;
	compRing->gen = VMXNET3_INIT_GEN;

	return (DDI_SUCCESS);
}

/*
 * Initialize the tx queue of a vmxnet3 device.
 *
 * Returns:
 *	0 on success, non-zero on failure.
 */
static int
vmxnet3_prepare_txqueue(vmxnet3_softc_t *dp)
{
	Vmxnet3_TxQueueDesc *tqdesc = VMXNET3_TQDESC(dp);
	vmxnet3_txqueue_t *txq = &dp->txQueue;
	int err;

	ASSERT(!(txq->cmdRing.size & VMXNET3_RING_SIZE_MASK));
	ASSERT(!(txq->compRing.size & VMXNET3_RING_SIZE_MASK));
	ASSERT(!txq->cmdRing.dma.buf && !txq->compRing.dma.buf);

	if ((err = vmxnet3_alloc_cmdring(dp, &txq->cmdRing)) != 0) {
		goto error;
	}
	tqdesc->conf.txRingBasePA = txq->cmdRing.dma.bufPA;
	tqdesc->conf.txRingSize = txq->cmdRing.size;
	tqdesc->conf.dataRingBasePA = 0;
	tqdesc->conf.dataRingSize = 0;

	if ((err = vmxnet3_alloc_compring(dp, &txq->compRing)) != 0) {
		goto error_cmdring;
	}
	tqdesc->conf.compRingBasePA = txq->compRing.dma.bufPA;
	tqdesc->conf.compRingSize = txq->compRing.size;

	txq->metaRing = kmem_zalloc(txq->cmdRing.size *
	    sizeof (vmxnet3_metatx_t), KM_SLEEP);
	ASSERT(txq->metaRing);

	if ((err = vmxnet3_txqueue_init(dp, txq)) != 0) {
		goto error_mpring;
	}

	return (0);

error_mpring:
	kmem_free(txq->metaRing, txq->cmdRing.size * sizeof (vmxnet3_metatx_t));
	vmxnet3_free_dma_mem(&txq->compRing.dma);
error_cmdring:
	vmxnet3_free_dma_mem(&txq->cmdRing.dma);
error:
	return (err);
}

/*
 * Initialize the rx queue of a vmxnet3 device.
 *
 * Returns:
 *	0 on success, non-zero on failure.
 */
static int
vmxnet3_prepare_rxqueue(vmxnet3_softc_t *dp)
{
	Vmxnet3_RxQueueDesc *rqdesc = VMXNET3_RQDESC(dp);
	vmxnet3_rxqueue_t *rxq = &dp->rxQueue;
	int err = 0;

	ASSERT(!(rxq->cmdRing.size & VMXNET3_RING_SIZE_MASK));
	ASSERT(!(rxq->compRing.size & VMXNET3_RING_SIZE_MASK));
	ASSERT(!rxq->cmdRing.dma.buf && !rxq->compRing.dma.buf);

	if ((err = vmxnet3_alloc_cmdring(dp, &rxq->cmdRing)) != 0) {
		goto error;
	}
	rqdesc->conf.rxRingBasePA[0] = rxq->cmdRing.dma.bufPA;
	rqdesc->conf.rxRingSize[0] = rxq->cmdRing.size;
	rqdesc->conf.rxRingBasePA[1] = 0;
	rqdesc->conf.rxRingSize[1] = 0;

	if ((err = vmxnet3_alloc_compring(dp, &rxq->compRing)) != 0) {
		goto error_cmdring;
	}
	rqdesc->conf.compRingBasePA = rxq->compRing.dma.bufPA;
	rqdesc->conf.compRingSize = rxq->compRing.size;

	rxq->bufRing = kmem_zalloc(rxq->cmdRing.size *
	    sizeof (vmxnet3_bufdesc_t), KM_SLEEP);
	ASSERT(rxq->bufRing);

	if ((err = vmxnet3_rxqueue_init(dp, rxq)) != 0) {
		goto error_bufring;
	}

	return (0);

error_bufring:
	kmem_free(rxq->bufRing, rxq->cmdRing.size * sizeof (vmxnet3_bufdesc_t));
	vmxnet3_free_dma_mem(&rxq->compRing.dma);
error_cmdring:
	vmxnet3_free_dma_mem(&rxq->cmdRing.dma);
error:
	return (err);
}

/*
 * Destroy the tx queue of a vmxnet3 device.
 */
static void
vmxnet3_destroy_txqueue(vmxnet3_softc_t *dp)
{
	vmxnet3_txqueue_t *txq = &dp->txQueue;

	ASSERT(txq->metaRing);
	ASSERT(txq->cmdRing.dma.buf && txq->compRing.dma.buf);

	vmxnet3_txqueue_fini(dp, txq);

	kmem_free(txq->metaRing, txq->cmdRing.size * sizeof (vmxnet3_metatx_t));

	vmxnet3_free_dma_mem(&txq->cmdRing.dma);
	vmxnet3_free_dma_mem(&txq->compRing.dma);
}

/*
 * Destroy the rx queue of a vmxnet3 device.
 */
static void
vmxnet3_destroy_rxqueue(vmxnet3_softc_t *dp)
{
	vmxnet3_rxqueue_t *rxq = &dp->rxQueue;

	ASSERT(rxq->bufRing);
	ASSERT(rxq->cmdRing.dma.buf && rxq->compRing.dma.buf);

	vmxnet3_rxqueue_fini(dp, rxq);

	kmem_free(rxq->bufRing, rxq->cmdRing.size * sizeof (vmxnet3_bufdesc_t));

	vmxnet3_free_dma_mem(&rxq->cmdRing.dma);
	vmxnet3_free_dma_mem(&rxq->compRing.dma);
}

/*
 * Apply new RX filters settings to a vmxnet3 device.
 */
static void
vmxnet3_refresh_rxfilter(vmxnet3_softc_t *dp)
{
	Vmxnet3_DriverShared *ds = VMXNET3_DS(dp);

	ds->devRead.rxFilterConf.rxMode = dp->rxMode;
	VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_CMD, VMXNET3_CMD_UPDATE_RX_MODE);
}

/*
 * Fetch the link state of a vmxnet3 device.
 */
static void
vmxnet3_refresh_linkstate(vmxnet3_softc_t *dp)
{
	uint32_t ret32;

	VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_CMD, VMXNET3_CMD_GET_LINK);
	ret32 = VMXNET3_BAR1_GET32(dp, VMXNET3_REG_CMD);
	if (ret32 & 1) {
		dp->linkState = LINK_STATE_UP;
		dp->linkSpeed = (ret32 >> 16) * 1000000ULL;
	} else {
		dp->linkState = LINK_STATE_DOWN;
		dp->linkSpeed = 0;
	}
}

/*
 * Start a vmxnet3 device: allocate and initialize the shared data
 * structures and send a start command to the device.
 *
 * Returns:
 *	0 on success, non-zero error on failure.
 */
static int
vmxnet3_start(void *data)
{
	vmxnet3_softc_t *dp = data;
	Vmxnet3_TxQueueDesc *tqdesc;
	Vmxnet3_RxQueueDesc *rqdesc;
	int txQueueSize, rxQueueSize;
	uint32_t ret32;
	int err, dmaerr;

	VMXNET3_DEBUG(dp, 1, "start()\n");

	/*
	 * Allocate vmxnet3's shared data and advertise its PA
	 */
	if ((err = vmxnet3_prepare_drivershared(dp)) != 0) {
		VMXNET3_WARN(dp, "vmxnet3_prepare_drivershared() failed: %d",
		    err);
		goto error;
	}
	tqdesc = VMXNET3_TQDESC(dp);
	rqdesc = VMXNET3_RQDESC(dp);

	/*
	 * Create and initialize the tx queue
	 */
	txQueueSize = vmxnet3_getprop(dp, "TxRingSize", 32, 4096,
	    VMXNET3_DEF_TX_RING_SIZE);
	if (!(txQueueSize & VMXNET3_RING_SIZE_MASK)) {
		dp->txQueue.cmdRing.size = txQueueSize;
		dp->txQueue.compRing.size = txQueueSize;
		dp->txQueue.sharedCtrl = &tqdesc->ctrl;
		if ((err = vmxnet3_prepare_txqueue(dp)) != 0) {
			VMXNET3_WARN(dp, "vmxnet3_prepare_txqueue() failed: %d",
			    err);
			goto error_shared_data;
		}
	} else {
		VMXNET3_WARN(dp, "invalid tx ring size (%d)\n", txQueueSize);
		err = EINVAL;
		goto error_shared_data;
	}

	/*
	 * Create and initialize the rx queue
	 */
	rxQueueSize = vmxnet3_getprop(dp, "RxRingSize", 32, 4096,
	    VMXNET3_DEF_RX_RING_SIZE);
	if (!(rxQueueSize & VMXNET3_RING_SIZE_MASK)) {
		dp->rxQueue.cmdRing.size = rxQueueSize;
		dp->rxQueue.compRing.size = rxQueueSize;
		dp->rxQueue.sharedCtrl = &rqdesc->ctrl;
		if ((err = vmxnet3_prepare_rxqueue(dp)) != 0) {
			VMXNET3_WARN(dp, "vmxnet3_prepare_rxqueue() failed: %d",
			    err);
			goto error_tx_queue;
		}
	} else {
		VMXNET3_WARN(dp, "invalid rx ring size (%d)\n", rxQueueSize);
		err = EINVAL;
		goto error_tx_queue;
	}

	/*
	 * Allocate the Tx DMA handle
	 */
	if ((dmaerr = ddi_dma_alloc_handle(dp->dip, &vmxnet3_dma_attrs_tx,
	    DDI_DMA_SLEEP, NULL, &dp->txDmaHandle)) != DDI_SUCCESS) {
		VMXNET3_WARN(dp, "ddi_dma_alloc_handle() failed: %d", dmaerr);
		err = vmxnet3_dmaerr2errno(dmaerr);
		goto error_rx_queue;
	}

	/*
	 * Activate the device
	 */
	VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_CMD, VMXNET3_CMD_ACTIVATE_DEV);
	ret32 = VMXNET3_BAR1_GET32(dp, VMXNET3_REG_CMD);
	if (ret32) {
		VMXNET3_WARN(dp, "ACTIVATE_DEV failed: 0x%x\n", ret32);
		err = ENXIO;
		goto error_txhandle;
	}
	dp->devEnabled = B_TRUE;

	VMXNET3_BAR0_PUT32(dp, VMXNET3_REG_RXPROD,
	    dp->txQueue.cmdRing.size - 1);

	/*
	 * Update the RX filters, must be done after ACTIVATE_DEV
	 */
	dp->rxMode = VMXNET3_RXM_UCAST | VMXNET3_RXM_BCAST;
	vmxnet3_refresh_rxfilter(dp);

	/*
	 * Get the link state now because no events will be generated
	 */
	vmxnet3_refresh_linkstate(dp);
	mac_link_update(dp->mac, dp->linkState);

	/*
	 * Finally, unmask the interrupt
	 */
	VMXNET3_BAR0_PUT32(dp, VMXNET3_REG_IMR, 0);

	return (0);

error_txhandle:
	ddi_dma_free_handle(&dp->txDmaHandle);
error_rx_queue:
	vmxnet3_destroy_rxqueue(dp);
error_tx_queue:
	vmxnet3_destroy_txqueue(dp);
error_shared_data:
	vmxnet3_destroy_drivershared(dp);
error:
	return (err);
}

/*
 * Stop a vmxnet3 device: send a stop command to the device and
 * de-allocate the shared data structures.
 */
static void
vmxnet3_stop(void *data)
{
	vmxnet3_softc_t *dp = data;

	VMXNET3_DEBUG(dp, 1, "stop()\n");

	/*
	 * Take the 2 locks related to asynchronous events.
	 * These events should always check dp->devEnabled before poking dp.
	 */
	mutex_enter(&dp->intrLock);
	mutex_enter(&dp->rxPoolLock);
	VMXNET3_BAR0_PUT32(dp, VMXNET3_REG_IMR, 1);
	dp->devEnabled = B_FALSE;
	VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_CMD, VMXNET3_CMD_QUIESCE_DEV);
	mutex_exit(&dp->rxPoolLock);
	mutex_exit(&dp->intrLock);

	ddi_dma_free_handle(&dp->txDmaHandle);

	vmxnet3_destroy_rxqueue(dp);
	vmxnet3_destroy_txqueue(dp);

	vmxnet3_destroy_drivershared(dp);
}

/*
 * Set or unset promiscuous mode on a vmxnet3 device.
 */
static int
vmxnet3_setpromisc(void *data, boolean_t promisc)
{
	vmxnet3_softc_t *dp = data;

	VMXNET3_DEBUG(dp, 2, "setpromisc(%s)\n", promisc ? "TRUE" : "FALSE");

	if (promisc) {
		dp->rxMode |= VMXNET3_RXM_PROMISC;
	} else {
		dp->rxMode &= ~VMXNET3_RXM_PROMISC;
	}

	vmxnet3_refresh_rxfilter(dp);

	return (0);
}

/*
 * Add or remove a multicast address from/to a vmxnet3 device.
 *
 * Returns:
 *	0 on success, non-zero on failure.
 */
static int
vmxnet3_multicst(void *data, boolean_t add, const uint8_t *macaddr)
{
	vmxnet3_softc_t *dp = data;
	vmxnet3_dmabuf_t newMfTable;
	int ret = 0;
	uint16_t macIdx;
	size_t allocSize;

	VMXNET3_DEBUG(dp, 2, "multicst(%s, "MACADDR_FMT")\n",
	    add ? "add" : "remove", MACADDR_FMT_ARGS(macaddr));

	/*
	 * First lookup the position of the given MAC to check if it is
	 * present in the existing MF table.
	 */
	for (macIdx = 0; macIdx < dp->mfTable.bufLen; macIdx += 6) {
		if (memcmp(&dp->mfTable.buf[macIdx], macaddr, 6) == 0) {
			break;
		}
	}

	/*
	 * Check for 2 situations we can handle gracefully by bailing out:
	 * Adding an already existing filter or removing a non-existing one.
	 */
	if (add && macIdx < dp->mfTable.bufLen) {
		VMXNET3_WARN(dp, MACADDR_FMT " already in MC filter list "
		    "@ %u\n", MACADDR_FMT_ARGS(macaddr), macIdx / 6);
		ASSERT(B_FALSE);
		goto done;
	}
	if (!add && macIdx == dp->mfTable.bufLen) {
		VMXNET3_WARN(dp, MACADDR_FMT " not in MC filter list @ %u\n",
		    MACADDR_FMT_ARGS(macaddr), macIdx / 6);
		ASSERT(B_FALSE);
		goto done;
	}

	/*
	 * Create the new MF table
	 */
	allocSize = dp->mfTable.bufLen + (add ? 6 : -6);
	if (allocSize) {
		ret = vmxnet3_alloc_dma_mem_1(dp, &newMfTable, allocSize,
		    B_TRUE);
		ASSERT(ret == 0);
		if (add) {
			(void) memcpy(newMfTable.buf, dp->mfTable.buf,
			    dp->mfTable.bufLen);
			(void) memcpy(newMfTable.buf + dp->mfTable.bufLen,
			    macaddr, 6);
		} else {
			(void) memcpy(newMfTable.buf, dp->mfTable.buf,
			    macIdx);
			(void) memcpy(newMfTable.buf + macIdx,
			    dp->mfTable.buf + macIdx + 6,
			    dp->mfTable.bufLen - macIdx - 6);
		}
	} else {
		newMfTable.buf = NULL;
		newMfTable.bufPA = 0;
		newMfTable.bufLen = 0;
	}

	/*
	 * Now handle 2 corner cases: if we're creating the first filter or
	 * removing the last one, we have to update rxMode accordingly.
	 */
	if (add && newMfTable.bufLen == 6) {
		ASSERT(!(dp->rxMode & VMXNET3_RXM_MCAST));
		dp->rxMode |= VMXNET3_RXM_MCAST;
		vmxnet3_refresh_rxfilter(dp);
	}
	if (!add && dp->mfTable.bufLen == 6) {
		ASSERT(newMfTable.buf == NULL);
		ASSERT(dp->rxMode & VMXNET3_RXM_MCAST);
		dp->rxMode &= ~VMXNET3_RXM_MCAST;
		vmxnet3_refresh_rxfilter(dp);
	}

	/*
	 * Now replace the old MF table with the new one
	 */
	if (dp->mfTable.buf) {
		vmxnet3_free_dma_mem(&dp->mfTable);
	}
	dp->mfTable = newMfTable;
	VMXNET3_DS(dp)->devRead.rxFilterConf.mfTablePA = newMfTable.bufPA;
	VMXNET3_DS(dp)->devRead.rxFilterConf.mfTableLen = newMfTable.bufLen;

done:
	/* Always update the filters */
	VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_CMD, VMXNET3_CMD_UPDATE_MAC_FILTERS);

	return (ret);
}

/*
 * Set the mac address of a vmxnet3 device.
 *
 * Returns:
 *	0
 */
static int
vmxnet3_unicst(void *data, const uint8_t *macaddr)
{
	vmxnet3_softc_t *dp = data;
	uint32_t val32;

	VMXNET3_DEBUG(dp, 2, "unicst("MACADDR_FMT")\n",
	    MACADDR_FMT_ARGS(macaddr));

	val32 = *((uint32_t *)(macaddr + 0));
	VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_MACL, val32);
	val32 = *((uint16_t *)(macaddr + 4));
	VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_MACH, val32);

	(void) memcpy(dp->macaddr, macaddr, 6);

	return (0);
}

/*
 * Change the MTU as seen by the driver. This is only supported when
 * the mac is stopped.
 *
 * Returns:
 *	EBUSY if the device is enabled.
 *	EINVAL for invalid MTU values.
 *	0 on success.
 */
static int
vmxnet3_change_mtu(vmxnet3_softc_t *dp, uint32_t new_mtu)
{
	int ret;

	if (dp->devEnabled)
		return (EBUSY);

	if (new_mtu == dp->cur_mtu) {
		VMXNET3_WARN(dp, "New MTU is same as old mtu : %d.\n", new_mtu);
		return (0);
	}

	if (new_mtu < VMXNET3_MIN_MTU || new_mtu > VMXNET3_MAX_MTU) {
		VMXNET3_WARN(dp, "New MTU not in valid range [%d, %d].\n",
		    VMXNET3_MIN_MTU, VMXNET3_MAX_MTU);
		return (EINVAL);
	} else if (new_mtu > ETHERMTU && !dp->allow_jumbo) {
		VMXNET3_WARN(dp, "MTU cannot be greater than %d because "
		    "accept-jumbo is not enabled.\n", ETHERMTU);
		return (EINVAL);
	}

	dp->cur_mtu = new_mtu;
	dp->alloc_ok = VMXNET3_ALLOC_OK(dp);

	if ((ret = mac_maxsdu_update(dp->mac, new_mtu)) != 0)
		VMXNET3_WARN(dp, "Unable to update mac with %d mtu: %d",
		    new_mtu, ret);

	return (ret);
}

/* ARGSUSED */
static int
vmxnet3_get_prop(void *data, const char *prop_name, mac_prop_id_t prop_id,
    uint_t prop_val_size, void *prop_val)
{
	vmxnet3_softc_t *dp = data;
	int ret = 0;

	switch (prop_id) {
	case MAC_PROP_MTU:
		ASSERT(prop_val_size >= sizeof (uint32_t));
		bcopy(&dp->cur_mtu, prop_val, sizeof (uint32_t));
		break;
	default:
		ret = ENOTSUP;
		break;
	}
	return (ret);
}

/* ARGSUSED */
static int
vmxnet3_set_prop(void *data, const char *prop_name, mac_prop_id_t prop_id,
    uint_t prop_val_size, const void *prop_val)
{
	vmxnet3_softc_t *dp = data;
	int ret;

	switch (prop_id) {
	case MAC_PROP_MTU: {
		uint32_t new_mtu;
		ASSERT(prop_val_size >= sizeof (uint32_t));
		bcopy(prop_val, &new_mtu, sizeof (new_mtu));
		ret = vmxnet3_change_mtu(dp, new_mtu);
		break;
	}
	default:
		ret = ENOTSUP;
		break;
	}

	return (ret);
}

/* ARGSUSED */
static void
vmxnet3_prop_info(void *data, const char *prop_name, mac_prop_id_t prop_id,
    mac_prop_info_handle_t prop_handle)
{
	switch (prop_id) {
	case MAC_PROP_MTU:
		mac_prop_info_set_range_uint32(prop_handle, VMXNET3_MIN_MTU,
		    VMXNET3_MAX_MTU);
		break;
	default:
		break;
	}
}

/*
 * DDI/DDK callback to handle IOCTL in driver. Currently it only handles
 * ND_SET ioctl. Rest all are ignored. The ND_SET is used to set/reset
 * accept-jumbo ndd parameted for the interface.
 *
 * Side effects:
 *	MTU can be changed and device can be reset. An ACK or NACK is conveyed
 *	to the calling function from the mblk which was used to call this
 *	function.
 */
static void
vmxnet3_ioctl(void *arg, queue_t *wq, mblk_t *mp)
{
	vmxnet3_softc_t *dp = arg;
	int ret = EINVAL;
	IOCP iocp;
	mblk_t *mp1;
	char *valp, *param;
	int data;

	iocp = (void *)mp->b_rptr;
	iocp->ioc_error = 0;

	switch (iocp->ioc_cmd) {
	case ND_SET:
		/*
		 * The mblk in continuation would contain the ndd parameter name
		 * and data value to be set
		 */
		mp1 = mp->b_cont;
		if (!mp1) {
			VMXNET3_WARN(dp, "Error locating parameter name.\n");
			ret = EINVAL;
			break;
		}

		/* Force null termination */
		mp1->b_datap->db_lim[-1] = '\0';

		/*
		 * From /usr/src/uts/common/inet/nd.c : nd_getset()
		 * "logic throughout nd_xxx assumes single data block for ioctl.
		 *  However, existing code sends in some big buffers."
		 */
		if (mp1->b_cont) {
			freemsg(mp1->b_cont);
			mp1->b_cont = NULL;
		}

		valp = (char *)mp1->b_rptr;	/* Points to param name */
		ASSERT(valp);
		param = valp;
		VMXNET3_DEBUG(dp, 3, "ND Set ioctl for %s\n", param);

		/*
		 * Go past the end of this null terminated string to get the
		 * data value.
		 */
		while (*valp && valp <= (char *)mp1->b_wptr)
			valp++;

		if (valp > (char *)mp1->b_wptr) {
			/*
			 * We are already beyond the readable area of mblk and
			 * still haven't found the end of param string.
			 */
			VMXNET3_WARN(dp,
			    "No data value found to be set to param\n");
			data = -1;
		} else {
			/* Now this points to data string */
			valp++;
			/* Get numeric value of first letter */
			data = (int)*valp - (int)'0';
		}

		if (strcmp("accept-jumbo", param) == 0) {
			if (data == 1) {
				VMXNET3_DEBUG(dp, 2,
				    "Accepting jumbo frames\n");
				dp->allow_jumbo = B_TRUE;
				ret = vmxnet3_change_mtu(dp, VMXNET3_MAX_MTU);
			} else if (data == 0) {
				VMXNET3_DEBUG(dp, 2,
				    "Rejecting jumbo frames\n");
				dp->allow_jumbo = B_FALSE;
				ret = vmxnet3_change_mtu(dp, ETHERMTU);
			} else {
				VMXNET3_WARN(dp, "Invalid data value to be set,"
				    " use 0 or 1\n");
				ret = -1;
			}
		}
		freemsg(mp1);
		mp->b_cont = NULL;
		break;

	default:
		if (mp->b_cont) {
			freemsg(mp->b_cont);
			mp->b_cont = NULL;
		}
		ret = -1;
		break;
	}

	if (ret == 0)
		miocack(wq, mp, 0, 0);
	else
		miocnak(wq, mp, 0, EINVAL);
}

/*
 * Get the capabilities of a vmxnet3 device.
 *
 * Returns:
 *	B_TRUE if the capability is supported, B_FALSE otherwise.
 */
static boolean_t
vmxnet3_getcapab(void *data, mac_capab_t capab, void *arg)
{
	vmxnet3_softc_t *dp = data;
	boolean_t ret;

	switch (capab) {
	case MAC_CAPAB_HCKSUM: {
		uint32_t *txflags = arg;
		*txflags = HCKSUM_INET_PARTIAL;
		ret = B_TRUE;
		break;
	}
	case MAC_CAPAB_LSO: {
		mac_capab_lso_t *lso = arg;
		lso->lso_flags = LSO_TX_BASIC_TCP_IPV4;
		lso->lso_basic_tcp_ipv4.lso_max = IP_MAXPACKET;
		ret = vmxnet3_getprop(dp, "EnableLSO", 0, 1, 1);
		break;
	}
	default:
		ret = B_FALSE;
	}

	VMXNET3_DEBUG(dp, 2, "getcapab(0x%x) -> %s\n", capab,
	    ret ? "yes" : "no");

	return (ret);
}

/*
 * Reset a vmxnet3 device. Only to be used when the device is wedged.
 *
 * Side effects:
 *	The device is reset.
 */
static void
vmxnet3_reset(void *data)
{
	int ret;

	vmxnet3_softc_t *dp = data;

	VMXNET3_DEBUG(dp, 1, "vmxnet3_reset()\n");

	atomic_inc_32(&dp->reset_count);
	vmxnet3_stop(dp);
	VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_CMD, VMXNET3_CMD_RESET_DEV);
	if ((ret = vmxnet3_start(dp)) != 0)
		VMXNET3_WARN(dp, "failed to reset the device: %d", ret);
}

/*
 * Process pending events on a vmxnet3 device.
 *
 * Returns:
 *	B_TRUE if the link state changed, B_FALSE otherwise.
 */
static boolean_t
vmxnet3_intr_events(vmxnet3_softc_t *dp)
{
	Vmxnet3_DriverShared *ds = VMXNET3_DS(dp);
	boolean_t linkStateChanged = B_FALSE;
	uint32_t events = ds->ecr;

	if (events) {
		VMXNET3_DEBUG(dp, 2, "events(0x%x)\n", events);
		if (events & (VMXNET3_ECR_RQERR | VMXNET3_ECR_TQERR)) {
			Vmxnet3_TxQueueDesc *tqdesc = VMXNET3_TQDESC(dp);
			Vmxnet3_RxQueueDesc *rqdesc = VMXNET3_RQDESC(dp);

			VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_CMD,
			    VMXNET3_CMD_GET_QUEUE_STATUS);
			if (tqdesc->status.stopped) {
				VMXNET3_WARN(dp, "tq error 0x%x\n",
				    tqdesc->status.error);
			}
			if (rqdesc->status.stopped) {
				VMXNET3_WARN(dp, "rq error 0x%x\n",
				    rqdesc->status.error);
			}

			if (ddi_taskq_dispatch(dp->resetTask, vmxnet3_reset,
			    dp, DDI_NOSLEEP) == DDI_SUCCESS) {
				VMXNET3_WARN(dp, "reset scheduled\n");
			} else {
				VMXNET3_WARN(dp,
				    "ddi_taskq_dispatch() failed\n");
			}
		}
		if (events & VMXNET3_ECR_LINK) {
			vmxnet3_refresh_linkstate(dp);
			linkStateChanged = B_TRUE;
		}
		if (events & VMXNET3_ECR_DIC) {
			VMXNET3_DEBUG(dp, 1, "device implementation change\n");
		}
		VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_ECR, events);
	}

	return (linkStateChanged);
}

/*
 * Interrupt handler of a vmxnet3 device.
 *
 * Returns:
 *	DDI_INTR_CLAIMED or DDI_INTR_UNCLAIMED.
 */
/* ARGSUSED1 */
static uint_t
vmxnet3_intr(caddr_t data1, caddr_t data2)
{
	vmxnet3_softc_t *dp = (void *) data1;

	VMXNET3_DEBUG(dp, 3, "intr()\n");

	mutex_enter(&dp->intrLock);

	if (dp->devEnabled) {
		boolean_t linkStateChanged;
		boolean_t mustUpdateTx;
		mblk_t *mps;

		if (dp->intrType == DDI_INTR_TYPE_FIXED &&
		    !VMXNET3_BAR1_GET32(dp, VMXNET3_REG_ICR)) {
			goto intr_unclaimed;
		}

		if (dp->intrMaskMode == VMXNET3_IMM_ACTIVE) {
			VMXNET3_BAR0_PUT32(dp, VMXNET3_REG_IMR, 1);
		}

		linkStateChanged = vmxnet3_intr_events(dp);
		mustUpdateTx = vmxnet3_tx_complete(dp, &dp->txQueue);
		mps = vmxnet3_rx_intr(dp, &dp->rxQueue);

		mutex_exit(&dp->intrLock);
		VMXNET3_BAR0_PUT32(dp, VMXNET3_REG_IMR, 0);

		if (linkStateChanged) {
			mac_link_update(dp->mac, dp->linkState);
		}
		if (mustUpdateTx) {
			mac_tx_update(dp->mac);
		}
		if (mps) {
			mac_rx(dp->mac, NULL, mps);
		}

		return (DDI_INTR_CLAIMED);
	}

intr_unclaimed:
	mutex_exit(&dp->intrLock);
	return (DDI_INTR_UNCLAIMED);
}

static int
vmxnet3_kstat_update(kstat_t *ksp, int rw)
{
	vmxnet3_softc_t *dp = ksp->ks_private;
	vmxnet3_kstats_t *statp = ksp->ks_data;

	if (rw == KSTAT_WRITE)
		return (EACCES);

	statp->reset_count.value.ul = dp->reset_count;
	statp->tx_pullup_needed.value.ul = dp->tx_pullup_needed;
	statp->tx_ring_full.value.ul = dp->tx_ring_full;
	statp->rx_alloc_buf.value.ul = dp->rx_alloc_buf;
	statp->rx_pool_empty.value.ul = dp->rx_pool_empty;
	statp->rx_num_bufs.value.ul = dp->rx_num_bufs;

	return (0);
}

static int
vmxnet3_kstat_init(vmxnet3_softc_t *dp)
{
	vmxnet3_kstats_t *statp;

	dp->devKstats = kstat_create(VMXNET3_MODNAME, dp->instance,
	    "statistics", "dev",  KSTAT_TYPE_NAMED,
	    sizeof (vmxnet3_kstats_t) / sizeof (kstat_named_t), 0);
	if (dp->devKstats == NULL)
		return (DDI_FAILURE);

	dp->devKstats->ks_update = vmxnet3_kstat_update;
	dp->devKstats->ks_private = dp;

	statp = dp->devKstats->ks_data;

	kstat_named_init(&statp->reset_count, "reset_count", KSTAT_DATA_ULONG);
	kstat_named_init(&statp->tx_pullup_needed, "tx_pullup_needed",
	    KSTAT_DATA_ULONG);
	kstat_named_init(&statp->tx_ring_full, "tx_ring_full",
	    KSTAT_DATA_ULONG);
	kstat_named_init(&statp->rx_alloc_buf, "rx_alloc_buf",
	    KSTAT_DATA_ULONG);
	kstat_named_init(&statp->rx_pool_empty, "rx_pool_empty",
	    KSTAT_DATA_ULONG);
	kstat_named_init(&statp->rx_num_bufs, "rx_num_bufs",
	    KSTAT_DATA_ULONG);

	kstat_install(dp->devKstats);

	return (DDI_SUCCESS);
}

/*
 * Probe and attach a vmxnet3 instance to the stack.
 *
 * Returns:
 *	DDI_SUCCESS or DDI_FAILURE.
 */
static int
vmxnet3_attach(dev_info_t *dip, ddi_attach_cmd_t cmd)
{
	vmxnet3_softc_t *dp;
	mac_register_t *macr;
	uint16_t vendorId, devId, ret16;
	uint32_t ret32;
	int ret, err;
	uint_t uret;

	if (cmd != DDI_ATTACH) {
		goto error;
	}

	/*
	 * Allocate the soft state
	 */
	dp = kmem_zalloc(sizeof (vmxnet3_softc_t), KM_SLEEP);
	ASSERT(dp);

	dp->dip = dip;
	dp->instance = ddi_get_instance(dip);
	dp->cur_mtu = ETHERMTU;
	dp->allow_jumbo = B_TRUE;
	dp->alloc_ok = VMXNET3_ALLOC_OK(dp);

	VMXNET3_DEBUG(dp, 1, "attach()\n");

	ddi_set_driver_private(dip, dp);

	/*
	 * Get access to the PCI bus configuration space
	 */
	if (pci_config_setup(dip, &dp->pciHandle) != DDI_SUCCESS) {
		VMXNET3_WARN(dp, "pci_config_setup() failed\n");
		goto error_soft_state;
	}

	/*
	 * Make sure the chip is a vmxnet3 device
	 */
	vendorId = pci_config_get16(dp->pciHandle, PCI_CONF_VENID);
	devId = pci_config_get16(dp->pciHandle, PCI_CONF_DEVID);
	if (vendorId != PCI_VENDOR_ID_VMWARE ||
	    devId != PCI_DEVICE_ID_VMWARE_VMXNET3) {
		VMXNET3_WARN(dp, "wrong PCI venid/devid (0x%x, 0x%x)\n",
		    vendorId, devId);
		goto error_pci_config;
	}

	/*
	 * Make sure we can access the registers through the I/O space
	 */
	ret16 = pci_config_get16(dp->pciHandle, PCI_CONF_COMM);
	ret16 |= PCI_COMM_IO | PCI_COMM_ME;
	pci_config_put16(dp->pciHandle, PCI_CONF_COMM, ret16);

	/*
	 * Map the I/O space in memory
	 */
	if (ddi_regs_map_setup(dip, 1, &dp->bar0, 0, 0, &vmxnet3_dev_attr,
	    &dp->bar0Handle) != DDI_SUCCESS) {
		VMXNET3_WARN(dp, "ddi_regs_map_setup() for BAR0 failed\n");
		goto error_pci_config;
	}

	if (ddi_regs_map_setup(dip, 2, &dp->bar1, 0, 0, &vmxnet3_dev_attr,
	    &dp->bar1Handle) != DDI_SUCCESS) {
		VMXNET3_WARN(dp, "ddi_regs_map_setup() for BAR1 failed\n");
		goto error_regs_map_0;
	}

	/*
	 * Check the version number of the virtual device
	 */
	if (VMXNET3_BAR1_GET32(dp, VMXNET3_REG_VRRS) & 1) {
		VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_VRRS, 1);
	} else {
		VMXNET3_WARN(dp, "incompatible h/w version\n");
		goto error_regs_map_1;
	}

	if (VMXNET3_BAR1_GET32(dp, VMXNET3_REG_UVRS) & 1) {
		VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_UVRS, 1);
	} else {
		VMXNET3_WARN(dp, "incompatible upt version\n");
		goto error_regs_map_1;
	}

	if (vmxnet3_kstat_init(dp) != DDI_SUCCESS) {
		VMXNET3_WARN(dp, "unable to initialize kstats");
		goto error_regs_map_1;
	}

	/*
	 * Read the MAC address from the device
	 */
	ret32 = VMXNET3_BAR1_GET32(dp, VMXNET3_REG_MACL);
	*((uint32_t *)(dp->macaddr + 0)) = ret32;
	ret32 = VMXNET3_BAR1_GET32(dp, VMXNET3_REG_MACH);
	*((uint16_t *)(dp->macaddr + 4)) = ret32;

	/*
	 * Register with the MAC framework
	 */
	if (!(macr = mac_alloc(MAC_VERSION))) {
		VMXNET3_WARN(dp, "mac_alloc() failed\n");
		goto error_kstat;
	}

	macr->m_type_ident = MAC_PLUGIN_IDENT_ETHER;
	macr->m_driver = dp;
	macr->m_dip = dip;
	macr->m_instance = 0;
	macr->m_src_addr = dp->macaddr;
	macr->m_dst_addr = NULL;
	macr->m_callbacks = &vmxnet3_mac_callbacks;
	macr->m_min_sdu = 0;
	macr->m_max_sdu = ETHERMTU;
	macr->m_margin = VLAN_TAGSZ;
	macr->m_pdata = NULL;
	macr->m_pdata_size = 0;

	ret = mac_register(macr, &dp->mac);
	mac_free(macr);
	if (ret != DDI_SUCCESS) {
		VMXNET3_WARN(dp, "mac_register() failed\n");
		goto error_kstat;
	}

	/*
	 * Register the interrupt(s) in this order of preference:
	 * MSI-X, MSI, INTx
	 */
	VMXNET3_BAR1_PUT32(dp, VMXNET3_REG_CMD, VMXNET3_CMD_GET_CONF_INTR);
	ret32 = VMXNET3_BAR1_GET32(dp, VMXNET3_REG_CMD);
	switch (ret32 & 0x3) {
	case VMXNET3_IT_AUTO:
	case VMXNET3_IT_MSIX:
		dp->intrType = DDI_INTR_TYPE_MSIX;
		err = ddi_intr_alloc(dip, &dp->intrHandle, dp->intrType, 0, 1,
		    &ret, DDI_INTR_ALLOC_STRICT);
		if (err == DDI_SUCCESS)
			break;
		VMXNET3_DEBUG(dp, 2, "DDI_INTR_TYPE_MSIX failed, err:%d\n",
		    err);
		/* FALLTHROUGH */
	case VMXNET3_IT_MSI:
		dp->intrType = DDI_INTR_TYPE_MSI;
		if (ddi_intr_alloc(dip, &dp->intrHandle, dp->intrType, 0, 1,
		    &ret, DDI_INTR_ALLOC_STRICT) == DDI_SUCCESS)
			break;
		VMXNET3_DEBUG(dp, 2, "DDI_INTR_TYPE_MSI failed\n");
		/* FALLTHROUGH */
	case VMXNET3_IT_INTX:
		dp->intrType = DDI_INTR_TYPE_FIXED;
		if (ddi_intr_alloc(dip, &dp->intrHandle, dp->intrType, 0, 1,
		    &ret, DDI_INTR_ALLOC_STRICT) == DDI_SUCCESS) {
			break;
		}
		VMXNET3_DEBUG(dp, 2, "DDI_INTR_TYPE_INTX failed\n");
		/* FALLTHROUGH */
	default:
		VMXNET3_WARN(dp, "ddi_intr_alloc() failed\n");
		goto error_mac;
	}
	dp->intrMaskMode = (ret32 >> 2) & 0x3;
	if (dp->intrMaskMode == VMXNET3_IMM_LAZY) {
		VMXNET3_WARN(dp, "Lazy masking is not supported\n");
		goto error_intr;
	}

	if (ddi_intr_get_pri(dp->intrHandle, &uret) != DDI_SUCCESS) {
		VMXNET3_WARN(dp, "ddi_intr_get_pri() failed\n");
		goto error_intr;
	}

	VMXNET3_DEBUG(dp, 2, "intrType=0x%x, intrMaskMode=0x%x, intrPrio=%u\n",
	    dp->intrType, dp->intrMaskMode, uret);

	/*
	 * Create a task queue to reset the device if it wedges.
	 */
	dp->resetTask = ddi_taskq_create(dip, "vmxnet3_reset_task", 1,
	    TASKQ_DEFAULTPRI, 0);
	if (!dp->resetTask) {
		VMXNET3_WARN(dp, "ddi_taskq_create() failed()\n");
		goto error_intr;
	}

	/*
	 * Initialize our mutexes now that we know the interrupt priority
	 * This _must_ be done before ddi_intr_enable()
	 */
	mutex_init(&dp->intrLock, NULL, MUTEX_DRIVER, DDI_INTR_PRI(uret));
	mutex_init(&dp->txLock, NULL, MUTEX_DRIVER, DDI_INTR_PRI(uret));
	mutex_init(&dp->rxPoolLock, NULL, MUTEX_DRIVER, DDI_INTR_PRI(uret));

	if (ddi_intr_add_handler(dp->intrHandle, vmxnet3_intr,
	    dp, NULL) != DDI_SUCCESS) {
		VMXNET3_WARN(dp, "ddi_intr_add_handler() failed\n");
		goto error_mutexes;
	}

	err = ddi_intr_get_cap(dp->intrHandle, &dp->intrCap);
	if (err != DDI_SUCCESS) {
		VMXNET3_WARN(dp, "ddi_intr_get_cap() failed %d", err);
		goto error_intr_handler;
	}

	if (dp->intrCap & DDI_INTR_FLAG_BLOCK) {
		err = ddi_intr_block_enable(&dp->intrHandle, 1);
		if (err != DDI_SUCCESS) {
			VMXNET3_WARN(dp, "ddi_intr_block_enable() failed, "
			    "err:%d\n", err);
			goto error_intr_handler;
		}
	} else {
		err = ddi_intr_enable(dp->intrHandle);
		if ((err != DDI_SUCCESS)) {
			VMXNET3_WARN(dp, "ddi_intr_enable() failed, err:%d\n",
			    err);
			goto error_intr_handler;
		}
	}

	return (DDI_SUCCESS);

error_intr_handler:
	(void) ddi_intr_remove_handler(dp->intrHandle);
error_mutexes:
	mutex_destroy(&dp->rxPoolLock);
	mutex_destroy(&dp->txLock);
	mutex_destroy(&dp->intrLock);
	ddi_taskq_destroy(dp->resetTask);
error_intr:
	(void) ddi_intr_free(dp->intrHandle);
error_mac:
	(void) mac_unregister(dp->mac);
error_kstat:
	kstat_delete(dp->devKstats);
error_regs_map_1:
	ddi_regs_map_free(&dp->bar1Handle);
error_regs_map_0:
	ddi_regs_map_free(&dp->bar0Handle);
error_pci_config:
	pci_config_teardown(&dp->pciHandle);
error_soft_state:
	kmem_free(dp, sizeof (vmxnet3_softc_t));
error:
	return (DDI_FAILURE);
}

/*
 * Detach a vmxnet3 instance from the stack.
 *
 * Returns:
 *	DDI_SUCCESS or DDI_FAILURE.
 */
static int
vmxnet3_detach(dev_info_t *dip, ddi_detach_cmd_t cmd)
{
	vmxnet3_softc_t *dp = ddi_get_driver_private(dip);
	unsigned int retries = 0;
	int ret;

	VMXNET3_DEBUG(dp, 1, "detach()\n");

	if (cmd != DDI_DETACH) {
		return (DDI_FAILURE);
	}

	while (dp->rx_num_bufs > 0) {
		if (retries++ < 10) {
			VMXNET3_WARN(dp, "rx pending (%u), waiting 1 second\n",
			    dp->rx_num_bufs);
			delay(drv_usectohz(1000000));
		} else {
			VMXNET3_WARN(dp, "giving up\n");
			return (DDI_FAILURE);
		}
	}

	if (dp->intrCap & DDI_INTR_FLAG_BLOCK) {
		ret = ddi_intr_block_disable(&dp->intrHandle, 1);
	} else {
		ret = ddi_intr_disable(dp->intrHandle);
	}
	if (ret != DDI_SUCCESS) {
		VMXNET3_WARN(dp, "unable to disable interrupts");
		return (DDI_FAILURE);
	}
	if (ddi_intr_remove_handler(dp->intrHandle) != DDI_SUCCESS) {
		VMXNET3_WARN(dp, "unable to remove interrupt handler");
		return (DDI_FAILURE);
	}
	(void) ddi_intr_free(dp->intrHandle);

	VERIFY(mac_unregister(dp->mac) == 0);

	kstat_delete(dp->devKstats);

	if (dp->mfTable.buf) {
		vmxnet3_free_dma_mem(&dp->mfTable);
	}

	mutex_destroy(&dp->rxPoolLock);
	mutex_destroy(&dp->txLock);
	mutex_destroy(&dp->intrLock);
	ddi_taskq_destroy(dp->resetTask);

	ddi_regs_map_free(&dp->bar1Handle);
	ddi_regs_map_free(&dp->bar0Handle);
	pci_config_teardown(&dp->pciHandle);

	kmem_free(dp, sizeof (vmxnet3_softc_t));

	return (DDI_SUCCESS);
}

/*
 * Structures used by the module loader
 */

#define	VMXNET3_IDENT "VMware Ethernet v3 " VMXNET3_DRIVER_VERSION_STRING

DDI_DEFINE_STREAM_OPS(
	vmxnet3_dev_ops,
	nulldev,
	nulldev,
	vmxnet3_attach,
	vmxnet3_detach,
	nodev,
	NULL,
	D_NEW | D_MP,
	NULL,
	ddi_quiesce_not_supported);

static struct modldrv vmxnet3_modldrv = {
	&mod_driverops,		/* drv_modops */
	VMXNET3_IDENT,		/* drv_linkinfo */
	&vmxnet3_dev_ops	/* drv_dev_ops */
};

static struct modlinkage vmxnet3_modlinkage = {
	MODREV_1,			/* ml_rev */
	{ &vmxnet3_modldrv, NULL }	/* ml_linkage */
};

/* Module load entry point */
int
_init(void)
{
	int ret;

	mac_init_ops(&vmxnet3_dev_ops, VMXNET3_MODNAME);
	ret = mod_install(&vmxnet3_modlinkage);
	if (ret != DDI_SUCCESS) {
		mac_fini_ops(&vmxnet3_dev_ops);
	}

	return (ret);
}

/* Module unload entry point */
int
_fini(void)
{
	int ret;

	ret = mod_remove(&vmxnet3_modlinkage);
	if (ret == DDI_SUCCESS) {
		mac_fini_ops(&vmxnet3_dev_ops);
	}

	return (ret);
}

/* Module info entry point */
int
_info(struct modinfo *modinfop)
{
	return (mod_info(&vmxnet3_modlinkage, modinfop));
}
