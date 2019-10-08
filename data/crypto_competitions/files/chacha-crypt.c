/* salsa20-crypt.c
 *
 * The crypt function in the ChaCha stream cipher.
 * Heavily based on the Salsa20 implementation in Nettle.
 */

/* nettle, low-level cryptographics library
 *
 * Copyright (C) 2013 Joachim Strömbergson
 * Copyright (C) 2012 Simon Josefsson
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

/* Based on:
   chacha-ref.c version 2008.01.20.
   D. J. Bernstein
   Public domain.
*/

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include <string.h>

#include "chacha.h"

#include "macros.h"
#include "memxor.h"

void
chachar12_crypt(struct chacha_ctx *ctx, size_t length, 
                uint8_t *dst, const uint8_t *src)
{
  chacha_crypt(ctx, length, 12, dst, src);
}


void
chachar20_crypt(struct chacha_ctx *ctx, size_t length, 
                uint8_t *dst, const uint8_t *src)
{
  chacha_crypt(ctx, length, 20, dst, src);
}


void
chacha_crypt(struct chacha_ctx *ctx, size_t length, uint8_t rounds,
             uint8_t *c, const uint8_t *m)
{
  if (!length)
    return;
  
  for (;;)
    {
      uint32_t x[_CHACHA_STATE_LENGTH];

      _chacha_core (x, ctx->state, rounds);

      ctx->state[9] += (++ctx->state[8] == 0);

      /* stopping at 2^70 length per nonce is user's responsibility */
      
      if (length <= CHACHA_BLOCK_SIZE)
	{
	  memxor3 (c, m, x, length);
	  return;
	}
      memxor3 (c, m, x, CHACHA_BLOCK_SIZE);

      length -= CHACHA_BLOCK_SIZE;
      c += CHACHA_BLOCK_SIZE;
      m += CHACHA_BLOCK_SIZE;
  }
}
