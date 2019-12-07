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

/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

#include <libelf.h>
#include "decl.h"

/*
 * Global data
 * _elf_byte		Fill byte for file padding.  See elf_fill().
 * _elf32_ehdr_init	Clean copy for to initialize new headers.
 * _elf64_ehdr_init	Clean copy for to initialize new class-64 headers.
 * _elf_encode		Host/internal data encoding.  If the host has
 *			an encoding that matches one known for the
 *			ELF format, this changes.  An machine with an
 *			unknown encoding keeps ELFDATANONE and forces
 *			conversion for host/target translation.
 * _elf_work		Working version given to the lib by application.
 *			See elf_version().
 * _elf_globals_mutex	mutex to protect access to all global data items.
 * _elf_execfill_func	Fill function for file padding of SHF_EXECINSTR
 *			sections. See _elf_execfill().
 */

/*
 * __libc_threaded is a private symbol exported from libc in Solaris 10.
 * It is used to tell if we are running in a threaded world or not.
 * Between Solaris 2.5 and Solaris 9, this was named __threaded.
 * The name had to be changed because the Sun Workshop 6 update 1
 * compilation system used it to mean "we are linked with libthread"
 * rather than its true meaning in Solaris 10, "more than one thread exists".
 */
#pragma weak		__libc_threaded
extern int		__libc_threaded;

int			_elf_byte = 0;
const Elf32_Ehdr	_elf32_ehdr_init = { 0 };
const Elf64_Ehdr	_elf64_ehdr_init = { 0 };
unsigned		_elf_encode = ELFDATANONE;
_elf_execfill_func_t	*_elf_execfill_func = NULL;
const Snode32		_elf32_snode_init = { 0 };
const Snode64		_elf64_snode_init = { 0 };
const Dnode		_elf_dnode_init = { 0 };
unsigned		_elf_work = EV_NONE;
mutex_t			_elf_globals_mutex = DEFAULTMUTEX;

int			*_elf_libc_threaded = &__libc_threaded;
