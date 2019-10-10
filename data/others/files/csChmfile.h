/*
 *  Copyright (C) 2011 Ji YongGang <jungleji@gmail.com>
 *
 *  ChmSee is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.

 *  ChmSee is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with ChmSee; see the file COPYING.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#ifndef __CS_CHMFILE_H__
#define __CS_CHMFILE_H__

#ifdef CS_DEBUG
#define d(x) x
#else
#define d(x)
#endif

struct fileinfo
{
        const char *bookfolder;
        char *homepage;
        char *bookname;
        char *hhc;
        char *hhk;
        u_int32_t lcid;
};

#ifdef __cplusplus
extern "C" {
#endif

long extract_chm(const char *, const char *);
void chm_fileinfo(struct fileinfo *);

#ifdef __cplusplus
}
#endif

#endif
