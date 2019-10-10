/*
 * Tool intended to help facilitate the process of booting Linux on Intel
 * Macintosh computers made by Apple from a USB stick or similar.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * Copyright (C) 2013 SevenBits
 *
 */

#pragma once
#ifndef _menu_h
#define _menu_h
#include "main.h"

EFI_STATUS DisplayMenu(void);
EFI_STATUS DisplayDistributionSelector(struct BootableLinuxDistro *, CHAR16 *, BOOLEAN);
EFI_STATUS ConfigureKernel(CHAR16 *, BOOLEAN[], int);

#endif
