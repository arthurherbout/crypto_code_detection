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
 * Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 *
 */

/* $Id: lpstat.c 173 2006-05-25 04:52:06Z njacobs $ */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <libintl.h>
#include <ctype.h>
#include <pwd.h>
#include <papi.h>
#include <uri.h>
#include "common.h"
#include "lp.h"

static void
usage(char *program)
{
	char *name;

	if ((name = strrchr(program, '/')) == NULL)
		name = program;
	else
		name++;

	fprintf(stdout, gettext("Usage: %s [-d] [-r] [-s] [-t] [-a [list]] "
	    "[-c [list]] [-o [list] [-l]] [-R [list] [-l]] "
	    "[-p [list] [-D] [-l]] [-v [list]] [-S [list] [-l]] "
	    "[-f [list] [-l]] [-u list]\n"),
	    name);
	exit(1);
}

static char *
nctime(time_t *t)
{
	static char buf[64];
	struct tm *tm = localtime(t);

	(void) strftime(buf, sizeof (buf), "%c", tm);

	return (buf);
}

static char *
printer_name(papi_printer_t printer)
{
	papi_attribute_t **attributes = papiPrinterGetAttributeList(printer);
	char *result = NULL;

	if (attributes != NULL)
		papiAttributeListGetString(attributes, NULL,
		    "printer-name", &result);

	return (result);
}

static int
lpstat_default_printer(papi_encryption_t encryption)
{
	papi_status_t status;
	papi_service_t svc = NULL;
	papi_printer_t p = NULL;
	char *name = NULL;

	status = papiServiceCreate(&svc, NULL, NULL, NULL, cli_auth_callback,
	    encryption, NULL);
	if (status == PAPI_OK) {
		char *req[] = { "printer-name", NULL };

		status = papiPrinterQuery(svc, DEFAULT_DEST, req, NULL, &p);
		if (p != NULL)
			name = printer_name(p);
	}
	if (name != NULL)
		printf(gettext("system default printer: %s\n"), name);
	else
		printf(gettext("no system default destination\n"));
	papiPrinterFree(p);
	papiServiceDestroy(svc);

	return (0);
}

static int
lpstat_service_status(papi_encryption_t encryption)
{
	papi_status_t status;
	papi_service_t svc = NULL;
	char *name = NULL;

	if (((name = getenv("PAPI_SERVICE_URI")) == NULL) &&
	    ((name = getenv("IPP_SERVER")) == NULL) &&
	    ((name = getenv("CUPS_SERVER")) == NULL))
		name = DEFAULT_SERVICE_URI;

	status = papiServiceCreate(&svc, name, NULL, NULL, cli_auth_callback,
	    encryption, NULL);
	if (status != PAPI_OK) {
		printf(gettext("scheduler is not running\n"));
	} else
		printf(gettext("scheduler is running\n"));
	papiServiceDestroy(svc);

	return (0);
}

static char *
get_device_uri(papi_service_t svc, char *name)
{
	papi_status_t status;
	papi_printer_t p = NULL;
	char *keys[] = { "device-uri", NULL };
	char *result = NULL;

	status = papiPrinterQuery(svc, name, keys, NULL, &p);
	if ((status == PAPI_OK) && (p != NULL)) {
		papi_attribute_t **attrs = papiPrinterGetAttributeList(p);

		(void) papiAttributeListGetString(attrs, NULL,
		    "device-uri", &result);
		if (result != NULL)
			result = strdup(result);

		papiPrinterFree(p);
	}

	return (result);
}

static void
print_description(papi_attribute_t **list, char *printer_name)
{
	char *str = "";

	(void) papiAttributeListGetString(list, NULL,
	    "printer-info", &str);

	/*
	 * If no printer-info is read then
	 * by default the printer-info is <printer-name>@<server>
	 */
	if (str[0] == '\0') {
		char *uri = NULL;
		uri_t *u = NULL;

		(void) papiAttributeListGetString(list, NULL,
		    "printer-uri-supported", &uri);

		if ((uri != NULL) && (uri_from_string(uri, &u) == 0)) {
			char *nodename = localhostname();

			if ((u->host == NULL) ||
			    (strcasecmp(u->host, "localhost") == 0) ||
			    (strcasecmp(u->host, nodename) == 0))
				printf(gettext("\tDescription:\n"));
			else
				printf(gettext("\tDescription: %s@%s\n"),
				    printer_name, u->host);

			uri_free(u);
		} else
			printf(gettext("\tDescription:\n"));
	} else
		printf(gettext("\tDescription: %s\n"), str);
}

static char *report_device_keys[] = { "printer-name", "printer-uri-supported",
					NULL };
/* ARGSUSED2 */
static int
report_device(papi_service_t svc, char *name, papi_printer_t printer,
		int verbose, int description)
{
	papi_status_t status;
	papi_attribute_t **attrs = papiPrinterGetAttributeList(printer);
	char *uri = NULL;
	char *device = NULL;
	uri_t *u = NULL;

	if (name == NULL) {
		status = papiAttributeListGetString(attrs, NULL,
		    "printer-name", &name);
		if (status != PAPI_OK)
			status = papiAttributeListGetString(attrs, NULL,
			    "printer-uri-supported", &name);
	}

	if (name == NULL)
		return (-1);

	(void) papiAttributeListGetString(attrs, NULL,
	    "printer-uri-supported", &uri);

	if ((uri != NULL) && (uri_from_string(uri, &u) == 0)) {
		char *nodename = localhostname();

		if ((u->host == NULL) ||
		    (strcasecmp(u->host, "localhost") == 0) ||
		    (strcasecmp(u->host, nodename) == 0))
			device = get_device_uri(svc, name);

		if (device != NULL) {
			printf(gettext("device for %s: %s\n"), name, device);
			return (0);
		} else if (uri != NULL) {
			printf(gettext("system for %s: %s (as %s)\n"), name,
			    u->host?u->host:"localhost", uri);
			return (0);
		}

		uri_free(u);
	}

	return (0);
}

static char *report_accepting_keys[] = { "printer-name",
			"printer-uri-supported", "printer-is-accepting-jobs",
			"printer-up-time", "printer-state-time",
			"lpsched-reject-date", "lpsched-reject-reason", NULL };
/* ARGSUSED2 */
static int
report_accepting(papi_service_t svc, char *name, papi_printer_t printer,
		int verbose, int description)
{
	papi_status_t status;
	papi_attribute_t **attrs = papiPrinterGetAttributeList(printer);
	time_t curr;
	char boolean = PAPI_FALSE;

	if (name == NULL) {
		status = papiAttributeListGetString(attrs, NULL,
		    "printer-name", &name);
		if (status != PAPI_OK)
			status = papiAttributeListGetString(attrs, NULL,
			    "printer-uri-supported", &name);
	}
	if (name == NULL)
		return (-1);

	(void) papiAttributeListGetBoolean(attrs, NULL,
	    "printer-is-accepting-jobs", &boolean);
	(void) time(&curr);
	(void) papiAttributeListGetDatetime(attrs, NULL,
	    "printer-up-time", &curr);
	(void) papiAttributeListGetDatetime(attrs, NULL,
	    "printer-state-time", &curr);
	(void) papiAttributeListGetDatetime(attrs, NULL,
	    "lpsched-reject-date", &curr);

	if (boolean == PAPI_TRUE) {
		printf(gettext("%s accepting requests since %s\n"),
		    name, nctime(&curr));
	} else {
		char *reason = "unknown reason";

		(void) papiAttributeListGetString(attrs, NULL,
		    "lpsched-reject-reason", &reason);

		printf(gettext("%s not accepting requests since %s\n\t%s\n"),
		    name, nctime(&curr), reason);
	}

	return (0);
}

static char *report_class_keys[] = { "printer-name", "printer-uri-supported",
					"member-names", NULL };
/* ARGSUSED2 */
static int
report_class(papi_service_t svc, char *name, papi_printer_t printer,
		int verbose, int description)
{
	papi_status_t status;
	papi_attribute_t **attrs = papiPrinterGetAttributeList(printer);
	char *member = NULL;
	void *iter = NULL;

	status = papiAttributeListGetString(attrs, &iter,
	    "member-names", &member);
	if (status == PAPI_NOT_FOUND)	/* it's not a class */
		return (0);

	if (name == NULL) {
		status = papiAttributeListGetString(attrs, NULL,
		    "printer-name", &name);
		if (status != PAPI_OK)
			status = papiAttributeListGetString(attrs, NULL,
			    "printer-uri-supported", &name);
	}
	if (name == NULL)
		return (-1);

	printf(gettext("members of class %s:\n\t%s\n"), name, member);
	while (papiAttributeListGetString(attrs, &iter, NULL, &member)
	    == PAPI_OK)
		printf("\t%s\n", member);

	return (0);
}

static int
get_remote_hostname(papi_attribute_t **attrs, char **host)
{
	char *uri = NULL;
	uri_t *u;
	char *nodename;

	*host = NULL;
	(void) papiAttributeListGetString(attrs, NULL,
	    "job-originating-host-name", host);
	(void) papiAttributeListGetString(attrs, NULL,
	    "printer-uri-supported", &uri);
	if (*host == NULL) {
		if (uri != NULL) {
			if (uri_from_string(uri, &u) == 0) {
				if (u->host == NULL) {
					uri_free(u);
					return (0);
				}
				*host = strdup(u->host);
				uri_free(u);
			} else {
				return (0);
			}
		} else {
			return (0);
		}
	}
	nodename = localhostname();
	if ((strcasecmp(*host, "localhost") == 0) ||
	    (strcasecmp(*host, nodename) == 0)) {
		return (0);
	}
	return (1);
}

static char *report_printer_keys[] = { "printer-name",
			"printer-uri-supported", "printer-state",
			"printer-up-time", "printer-state-time",
			"lpsched-disable-date", "printer-state-reasons",
			"lpsched-disable-reason", NULL };
/* ARGSUSED2 */
static int
report_printer(papi_service_t svc, char *name, papi_printer_t printer,
		int verbose, int description)
{
	papi_status_t status;
	papi_attribute_t **attrs = papiPrinterGetAttributeList(printer);
	time_t curr;
	int32_t pstat = 0;
	char *member = NULL;
	papi_job_t *j = NULL;

	status = papiAttributeListGetString(attrs, NULL,
	    "member-names", &member);
	if (status == PAPI_OK)	/* it's a class */
		return (0);

	if (name == NULL) {
		status = papiAttributeListGetString(attrs, NULL,
		    "printer-name", &name);
		if (status != PAPI_OK)
			status = papiAttributeListGetString(attrs, NULL,
			    "printer-uri-supported", &name);
	}
	if (name == NULL)
		return (-1);

	printf(gettext("printer %s "), name);

	status = papiAttributeListGetInteger(attrs, NULL,
	    "printer-state", &pstat);

	switch (pstat) {
	case 0x03:	/* idle */
		printf(gettext("is idle. enabled"));
		break;
	case 0x04: /* processing */
	case 0x06: /* faulted printing */
		status = papiPrinterListJobs(svc, name, NULL,
		    0, 0, &j);

		if (status == PAPI_OK) {
			if (j != NULL) {
				int i = 0;
				int32_t jobid = 0;
				int32_t jstate = 0;

				for (i = 0; j[i] != NULL; ++i) {
					papi_attribute_t **attr =
					    papiJobGetAttributeList(j[i]);

					papiAttributeListGetInteger(attr,
					    NULL, "job-state", &jstate);
					papiAttributeListGetInteger(attr,
					    NULL, "job-id", &jobid);
					/*
					 * For lpd protocol "job-id-requested"
					 * should be read.
					 */
					papiAttributeListGetInteger(attr,
					    NULL, "job-id-requested", &jobid);

					/*
					 * When lpd protocol is used job-state
					 * cannot be retrieved, therefore
					 * job-state will be 0.
					 * When ipp protocol is used, the
					 * active/printing job-state will be
					 * RS_PRINTING (0x0008) post s10u5.
					 * For pre-s10u5 job-state will be
					 * RS_ACTIVE (0x05). So print only when
					 * the job-state is RS_PRINTING (0x0008)
					 * or RS_ACTIVE (0x05) or 0
					 */
					if ((jstate == 0x0008) ||
					    (jstate == 0x05) ||
					    (jstate == 0)) {
						if (pstat == 0x04)
							printf(gettext
							    ("now printing"\
							    " %s-%d. enabled"),
							    name, jobid);
						if (pstat == 0x06)
							printf(gettext
							    ("faulted printing"\
							    " %s-%d. enabled"),
							    name, jobid);
						break;
					}
				}
				papiJobListFree(j);
			}
		}
		break;
	case 0x05:	/* stopped */
		printf(gettext("disabled"));
		break;
	case 0x07:	/* faulted printer */
		printf(gettext("faulted. enabled"));
		break;
	case 0x08:	/* waiting for auto retry */
		printf(gettext("waiting for auto-retry."));
		break;
	default:
		printf(gettext("unknown state(0x%x)."), pstat);
		break;
	}

	if (pstat == 0x08)
		printf(gettext(" available.\n"));
	else {
		(void) time(&curr);
		(void) papiAttributeListGetDatetime(attrs, NULL,
		    "printer-up-time", &curr);
		(void) papiAttributeListGetDatetime(attrs, NULL,
		    "printer-state-time", &curr);
		(void) papiAttributeListGetDatetime(attrs, NULL,
		    "lpsched-disable-date", &curr);
		printf(gettext(" since %s. available.\n"), nctime(&curr));
	}

	if ((pstat == 0x05) ||
	    (pstat == 0x06) ||
	    (pstat == 0x07) ||
	    (pstat == 0x08)) {
		char *reason = "unknown reason";

		(void) papiAttributeListGetString(attrs, NULL,
		    "printer-state-reasons", &reason);
		(void) papiAttributeListGetString(attrs, NULL,
		    "lpsched-disable-reason", &reason);
		printf(gettext("\t%s\n"), reason);
	}

	if (verbose == 1) {
		void *iter;
		char *str;
		char *host = NULL;

		if ((get_remote_hostname(attrs, &host)) != 0) {
			(void) printf(
			    gettext("\tRemote Name: %s\n\tRemote Server: "
			    "%s\n"), name, host);
			free(host);
			return (0);
		}
		str = "";
		(void) papiAttributeListGetString(attrs, NULL,
		    "form-ready", &str);
		printf(gettext("\tForm mounted: %s\n"), str);

		str = "";
		iter = NULL;
		(void) papiAttributeListGetString(attrs, &iter,
		    "document-format-supported", &str);
		printf(gettext("\tContent types: %s"), str);
		while (papiAttributeListGetString(attrs, &iter, NULL, &str)
		    == PAPI_OK)
			printf(", %s", str);
		printf("\n");

		/* Display the printer description */
		print_description(attrs, name);

		str = "";
		iter = NULL;
		(void) papiAttributeListGetString(attrs, &iter,
		    "lpsched-printer-type", &str);
		printf(gettext("\tPrinter types: %s"), str);
		while (papiAttributeListGetString(attrs, &iter, NULL, &str)
		    == PAPI_OK)
			printf(", %s", str);
		printf("\n");

		str = "";
		(void) papiAttributeListGetString(attrs, NULL,
		    "lpsched-dial-info", &str);
		printf(gettext("\tConnection: %s\n"),
		    ((str[0] == '\0') ? gettext("direct") : str));

		str = "";
		(void) papiAttributeListGetString(attrs, NULL,
		    "lpsched-interface-script", &str);
		printf(gettext("\tInterface: %s\n"), str);

		str = NULL;
		(void) papiAttributeListGetString(attrs, NULL,
		    "ppd-file-uri", &str);
		(void) papiAttributeListGetString(attrs, NULL,
		    "lpsched-ppd-source-path", &str);
		if (str != NULL)
			printf(gettext("\tPPD: %s\n"), str);

		str = NULL;
		(void) papiAttributeListGetString(attrs, NULL,
		    "lpsched-fault-alert-command", &str);
		if (str != NULL)
			printf(gettext("\tOn fault: %s\n"), str);

		str = "";
		(void) papiAttributeListGetString(attrs, NULL,
		    "lpsched-fault-recovery", &str);
		printf(gettext("\tAfter fault: %s\n"),
		    ((str[0] == '\0') ? gettext("continue") : str));

		str = "(all)";
		iter = NULL;
		(void) papiAttributeListGetString(attrs, &iter,
		    "requesting-user-name-allowed", &str);
		printf(gettext("\tUsers allowed:\n\t\t%s\n"),
		    ((str[0] == '\0') ? gettext("(none)") : str));
		if ((str != NULL) && (str[0] != '\0'))
			while (papiAttributeListGetString(attrs, &iter, NULL,
			    &str) == PAPI_OK)
				printf("\t\t%s\n", str);

		str = NULL;
		iter = NULL;
		(void) papiAttributeListGetString(attrs, &iter,
		    "requesting-user-name-denied", &str);
		if (str != NULL) {
			printf(gettext("\tUsers denied:\n\t\t%s\n"),
			    ((str[0] == '\0') ? gettext("(none)") : str));
			if ((str != NULL) && (str[0] != '\0'))
				while (papiAttributeListGetString(attrs, &iter,
				    NULL, &str) == PAPI_OK)
					printf("\t\t%s\n", str);
		}

		str = "none";
		iter = NULL;
		(void) papiAttributeListGetString(attrs, &iter,
		    "form-supported", &str);
		printf(gettext("\tForms allowed:\n\t\t(%s)\n"),
		    ((str[0] == '\0') ? gettext("none") : str));
		if ((str != NULL) && (str[0] != '\0'))
			while (papiAttributeListGetString(attrs, &iter, NULL,
			    &str) == PAPI_OK)
				printf("\t\t(%s)\n", str);

		str = "";
		iter = NULL;
		(void) papiAttributeListGetString(attrs, &iter,
		    "media-supported", &str);
		printf(gettext("\tMedia supported:\n\t\t%s\n"),
		    ((str[0] == '\0') ? gettext("(none)") : str));
		if ((str != NULL) && (str[0] != '\0'))
			while (papiAttributeListGetString(attrs, &iter, NULL,
			    &str) == PAPI_OK)
				printf("\t\t%s\n", str);

		str = "";
		(void) papiAttributeListGetString(attrs, NULL,
		    "job-sheets-supported", &str);
		if ((strcasecmp(str, "none")) == 0)
			str = gettext("page never printed");
		else if (strcasecmp(str, "optional") == 0)
			str = gettext("not required");
		else
			str = gettext("required");

		printf(gettext("\tBanner %s\n"), str);


		str = "";
		iter = NULL;
		(void) papiAttributeListGetString(attrs, &iter,
		    "lpsched-print-wheels", &str);
		printf(gettext("\tCharacter sets:\n\t\t%s\n"),
		    ((str[0] == '\0') ? gettext("(none)") : str));
		if ((str != NULL) && (str[0] != '\0'))
			while (papiAttributeListGetString(attrs, &iter, NULL,
			    &str) == PAPI_OK)
				printf("\t\t%s\n", str);

		printf(gettext("\tDefault pitch:\n"));
		printf(gettext("\tDefault page size:\n"));
		printf(gettext("\tDefault port setting:\n"));

		str = "";
		iter = NULL;
		(void) papiAttributeListGetString(attrs, &iter,
		    "lpsched-options", &str);
		if (str != NULL) {
			printf(gettext("\tOptions: %s"), str);
			while (papiAttributeListGetString(attrs, &iter, NULL,
			    &str) == PAPI_OK)
				printf(", %s", str);
			printf("\n");
		}

	} else if (description == 1)
		/* Display printer description */
		print_description(attrs, name);
	else if (verbose > 1)
		papiAttributeListPrint(stdout, attrs, "\t");

	if (verbose > 0)
		printf("\n");

	return (0);
}

static int
printer_query(char *name, int (*report)(papi_service_t, char *, papi_printer_t,
					int, int), papi_encryption_t encryption,
		int verbose, int description)
{
	int result = 0, i = 0;
	papi_status_t status;
	papi_service_t svc = NULL;
	char **list = getlist(name, LP_WS, LP_SEP);

	if (list == NULL) {
		list = (char **)malloc(sizeof (char *));
		list[0] = name;
	}

	/*
	 * The for loop executes once for every printer
	 * entry in list. If list is NULL that implies
	 * name is also NULL, the loop runs only one time.
	 */

	for (i = 0; name == NULL || list[i] != NULL; i++) {
		name = list[i];

		status = papiServiceCreate(&svc, name, NULL, NULL,
		    cli_auth_callback, encryption, NULL);
		if (status != PAPI_OK) {
			if (status == PAPI_NOT_FOUND)
				fprintf(stderr,
				    gettext("%s: unknown printer\n"),
				    name ? name : "(NULL)");
			else
				fprintf(stderr, gettext(
				    "Failed to contact service for %s: %s\n"),
				    name ? name : "(NULL)",
				    verbose_papi_message(svc, status));
			papiServiceDestroy(svc);
			result--;
			continue;
		}

		if (name == NULL) { /* all */
			char **interest = interest_list(svc);

			if (interest != NULL) {
				int i;

				for (i = 0; interest[i] != NULL; i++)
					result += printer_query(interest[i],
					    report, encryption, verbose,
					    description);
			}
		} else {
			papi_printer_t printer = NULL;
			char **keys = NULL;

			/*
			 * Limit the query to only required data
			 * to reduce the need to go remote for
			 * information.
			 */
			if (report == report_device)
				keys = report_device_keys;
			else if (report == report_class)
				keys = report_class_keys;
			else if (report == report_accepting)
				keys = report_accepting_keys;
			else if ((report == report_printer) && (verbose == 0))
				keys = report_printer_keys;

			status = papiPrinterQuery(svc, name, keys,
			    NULL, &printer);
			if (status != PAPI_OK) {
				fprintf(stderr, gettext(
				    "Failed to get printer info for %s: %s\n"),
				    name, verbose_papi_message(svc, status));
				papiServiceDestroy(svc);
				result--;
				continue;
			}

			if (printer != NULL)
				result += report(svc, name, printer, verbose,
				    description);

			papiPrinterFree(printer);
		}

		papiServiceDestroy(svc);

		if (name == NULL)
			break;
	}

	freelist(list);

	return (result);
}

static int
match_user(char *user, char **list)
{
	int i;

	for (i = 0; list[i] != NULL; i++) {
		if (strcmp(user, list[i]) == 0)
			return (0);
	}

	return (-1);
}

static char **users = NULL;

static int
report_job(char *printer, papi_job_t job, int show_rank, int verbose)
{
	papi_attribute_t **attrs = papiJobGetAttributeList(job);
	time_t clock = 0;
	char date[24];
	char request[26];
	char *user = "unknown";
	char *host = NULL;
	int32_t size = 0;
	int32_t jstate = 0;
	char User[50];

	char *destination = "unknown";
	int32_t id = -1;
	static int check = 0;
	static char *uri = NULL;
	static char *puri = NULL;	/* printer-uri */
	static char *pname = NULL;	/* printer-name */

	(void) papiAttributeListGetString(attrs, NULL,
	    "job-originating-user-name", &user);

	if ((users != NULL) && (match_user(user, users) < 0))
		return (0);

	(void) papiAttributeListGetString(attrs, NULL,
	    "job-originating-host-name", &host);

	/*
	 * When lpstat is called for multiple printers
	 * internally the function 'report_job' gets
	 * called multiple times with different printer-names.
	 * The following block of code handles the case when lpstat is
	 * executed for multiple printers. In other words when 'report_job'
	 * is called multiple times for different printers for
	 * one lpstat command
	 * For e.g: lpstat printer1 printer2 printer3
	 */
	if (pname == NULL) {
		/*
		 * When lpstat is queried for the first time
		 * pname is NULL so this part of the code gets executed.
		 * Read the attribute "job-printer-uri"
		 * first time
		 */
		(void) papiAttributeListGetString(attrs, NULL,
		    "job-printer-uri", &uri);

		if (printer != NULL) {
			/*
			 * Set pname to the printer that is being
			 * queried so that this can be used later
			 * if 'report_job' is called multiple times for
			 * different printers for one lpstat command
			 */
			pname = printer;
		}

		if (uri != NULL) {
			/*
			 * Set puri so that "job-printer-uri" corresponding
			 * to a particular printer can be used later when
			 * lpstat is queried for the same printer as
			 * "job-printer-uri" for a printer is read just once.
			 */
			puri = strdup(uri);
		}
	} else {
		/*
		 * This part of the code will get executed when
		 * 'report_job' is called more than once for the same
		 * lpstat command
		 */
		if (printer != NULL) {
			if (strcasecmp(pname, printer) != 0) {
				/*
				 * Read the job-printer-uri as
				 * it will be different for
				 * different printers
				 */
				uri = NULL;
				(void) papiAttributeListGetString(attrs,
				    NULL, "job-printer-uri", &uri);
				pname = printer;
				if (uri != NULL)
					puri = strdup(uri);
				else
					puri = NULL;
			} else {
				/*
				 * Same printer queried twice
				 * uri should be the same as
				 * already read in the previous call
				 * to 'report_job'.
				 * For the same printer 'job-printer-uri'
				 * is read just once because only in the
				 * first call it contains the host information
				 */
				uri = puri;
			}
		}
	}

	if (host) {
		/* Check if it is local printer or remote printer */
		uri_t *u = NULL;

		if ((uri != NULL) && (uri_from_string(uri, &u) == 0)) {
			char *nodename = localhostname();

			if ((u->host == NULL) ||
			    (strcasecmp(u->host, "localhost") == 0) ||
			    (strcasecmp(u->host, nodename) == 0)) {

				if (strcasecmp(host, nodename) == 0) {
					/*
					 * Request submitted locally
					 * for the local queue.
					 * Hostname will not be displayed
					 */
					snprintf(User, sizeof (User), "%s",
					    user);
				}
				else
					snprintf(User, sizeof (User), "%s@%s",
					    user, host);
			} else if (uri != NULL) {
				/*
				 * It's a remote printer.
				 * In case of remote printers hostname is
				 * always displayed.
				 */
				snprintf(User, sizeof (User), "%s@%s",
				    user, host);
			}
			uri_free(u);
		} else {
			/*
			 * If attribute "job-printer-uri"
			 * cannot be read
			 * by default append the hostname
			 */
			snprintf(User, sizeof (User), "%s@%s", user, host);
		}
	} else {
		/*
		 * When print server is s10u4 and ipp service is used
		 * "job-originating-hostname" attribute is not set
		 * So get the host information from the uri
		 */
		uri_t *u = NULL;
		if ((uri != NULL) && (uri_from_string(uri, &u) == 0)) {
			if ((u != NULL) && (u->host != NULL))
				snprintf(User, sizeof (User), "%s@%s",
				    user, u->host);
			else
				snprintf(User, sizeof (User), "%s", user);

			uri_free(u);
		} else
			snprintf(User, sizeof (User), "%s", user);
	}
	(void) papiAttributeListGetInteger(attrs, NULL, "job-k-octets", &size);
	size *= 1024;	/* for the approximate byte size */
	(void) papiAttributeListGetInteger(attrs, NULL, "job-octets", &size);

	(void) time(&clock);
	(void) papiAttributeListGetInteger(attrs, NULL,
	    "time-at-creation", (int32_t *)&clock);
	(void) strftime(date, sizeof (date), "%b %d %R", localtime(&clock));

	(void) papiAttributeListGetString(attrs, NULL,
	    "job-printer-uri", &destination);
	(void) papiAttributeListGetString(attrs, NULL,
	    "printer-name", &destination);
	(void) papiAttributeListGetInteger(attrs, NULL,
	    "job-id", &id);
	(void) papiAttributeListGetInteger(attrs, NULL,
	    "job-id-requested", &id);


	snprintf(request, sizeof (request), "%s-%d", printer, id);

	if (show_rank != 0) {
		int32_t rank = -1;

		(void) papiAttributeListGetInteger(attrs, NULL,
		    "number-of-intervening-jobs", &rank);
		rank++;

		printf("%3d %-21s %-14s %7ld %s",
		    rank, request, User, size, date);
	} else
		printf("%-23s %-14s %7ld   %s", request, User, size, date);

	(void) papiAttributeListGetInteger(attrs, NULL,
	    "job-state", &jstate);

	if (jstate == 0x0001)
		printf(gettext(" being held"));
	else if (jstate == 0x0800)
		printf(gettext(" notifying user"));
	else if (jstate == 0x0040)
		printf(gettext(" cancelled"));
	else if (jstate == 0x0010)
		printf(gettext(" finished printing"));
	else if (jstate == 0x0008)
		printf(gettext(" on %s"), destination);
	else if (jstate == 0x2000)
		printf(gettext(" held by admin"));
	else if (jstate == 0x0002)
		printf(gettext(" being filtered"));
	else if (jstate == 0x0004)
		printf(gettext(" filtered"));
	else if (jstate == 0x0020)
		printf(gettext(" held for change"));

	if (verbose == 1) {
		char *form = NULL;

		(void) papiAttributeListGetString(attrs, NULL,
		    "output-device-assigned", &destination);
		printf("\n\t assigned %s", destination);

		(void) papiAttributeListGetString(attrs, NULL, "form", &form);
		if (form != NULL)
			printf(", form %s", form);
	} else if (verbose > 1) {
		printf("\n");
		papiAttributeListPrint(stdout, attrs, "\t");
	}

	printf("\n");

	return (0);
}

static int
job_query(char *request, int (*report)(char *, papi_job_t, int, int),
		papi_encryption_t encryption, int show_rank, int verbose)
{
	int result = 0;
	papi_status_t status;
	papi_service_t svc = NULL;
	char *printer = request;
	int32_t id = -1;
	int flag1 = 0;
	int flag = 1;
	int print_flag = 0;

	do {
		status = papiServiceCreate(&svc, printer, NULL, NULL,
		    cli_auth_callback, encryption, NULL);

		if ((status == PAPI_OK) && (printer != NULL))
			print_flag = 1;

		/* <name>-# printer name does not exist */
		if (status != PAPI_OK) {
			/*
			 * Check if <name>-# is a request-id
			 * Once this check is done flag1 is set
			 */
			if (flag1 == 1)
				break;

			get_printer_id(printer, &printer, &id);

			status = papiServiceCreate(&svc, printer, NULL, NULL,
			    cli_auth_callback, encryption, NULL);

			if (status != PAPI_OK) {
				fprintf(stderr, gettext(
				    "Failed to contact service for %s: %s\n"),
				    (printer ? printer : "all"),
				    verbose_papi_message(svc, status));
				return (-1);
			}
		}

		if (printer == NULL) { /* all */
			char **interest = interest_list(svc);

			if (interest != NULL) {
				int i;

				for (i = 0; interest[i] != NULL; i++)
					result += job_query(interest[i], report,
					    encryption, show_rank, verbose);
			}
		} else if (id == -1) { /* a printer */
			papi_job_t *jobs = NULL;

			status = papiPrinterListJobs(svc, printer, NULL,
			    0, 0, &jobs);
			if (status != PAPI_OK) {
				fprintf(stderr, gettext(
				    "Failed to get job list: %s\n"),
				    verbose_papi_message(svc, status));
				papiServiceDestroy(svc);
				return (-1);
			}

			if (jobs != NULL) {
				int i;

				for (i = 0; jobs[i] != NULL; i++)
					result += report(printer,
					    jobs[i], show_rank,
					    verbose);
			}

			papiJobListFree(jobs);
		} else {	/* a job */
			papi_job_t job = NULL;

			/* Once a job has been found stop processing */
			flag = 0;

			/*
			 * Job-id could be the job-id requested
			 * Check if it is job-id or job-id-requested
			 */
			id = job_to_be_queried(svc, printer, id);

			if (id >= 0)
				status = papiJobQuery(svc, printer, id,
				    NULL, &job);
			else
				/* id not found */
				status = PAPI_NOT_FOUND;

			if (status != PAPI_OK) {
				if (!print_flag)
					fprintf(stderr, gettext(
					    "Failed to get job"\
					    " info for %s: %s\n"),
					    request,
					    verbose_papi_message(svc, status));
				papiServiceDestroy(svc);
				return (-1);
			}

			if (job != NULL)
				result = report(printer, job,
				    show_rank, verbose);

			papiJobFree(job);
		}

		if (flag) {
			id = -1;
			get_printer_id(printer, &printer, &id);
			if (id == -1)
				flag = 0;
			else
				flag1 = 1;
		}
	} while (flag);

	papiServiceDestroy(svc);

	return (result);
}

static int
report_form(char *name, papi_attribute_t **attrs, int verbose)
{
	papi_status_t status;
	char *form = NULL;
	void *iter = NULL;

	for (status = papiAttributeListGetString(attrs, &iter,
	    "form-supported", &form);
	    status == PAPI_OK;
	    status = papiAttributeListGetString(attrs, &iter,
	    NULL, &form)) {
		if ((name == NULL) || (strcmp(name, form) == 0)) {
			printf(gettext("form %s is available to you\n"), form);
			if (verbose != 0) {
				char *detail = NULL;
				status = papiAttributeListGetString(attrs, NULL,
				    "form-supported-detail", &detail);
				if (status == PAPI_OK)
					printf("%s\n", detail);
			}
		}
	}

	return (0);
}

static int
report_print_wheels(char *name, papi_attribute_t **attrs, int verbose)
{
	papi_status_t status;
	char *pw = NULL;
	void *iter = NULL;

	for (status = papiAttributeListGetString(attrs, &iter,
	    "pw-supported", &pw);
	    status == PAPI_OK;
	    status = papiAttributeListGetString(attrs, &iter, NULL, &pw)) {
		if ((name == NULL) || (strcmp(name, pw) == 0)) {
			printf(gettext("charset %s is available\n"), pw);
			if (verbose != 0) {
				char *info = NULL;
				status = papiAttributeListGetString(attrs, NULL,
				    "pw-supported-extra", &info);
				if (status == PAPI_OK)
					printf("%s\n", info);
			}
		}
	}

	return (0);
}

static int
service_query(char *name, int (*report)(char *, papi_attribute_t **, int),
		papi_encryption_t encryption, int verbose)
{
	int result = 0;
	papi_status_t status;
	papi_service_t svc = NULL;
	papi_attribute_t **attrs = NULL;

	status = papiServiceCreate(&svc, name, NULL, NULL, cli_auth_callback,
	    encryption, NULL);
	if (status != PAPI_OK) {
		papiServiceDestroy(svc);
		return (-1);
	}

	attrs = papiServiceGetAttributeList(svc);
	if (attrs != NULL) {
		result = report(name, attrs, verbose);

		if (verbose > 1) {
			printf("\n");
			papiAttributeListPrint(stdout, attrs, "\t");
			printf("\n");
		}
	}

	papiServiceDestroy(svc);

	return (result);
}

int
main(int ac, char *av[])
{
	int exit_code = 0;
	papi_encryption_t encryption = PAPI_ENCRYPT_NEVER;
	int rank = 0;
	int verbose = 0;
	int description = 0;
	int c;
	char **argv;

	(void) setlocale(LC_ALL, "");
	(void) textdomain("SUNW_OST_OSCMD");

	argv = (char **)calloc((ac + 1), sizeof (char *));
	for (c = 0; c < ac; c++) {
		if ((av[c][0] == '-') && (av[c][1] == 'l') &&
		    (isalpha(av[c][2]) != 0)) {
			/* preserve old "-l[po...]" behavior */
			argv[c] = &av[c][1];
			argv[c][0] = '-';
			verbose = 1;

		} else
			argv[c] = av[c];
	}

	argv[c++] = "--";
	ac = c;

	/* preprocess argument list looking for '-l' or '-R' so it can trail */
	while ((c = getopt(ac, argv, "LEDf:S:stc:p:a:drs:v:l:o:R:u:")) != EOF) {
		switch (c) {    /* these may or may not have an option */
		case 'a':
		case 'c':
		case 'p':
		case 'o':
		case 'R':
		case 'u':
		case 'v':
		case 'l':
		case 'f':
		case 'S':
			if (optarg[0] == '-') {
				/* this check stop a possible infinite loop */
				if ((optind > 1) && (argv[optind-1][1] != c))
					optind--;
				optarg = NULL;
			} else if (strcmp(optarg, "all") == 0)
				optarg = NULL;
		}

		switch (c) {
		case 'l':
			if ((optarg == NULL) || (optarg[0] == '-'))
				optarg = "1";
			verbose = atoi(optarg);
			break;
		case 'D':
			description = 1;
			break;
		case 'R':
			rank = 1;
			break;
		case 'E':
			encryption = PAPI_ENCRYPT_REQUIRED;
			break;
		default:
			break;
		}
	}
	optind = 1;

	/* process command line arguments */
	while ((c = getopt(ac, argv, "LEDf:S:stc:p:a:drs:v:l:o:R:u:")) != EOF) {
		switch (c) {	/* these may or may not have an option */
		case 'a':
		case 'c':
		case 'p':
		case 'o':
		case 'R':
		case 'u':
		case 'v':
		case 'l':
		case 'f':
		case 'S':
			if (optarg[0] == '-') {
				/* this check stop a possible infinite loop */
				if ((optind > 1) && (argv[optind-1][1] != c))
					optind--;
				optarg = NULL;
			} else if (strcmp(optarg, "all") == 0)
				optarg = NULL;
		}

		switch (c) {
		case 'a':
			exit_code += printer_query(optarg, report_accepting,
			    encryption, verbose, 0);
			break;
		case 'c':
			exit_code += printer_query(optarg, report_class,
			    encryption, verbose, 0);
			break;
		case 'p':
			exit_code += printer_query(optarg, report_printer,
			    encryption, verbose, description);
			break;
		case 'd':
			exit_code += lpstat_default_printer(encryption);
			break;
		case 'r':
			exit_code += lpstat_service_status(encryption);
			break;
		case 'u':
			if (optarg != NULL)
				users = strsplit(optarg, ", \n");
			exit_code += job_query(NULL, report_job,
			    encryption, rank, verbose);
			if (users != NULL) {
				free(users);
				users = NULL;
			}
			break;
		case 'v':
			exit_code += printer_query(optarg, report_device,
			    encryption, verbose, 0);
			break;
		case 'R':	/* set "rank" flag in first pass */
		case 'o':
			exit_code += job_query(optarg, report_job,
			    encryption, rank, verbose);
			break;
		case 'f':
			exit_code += service_query(optarg, report_form,
			    encryption, verbose);
			break;
		case 'S':
			exit_code += service_query(optarg, report_print_wheels,
			    encryption, verbose);
			break;
		case 's':
			exit_code += lpstat_service_status(encryption);
			exit_code += lpstat_default_printer(encryption);
			exit_code += printer_query(NULL, report_class,
			    encryption, verbose, 0);
			exit_code += printer_query(NULL, report_device,
			    encryption, verbose, 0);
			exit_code += service_query(optarg, report_form,
			    encryption, verbose);
			exit_code += service_query(optarg, report_print_wheels,
			    encryption, verbose);
			break;
		case 't':
			exit_code += lpstat_service_status(encryption);
			exit_code += lpstat_default_printer(encryption);
			exit_code += printer_query(NULL, report_class,
			    encryption, verbose, 0);
			exit_code += printer_query(NULL, report_device,
			    encryption, verbose, 0);
			exit_code += printer_query(NULL, report_accepting,
			    encryption, verbose, 0);
			exit_code += printer_query(NULL, report_printer,
			    encryption, verbose, 0);
			exit_code += service_query(optarg, report_form,
			    encryption, verbose);
			exit_code += service_query(optarg, report_print_wheels,
			    encryption, verbose);
			exit_code += job_query(NULL, report_job,
			    encryption, rank, verbose);
			break;
		case 'L':	/* local-only, ignored */
		case 'l':	/* increased verbose level in first pass */
		case 'D':	/* set "description" flag in first pass */
		case 'E':	/* set encryption in the first pass */
			break;
		default:
			usage(av[0]);
		}
	}
	ac--;

	if (ac == 1) {	/* report on my jobs */
		struct passwd *pw = getpwuid(getuid());

		if (pw != NULL)
			users = strsplit(pw->pw_name, "");
		exit_code += job_query(NULL, report_job, encryption,
		    rank, verbose);
		if (users != NULL) {
			free(users);
			users = NULL;
		}
	} else {
		for (c = optind; c < ac; c++)
			exit_code += job_query(argv[c], report_job, encryption,
			    rank, verbose);
	}


	if (exit_code != 0)
		exit_code = 1;

	return (exit_code);
}
