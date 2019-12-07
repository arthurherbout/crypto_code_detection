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

#include <syslog.h>
#include <errno.h>
#include <unistd.h>
#include <stropts.h>

#include "mp_utils.h"

#include <libdevinfo.h>

/*
 * Checks whether there is online path or not.
 *  - no path found returns -1.
 *  - online/standby path found returns 1.
 *  - path exists but no online/standby path found returns 0.
 */
static int checkAvailablePath(di_node_t node)
{
	di_path_t path;
	di_path_state_t state;

	if ((path = di_path_client_next_path(node, DI_PATH_NIL))
	    == DI_PATH_NIL) {
		log(LOG_INFO, "checkAvailalblePath()",
		    " - No path found");
		return (-1);
	}

	do {
		/* ignore the path that is neither online nor standby. */
		if (((state = di_path_state(path)) == DI_PATH_STATE_ONLINE) ||
		    (state == DI_PATH_STATE_STANDBY)) {
			return (1);
		}
	} while ((path = di_path_client_next_path(node, path)) != DI_PATH_NIL);

	/* return 0 for the case that there is no online path to the node. */
	log(LOG_INFO, "checkAvailalblePath()", " - No online path found");
	return (0);
}

static int getOidList(di_node_t root_node, MP_OID_LIST *pOidList)
{
	int numNodes = 0, state;

	int instNum;
	int majorNum;
	MP_UINT64 osn;

	di_node_t sv_node	= DI_NODE_NIL;
	di_node_t sv_child_node = DI_NODE_NIL;

	int haveList = (NULL != pOidList);


	log(LOG_INFO, "getOidList()", " - enter");


	sv_node = di_drv_first_node("scsi_vhci", root_node);
	if (DI_NODE_NIL == sv_node) {
		log(LOG_INFO, "getOidList()",
		    " - di_drv_first_node() failed");

		return (-1);
	}

	sv_child_node = di_child_node(sv_node);

	while (DI_NODE_NIL != sv_child_node) {

		/* skip the node which is offline, down or detached. */
		state = di_state(sv_child_node);
		if ((state & DI_DEVICE_DOWN) ||
		    (state & DI_DEVICE_OFFLINE)) {
			sv_child_node = di_sibling_node(sv_child_node);
			continue;
		}

		/*
		 * skip if the node doesn't have any path avaialble.
		 * If any path is found from the DINFOCACHE snaphost
		 * that means the driver keeps track of the path regadless
		 * of state.
		 */
		if (checkAvailablePath(sv_child_node) == -1) {
			sv_child_node = di_sibling_node(sv_child_node);
			continue;
		}

		if (haveList && (numNodes < pOidList->oidCount)) {
			instNum = di_instance(sv_child_node);
			majorNum = di_driver_major(sv_child_node);

			log(LOG_INFO, "getOidList()",
			    "instNum = %d", instNum);
			log(LOG_INFO, "getOidList()",
			    "majorNum = %d", majorNum);

			osn = 0;
			osn = MP_STORE_INST_TO_ID(instNum, osn);
			osn = MP_STORE_MAJOR_TO_ID(majorNum, osn);

			pOidList->oids[numNodes].objectType =
			    MP_OBJECT_TYPE_MULTIPATH_LU;

			pOidList->oids[numNodes].ownerId =
			    g_pluginOwnerID;

			pOidList->oids[numNodes].objectSequenceNumber =
			    osn;
		}

		++numNodes;

		sv_child_node = di_sibling_node(sv_child_node);
	}

	log(LOG_INFO,
	    "getOidList()",
	    " - numNodes: %d",
	    numNodes);



	log(LOG_INFO, "getOidList()", " - exit");

	return (numNodes);
}


MP_STATUS
MP_GetMultipathLusPlugin(MP_OID_LIST **ppList)
{
	di_node_t root_node	= DI_NODE_NIL;
	MP_OID_LIST *pOidList   = NULL;

	int numNodes = 0;
	int i = 0;

	log(LOG_INFO, "MP_GetMultipathLusPlugin()", " - enter");


	root_node = di_init("/", DINFOCACHE);
	if (DI_NODE_NIL == root_node) {
		log(LOG_INFO, "MP_GetMultipathLusPlugin()",
		    " - di_init() failed");

		return (MP_STATUS_FAILED);
	}

	numNodes = getOidList(root_node, NULL);

	if (numNodes < 0) {

		log(LOG_INFO,
		    "MP_GetMultipathLusPlugin()",
		    " - unable to get OID list.");

		log(LOG_INFO, "MP_GetMultipathLusPlugin()",
		    " - error exit");

		di_fini(root_node);

		return (MP_STATUS_FAILED);
	}

	if (0 == numNodes) {

		pOidList = createOidList(1);
		if (NULL == pOidList) {

			log(LOG_INFO,
			    "MP_GetMultipathLusPlugin()",
			    " - unable to create OID list.");

			di_fini(root_node);

			return (MP_STATUS_INSUFFICIENT_MEMORY);
		}

		pOidList->oids[0].objectType =
		    MP_OBJECT_TYPE_MULTIPATH_LU;

		pOidList->oids[0].ownerId =
		    g_pluginOwnerID;

		*ppList = pOidList;

		log(LOG_INFO, "MP_GetMultipathLusPlugin()",
		    " - returning empty list.");

		di_fini(root_node);

		return (MP_STATUS_SUCCESS);
	}

	*ppList = createOidList(numNodes);
	if (NULL == *ppList) {
		log(LOG_INFO, "MP_GetMultipathLusPlugin()",
		    "no memory for *ppList");
		log(LOG_INFO, "MP_GetMultipathLusPlugin()",
		    " - error exit");
		return (MP_STATUS_INSUFFICIENT_MEMORY);
	}

	(*ppList)->oidCount = numNodes;

	numNodes = getOidList(root_node, *ppList);

	for (i = 0; i < (*ppList)->oidCount; i++) {

		log(LOG_INFO, "MP_GetMultipathLusPlugin()",
		    "(*ppList)->oids[%d].objectType           = %d",
		    i, (*ppList)->oids[i].objectType);
		log(LOG_INFO, "MP_GetMultipathLusPlugin()",
		    "(*ppList)->oids[%d].ownerId              = %d",
		    i, (*ppList)->oids[i].ownerId);
		log(LOG_INFO, "MP_GetMultipathLusPlugin()",
		    "(*ppList)->oids[%d].objectSequenceNumber = %llx",
		    i, (*ppList)->oids[i].objectSequenceNumber);
	}


	di_fini(root_node);

	log(LOG_INFO, "MP_GetMultipathLusPlugin()", " - exit");

	return (MP_STATUS_SUCCESS);

}
