/*
* Modified from NORX reference source code package - reference C implementations
*
* Written 2014-2016 by:
*
*      - Samuel Neves <sneves@dei.uc.pt>
*      - Philipp Jovanovic <philipp@jovanovic.io>
*
* Modified 2017-2019 by:
*      - Dustin Sparks <sparkdustjoe@gmail.com>
*
* To the extent possible under law, the author(s) have dedicated all copyright
* and related and neighboring rights to this software to the public domain
* worldwide. This software is distributed without any warranty.
*
* You should have received a copy of the CC0 Public Domain Dedication along with
* this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

#pragma once
#ifndef NORX_NORX_32H
#define NORX_NORX_32H
#include <stddef.h>
#include <stdint.h>

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;

namespace NorxManaged
{
	typedef enum _domain_separator_32 : Byte
	{
		HEADER_TAG_32 = 0x01,
		PAYLOAD_TAG_32 = 0x02,
		TRAILER_TAG_32 = 0x04,
		FINAL_TAG_32 = 0x08,
		BRANCH_TAG_32 = 0x10,
		MERGE_TAG_32 = 0x20
	} _domain_separator_32;

	public ref class Norx32
	{
	internal:
		static int norx_verify_tag32(array<const Byte>^ tag1, array<const Byte>^ tag2)
		{
			unsigned acc = 0;
			if (tag1->Length != tag2->Length) acc = 0xF0;
			for (Byte i = 0; i < tag1->Length; ++i) {
				acc |= tag1[i] ^ tag2[i];
			}
			return (((acc - 1) >> 8) & 1) - 1;
		}
	public:
		static int Encrypt(
			array<const Byte>^ Header,
			array<const Byte>^ Message, int Index, int Length,
			array<const Byte>^ Footer,
			array<const Byte>^ Key, array<const Byte>^ Nonce,
			const Byte Rounds, const Byte Parallelism,
			const short TagBitSize,
			[Out] array<Byte>^% output);
		static int Encrypt(
			array<const Byte>^ Header,
			array<const Byte>^ Message,
			array<const Byte>^ Footer,
			array<const Byte>^ Key, array<const Byte>^ Nonce,
			const Byte Rounds,
			const Byte Parallelism,
			const short TagBitSize,
			[Out] array<Byte>^% output);
		static int DecryptVerify(
			array<const Byte>^ Header, 
			array<const Byte>^ Message, int Index, int Length, 
			array<const Byte>^ Trailer, 
			array<const Byte>^ Key, array<const Byte>^ Nonce, 
			const Byte Rounds, const Byte Parallelism, 
			const short TagBitSize, 
			[Out] array<Byte>^% Output);
		static int DecryptVerify(
			array<const Byte>^ Header,
			array<const Byte>^ Message,
			array<const Byte>^ Footer,
			array<const Byte>^ Key, array<const Byte>^ Nonce,
			const Byte Rounds, const Byte Parallelism,
			const short TagBitSize,
			[Out] array<Byte>^% output);
		static int Encrypt_Detached(
			array<const Byte>^ Header,
			array<const Byte>^ Message, int Index, int Length,
			array<const Byte>^ Footer,
			array<const Byte>^ Key, array<const Byte>^ Nonce,
			const Byte Rounds, const Byte Parallelism,
			const short TagBitSize,
			[Out] array<Byte>^% output,
			[Out] array<Byte>^% output_tag);
		static int Encrypt_Detached(
			array<const Byte>^ Header,
			array<const Byte>^ Message,
			array<const Byte>^ Footer,
			array<const Byte>^ Key, array<const Byte>^ Nonce,
			const Byte Rounds, const Byte Parallelism,
			const short TagBitSize,
			[Out] array<Byte>^% output,
			[Out] array<Byte>^% output_tag);
		static int DecryptVerify_Detached(
			array<const Byte>^ Header,
			array<const Byte>^ Message, int Index, int Length,
			array<const Byte>^ Footer,
			array<const Byte>^ Key, array<const Byte>^ Nonce,
			const Byte Rounds, const Byte Parallelism,
			array<const Byte>^ tag,
			[Out] array<Byte>^% output);
		static int DecryptVerify_Detached(
			array<const Byte>^ Header,
			array<const Byte>^ Message,
			array<const Byte>^ Footer,
			array<const Byte>^ Key, array<const Byte>^ Nonce,
			const Byte Rounds, const Byte Parallelism,
			array<const Byte>^ tag,
			[Out] array<Byte>^% output);

		static int EncryptStream(
			MemoryStream^ Header,
			MemoryStream^ Message,
			MemoryStream^ Footer,
			array<const Byte>^ Key, array<const Byte>^ Nonce,
			const Byte Rounds, const Byte Parallelism,
			const short TagBitSize,
			MemoryStream^ output);
		static int DecryptVerifyStream(
			MemoryStream^ Header,
			MemoryStream^ Message,
			MemoryStream^ Footer,
			array<const Byte>^ Key, array<const Byte>^ Nonce,
			const Byte Rounds, const Byte Parallelism,
			const short TagBitSize,
			MemoryStream^ output);
		static int EncryptStream_Detached(
			MemoryStream^ Header,
			MemoryStream^ Message,
			MemoryStream^ Footer,
			array<const Byte>^ Key, array<const Byte>^ Nonce,
			const Byte Rounds, const Byte Parallelism,
			const short TagBitSize,
			MemoryStream^ output,
			MemoryStream^ output_tag);
		static int DecryptVerifyStream_Detached(
			MemoryStream^ Header,
			MemoryStream^ Message,
			MemoryStream^ Footer,
			array<const Byte>^ Key, array<const Byte>^ Nonce,
			const Byte Rounds, const Byte Parallelism,
			array<const Byte>^ tag,
			MemoryStream^ output);
	};
}

#endif // NORX_NORX_32H