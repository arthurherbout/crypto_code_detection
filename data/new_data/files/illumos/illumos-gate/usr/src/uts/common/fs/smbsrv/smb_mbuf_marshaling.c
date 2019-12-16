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
 * Copyright 2018 Nexenta Systems, Inc.  All rights reserved.
 */

/*
 * SMB mbuf marshaling encode/decode.
 */

#include <smbsrv/smb_kproto.h>


#define	MALLOC_QUANTUM	80

#define	DECODE_NO_ERROR		0
#define	DECODE_NO_MORE_DATA	1
#define	DECODE_ALLOCATION_ERROR	2
#define	DECODE_CONVERSION_ERROR	3

static int mbc_marshal_make_room(mbuf_chain_t *, int32_t);
static void mbc_marshal_store_byte(mbuf_chain_t *, uint8_t);
static int mbc_marshal_put_char(mbuf_chain_t *mbc, uint8_t);
static int mbc_marshal_put_short(mbuf_chain_t *mbc, uint16_t);
static int mbc_marshal_put_long(mbuf_chain_t *mbc, uint32_t);
static int mbc_marshal_put_long_long(mbuf_chain_t *mbc, uint64_t);
static int mbc_marshal_put_oem_string(mbuf_chain_t *, char *, int);
static int mbc_marshal_put_unicode_string(mbuf_chain_t *, char *, int);
static int mbc_marshal_put_uio(mbuf_chain_t *, struct uio *);
static int mbc_marshal_put_mbufs(mbuf_chain_t *mbc, mbuf_t *m);
static int mbc_marshal_put_mbuf_chain(mbuf_chain_t *mbc, mbuf_chain_t *nmbc);
static uint8_t mbc_marshal_fetch_byte(mbuf_chain_t *mbc);
static int mbc_marshal_get_char(mbuf_chain_t *mbc, uint8_t *data);
static int mbc_marshal_get_short(mbuf_chain_t *mbc, uint16_t *data);
static int mbc_marshal_get_long(mbuf_chain_t *mbc, uint32_t *data);
static uint64_t qswap(uint64_t ll);
static int mbc_marshal_get_odd_long_long(mbuf_chain_t *mbc, uint64_t *data);
static int mbc_marshal_get_long_long(mbuf_chain_t *mbc, uint64_t *data);
static int mbc_marshal_get_oem_string(smb_request_t *, mbuf_chain_t *,
    char **, int);
static int mbc_marshal_get_unicode_string(smb_request_t *, mbuf_chain_t *,
    char **, int);
static int mbc_marshal_get_mbufs(mbuf_chain_t *, int32_t, mbuf_t **);
static int mbc_marshal_get_mbuf_chain(mbuf_chain_t *, int32_t, mbuf_chain_t *);
static int mbc_marshal_get_uio(mbuf_chain_t *, struct uio *);
static int mbc_marshal_get_skip(mbuf_chain_t *, uint_t);

/*
 * smb_mbc_vdecodef
 *
 * This function reads the contents of the mbc chain passed in under the list
 * of arguments passed in.
 *
 * The format string provides a description of the parameters passed in as well
 * as an action to be taken by smb_mbc_vdecodef().
 *
 *	%	Pointer to an SMB request structure (smb_request_t *). There
 *		should be only one of these in the string.
 *
 *	C	Pointer to an mbuf chain. Copy to that mbuf chain the number of
 *		bytes specified (number preceding C).
 *
 *	m	Pointer to an mbuf. Copy to that mbuf the number of bytes
 *		specified (number preceding m).
 *
 *	M	Read the 32 bit value at the current location of the mbuf chain
 *		and check if it matches the signature of an SMB1 request (SMBx).
 *
 *	N	Read the 32 bit value at the current location of the mbuf chain
 *		and check if it matches the signature of an SMB2 request (SMBx).
 *
 *	b	Pointer to a buffer. Copy to that buffer the number of bytes
 *		specified (number preceding b).
 *
 *	c	Same as 'b'.
 *
 *	w	Pointer to a word (16bit value). Copy the next 16bit value into
 *		that location.
 *
 *	l	Pointer to a long (32bit value). Copy the next 32bit value into
 *		that location.
 *
 *	q	Pointer to a quad (64bit value). Copy the next 64bit value into
 *		that location.
 *
 *	Q	Same as above with a call to qswap().
 *
 *	B	Pointer to a vardata_block structure. That structure is used to
 *		retrieve data from the mbuf chain (an iovec type structure is
 *		embedded in a vardata_block).
 *
 *	D	Pointer to a vardata_block structure. That structure is used to
 *		retrieve data from the mbuf chain, however, two fields of the
 *		vardata_block structure (tag and len) are first initialized
 *		using the mbuf chain itself.
 *
 *	V	Same as 'D'.
 *
 *	L
 *
 *	A
 *
 *	P	Same as 'A'
 *
 *	S	Same as 'A'
 *
 *	u	Pointer to a string pointer. Allocate memory and retrieve the
 *		string at the current location in the mbuf chain. Store the
 *		address to the buffer allocated at the address specified by
 *		the pointer. In addition if an sr was passed and it indicates
 *		that the string is an unicode string, convert it.
 *
 *	s	Same as 'u' without convertion.
 *
 *	U	Same as 'u'. The string to retrieve is unicode.
 *
 *	y	Pointer to a 32bit value. Read the dos time at the current mbuf
 *		chain location, convert it to unix time and store it at the
 *		location indicated by the pointer.
 *
 *	Y	Same as 'y' bt the dos time coded in the mbuf chain is inverted.
 *
 *	.	Skip the number of bytes indicated by the number preceding '.'.
 *
 *	,	Same as '.' but take in account it is an unicode string.
 */
int
smb_mbc_vdecodef(mbuf_chain_t *mbc, const char *fmt, va_list ap)
{
	uint8_t		c;
	uint8_t		cval;
	uint8_t		*cvalp;
	char		**charpp;
	uint16_t	wval;
	uint16_t	*wvalp;
	uint32_t	*lvalp;
	uint64_t	*llvalp;
	smb_vdb_t	*vdp;
	smb_request_t	*sr = NULL;
	uint32_t	lval;
	int		unicode = 0;
	int		repc;
	boolean_t	repc_specified;

	while ((c = *fmt++) != 0) {
		repc_specified = B_FALSE;
		repc = 1;

		if ('0' <= c && c <= '9') {
			repc = 0;
			do {
				repc = repc * 10 + c - '0';
				c = *fmt++;
			} while ('0' <= c && c <= '9');
			repc_specified = B_TRUE;
		} else if (c == '#') {
			repc = va_arg(ap, int);
			c = *fmt++;
			repc_specified = B_TRUE;
		}

		switch (c) {
		case '%':
			sr = va_arg(ap, struct smb_request *);
			if (sr->session->dialect >= SMB_VERS_2_BASE) {
				unicode = 1;
				break;
			}
			unicode = sr->smb_flg2 & SMB_FLAGS2_UNICODE;
			break;

		case 'C':	/* Mbuf_chain */
			if (mbc_marshal_get_mbuf_chain(mbc, repc,
			    va_arg(ap, mbuf_chain_t *)) != 0)
				return (-1);
			break;

		case 'm':	/* struct_mbuf */
			if (mbc_marshal_get_mbufs(mbc, repc,
			    va_arg(ap, mbuf_t **)) != 0)
				return (-1);
			break;

		case 'M':
			if (mbc_marshal_get_long(mbc, &lval) != 0)
				return (-1);
			if (lval != 0x424D53FF) /* 0xFF S M B */
				return (-1);
			break;

		case 'N':
			if (mbc_marshal_get_long(mbc, &lval) != 0)
				return (-1);
			if (lval != 0x424D53FE) /* 0xFE S M B */
				return (-1);
			break;

		case 'b':
		case 'c':
			cvalp = va_arg(ap, uint8_t *);
			if (MBC_ROOM_FOR(mbc, repc) == 0)
				/* Data will never be available */
				return (-1);

			while (repc-- > 0)
				*cvalp++ = mbc_marshal_fetch_byte(mbc);
			break;

		case 'w':
			wvalp = va_arg(ap, uint16_t *);
			while (repc-- > 0)
				if (mbc_marshal_get_short(mbc, wvalp++) != 0)
					return (-1);
			break;

		case 'l':
			lvalp = va_arg(ap, uint32_t *);
			while (repc-- > 0)
				if (mbc_marshal_get_long(mbc, lvalp++) != 0)
					return (-1);
			break;

		case 'q':
			llvalp = va_arg(ap, uint64_t *);
			while (repc-- > 0)
				if (mbc_marshal_get_long_long(
				    mbc, llvalp++) != 0)
					return (-1);
			break;

		case 'Q':
			llvalp = va_arg(ap, uint64_t *);
			while (repc-- > 0)
				if (mbc_marshal_get_odd_long_long(
				    mbc, llvalp++) != 0)
					return (-1);
			break;

		case 'B':
			vdp = va_arg(ap, struct vardata_block *);
			vdp->vdb_tag = 0;
			vdp->vdb_len = repc;
			vdp->vdb_uio.uio_iov = &vdp->vdb_iovec[0];
			vdp->vdb_uio.uio_iovcnt = MAX_IOVEC;
			vdp->vdb_uio.uio_extflg = UIO_COPY_DEFAULT;
			vdp->vdb_uio.uio_resid = repc;
			if (mbc_marshal_get_uio(mbc, &vdp->vdb_uio) != 0)
				return (-1);
			break;

		case 'D':
		case 'V':
			vdp = va_arg(ap, struct vardata_block *);
			if (mbc_marshal_get_char(mbc, &vdp->vdb_tag) != 0)
				return (-1);
			if (mbc_marshal_get_short(mbc, &wval) != 0)
				return (-1);
			vdp->vdb_len = (uint32_t)wval;
			vdp->vdb_uio.uio_iov = &vdp->vdb_iovec[0];
			vdp->vdb_uio.uio_iovcnt = MAX_IOVEC;
			vdp->vdb_uio.uio_extflg = UIO_COPY_DEFAULT;
			vdp->vdb_uio.uio_resid = vdp->vdb_len;
			if (vdp->vdb_len != 0) {
				if (mbc_marshal_get_uio(mbc,
				    &vdp->vdb_uio) != 0)
					return (-1);
			}
			break;

		case 'L':
			if (mbc_marshal_get_char(mbc, &cval) != 0)
				return (-1);
			if (cval != 2)
				return (-1);
			goto oem_conversion;

		case 'A':
		case 'S':
			if (mbc_marshal_get_char(mbc, &cval) != 0)
				return (-1);
			if (((c == 'A' || c == 'S') && cval != 4) ||
			    (c == 'L' && cval != 2))
				return (-1);
			/* FALLTHROUGH */

		case 'u': /* Convert from unicode if flags are set */
			if (unicode)
				goto unicode_translation;
			/* FALLTHROUGH */

		case 's':	/* get OEM string */
oem_conversion:
			ASSERT(sr != NULL);
			charpp = va_arg(ap, char **);
			if (!repc_specified)
				repc = 0;
			if (mbc_marshal_get_oem_string(sr,
			    mbc, charpp, repc) != 0)
				return (-1);
			break;

		case 'U':	/* get UTF-16 string */
unicode_translation:
			ASSERT(sr != 0);
			charpp = va_arg(ap, char **);
			if (!repc_specified)
				repc = 0;
			if (mbc_marshal_get_unicode_string(sr,
			    mbc, charpp, repc) != 0)
				return (-1);
			break;

		case 'Y': /* dos time to unix time tt/dd */
			lvalp = va_arg(ap, uint32_t *);
			while (repc-- > 0) {
				short	d, t;

				if (mbc_marshal_get_short(mbc,
				    (uint16_t *)&t) != 0)
					return (-1);
				if (mbc_marshal_get_short(mbc,
				    (uint16_t *)&d) != 0)
					return (-1);
				*lvalp++ = smb_time_dos_to_unix(d, t);
			}
			break;

		case 'y': /* dos time to unix time dd/tt */
			lvalp = va_arg(ap, uint32_t *);
			while (repc-- > 0) {
				short	d, t;

				if (mbc_marshal_get_short(mbc,
				    (uint16_t *)&d) != 0)
					return (-1);
				if (mbc_marshal_get_short(mbc,
				    (uint16_t *)&t) != 0)
					return (-1);
				*lvalp++ = smb_time_dos_to_unix(d, t);
			}
			break;

		case ',':
			if (unicode)
				repc *= 2;
			/* FALLTHROUGH */

		case '.':
			if (mbc_marshal_get_skip(mbc, repc) != 0)
				return (-1);
			break;

		default:
			ASSERT(0);
			return (-1);
		}
	}
	return (0);
}

/*
 * smb_mbc_decodef
 *
 * This function reads the contents of the mbc chain passed in under the
 * control of the format fmt.
 *
 * (for a description of the format string see smb_mbc_vencodef()).
 */
int
smb_mbc_decodef(mbuf_chain_t *mbc, const char *fmt, ...)
{
	int	xx;
	va_list	ap;

	va_start(ap, fmt);
	xx = smb_mbc_vdecodef(mbc, fmt, ap);
	va_end(ap);
	return (xx);
}

/*
 * smb_mbc_peek
 *
 * This function reads the contents of the mbc passed in at the specified offset
 * under the control of the format fmt. The offset of the chain passed in is not
 * modified.
 *
 * (for a description of the format string see smb_mbc_vdecodef()).
 */
int
smb_mbc_peek(mbuf_chain_t *mbc, int offset, const char *fmt, ...)
{
	mbuf_chain_t	tmp;
	va_list		ap;
	int		xx;

	va_start(ap, fmt);

	(void) MBC_SHADOW_CHAIN(&tmp, mbc, offset, mbc->max_bytes - offset);
	xx = smb_mbc_vdecodef(&tmp, fmt, ap);
	va_end(ap);
	return (xx);
}

/*
 * smb_mbc_vencodef
 *
 * This function builds a stream of bytes in the mbc chain passed in under the
 * control of the list of arguments passed in.
 *
 * The format string provides a description of the parameters passed in as well
 * as an action to be taken by smb_mbc_vencodef().
 *
 *	\b	Restore the mbuf chain offset to its initial value.
 *
 *	%	Pointer to an SMB request structure (smb_request_t *). There
 *		should be only one of these in the string. If an sr in present
 *		it will be used to determine if unicode conversion should be
 *		applied to the strings.
 *
 *	C	Pointer to an mbuf chain. Copy that mbuf chain into the
 *		destination mbuf chain.
 *
 *	D	Pointer to a vardata_block structure. Copy the data described
 *		by that structure into the mbuf chain. The tag field is hard
 *		coded to '1'.
 *
 *	M	Write the SMB1 request signature ('SMBX') into the mbuf chain.
 *
 *	N	Write the SMB2 request signature ('SMBX') into the mbuf chain.
 *
 *	T	Pointer to a timestruc_t. Convert the content of the structure
 *		into NT time and store the result of the conversion in the
 *		mbuf chain.
 *
 *	V	Same as 'D' but the tag field is hard coded to '5'.
 *
 *	b	Byte. Store the byte or the nymber of bytes specified into the
 *		the mbuf chain. A format string like this "2b" would require 2
 *		bytes to be passed in.
 *
 *	m	Pointer to an mbuf. Copy the contents of the mbuf into the mbuf
 *		chain.
 *
 *	c	Pointer to a buffer. Copy the buffer into the mbuf chain. The
 *		size of the buffer is indicated by the number preceding 'c'.
 *
 *	w	Word (16bit value). Store the word or the number of words
 *              specified into the the mbuf chain. A format string like this
 *		"2w" would require 2 words to be passed in.
 *
 *	l	Long (32bit value). Store the long or the number of longs
 *		specified into the the mbuf chain. A format string like this
 *		"2l" would require 2 longs to be passed in.
 *
 *	q	Quad (64bit value). Store the quad or the number of quads
 *		specified into the the mbuf chain. A format string like this
 *		"2q" would require 2 quads to be passed in.
 *
 *	L	Pointer to a string. Store the string passed in into the mbuf
 *		chain preceded with a tag value of '2'.
 *
 *	S	Pointer to a string. Store the string passed in into the mbuf
 *		chain preceded with a tag value of '4'. Applied a unicode
 *		conversion is appropriate.
 *
 *	A	Same as 'S'
 *
 *	P	Pointer to a string. Store the string passed in into the mbuf
 *		chain preceded with a tag value of '5'. Applied a unicode
 *		conversion is appropriate.
 *
 *	u	Pointer to a string. Store the string passed in into the mbuf
 *		chain. Applied a unicode conversion is appropriate.
 *
 *	s	Pointer to a string. Store the string passed in into the mbuf
 *		chain.
 *
 *	Y	Date/Time.  Store the Date/Time or the number of Date/Time(s)
 *		specified into the the mbuf chain. A format string like this
 *		"2Y" would require 2 Date/Time values. The Date/Time is
 *		converted to DOS before storing.
 *
 *	y	Same as 'Y'. The order of Date and Time is reversed.
 *
 *	,	Character. Store the character or number of character specified
 *		into the mbuf chain.  A format string like this "2c" would
 *		require 2 characters to be passed in. A unicode conversion is
 *		applied if appropriate.
 *
 *	.	Same as '`' without unicode conversion.
 *
 *	U	Align the offset of the mbuf chain on a 16bit boundary.
 */
int
smb_mbc_vencodef(mbuf_chain_t *mbc, const char *fmt, va_list ap)
{
	char		*charp;
	uint8_t		*cvalp;
	timestruc_t	*tvp;
	smb_vdb_t	*vdp;
	smb_request_t	*sr = NULL;
	uint64_t	llval;
	int64_t		nt_time;
	uint32_t	lval;
	uint_t		tag;
	int		unicode = 0;
	int		repc;
	boolean_t	repc_specified;
	uint16_t	wval;
	uint8_t		cval;
	uint8_t		c;

	while ((c = *fmt++) != 0) {
		repc_specified = B_FALSE;
		repc = 1;

		if ('0' <= c && c <= '9') {
			repc = 0;
			do {
				repc = repc * 10 + c - '0';
				c = *fmt++;
			} while ('0' <= c && c <= '9');
			repc_specified = B_TRUE;
		} else if (c == '#') {
			repc = va_arg(ap, int);
			c = *fmt++;
			repc_specified = B_TRUE;
		}

		switch (c) {
		case '%':
			sr = va_arg(ap, struct smb_request *);
			if (sr->session->dialect >= SMB_VERS_2_BASE) {
				unicode = 1;
				break;
			}
			unicode = sr->smb_flg2 & SMB_FLAGS2_UNICODE;
			break;

		case 'C':	/* Mbuf_chain */
			if (mbc_marshal_put_mbuf_chain(mbc,
			    va_arg(ap, mbuf_chain_t *)) != 0)
				return (DECODE_NO_MORE_DATA);
			break;

		case 'D':
			vdp = va_arg(ap, struct vardata_block *);

			if (mbc_marshal_put_char(mbc, 1) != 0)
				return (DECODE_NO_MORE_DATA);
			if (mbc_marshal_put_short(mbc, vdp->vdb_len) != 0)
				return (DECODE_NO_MORE_DATA);
			if (mbc_marshal_put_uio(mbc, &vdp->vdb_uio) != 0)
				return (DECODE_NO_MORE_DATA);
			break;

		case 'M':
			/* 0xFF S M B */
			if (mbc_marshal_put_long(mbc, 0x424D53FF))
				return (DECODE_NO_MORE_DATA);
			break;

		case 'N':
			/* 0xFE S M B */
			if (mbc_marshal_put_long(mbc, 0x424D53FE))
				return (DECODE_NO_MORE_DATA);
			break;

		case 'T':
			tvp = va_arg(ap, timestruc_t *);
			nt_time = smb_time_unix_to_nt(tvp);
			if (mbc_marshal_put_long_long(mbc, nt_time) != 0)
				return (DECODE_NO_MORE_DATA);
			break;

		case 'V':
			vdp = va_arg(ap, struct vardata_block *);

			if (mbc_marshal_put_char(mbc, 5) != 0)
				return (DECODE_NO_MORE_DATA);
			if (mbc_marshal_put_short(mbc, vdp->vdb_len) != 0)
				return (DECODE_NO_MORE_DATA);
			if (mbc_marshal_put_uio(mbc, &vdp->vdb_uio) != 0)
				return (DECODE_NO_MORE_DATA);
			break;

		case 'b':
			while (repc-- > 0) {
				cval = va_arg(ap, int);
				if (mbc_marshal_put_char(mbc, cval) != 0)
					return (DECODE_NO_MORE_DATA);
			}
			break;

		case 'm':	/* struct_mbuf */
			if (mbc_marshal_put_mbufs(mbc,
			    va_arg(ap, mbuf_t *)) != 0)
				return (DECODE_NO_MORE_DATA);
			break;

		case 'c':
			cvalp = va_arg(ap, uint8_t *);
			while (repc-- > 0) {
				if (mbc_marshal_put_char(mbc,
				    *cvalp++) != 0)
					return (DECODE_NO_MORE_DATA);
			}
			break;

		case 'w':
			while (repc-- > 0) {
				wval = va_arg(ap, int);
				if (mbc_marshal_put_short(mbc, wval) != 0)
					return (DECODE_NO_MORE_DATA);
			}
			break;

		case 'l':
			while (repc-- > 0) {
				lval = va_arg(ap, uint32_t);
				if (mbc_marshal_put_long(mbc, lval) != 0)
					return (DECODE_NO_MORE_DATA);
			}
			break;

		case 'q':
			while (repc-- > 0) {
				llval = va_arg(ap, uint64_t);
				if (mbc_marshal_put_long_long(mbc, llval) != 0)
					return (DECODE_NO_MORE_DATA);
			}
			break;


		case 'L':
			tag = 2;
			goto oem_conversion;

		case 'S':
		case 'A':
			tag = 4;
			goto tagged_str;

		case 'P':
			tag = 3;
			goto tagged_str;

		tagged_str:
			if (mbc_marshal_put_char(mbc, tag) != 0)
				return (DECODE_NO_MORE_DATA);
			/* FALLTHROUGH */

		case 'u':	/* Convert from unicode if flags are set */
			if (unicode)
				goto unicode_translation;
			/* FALLTHROUGH */

		case 's':	/* put OEM string */
oem_conversion:
			charp = va_arg(ap, char *);
			if (!repc_specified)
				repc = 0;
			if (mbc_marshal_put_oem_string(mbc,
			    charp, repc) != 0)
				return (DECODE_NO_MORE_DATA);
			break;

		case 'U':	/* put UTF-16 string */
unicode_translation:
			charp = va_arg(ap, char *);
			if (!repc_specified)
				repc = 0;
			if (mbc_marshal_put_unicode_string(mbc,
			    charp, repc) != 0)
				return (DECODE_NO_MORE_DATA);
			break;

		case 'Y':		/* int32_t, encode dos date/time */
			while (repc-- > 0) {
				uint16_t	d, t;

				lval = va_arg(ap, uint32_t);
				smb_time_unix_to_dos(lval,
				    (short *)&d, (short *)&t);
				if (mbc_marshal_put_short(mbc, t) != 0)
					return (DECODE_NO_MORE_DATA);
				if (mbc_marshal_put_short(mbc, d) != 0)
					return (DECODE_NO_MORE_DATA);
			}
			break;

		case 'y':		/* int32_t, encode dos date/time */
			while (repc-- > 0) {
				uint16_t	d, t;

				lval = va_arg(ap, uint32_t);
				smb_time_unix_to_dos(lval,
				    (short *)&d, (short *)&t);
				if (mbc_marshal_put_short(mbc, d) != 0)
					return (DECODE_NO_MORE_DATA);
				if (mbc_marshal_put_short(mbc, t) != 0)
					return (DECODE_NO_MORE_DATA);
			}
			break;

		case ',':
			if (unicode)
				repc *= 2;
			/* FALLTHROUGH */

		case '.':
			while (repc-- > 0)
				if (mbc_marshal_put_char(mbc, 0) != 0)
					return (DECODE_NO_MORE_DATA);
			break;

		default:
			ASSERT(0);
			return (-1);
		}
	}
	return (0);
}

/*
 * smb_mbc_encodef
 *
 * This function builds a stream of bytes in the mbc chain passed in under the
 * control of the format fmt.
 *
 * (for a description of the format string see smb_mbc_vencodef()).
 */
int
smb_mbc_encodef(mbuf_chain_t *mbc, const char *fmt, ...)
{
	int	rc;
	va_list	ap;

	va_start(ap, fmt);
	rc = smb_mbc_vencodef(mbc, fmt, ap);
	va_end(ap);
	return (rc);
}

/*
 * smb_mbc_poke
 *
 * This function writes a stream of bytes in the mbc passed in at the specified
 * offset under the control of the format fmt. The offset of the chain passed in
 * is not modified.
 *
 * (for a description of the format string see smb_mbc_vencodef()).
 */
int
smb_mbc_poke(mbuf_chain_t *mbc, int offset, const char *fmt, ...)
{
	int		len, rc;
	mbuf_chain_t	tmp;
	va_list		ap;

	if ((len = mbc->max_bytes - offset) < 0)
		return (DECODE_NO_MORE_DATA);
	rc = MBC_SHADOW_CHAIN(&tmp, mbc, offset, len);
	if (rc)
		return (DECODE_NO_MORE_DATA);

	va_start(ap, fmt);
	rc = smb_mbc_vencodef(&tmp, fmt, ap);
	va_end(ap);

	return (rc);
}

/*
 * Copy data from the src mbuf chain to the dst mbuf chain,
 * at the given offset in the src and current offset in dst,
 * for copy_len bytes.  Does NOT update src->chain_offset.
 */
int
smb_mbc_copy(mbuf_chain_t *dst_mbc, const mbuf_chain_t *src_mbc,
	int copy_offset, int copy_len)
{
	mbuf_t	*src_m;
	int offset, len;
	int rc;

	if (copy_len <= 0)
		return (0);
	if (copy_offset < 0)
		return (EINVAL);
	if ((copy_offset + copy_len) > src_mbc->max_bytes)
		return (EMSGSIZE);

	/*
	 * Advance to the src mbuf where we start copying.
	 */
	offset = copy_offset;
	src_m = src_mbc->chain;
	while (src_m && offset >= src_m->m_len) {
		offset -= src_m->m_len;
		src_m = src_m->m_next;
	}
	if (src_m == NULL)
		return (EFAULT);

	/*
	 * Copy the first part, which may start somewhere past
	 * the beginning of the current mbuf.
	 */
	len = src_m->m_len - offset;
	if (len > copy_len)
		len = copy_len;
	rc = smb_mbc_put_mem(dst_mbc, src_m->m_data + offset, len);
	if (rc != 0)
		return (rc);
	copy_len -= len;

	/*
	 * Copy remaining mbufs...
	 */
	while (copy_len > 0) {
		src_m = src_m->m_next;
		if (src_m == NULL)
			break;
		len = src_m->m_len;
		if (len > copy_len)
			len = copy_len;
		rc = smb_mbc_put_mem(dst_mbc, src_m->m_data, len);
		copy_len -= len;
	}

	return (0);
}

/*
 * Copy data from the passed memory buffer into the mbuf chain
 * at the current offset.
 */
int
smb_mbc_put_mem(mbuf_chain_t *mbc, void *vmem, int mem_len)
{
	caddr_t mem = vmem;
	mbuf_t	*m;
	int32_t	offset, tlen;
	int rc;

	if (mem_len <= 0)
		return (0);

	if ((rc = mbc_marshal_make_room(mbc, mem_len)) != 0)
		return (rc);

	/*
	 * Advance to the dst mbuf where we start copying.
	 * Allocations were done by _make_room().
	 */
	offset = mbc->chain_offset;
	m = mbc->chain;
	while (offset >= m->m_len) {
		ASSERT(m->m_len > 0);
		offset -= m->m_len;
		m = m->m_next;
	}

	/*
	 * Copy the first part, which may start somewhere past
	 * the beginning of the current mbuf.
	 */
	tlen = m->m_len - offset;
	if (tlen > mem_len)
		tlen = mem_len;
	bcopy(mem, m->m_data + offset, tlen);
	mbc->chain_offset += tlen;
	mem += tlen;
	mem_len -= tlen;

	/*
	 * Copy remaining mem into mbufs.  These all start
	 * at the beginning of each mbuf, and the last may
	 * end somewhere short of m_len.
	 */
	while (mem_len > 0) {
		m = m->m_next;
		tlen = m->m_len;
		if (tlen > mem_len)
			tlen = mem_len;
		bcopy(mem, m->m_data, tlen);
		mbc->chain_offset += tlen;
		mem += tlen;
		mem_len -= tlen;
	}

	return (0);
}

/*
 * Put padding sufficient to align to A, where
 * A is some power of 2 greater than zero.
 */
int
smb_mbc_put_align(mbuf_chain_t *mbc, int align)
{
	int mask = align - 1;
	int padsz;

	ASSERT(align > 0 && (align & mask) == 0);
	if ((mbc->chain_offset & mask) == 0)
		return (0);
	padsz = align - (mbc->chain_offset & mask);
	return (smb_mbc_encodef(mbc, "#.", padsz));
}

/*
 * Put data into mbuf chain allocating as needed.
 * Adds room to end of mbuf chain if needed.
 */
static int
mbc_marshal_make_room(mbuf_chain_t *mbc, int32_t bytes_needed)
{
	mbuf_t	*m;
	mbuf_t	*l;
	int32_t	bytes_available;

	bytes_needed += mbc->chain_offset;
	if (bytes_needed > mbc->max_bytes)
		return (EMSGSIZE);

	if ((m = mbc->chain) == 0) {
		MGET(m, M_WAIT, MT_DATA);
		m->m_len = 0;
		MCLGET(m, M_WAIT);
		mbc->chain = m;
		/* xxxx */
		/* ^    */
	}

	/* ---- ----- --xx ---xxx */
	/* ^			  */

	l = 0;
	while ((m != 0) && (bytes_needed >= m->m_len)) {
		l = m;
		bytes_needed -= m->m_len;
		m = m->m_next;
	}

	if ((bytes_needed == 0) || (m != 0)) {
		/* We have enough room already */
		return (0);
	}

	/* ---- ----- --xx ---xxx */
	/*			 ^ */
	/* Back up to start of last mbuf */
	m = l;
	bytes_needed += m->m_len;

	/* ---- ----- --xx ---xxx */
	/*		   ^	  */

	bytes_available = (m->m_flags & M_EXT) ?
	    m->m_ext.ext_size : MLEN;

	/* ---- ----- --xx ---xxx */
	/*		   ^	  */
	while ((bytes_needed != 0) && (bytes_needed > bytes_available)) {
		m->m_len = bytes_available;
		bytes_needed -= m->m_len;
		/* ---- ----- --xx ------ */
		/*		   ^	  */

		MGET(m->m_next, M_WAIT, MT_DATA);
		m = m->m_next;
		m->m_len = 0;
		MCLGET(m, M_WAIT);

		ASSERT((m->m_flags & M_EXT) != 0);
		bytes_available = m->m_ext.ext_size;

		/* ---- ----- --xx ------ xxxx */
		/*			  ^    */
	}

	/* ---- ----- --xx ------ xxxx */
	/*			  ^    */
	/* Expand last tail as needed */
	if (m->m_len <= bytes_needed) {
		m->m_len = bytes_needed;
		/* ---- ----- --xx ------ --xx */
		/*			   ^   */
	}

	return (0);
}

static void
mbc_marshal_store_byte(mbuf_chain_t *mbc, uint8_t data)
{
	mbuf_t	*m = mbc->chain;
	int32_t	cur_offset = mbc->chain_offset;

	/*
	 * Scan forward looking for the last data currently in chain.
	 */
	while (cur_offset >= m->m_len) {
		cur_offset -= m->m_len;
		m = m->m_next;
	}
	((char *)m->m_data)[cur_offset] = data;
	mbc->chain_offset++;
}

static int
mbc_marshal_put_char(mbuf_chain_t *mbc, uint8_t data)
{
	if (mbc_marshal_make_room(mbc, sizeof (char)) != 0)
		return (DECODE_NO_MORE_DATA);
	mbc_marshal_store_byte(mbc, data);
	return (0);
}

static int
mbc_marshal_put_short(mbuf_chain_t *mbc, uint16_t data)
{
	if (mbc_marshal_make_room(mbc, sizeof (short)))
		return (DECODE_NO_MORE_DATA);
	mbc_marshal_store_byte(mbc, data);
	mbc_marshal_store_byte(mbc, data >> 8);
	return (0);
}

static int
mbc_marshal_put_long(mbuf_chain_t *mbc, uint32_t data)
{
	if (mbc_marshal_make_room(mbc, sizeof (int32_t)))
		return (DECODE_NO_MORE_DATA);
	mbc_marshal_store_byte(mbc, data);
	mbc_marshal_store_byte(mbc, data >> 8);
	mbc_marshal_store_byte(mbc, data >> 16);
	mbc_marshal_store_byte(mbc, data >> 24);
	return (0);
}

static int
mbc_marshal_put_long_long(mbuf_chain_t *mbc, uint64_t data)
{
	if (mbc_marshal_make_room(mbc, sizeof (int64_t)))
		return (DECODE_NO_MORE_DATA);

	mbc_marshal_store_byte(mbc, data);
	mbc_marshal_store_byte(mbc, data >> 8);
	mbc_marshal_store_byte(mbc, data >> 16);
	mbc_marshal_store_byte(mbc, data >> 24);
	mbc_marshal_store_byte(mbc, data >> 32);
	mbc_marshal_store_byte(mbc, data >> 40);
	mbc_marshal_store_byte(mbc, data >> 48);
	mbc_marshal_store_byte(mbc, data >> 56);
	return (0);
}

/*
 * Marshal a UTF-8 string (str) into mbc, converting to OEM codeset.
 * Also write a null unless the repc count limits the length we put.
 * When (repc > 0) the length we marshal must be exactly repc, and
 * truncate or pad the mbc data as necessary.
 * See also: msgbuf_put_oem_string
 */
static int
mbc_marshal_put_oem_string(mbuf_chain_t *mbc, char *mbs, int repc)
{
	uint8_t		*oembuf = NULL;
	uint8_t		*s;
	int		oemlen;
	int		rlen;
	int		rc;

	/*
	 * Compute length of converted OEM string,
	 * NOT including null terminator
	 */
	if ((oemlen = smb_sbequiv_strlen(mbs)) == -1)
		return (DECODE_NO_MORE_DATA);

	/*
	 * If repc not specified, put whole string + NULL,
	 * otherwise will truncate or pad as needed.
	 */
	if (repc <= 0)
		repc = oemlen + 1;
	if (mbc_marshal_make_room(mbc, repc))
		return (DECODE_NO_MORE_DATA);

	/*
	 * Convert into a temporary buffer
	 * Free oembuf before return.
	 */
	oembuf = smb_mem_zalloc(oemlen + 1);
	ASSERT(oembuf != NULL);
	rlen = smb_mbstooem(oembuf, mbs, oemlen);
	if (rlen < 0) {
		rc = DECODE_NO_MORE_DATA;
		goto out;
	}
	if (rlen > oemlen)
		rlen = oemlen;
	oembuf[rlen] = '\0';

	/*
	 * Copy the converted string into the message,
	 * truncated or paded as required.
	 */
	s = oembuf;
	while (repc > 0) {
		mbc_marshal_store_byte(mbc, *s);
		if (*s != '\0')
			s++;
		repc--;
	}
	rc = 0;

out:
	if (oembuf != NULL)
		smb_mem_free(oembuf);
	return (rc);
}

/*
 * Marshal a UTF-8 string (str) into mbc, converting to UTF-16.
 * Also write a null unless the repc count limits the length.
 * When (repc > 0) the length we marshal must be exactly repc,
 * and truncate or pad the mbc data as necessary.
 * See also: msgbuf_put_unicode_string
 */
static int
mbc_marshal_put_unicode_string(mbuf_chain_t *mbc, char *mbs, int repc)
{
	smb_wchar_t	*wcsbuf = NULL;
	smb_wchar_t	*wp;
	size_t		wcslen, wcsbytes;
	size_t		rlen;
	int		rc;

	/* align to word boundary */
	if (mbc->chain_offset & 1) {
		if (mbc_marshal_make_room(mbc, 1))
			return (DECODE_NO_MORE_DATA);
		mbc_marshal_store_byte(mbc, 0);
	}

	/*
	 * Compute length of converted UTF-16 string,
	 * NOT including null terminator (in bytes).
	 */
	wcsbytes = smb_wcequiv_strlen(mbs);
	if (wcsbytes == (size_t)-1)
		return (DECODE_NO_MORE_DATA);

	/*
	 * If repc not specified, put whole string + NULL,
	 * otherwise will truncate or pad as needed.
	 */
	if (repc <= 0)
		repc = wcsbytes + 2;
	if (mbc_marshal_make_room(mbc, repc))
		return (DECODE_NO_MORE_DATA);

	/*
	 * Convert into a temporary buffer
	 * Free wcsbuf before return.
	 */
	wcslen = wcsbytes / 2;
	wcsbuf = smb_mem_zalloc(wcsbytes + 2);
	ASSERT(wcsbuf != NULL);
	rlen = smb_mbstowcs(wcsbuf, mbs, wcslen);
	if (rlen == (size_t)-1) {
		rc = DECODE_NO_MORE_DATA;
		goto out;
	}
	if (rlen > wcslen)
		rlen = wcslen;
	wcsbuf[rlen] = 0;

	/*
	 * Copy the converted string into the message,
	 * truncated or paded as required.  Preserve
	 * little-endian order while copying.
	 */
	wp = wcsbuf;
	while (repc > 1) {
		smb_wchar_t wchar = LE_IN16(wp);
		mbc_marshal_store_byte(mbc, wchar);
		mbc_marshal_store_byte(mbc, wchar >> 8);
		if (wchar != 0)
			wp++;
		repc -= sizeof (smb_wchar_t);
	}
	if (repc > 0)
		mbc_marshal_store_byte(mbc, 0);

	rc = 0;
out:
	if (wcsbuf != NULL)
		smb_mem_free(wcsbuf);
	return (rc);
}

static int /*ARGSUSED*/
uiorefnoop(caddr_t p, int size, int adj)
{
	return (0);
}

static int
mbc_marshal_put_uio(mbuf_chain_t *mbc, struct uio *uio)
{
	mbuf_t		**t;
	mbuf_t		*m = NULL;
	struct iovec	*iov = uio->uio_iov;
	int32_t		i, iov_cnt = uio->uio_iovcnt;

	iov = uio->uio_iov;
	t = &mbc->chain;
	for (i = 0; i < iov_cnt; i++) {
		MGET(m, M_WAIT, MT_DATA);
		m->m_ext.ext_buf = iov->iov_base;
		m->m_ext.ext_ref = uiorefnoop;
		m->m_data = m->m_ext.ext_buf;
		m->m_flags |= M_EXT;
		m->m_len = m->m_ext.ext_size = iov->iov_len;
		mbc->max_bytes += m->m_len;
		m->m_next = 0;
		*t = m;
		t = &m->m_next;
		iov++;
	}
	return (0);
}

static int
mbc_marshal_put_mbufs(mbuf_chain_t *mbc, mbuf_t *m)
{
	mbuf_t	*mt;
	mbuf_t	**t;
	int	bytes;

	if (m != NULL) {
		mt = m;
		bytes = mt->m_len;
		while (mt->m_next != 0) {
			mt = mt->m_next;
			bytes += mt->m_len;
		}
		if (bytes != 0) {
			t = &mbc->chain;
			while (*t != 0) {
				bytes += (*t)->m_len;
				t = &(*t)->m_next;
			}
			*t = m;
			mbc->chain_offset = bytes;
		} else {
			m_freem(m);
		}
	}
	return (0);
}

static int
mbc_marshal_put_mbuf_chain(mbuf_chain_t *mbc, mbuf_chain_t *nmbc)
{
	if (nmbc->chain != 0) {
		if (mbc_marshal_put_mbufs(mbc, nmbc->chain))
			return (DECODE_NO_MORE_DATA);
		MBC_SETUP(nmbc, nmbc->max_bytes);
	}
	return (0);
}

static uint8_t
mbc_marshal_fetch_byte(mbuf_chain_t *mbc)
{
	uint8_t	data;
	mbuf_t	*m = mbc->chain;
	int32_t	offset = mbc->chain_offset;

	while (offset >= m->m_len) {
		offset -= m->m_len;
		m = m->m_next;
	}
	data = ((uint8_t *)m->m_data)[offset];
	mbc->chain_offset++;
	return (data);
}

static int
mbc_marshal_get_char(mbuf_chain_t *mbc, uint8_t *data)
{
	if (MBC_ROOM_FOR(mbc, sizeof (char)) == 0) {
		/* Data will never be available */
		return (DECODE_NO_MORE_DATA);
	}
	*data = mbc_marshal_fetch_byte(mbc);
	return (0);
}

static int
mbc_marshal_get_short(mbuf_chain_t *mbc, uint16_t *data)
{
	uint16_t	tmp;
	mbuf_t		*m = mbc->chain;
	int32_t		offset = mbc->chain_offset;

	if (MBC_ROOM_FOR(mbc, sizeof (short)) == 0) {
		/* Data will never be available */
		return (DECODE_NO_MORE_DATA);
	}

	while (offset >= m->m_len) {
		offset -= m->m_len;
		m = m->m_next;
	}
	if ((m->m_len - offset) >= sizeof (short)) {
		*data = LE_IN16(m->m_data + offset);
		mbc->chain_offset += sizeof (short);
	} else {
		tmp = (uint16_t)mbc_marshal_fetch_byte(mbc);
		tmp |= ((uint16_t)mbc_marshal_fetch_byte(mbc)) << 8;
		*data = tmp;
	}
	return (0);
}

static int
mbc_marshal_get_long(mbuf_chain_t *mbc, uint32_t *data)
{
	uint32_t	tmp;
	mbuf_t		*m = mbc->chain;
	int32_t		offset = mbc->chain_offset;

	if (MBC_ROOM_FOR(mbc, sizeof (int32_t)) == 0) {
		/* Data will never be available */
		return (DECODE_NO_MORE_DATA);
	}
	while (offset >= m->m_len) {
		offset -= m->m_len;
		m = m->m_next;
	}
	if ((m->m_len - offset) >= sizeof (int32_t)) {
		*data = LE_IN32(m->m_data + offset);
		mbc->chain_offset += sizeof (int32_t);
	} else {
		tmp = (uint32_t)mbc_marshal_fetch_byte(mbc);
		tmp |= ((uint32_t)mbc_marshal_fetch_byte(mbc)) << 8;
		tmp |= ((uint32_t)mbc_marshal_fetch_byte(mbc)) << 16;
		tmp |= ((uint32_t)mbc_marshal_fetch_byte(mbc)) << 24;
		*data = tmp;
	}
	return (0);
}

static uint64_t
qswap(uint64_t ll)
{
	uint64_t v;

	v = ll >> 32;
	v |= ll << 32;

	return (v);
}

static int
mbc_marshal_get_odd_long_long(mbuf_chain_t *mbc, uint64_t *data)
{
	uint64_t	tmp;
	mbuf_t		*m = mbc->chain;
	int32_t		offset = mbc->chain_offset;

	if (MBC_ROOM_FOR(mbc, sizeof (int64_t)) == 0) {
		/* Data will never be available */
		return (DECODE_NO_MORE_DATA);
	}
	while (offset >= m->m_len) {
		offset -= m->m_len;
		m = m->m_next;
	}

	if ((m->m_len - offset) >= sizeof (int64_t)) {
		*data = qswap(LE_IN64(m->m_data + offset));
		mbc->chain_offset += sizeof (int64_t);
	} else {
		tmp = (uint64_t)mbc_marshal_fetch_byte(mbc) << 32;
		tmp |= (uint64_t)mbc_marshal_fetch_byte(mbc) << 40;
		tmp |= (uint64_t)mbc_marshal_fetch_byte(mbc) << 48;
		tmp |= (uint64_t)mbc_marshal_fetch_byte(mbc) << 56;
		tmp |= (uint64_t)mbc_marshal_fetch_byte(mbc);
		tmp |= (uint64_t)mbc_marshal_fetch_byte(mbc) << 8;
		tmp |= (uint64_t)mbc_marshal_fetch_byte(mbc) << 16;
		tmp |= (uint64_t)mbc_marshal_fetch_byte(mbc) << 24;

		*(uint64_t *)data = tmp;
	}
	return (0);
}

static int
mbc_marshal_get_long_long(mbuf_chain_t *mbc, uint64_t *data)
{
	uint64_t	tmp;
	mbuf_t		*m = mbc->chain;
	int32_t		offset = mbc->chain_offset;

	if (MBC_ROOM_FOR(mbc, sizeof (int64_t)) == 0) {
		/* Data will never be available */
		return (DECODE_NO_MORE_DATA);
	}
	while (offset >= m->m_len) {
		offset -= m->m_len;
		m = m->m_next;
	}
	if ((m->m_len - offset) >= sizeof (int64_t)) {
		*data = LE_IN64(m->m_data + offset);
		mbc->chain_offset += sizeof (int64_t);
	} else {
		tmp = (uint32_t)mbc_marshal_fetch_byte(mbc);
		tmp |= ((uint64_t)mbc_marshal_fetch_byte(mbc)) << 8;
		tmp |= ((uint64_t)mbc_marshal_fetch_byte(mbc)) << 16;
		tmp |= ((uint64_t)mbc_marshal_fetch_byte(mbc)) << 24;
		tmp |= ((uint64_t)mbc_marshal_fetch_byte(mbc)) << 32;
		tmp |= ((uint64_t)mbc_marshal_fetch_byte(mbc)) << 40;
		tmp |= ((uint64_t)mbc_marshal_fetch_byte(mbc)) << 48;
		tmp |= ((uint64_t)mbc_marshal_fetch_byte(mbc)) << 56;
		*(uint64_t *)data = tmp;
	}
	return (0);
}

/*
 * mbc_marshal_get_oem_string
 *
 * Decode an OEM string, returning its UTF-8 form in strpp,
 * allocated using smb_srm_zalloc (automatically freed).
 * If max_bytes != 0, consume at most max_bytes of the mbc.
 * See also: msgbuf_get_oem_string
 */
static int
mbc_marshal_get_oem_string(smb_request_t *sr,
    mbuf_chain_t *mbc, char **strpp, int max_bytes)
{
	char		*mbs;
	uint8_t		*oembuf = NULL;
	int		oemlen, oemmax;
	int		mbsmax;
	int		rlen;
	int		rc;

	if (max_bytes == 0)
		max_bytes = 0xffff;

	/*
	 * Get the OtW data into a temporary buffer.
	 * Free oembuf before return.
	 */
	oemlen = 0;
	oemmax = MALLOC_QUANTUM;
	oembuf = smb_mem_alloc(oemmax);
	for (;;) {
		uint8_t ch;

		if (oemlen >= max_bytes)
			break;
		if ((oemlen + 2) >= oemmax) {
			oemmax += MALLOC_QUANTUM;
			oembuf = smb_mem_realloc(oembuf, oemmax);
		}
		if (mbc_marshal_get_char(mbc, &ch) != 0) {
			rc = DECODE_NO_MORE_DATA;
			goto out;
		}
		if (ch == 0)
			break;
		oembuf[oemlen++] = ch;
	}
	oembuf[oemlen] = '\0';

	/*
	 * Get the buffer we'll return and convert to UTF-8.
	 * May take as much as double the space.
	 */
	mbsmax = oemlen * 2;
	mbs = smb_srm_zalloc(sr, mbsmax + 1);
	ASSERT(mbs != NULL);
	rlen = smb_oemtombs(mbs, oembuf, mbsmax);
	if (rlen < 0) {
		rc = DECODE_NO_MORE_DATA;
		goto out;
	}
	if (rlen > mbsmax)
		rlen = mbsmax;
	mbs[rlen] = '\0';
	*strpp = mbs;
	rc = 0;

out:
	if (oembuf != NULL)
		smb_mem_free(oembuf);
	return (rc);
}

/*
 * mbc_marshal_get_unicode_string
 *
 * Decode a UTF-16 string, returning its UTF-8 form in strpp,
 * allocated using smb_srm_zalloc (automatically freed).
 * If max_bytes != 0, consume at most max_bytes of the mbc.
 * See also: msgbuf_get_unicode_string
 */
static int
mbc_marshal_get_unicode_string(smb_request_t *sr,
    mbuf_chain_t *mbc, char **strpp, int max_bytes)
{
	char		*mbs;
	uint16_t	*wcsbuf = NULL;
	int		wcslen;		// wchar count
	int		wcsmax;		// byte count
	size_t		mbsmax;
	size_t		rlen;
	int		rc;

	if (max_bytes == 0)
		max_bytes = 0xffff;

	/*
	 * Unicode strings are always word aligned.
	 */
	if (mbc->chain_offset & 1) {
		if (MBC_ROOM_FOR(mbc, sizeof (char)) == 0)
			return (DECODE_NO_MORE_DATA);
		mbc->chain_offset++;
	}

	/*
	 * Get the OtW data into a temporary buffer.
	 * Free wcsbuf before return.
	 */
	wcslen = 0;
	wcsmax = MALLOC_QUANTUM;
	wcsbuf = smb_mem_alloc(wcsmax);
	for (;;) {
		uint16_t	wchar;

		if ((wcslen * 2) >= max_bytes)
			break;
		if (((wcslen * 2) + 4) >= wcsmax) {
			wcsmax += MALLOC_QUANTUM;
			wcsbuf = smb_mem_realloc(wcsbuf, wcsmax);
		}
		if (mbc_marshal_get_short(mbc, &wchar) != 0) {
			rc = DECODE_NO_MORE_DATA;
			goto out;
		}
		if (wchar == 0)
			break;
		/* Keep in little-endian form. */
		LE_OUT16(wcsbuf + wcslen, wchar);
		wcslen++;
	}
	wcsbuf[wcslen] = 0;

	/*
	 * Get the buffer we'll return and convert to UTF-8.
	 * May take as much 4X number of wide chars.
	 */
	mbsmax = wcslen * MTS_MB_CUR_MAX;
	mbs = smb_srm_zalloc(sr, mbsmax + 1);
	ASSERT(mbs != NULL);
	rlen = smb_wcstombs(mbs, wcsbuf, mbsmax);
	if (rlen == (size_t)-1) {
		rc = DECODE_NO_MORE_DATA;
		goto out;
	}
	if (rlen > mbsmax)
		rlen = mbsmax;
	mbs[rlen] = '\0';
	*strpp = mbs;
	rc = 0;

out:
	if (wcsbuf != NULL)
		smb_mem_free(wcsbuf);
	return (rc);
}

static int /*ARGSUSED*/
mbc_marshal_get_mbufs(mbuf_chain_t *mbc, int32_t bytes, mbuf_t **m)
{
	*m = NULL;
	if (MBC_ROOM_FOR(mbc, bytes) == 0) {
		/* Data will never be available */
		return (DECODE_NO_MORE_DATA);
	}
	/* not yet implemented */
	return (-1);
}

static int
mbc_marshal_get_mbuf_chain(mbuf_chain_t *mbc, int32_t bytes, mbuf_chain_t *nmbc)
{
	int	rc;
	mbuf_t	*m;

	if (bytes == 0) {
		/* Get all the rest */
		bytes = mbc->max_bytes - mbc->chain_offset;
	}

	MBC_SETUP(nmbc, mbc->max_bytes);
	if ((rc = mbc_marshal_get_mbufs(mbc, bytes, &m)) != 0) {
		if (m)
			m_freem(m);
		return (rc);
	}
	nmbc->chain = m;
	while (m != 0) {
		bytes += m->m_len;
		m = m->m_next;
	}
	nmbc->max_bytes = bytes;
	return (0);
}

static int
mbc_marshal_get_uio(mbuf_chain_t *mbc, struct uio *uio)
{
	int		i, offset;
	int32_t		bytes = uio->uio_resid;
	int32_t		remainder;
	struct iovec	*iov;
	mbuf_t		*m;

	/*
	 * The residual count is tested because in the case of write requests
	 * with no data (smbtorture RAW-WRITE test will generate that type of
	 * request) this function is called with a residual count of zero
	 * bytes.
	 */
	if (bytes != 0) {
		iov = uio->uio_iov;
		uio->uio_segflg = UIO_SYSSPACE;
		uio->uio_extflg = UIO_COPY_DEFAULT;

		if (MBC_ROOM_FOR(mbc, bytes) == 0) {
			/* Data will never be available */
			return (DECODE_NO_MORE_DATA);
		}

		m = mbc->chain;
		offset = mbc->chain_offset;
		while (offset >= m->m_len) {
			offset -= m->m_len;
			m = m->m_next;
			ASSERT((offset == 0) || (offset && m));
		}

		for (i = 0; (bytes > 0) && (i < uio->uio_iovcnt); i++) {
			iov[i].iov_base = &m->m_data[offset];
			remainder = m->m_len - offset;
			if (remainder >= bytes) {
				iov[i].iov_len = bytes;
				mbc->chain_offset += bytes;
				uio->uio_iovcnt = i + 1;
				return (0);
			}
			iov[i].iov_len = remainder;
			mbc->chain_offset += remainder;
			bytes -= remainder;
			m = m->m_next;
			offset = 0;
		}
		return (DECODE_NO_MORE_DATA);
	}
	return (0);
}

static int
mbc_marshal_get_skip(mbuf_chain_t *mbc, uint_t skip)
{
	if (MBC_ROOM_FOR(mbc, skip) == 0)
		return (DECODE_NO_MORE_DATA);
	mbc->chain_offset += skip;
	return (0);
}
