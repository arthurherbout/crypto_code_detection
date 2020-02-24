/* chacha.h
 *
 * The ChaCha stream cipher.
 * Heavily based on the Salsa20 source code in Nettle.
 */

/* nettle, low-level cryptographics library
 *
 * Copyright (C) 2013 Joachim Strömbergson
 * Copyright (C) 2012 Simon Josefsson
 * Copyright (C) 2001 Niels Möller
 *
 * The nettle library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * The nettle library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the nettle library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02111-1301, USA.
 */

#ifndef NETTLE_CHACHA_H_INCLUDED
#define NETTLE_CHACHA_H_INCLUDED

#include "nettle-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Name mangling */
#define chacha_set_key nettle_chacha_set_key
#define chacha_set_iv nettle_chacha_set_iv
#define chacha_crypt nettle_chacha_crypt
#define _chacha_core _nettle_chacha_core

/* Minimum and maximum keysizes, and a reasonable default. 
   In octets. */
#define CHACHA_MIN_KEY_SIZE 16
#define CHACHA_MAX_KEY_SIZE 32
#define CHACHA_KEY_SIZE 32
#define CHACHA_BLOCK_SIZE 64

#define CHACHA_IV_SIZE 8

#define CHACHA_NUM_ROUNDS 8

#define _CHACHA_STATE_LENGTH 16

struct chacha_ctx
{
  /* Indices 0-3 holds a constant (SIGMA or TAU).
     Indices 4-11 holds the key.
     Indices 12-13 holds the block counter.
     Indices 14-15 holds the IV:

     This creates the state matrix:
     C C C C
     K K K K
     K K K K
     B B I I
  */
  uint32_t state[_CHACHA_STATE_LENGTH];
};

void
chacha_set_key(struct chacha_ctx *ctx,
		size_t length, const uint8_t *key);

void
chacha_set_iv(struct chacha_ctx *ctx, const uint8_t *iv);

void
chacha_crypt(struct chacha_ctx *ctx, size_t length, 
             uint8_t rounds, uint8_t *dst, const uint8_t *src);

void
chachar12_crypt(struct chacha_ctx *ctx, size_t length, 
                uint8_t *dst, const uint8_t *src);

void
chachar20_crypt(struct chacha_ctx *ctx, size_t length, 
                uint8_t *dst, const uint8_t *src);

void
_chacha_core(uint32_t *dst, const uint32_t *src, uint8_t rounds);

#ifdef __cplusplus
}
#endif

#endif /* NETTLE_CHACHA_H_INCLUDED */
