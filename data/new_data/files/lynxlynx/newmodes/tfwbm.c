#include <string.h>
#include "tfdef.h"

static inline void xor_block(void *d, const void *s, size_t n)
{
	const size_t *ss = s;
	size_t x, *dd = d;
	for (x = 0; x < n; x++) dd[x] ^= ss[x];
}

void tf_wbm_encrypt(const void *key, void *ctr, void *out, const void *in, size_t sz, size_t bpi)
{
	const TF_BYTE_TYPE *uin = in;
	TF_BYTE_TYPE *uout = out;
	TF_UNIT_TYPE tctr[TF_NR_BLOCK_UNITS], bctr[TF_NR_BLOCK_UNITS];
	TF_UNIT_TYPE *uctr = ctr, z;
	const TF_UNIT_TYPE *ukey = key;
	size_t sl = sz, sx = TF_BLOCKS_TO_BYTES(bpi), i;

	if (sl >= sx) {
		z = (TF_UNIT_TYPE)bpi;
		do {
			tf_encrypt_rawblk(tctr, uctr, ukey);
			memcpy(bctr, tctr, TF_BLOCK_SIZE);

			tf_encrypt_block(key, uout, uin);
			tf_cbc_encrypt(key, tctr, uout+TF_BLOCK_SIZE, uin+TF_BLOCK_SIZE, sx-TF_BLOCK_SIZE);
			for (i = 1; i < bpi; i++) xor_block(uout, uout+(TF_BLOCK_SIZE*i), TF_BLOCK_SIZE / sizeof(size_t));
			memcpy(tctr, bctr, TF_BLOCK_SIZE);
			tf_pcbc_encrypt(key, tctr, uout, uout, sx);

			ctr_add(uctr, TF_NR_BLOCK_UNITS, &z, 1);

			uout += sx;
			uin += sx;
		} while ((sl -= sx) >= sx);
	}

	if (sl) {
		if (sl <= TF_BLOCK_SIZE) {
			tf_encrypt_rawblk(tctr, uctr, ukey);
			tf_ctr_crypt(key, tctr, uout, uin, sl);
			ctr_inc(uctr, TF_NR_BLOCK_UNITS);
			return;
		}

		z = TF_BLOCKS_FROM_BYTES(sl);
		tf_encrypt_rawblk(tctr, uctr, ukey);
		memcpy(bctr, tctr, TF_BLOCK_SIZE);

		tf_encrypt_block(key, uout, uin);
		tf_cbc_encrypt(key, tctr, uout+TF_BLOCK_SIZE, uin+TF_BLOCK_SIZE, sl-TF_BLOCK_SIZE);
		for (i = 1; i < TF_BLOCKS_FROM_BYTES(sl); i++) xor_block(uout, uout+(TF_BLOCK_SIZE*i), TF_BLOCK_SIZE / sizeof(size_t));
		memcpy(tctr, bctr, TF_BLOCK_SIZE);
		tf_pcbc_encrypt(key, tctr, uout, uout, sl);

		ctr_add(uctr, TF_NR_BLOCK_UNITS, &z, 1);
	}

	memset(bctr, 0, TF_BLOCK_SIZE);
	memset(tctr, 0, TF_BLOCK_SIZE);
}

void tf_wbm_decrypt(const void *key, void *ctr, void *out, const void *in, size_t sz, size_t bpi)
{
	const TF_BYTE_TYPE *uin = in;
	TF_BYTE_TYPE *uout = out;
	TF_UNIT_TYPE tctr[TF_NR_BLOCK_UNITS], bctr[TF_NR_BLOCK_UNITS];
	TF_UNIT_TYPE *uctr = ctr, z;
	const TF_UNIT_TYPE *ukey = key;
	size_t sl = sz, sx = TF_BLOCKS_TO_BYTES(bpi), i;

	if (sl >= sx) {
		z = (TF_UNIT_TYPE)bpi;
		do {
			tf_encrypt_rawblk(tctr, uctr, ukey);
			memcpy(bctr, tctr, TF_BLOCK_SIZE);

			tf_pcbc_decrypt(key, tctr, uout, uin, sx);
			for (i = 1; i < bpi; i++) xor_block(uout, uout+(TF_BLOCK_SIZE*i), TF_BLOCK_SIZE / sizeof(size_t));
			memcpy(tctr, bctr, TF_BLOCK_SIZE);
			tf_cbc_decrypt(key, tctr, uout+TF_BLOCK_SIZE, uout+TF_BLOCK_SIZE, sx-TF_BLOCK_SIZE);
			tf_decrypt_block(key, uout, uout);

			ctr_add(uctr, TF_NR_BLOCK_UNITS, &z, 1);

			uout += sx;
			uin += sx;
		} while ((sl -= sx) >= sx);
	}

	if (sl) {
		if (sl <= TF_BLOCK_SIZE) {
			tf_encrypt_rawblk(tctr, uctr, ukey);
			tf_ctr_crypt(key, tctr, uout, uin, sl);
			ctr_inc(uctr, TF_NR_BLOCK_UNITS);
			return;
		}

		z = TF_BLOCKS_FROM_BYTES(sl);
		tf_encrypt_rawblk(tctr, uctr, ukey);
		memcpy(bctr, tctr, TF_BLOCK_SIZE);

		tf_pcbc_decrypt(key, tctr, uout, uin, sl);
		for (i = 1; i < TF_BLOCKS_FROM_BYTES(sl); i++) xor_block(uout, uout+(TF_BLOCK_SIZE*i), TF_BLOCK_SIZE / sizeof(size_t));
		memcpy(tctr, bctr, TF_BLOCK_SIZE);
		tf_cbc_decrypt(key, tctr, uout+TF_BLOCK_SIZE, uout+TF_BLOCK_SIZE, sl-TF_BLOCK_SIZE);
		tf_decrypt_block(key, uout, uout);

		ctr_add(uctr, TF_NR_BLOCK_UNITS, &z, 1);
	}

	memset(bctr, 0, TF_BLOCK_SIZE);
	memset(tctr, 0, TF_BLOCK_SIZE);
}
