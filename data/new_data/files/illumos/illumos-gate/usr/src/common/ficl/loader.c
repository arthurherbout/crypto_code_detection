/*
 * Copyright (c) 2000 Daniel Capo Sobral
 * Copyright 2019 OmniOS Community Edition (OmniOSce) Association.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * l o a d e r . c
 * Additional FICL words designed for FreeBSD's loader
 */

#ifndef _STANDALONE
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <termios.h>
#else
#include <stand.h>
#include <gfx_fb.h>
#include "bootstrap.h"
#endif
#ifdef _STANDALONE
#include <uuid.h>
#else
#include <uuid/uuid.h>
#endif
#include <string.h>
#include <gfx_fb.h>
#include "ficl.h"

/*
 *		FreeBSD's loader interaction words and extras
 *
 *		setenv      ( value n name n' -- )
 *		setenv?     ( value n name n' flag -- )
 *		getenv      ( addr n -- addr' n' | -1 )
 *		unsetenv    ( addr n -- )
 *		copyin      ( addr addr' len -- )
 *		copyout     ( addr addr' len -- )
 *		findfile    ( name len type len' -- addr )
 *		ccall       ( [[...[p10] p9] ... p1] n addr -- result )
 *		uuid-from-string ( addr n -- addr' )
 *		uuid-to-string ( addr' -- addr n | -1 )
 *		.#	    ( value -- )
 */

/* ( flags x1 y1 x2 y2 -- flag ) */
void
ficl_fb_putimage(ficlVm *pVM)
{
	char *namep, *name;
	ficlUnsigned names;
	ficlInteger ret = FICL_FALSE;
	uint32_t x1, y1, x2, y2, f;
	png_t png;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 7, 1);

	names = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	namep = (char *)ficlStackPopPointer(ficlVmGetDataStack(pVM));
	y2 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	x2 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	y1 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	x1 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	f = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));

	name = ficlMalloc(names + 1);
	if (!name)
		ficlVmThrowError(pVM, "Error: out of memory");
	(void) strncpy(name, namep, names);
	name[names] = '\0';

	if (png_open(&png, name) == PNG_NO_ERROR) {
		if (gfx_fb_putimage(&png, x1, y1, x2, y2, f) == 0)
			ret = FICL_TRUE;	/* success */
		(void) png_close(&png);
	}
	ficlFree(name);
	ficlStackPushInteger(ficlVmGetDataStack(pVM), ret);
}

void
ficl_fb_setpixel(ficlVm *pVM)
{
	ficlUnsigned x, y;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 2, 0);

	y = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	x = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	gfx_fb_setpixel(x, y);
}

void
ficl_fb_line(ficlVm *pVM)
{
	ficlUnsigned x0, y0, x1, y1, wd;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 5, 0);

	wd = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	y1 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	x1 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	y0 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	x0 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	gfx_fb_line(x0, y0, x1, y1, wd);
}

void
ficl_fb_bezier(ficlVm *pVM)
{
	ficlUnsigned x0, y0, x1, y1, x2, y2, width;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 7, 0);

	width = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	y2 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	x2 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	y1 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	x1 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	y0 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	x0 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	gfx_fb_bezier(x0, y0, x1, y1, x2, y2, width);
}

void
ficl_fb_drawrect(ficlVm *pVM)
{
	ficlUnsigned x1, x2, y1, y2, fill;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 5, 0);

	fill = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	y2 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	x2 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	y1 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	x1 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	gfx_fb_drawrect(x1, y1, x2, y2, fill);
}

void
ficl_term_drawrect(ficlVm *pVM)
{
	ficlUnsigned x1, x2, y1, y2;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 4, 0);

	y2 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	x2 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	y1 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	x1 = ficlStackPopUnsigned(ficlVmGetDataStack(pVM));
	gfx_term_drawrect(x1, y1, x2, y2);
}

void
ficlSetenv(ficlVm *pVM)
{
	char *name, *value;
	char *namep, *valuep;
	int names, values;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 4, 0);

	names = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	namep = (char *)ficlStackPopPointer(ficlVmGetDataStack(pVM));
	values = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	valuep = (char *)ficlStackPopPointer(ficlVmGetDataStack(pVM));

	name = (char *)ficlMalloc(names+1);
	if (!name)
		ficlVmThrowError(pVM, "Error: out of memory");
	(void) strncpy(name, namep, names);
	name[names] = '\0';
	value = (char *)ficlMalloc(values+1);
	if (!value)
		ficlVmThrowError(pVM, "Error: out of memory");
	(void) strncpy(value, valuep, values);
	value[values] = '\0';

	(void) setenv(name, value, 1);
	ficlFree(name);
	ficlFree(value);
}

void
ficlSetenvq(ficlVm *pVM)
{
	char *name, *value;
	char *namep, *valuep;
	int names, values, overwrite;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 5, 0);

	overwrite = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	names = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	namep = (char *)ficlStackPopPointer(ficlVmGetDataStack(pVM));
	values = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	valuep = (char *)ficlStackPopPointer(ficlVmGetDataStack(pVM));

	name = (char *)ficlMalloc(names+1);
	if (!name)
		(void) ficlVmThrowError(pVM, "Error: out of memory");
	(void) strncpy(name, namep, names);
	name[names] = '\0';
	value = (char *)ficlMalloc(values+1);
	if (!value)
		ficlVmThrowError(pVM, "Error: out of memory");
	(void) strncpy(value, valuep, values);
	value[values] = '\0';

	(void) setenv(name, value, overwrite);
	ficlFree(name);
	ficlFree(value);
}

void
ficlGetenv(ficlVm *pVM)
{
	char *name, *value;
	char *namep;
	int names;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 2, 2);

	names = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	namep = (char *)ficlStackPopPointer(ficlVmGetDataStack(pVM));

	name = (char *)ficlMalloc(names+1);
	if (!name)
		ficlVmThrowError(pVM, "Error: out of memory");
	(void) strncpy(name, namep, names);
	name[names] = '\0';

	value = getenv(name);
	ficlFree(name);

	if (value != NULL) {
		ficlStackPushPointer(ficlVmGetDataStack(pVM), value);
		ficlStackPushInteger(ficlVmGetDataStack(pVM), strlen(value));
	} else
		ficlStackPushInteger(ficlVmGetDataStack(pVM), -1);
}

void
ficlUnsetenv(ficlVm *pVM)
{
	char *name;
	char *namep;
	int names;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 2, 0);

	names = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	namep = (char *)ficlStackPopPointer(ficlVmGetDataStack(pVM));

	name = (char *)ficlMalloc(names+1);
	if (!name)
		ficlVmThrowError(pVM, "Error: out of memory");
	(void) strncpy(name, namep, names);
	name[names] = '\0';

	(void) unsetenv(name);
	ficlFree(name);
}

void
ficlCopyin(ficlVm *pVM)
{
#ifdef _STANDALONE
	void*		src;
	vm_offset_t	dest;
	size_t		len;
#endif

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 3, 0);

#ifdef _STANDALONE
	len = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	dest = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	src = ficlStackPopPointer(ficlVmGetDataStack(pVM));
	archsw.arch_copyin(src, dest, len);
#else
	(void) ficlStackPopInteger(ficlVmGetDataStack(pVM));
	(void) ficlStackPopInteger(ficlVmGetDataStack(pVM));
	(void) ficlStackPopPointer(ficlVmGetDataStack(pVM));
#endif
}

void
ficlCopyout(ficlVm *pVM)
{
#ifdef _STANDALONE
	void*		dest;
	vm_offset_t	src;
	size_t		len;
#endif

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 3, 0);

#ifdef _STANDALONE
	len = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	dest = ficlStackPopPointer(ficlVmGetDataStack(pVM));
	src = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	archsw.arch_copyout(src, dest, len);
#else
	(void) ficlStackPopInteger(ficlVmGetDataStack(pVM));
	(void) ficlStackPopPointer(ficlVmGetDataStack(pVM));
	(void) ficlStackPopInteger(ficlVmGetDataStack(pVM));
#endif
}

void
ficlFindfile(ficlVm *pVM)
{
#ifdef _STANDALONE
	char	*name, *type;
	char	*namep, *typep;
	int	names, types;
#endif
	struct	preloaded_file *fp;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 4, 1);

#ifdef _STANDALONE
	types = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	typep = (char *)ficlStackPopPointer(ficlVmGetDataStack(pVM));
	names = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	namep = (char *)ficlStackPopPointer(ficlVmGetDataStack(pVM));

	name = (char *)ficlMalloc(names+1);
	if (!name)
		ficlVmThrowError(pVM, "Error: out of memory");
	strncpy(name, namep, names);
	name[names] = '\0';
	type = (char *)ficlMalloc(types+1);
	if (!type)
		ficlVmThrowError(pVM, "Error: out of memory");
	strncpy(type, typep, types);
	type[types] = '\0';

	fp = file_findfile(name, type);
#else
	(void) ficlStackPopInteger(ficlVmGetDataStack(pVM));
	(void) ficlStackPopPointer(ficlVmGetDataStack(pVM));
	(void) ficlStackPopInteger(ficlVmGetDataStack(pVM));
	(void) ficlStackPopPointer(ficlVmGetDataStack(pVM));

	fp = NULL;
#endif
	ficlStackPushPointer(ficlVmGetDataStack(pVM), fp);
}

void
ficlCcall(ficlVm *pVM)
{
	int (*func)(int, ...);
	int result, p[10];
	int nparam, i;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 2, 0);

	func = (int (*)(int, ...))ficlStackPopPointer(ficlVmGetDataStack(pVM));
	nparam = ficlStackPopInteger(ficlVmGetDataStack(pVM));

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), nparam, 1);

	for (i = 0; i < nparam; i++)
		p[i] = ficlStackPopInteger(ficlVmGetDataStack(pVM));

	result = func(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8],
	    p[9]);

	ficlStackPushInteger(ficlVmGetDataStack(pVM), result);
}

void
ficlUuidFromString(ficlVm *pVM)
{
	char	*uuid;
	char	*uuid_ptr;
	int	uuid_size;
	uuid_t	*u;
#ifdef _STANDALONE
	uint32_t status;
#else
	int status;
#endif

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 2, 0);

	uuid_size = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	uuid_ptr = ficlStackPopPointer(ficlVmGetDataStack(pVM));

	uuid = ficlMalloc(uuid_size + 1);
	if (!uuid)
		ficlVmThrowError(pVM, "Error: out of memory");
	(void) memcpy(uuid, uuid_ptr, uuid_size);
	uuid[uuid_size] = '\0';

	u = ficlMalloc(sizeof (*u));
#ifdef _STANDALONE
	uuid_from_string(uuid, u, &status);
	ficlFree(uuid);
	if (status != uuid_s_ok) {
		ficlFree(u);
		u = NULL;
	}
#else
	status = uuid_parse(uuid, *u);
	ficlFree(uuid);
	if (status != 0) {
		ficlFree(u);
		u = NULL;
	}
#endif
	ficlStackPushPointer(ficlVmGetDataStack(pVM), u);
}

void
ficlUuidToString(ficlVm *pVM)
{
	char	*uuid;
	uuid_t	*u;
#ifdef _STANDALONE
	uint32_t status;
#endif

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 1, 0);

	u = ficlStackPopPointer(ficlVmGetDataStack(pVM));
#ifdef _STANDALONE
	uuid_to_string(u, &uuid, &status);
	if (status == uuid_s_ok) {
		ficlStackPushPointer(ficlVmGetDataStack(pVM), uuid);
		ficlStackPushInteger(ficlVmGetDataStack(pVM), strlen(uuid));
	} else
#else
	uuid = ficlMalloc(UUID_PRINTABLE_STRING_LENGTH);
	if (uuid != NULL) {
		uuid_unparse(*u, uuid);
		ficlStackPushPointer(ficlVmGetDataStack(pVM), uuid);
		ficlStackPushInteger(ficlVmGetDataStack(pVM), strlen(uuid));
	} else
#endif
		ficlStackPushInteger(ficlVmGetDataStack(pVM), -1);
}

/*
 * f i c l E x e c F D
 * reads in text from file fd and passes it to ficlExec()
 * returns FICL_VM_STATUS_OUT_OF_TEXT on success or the ficlExec() error
 * code on failure.
 */
#define	nLINEBUF	256
int
ficlExecFD(ficlVm *pVM, int fd)
{
	char cp[nLINEBUF];
	int nLine = 0, rval = FICL_VM_STATUS_OUT_OF_TEXT;
	char ch;
	ficlCell id;
	ficlString s;

	id = pVM->sourceId;
	pVM->sourceId.i = fd+1; /* in loader we can get 0, there is no stdin */

	/* feed each line to ficlExec */
	while (1) {
		int status, i;

		i = 0;
		while ((status = read(fd, &ch, 1)) > 0 && ch != '\n')
			cp[i++] = ch;
		nLine++;
		if (!i) {
			if (status < 1)
				break;
			continue;
		}
		if (cp[i] == '\n')
			cp[i] = '\0';

		FICL_STRING_SET_POINTER(s, cp);
		FICL_STRING_SET_LENGTH(s, i);

		rval = ficlVmExecuteString(pVM, s);
		if (rval != FICL_VM_STATUS_QUIT &&
		    rval != FICL_VM_STATUS_USER_EXIT &&
		    rval != FICL_VM_STATUS_OUT_OF_TEXT) {
			pVM->sourceId = id;
			(void) ficlVmEvaluate(pVM, "");
			return (rval);
		}
	}
	pVM->sourceId = id;

	/*
	 * Pass an empty line with SOURCE-ID == -1 to flush
	 * any pending REFILLs (as required by FILE wordset)
	 */
	(void) ficlVmEvaluate(pVM, "");

	if (rval == FICL_VM_STATUS_USER_EXIT)
		ficlVmThrow(pVM, FICL_VM_STATUS_USER_EXIT);

	return (rval);
}

static void displayCellNoPad(ficlVm *pVM)
{
	ficlCell c;
	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 1, 0);

	c = ficlStackPop(ficlVmGetDataStack(pVM));
	(void) ficlLtoa((c).i, pVM->pad, pVM->base);
	ficlVmTextOut(pVM, pVM->pad);
}

/*
 * isdir? - Return whether an fd corresponds to a directory.
 *
 * isdir? ( fd -- bool )
 */
static void
isdirQuestion(ficlVm *pVM)
{
	struct stat sb;
	ficlInteger flag;
	int fd;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 1, 1);

	fd = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	flag = FICL_FALSE;
	do {
		if (fd < 0)
			break;
		if (fstat(fd, &sb) < 0)
			break;
		if (!S_ISDIR(sb.st_mode))
			break;
		flag = FICL_TRUE;
	} while (0);
	ficlStackPushInteger(ficlVmGetDataStack(pVM), flag);
}

/*
 * fopen - open a file and return new fd on stack.
 *
 * fopen ( ptr count mode -- fd )
 */
extern char *get_dev(const char *);

static void
pfopen(ficlVm *pVM)
{
	int mode, fd, count;
	char *ptr, *name;
#ifndef _STANDALONE
	char *tmp;
#endif

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 3, 1);

	mode = ficlStackPopInteger(ficlVmGetDataStack(pVM));	/* get mode */
	count = ficlStackPopInteger(ficlVmGetDataStack(pVM));	/* get count */
	ptr = ficlStackPopPointer(ficlVmGetDataStack(pVM));	/* get ptr */

	if ((count < 0) || (ptr == NULL)) {
		ficlStackPushInteger(ficlVmGetDataStack(pVM), -1);
		return;
	}

	/* ensure that the string is null terminated */
	name = (char *)malloc(count+1);
	bcopy(ptr, name, count);
	name[count] = 0;
#ifndef _STANDALONE
	tmp = get_dev(name);
	free(name);
	name = tmp;
#endif

	/* open the file */
	fd = open(name, mode);
	free(name);
	ficlStackPushInteger(ficlVmGetDataStack(pVM), fd);
}

/*
 * fclose - close a file who's fd is on stack.
 * fclose ( fd -- )
 */
static void
pfclose(ficlVm *pVM)
{
	int fd;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 1, 0);

	fd = ficlStackPopInteger(ficlVmGetDataStack(pVM)); /* get fd */
	if (fd != -1)
		(void) close(fd);
}

/*
 * fread - read file contents
 * fread  ( fd buf nbytes  -- nread )
 */
static void
pfread(ficlVm *pVM)
{
	int fd, len;
	char *buf;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 3, 1);

	len = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	buf = ficlStackPopPointer(ficlVmGetDataStack(pVM)); /* get buffer */
	fd = ficlStackPopInteger(ficlVmGetDataStack(pVM)); /* get fd */
	if (len > 0 && buf && fd != -1)
		ficlStackPushInteger(ficlVmGetDataStack(pVM),
		    read(fd, buf, len));
	else
		ficlStackPushInteger(ficlVmGetDataStack(pVM), -1);
}

/*
 * fopendir - open directory
 *
 * fopendir ( addr len -- ptr TRUE | FALSE )
 */
static void pfopendir(ficlVm *pVM)
{
#ifndef _STANDALONE
	DIR *dir;
	char *tmp;
#else
	struct stat sb;
	int fd;
#endif
	ficlInteger count;
	char *ptr, *name;
	ficlInteger flag = FICL_FALSE;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 2, 1);

	count = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	ptr = ficlStackPopPointer(ficlVmGetDataStack(pVM));	/* get ptr */

	if ((count < 0) || (ptr == NULL)) {
		ficlStackPushInteger(ficlVmGetDataStack(pVM), flag);
		return;
	}
	/* ensure that the string is null terminated */
	if ((name = malloc(count + 1)) == NULL) {
		ficlStackPushInteger(ficlVmGetDataStack(pVM), flag);
		return;
	}

	bcopy(ptr, name, count);
	name[count] = 0;
#ifndef _STANDALONE
	tmp = get_dev(name);
	free(name);
	name = tmp;

	dir = opendir(name);
	if (dir == NULL) {
		ficlStackPushInteger(ficlVmGetDataStack(pVM), flag);
		return;
	} else
		flag = FICL_TRUE;

	ficlStackPushPointer(ficlVmGetDataStack(pVM), dir);
	ficlStackPushInteger(ficlVmGetDataStack(pVM), flag);
#else
	fd = open(name, O_RDONLY);
	free(name);
	do {
		if (fd < 0)
			break;
		if (fstat(fd, &sb) < 0)
			break;
		if (!S_ISDIR(sb.st_mode))
			break;
		flag = FICL_TRUE;
		ficlStackPushInteger(ficlVmGetDataStack(pVM), fd);
		ficlStackPushInteger(ficlVmGetDataStack(pVM), flag);
		return;
	} while (0);

	if (fd >= 0)
		close(fd);

	ficlStackPushInteger(ficlVmGetDataStack(pVM), flag);
#endif
}

/*
 * freaddir - read directory contents
 * freaddir ( fd -- ptr len TRUE | FALSE )
 */
static void
pfreaddir(ficlVm *pVM)
{
#ifndef _STANDALONE
	static DIR *dir = NULL;
#else
	int fd;
#endif
	struct dirent *d = NULL;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 1, 3);
	/*
	 * libstand readdir does not always return . nor .. so filter
	 * them out to have consistent behaviour.
	 */
#ifndef _STANDALONE
	dir = ficlStackPopPointer(ficlVmGetDataStack(pVM));
	if (dir != NULL)
		do {
			d = readdir(dir);
			if (d != NULL && strcmp(d->d_name, ".") == 0)
				continue;
			if (d != NULL && strcmp(d->d_name, "..") == 0)
				continue;
			break;
		} while (d != NULL);
#else
	fd = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	if (fd != -1)
		do {
			d = readdirfd(fd);
			if (d != NULL && strcmp(d->d_name, ".") == 0)
				continue;
			if (d != NULL && strcmp(d->d_name, "..") == 0)
				continue;
			break;
		} while (d != NULL);
#endif
	if (d != NULL) {
		ficlStackPushPointer(ficlVmGetDataStack(pVM), d->d_name);
		ficlStackPushInteger(ficlVmGetDataStack(pVM),
		    strlen(d->d_name));
		ficlStackPushInteger(ficlVmGetDataStack(pVM), FICL_TRUE);
	} else {
		ficlStackPushInteger(ficlVmGetDataStack(pVM), FICL_FALSE);
	}
}

/*
 * fclosedir - close a dir on stack.
 *
 * fclosedir ( fd -- )
 */
static void
pfclosedir(ficlVm *pVM)
{
#ifndef _STANDALONE
	DIR *dir;
#else
	int fd;
#endif

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 1, 0);

#ifndef _STANDALONE
	dir = ficlStackPopPointer(ficlVmGetDataStack(pVM)); /* get dir */
	if (dir != NULL)
		(void) closedir(dir);
#else
	fd = ficlStackPopInteger(ficlVmGetDataStack(pVM)); /* get fd */
	if (fd != -1)
		(void) close(fd);
#endif
}

/*
 * fload - interpret file contents
 *
 * fload  ( fd -- )
 */
static void pfload(ficlVm *pVM)
{
	int fd;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 1, 0);

	fd = ficlStackPopInteger(ficlVmGetDataStack(pVM)); /* get fd */
	if (fd != -1)
		(void) ficlExecFD(pVM, fd);
}

/*
 * fwrite - write file contents
 *
 * fwrite  ( fd buf nbytes  -- nwritten )
 */
static void
pfwrite(ficlVm *pVM)
{
	int fd, len;
	char *buf;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 3, 1);

	len = ficlStackPopInteger(ficlVmGetDataStack(pVM)); /* bytes to read */
	buf = ficlStackPopPointer(ficlVmGetDataStack(pVM)); /* get buffer */
	fd = ficlStackPopInteger(ficlVmGetDataStack(pVM)); /* get fd */
	if (len > 0 && buf && fd != -1)
		ficlStackPushInteger(ficlVmGetDataStack(pVM),
		    write(fd, buf, len));
	else
		ficlStackPushInteger(ficlVmGetDataStack(pVM), -1);
}

/*
 * fseek - seek to a new position in a file
 *
 * fseek  ( fd ofs whence  -- pos )
 */
static void
pfseek(ficlVm *pVM)
{
	int fd, pos, whence;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 3, 1);

	whence = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	pos = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	fd = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	ficlStackPushInteger(ficlVmGetDataStack(pVM), lseek(fd, pos, whence));
}

/*
 * key - get a character from stdin
 *
 * key ( -- char )
 */
static void
key(ficlVm *pVM)
{
	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 0, 1);

	ficlStackPushInteger(ficlVmGetDataStack(pVM), getchar());
}

/*
 * key? - check for a character from stdin (FACILITY)
 * key? ( -- flag )
 */
static void
keyQuestion(ficlVm *pVM)
{
#ifndef _STANDALONE
	char ch = -1;
	struct termios oldt;
	struct termios newt;
#endif

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 0, 1);

#ifndef _STANDALONE
	(void) tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	newt.c_cc[VMIN] = 0;
	newt.c_cc[VTIME] = 0;
	(void) tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	(void) tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	if (ch != -1)
		(void) ungetc(ch, stdin);

	ficlStackPushInteger(ficlVmGetDataStack(pVM),
	    ch != -1? FICL_TRUE : FICL_FALSE);
#else
	ficlStackPushInteger(ficlVmGetDataStack(pVM),
	    ischar()? FICL_TRUE : FICL_FALSE);
#endif
}

/*
 * seconds - gives number of seconds since beginning of time
 *
 * beginning of time is defined as:
 *
 *	BTX	- number of seconds since midnight
 *	FreeBSD	- number of seconds since Jan 1 1970
 *
 * seconds ( -- u )
 */
static void
pseconds(ficlVm *pVM)
{
	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 0, 1);

	ficlStackPushUnsigned(ficlVmGetDataStack(pVM),
	    (ficlUnsigned) time(NULL));
}

/*
 * ms - wait at least that many milliseconds (FACILITY)
 * ms ( u -- )
 */
static void
ms(ficlVm *pVM)
{
	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 1, 0);

#ifndef _STANDALONE
	(void) usleep(ficlStackPopUnsigned(ficlVmGetDataStack(pVM)) * 1000);
#else
	delay(ficlStackPopUnsigned(ficlVmGetDataStack(pVM)) * 1000);
#endif
}

/*
 * fkey - get a character from a file
 * fkey ( file -- char )
 */
static void
fkey(ficlVm *pVM)
{
	int i, fd;
	char ch;

	FICL_STACK_CHECK(ficlVmGetDataStack(pVM), 1, 1);

	fd = ficlStackPopInteger(ficlVmGetDataStack(pVM));
	i = read(fd, &ch, 1);
	ficlStackPushInteger(ficlVmGetDataStack(pVM), i > 0 ? ch : -1);
}

/*
 * Retrieves free space remaining on the dictionary
 */
static void
freeHeap(ficlVm *pVM)
{
	ficlStackPushInteger(ficlVmGetDataStack(pVM),
	    ficlDictionaryCellsAvailable(ficlVmGetDictionary(pVM)));
}

/*
 * f i c l C o m p i l e P l a t f o r m
 * Build FreeBSD platform extensions into the system dictionary
 */
void
ficlSystemCompilePlatform(ficlSystem *pSys)
{
	ficlDictionary *dp = ficlSystemGetDictionary(pSys);
	ficlDictionary *env = ficlSystemGetEnvironment(pSys);
#ifdef _STANDALONE
	ficlCompileFcn **fnpp;
#endif

	FICL_SYSTEM_ASSERT(pSys, dp);
	FICL_SYSTEM_ASSERT(pSys, env);

	(void) ficlDictionarySetPrimitive(dp, ".#", displayCellNoPad,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "isdir?", isdirQuestion,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "fopen", pfopen,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "fclose", pfclose,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "fread", pfread,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "fopendir", pfopendir,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "freaddir", pfreaddir,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "fclosedir", pfclosedir,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "fload", pfload,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "fkey", fkey,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "fseek", pfseek,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "fwrite", pfwrite,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "key", key, FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "key?", keyQuestion,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "ms", ms, FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "seconds", pseconds,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "heap?", freeHeap,
	    FICL_WORD_DEFAULT);

	(void) ficlDictionarySetPrimitive(dp, "setenv", ficlSetenv,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "setenv?", ficlSetenvq,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "getenv", ficlGetenv,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "unsetenv", ficlUnsetenv,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "copyin", ficlCopyin,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "copyout", ficlCopyout,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "findfile", ficlFindfile,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "ccall", ficlCcall,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "uuid-from-string",
	    ficlUuidFromString, FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "uuid-to-string",
	    ficlUuidToString, FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "fb-setpixel", ficl_fb_setpixel,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "fb-line", ficl_fb_line,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "fb-bezier", ficl_fb_bezier,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "fb-drawrect", ficl_fb_drawrect,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "fb-putimage", ficl_fb_putimage,
	    FICL_WORD_DEFAULT);
	(void) ficlDictionarySetPrimitive(dp, "term-drawrect",
	    ficl_term_drawrect, FICL_WORD_DEFAULT);
#ifdef _STANDALONE
	/* Register words from linker set. */
	SET_FOREACH(fnpp, Xficl_compile_set)
		(*fnpp)(pSys);
#endif

#if defined(__i386__) || defined(__amd64__)
	(void) ficlDictionarySetConstant(env, "arch-i386", FICL_TRUE);
	(void) ficlDictionarySetConstant(env, "arch-sparc", FICL_FALSE);
#endif
#ifdef __sparc
	(void) ficlDictionarySetConstant(env, "arch-i386", FICL_FALSE);
	(void) ficlDictionarySetConstant(env, "arch-sparc", FICL_TRUE);
#endif
}
