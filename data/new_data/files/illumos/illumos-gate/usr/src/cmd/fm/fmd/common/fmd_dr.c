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
 * Copyright (c) 2004, 2010, Oracle and/or its affiliates. All rights reserved.
 */

/*
 * FMD Dynamic Reconfiguration (DR) Event Handling
 *
 * Fault manager scheme plug-ins must track characteristics of individual
 * pieces of hardware.  As these components can be added or removed by a DR
 * operation, we need to provide a means by which plug-ins can determine when
 * they need to re-examine the current configuration.  We provide a simple
 * mechanism whereby this task can be implemented using lazy evaluation: a
 * simple 64-bit generation counter is maintained and incremented on *any* DR.
 * Schemes can store the generation number in scheme-specific data structures,
 * and then revalidate their contents if the current generation number has
 * changed since the resource information was cached.  This method saves time,
 * avoids the complexity of direct participation in DR, avoids the need for
 * resource-specific processing of DR events, and is relatively easy to port
 * to other systems that support dynamic reconfiguration.
 *
 * The dr generation is only incremented in response to hardware changes.  Since
 * ASRUs can be in any scheme, including the device scheme, we must also be
 * aware of software configuration changes which may affect the resource cache.
 * In addition, we take a snapshot of the topology whenever a reconfiguration
 * event occurs and notify any modules of the change.
 */

#include <sys/types.h>
#include <sys/sunddi.h>
#include <sys/sysevent/dr.h>
#include <sys/sysevent/eventdefs.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libsysevent.h>

#undef MUTEX_HELD
#undef RW_READ_HELD
#undef RW_WRITE_HELD

#include <fmd_asru.h>
#include <fmd_error.h>
#include <fmd_event.h>
#include <fmd_fmri.h>
#include <fmd_module.h>
#include <fmd_subr.h>
#include <fmd_topo.h>
#include <fmd.h>

void
fmd_dr_event(sysevent_t *sep)
{
	uint64_t gen;
	fmd_event_t *e;
	const char *class = sysevent_get_class_name(sep);
	const char *subclass = sysevent_get_subclass_name(sep);
	hrtime_t evtime;
	fmd_topo_t *ftp, *prev;

	if (strcmp(class, EC_DR) == 0) {
		if (strcmp(subclass, ESC_DR_AP_STATE_CHANGE) != 0 &&
		    strcmp(subclass, ESC_DR_TARGET_STATE_CHANGE) != 0)
			return;
	/* LINTED: E_NOP_IF_STMT */
	} else if (strcmp(class, EC_DEVFS) == 0) {
		/*
		 * A devfs configuration event can change the topology,
		 * as disk nodes only exist when the device is configured.
		 */
	} else if (strcmp(class, EC_PLATFORM) == 0) {
		/*
		 * Since we rely on the SP to enumerate fans,
		 * power-supplies and sensors/leds, it would be prudent
		 * to take a new snapshot if the SP resets.
		 */
		if (strcmp(subclass, ESC_PLATFORM_SP_RESET) != 0)
			return;
	} else if (strcmp(class, EC_DEV_ADD) == 0 ||
	    strcmp(class, EC_DEV_REMOVE) == 0) {
		if (strcmp(subclass, ESC_DISK) != 0)
			return;
	} else
		return;

	/*
	 * Take a topo snapshot and notify modules of the change.  Picking an
	 * accurate time here is difficult.  On one hand, we have the timestamp
	 * of the underlying sysevent, indicating when the reconfiguration event
	 * occurred.  On the other hand, we are taking the topo snapshot
	 * asynchronously, and hence the timestamp of the snapshot is the
	 * current time.  Pretending this topo snapshot was valid at the time
	 * the sysevent was posted seems wrong, so we instead opt for the
	 * current time as an upper bound on the snapshot validity.
	 *
	 * Along these lines, we keep track of the last time we dispatched a
	 * topo snapshot.  If the sysevent occurred before the last topo
	 * snapshot, then don't bother dispatching another topo change event.
	 * We've already indicated (to the best of our ability) the change in
	 * topology.  This prevents endless topo snapshots in response to a
	 * flurry of sysevents.
	 */
	sysevent_get_time(sep, &evtime);
	prev = fmd_topo_hold();
	if (evtime <= prev->ft_time_begin &&
	    fmd.d_clockops == &fmd_timeops_native) {
		fmd_topo_rele(prev);
		return;
	}
	fmd_topo_rele(prev);

	(void) pthread_mutex_lock(&fmd.d_stats_lock);
	gen = fmd.d_stats->ds_dr_gen.fmds_value.ui64++;
	(void) pthread_mutex_unlock(&fmd.d_stats_lock);

	TRACE((FMD_DBG_XPRT, "dr event %p, gen=%llu", (void *)sep, gen));
	fmd_topo_update();

	ftp = fmd_topo_hold();
	e = fmd_event_create(FMD_EVT_TOPO, ftp->ft_time_end, NULL, ftp);
	fmd_modhash_dispatch(fmd.d_mod_hash, e);
}
