#include <string.h>
#include "tfdef.h"

void tf_xpcbc_encrypt(const void *key, void *ctr, void *out, const void *in, size_t sz, size_t bpi)
{
	const TF_BYTE_TYPE *uin = in;
	TF_BYTE_TYPE *uout = out;
	TF_UNIT_TYPE tctr[TF_NR_BLOCK_UNITS], *uctr = ctr, z;
	const TF_UNIT_TYPE *ukey = key;
	size_t sl = sz, sx = TF_BLOCKS_TO_BYTES(bpi);

	if (sl >= sx) {
		z = (TF_UNIT_TYPE)bpi;
		do {
			tf_encrypt_rawblk(tctr, uctr, ukey);
			tf_pcbc_encrypt(key, tctr, uout, uin, sx);
			ctr_add(uctr, TF_NR_BLOCK_UNITS, &z, 1);
			uout += sx;
			uin += sx;
		} while ((sl -= sx) >= sx);
	}

	if (sl) {
		z = TF_BLOCKS_FROM_BYTES(sl);
		tf_encrypt_rawblk(tctr, uctr, ukey);
		tf_pcbc_encrypt(key, tctr, uout, uin, sl);
		ctr_add(uctr, TF_NR_BLOCK_UNITS, &z, 1);
	}

	memset(tctr, 0, TF_BLOCK_SIZE);
}

void tf_xpcbc_decrypt(const void *key, void *ctr, void *out, const void *in, size_t sz, size_t bpi)
{
	const TF_BYTE_TYPE *uin = in;
	TF_BYTE_TYPE *uout = out;
	TF_UNIT_TYPE tctr[TF_NR_BLOCK_UNITS], *uctr = ctr, z;
	const TF_UNIT_TYPE *ukey = key;
	size_t sl = sz, sx = TF_BLOCKS_TO_BYTES(bpi);

	if (sl >= sx) {
		z = (TF_UNIT_TYPE)bpi;
		do {
			tf_encrypt_rawblk(tctr, uctr, ukey);
			tf_pcbc_decrypt(key, tctr, uout, uin, sx);
			ctr_add(uctr, TF_NR_BLOCK_UNITS, &z, 1);
			uout += sx;
			uin += sx;
		} while ((sl -= sx) >= sx);
	}

	if (sl) {
		z = TF_BLOCKS_FROM_BYTES(sl);
		tf_encrypt_rawblk(tctr, uctr, ukey);
		tf_pcbc_decrypt(key, tctr, uout, uin, sl);
		ctr_add(uctr, TF_NR_BLOCK_UNITS, &z, 1);
	}

	memset(tctr, 0, TF_BLOCK_SIZE);
}
