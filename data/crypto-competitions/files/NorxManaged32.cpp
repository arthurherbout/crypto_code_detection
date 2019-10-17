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

#include "NorxManaged32.h"
#include "NorxCore32.cpp"

namespace NorxManaged
{
	// 32 BIT PUBLIC METHODS
	int NorxManaged::Norx32::Encrypt(
		array<const Byte>^ Header,
		array<const Byte>^ Message, int Index, int Length,
		array<const Byte>^ Trailer,
		array<const Byte>^ Key, array<const Byte>^ Nonce,
		const Byte Rounds, const Byte Parallelism,
		const short TagBitSize,
		[Out] array<Byte>^% Output)
	{
		if (Key == nullptr || Key->Length != NORX32_KEYBYTES)
			throw gcnew InvalidOperationException("Key not optional. Must be " + NORX32_KEYBYTES + " bytes.");
		if (Nonce != nullptr && Nonce->Length != NORX32_NONCEBYTES)
			throw gcnew InvalidOperationException("Nonce is optional, but must be " + NORX32_NONCEBYTES + " when specified.");
		if (Rounds < 4)
			throw gcnew ArgumentOutOfRangeException("Rounds", "Rounds are limited to a minimum of 4.");
		if (Parallelism == 0)
			throw gcnew NotImplementedException("Parallelism must be > 0.");
		if (TagBitSize == 0 || TagBitSize % 8 != 0)
			throw gcnew ArgumentOutOfRangeException("TagBitSize", "Tag length must be specified as a multiple of 8 bits, and (0 < x <= " + NORX32_TAGBYTES * 8 + ").");
		if (Message != nullptr && (Index + Length) > Message->Length)
			throw gcnew ArgumentOutOfRangeException("Length", "The specified length at the given index exceeds the length of the message.");
		array<const UInt32>^ kt = reinterpret_cast<array<const UInt32>^>(Key); // prevent unneccessary memory copying of the key!
		//array<UInt32>^ kt = gcnew array<UInt32>(NORX32_KEYWORDS);
		//Buffer::BlockCopy(Key, 0, kt, 0, NORX32_KEYBYTES);
		array<UInt32>^ state = NorxCore32::_init(Nonce, kt, Rounds, Parallelism, TagBitSize);
		NorxCore32::_absorb(state, Header, HEADER_TAG_32, Rounds);
		Output = gcnew array<Byte>(Message != nullptr ? Length + NORX32_TAGBYTES : NORX32_TAGBYTES);
		if (Parallelism == 1)
		{
			NorxCore32::_encrypt_p1(state, Message, Index, Length, PAYLOAD_TAG_32, Rounds, Output, 0);
		}
		else if (Parallelism > 1)
		{
			array<array<UInt32>^>^ lanes = gcnew array<array<UInt32>^>(Parallelism);
			// all branches get the same base state value operated on by a delimiter and then _F
			state[15] ^= BRANCH_TAG_32;
			NorxCore32::_F(state, Rounds);
			// the Lane ID is XOR'd in after _F
			for (Byte i = 0; i < Parallelism; i++) // THIS IS RIPE FOR .NET PARALLELISM
			{
				lanes[i] = (array<UInt32>^)state->Clone();
				for (Byte j = 0; j < NORX32_RATEWORDS; ++j) 
				{
					lanes[i][j] ^= i;
				}
			} //end .net parallelism
			NorxCore32::_burn(state);
			NorxCore32::_encrypt_p2(lanes, Message, Index, Length, PAYLOAD_TAG_32, Rounds, Parallelism, Output, 0);
			for (Byte i = 0; i < Parallelism; i++)
			{
				NorxCore32::_merge(state, lanes[i], Rounds); // merge back into the main state and destroy the lane
			}
		}
		//else // Infinite parallelism not implemented (p=0)
		//{ }
		NorxCore32::_absorb(state, Trailer, TRAILER_TAG_32, Rounds);
		array<Byte>^ tempTag = gcnew array<Byte>(NORX32_TAGBYTES);
		NorxCore32::_finalize(state, kt, Rounds, TagBitSize, tempTag);
		Buffer::BlockCopy(tempTag, 0, Output, Length, NORX32_TAGBYTES);
		NorxCore32::_burn(tempTag);
		kt = nullptr;
		return 0; // OK
	}

	int NorxManaged::Norx32::Encrypt(
		array<const Byte>^ Header, array<const Byte>^ Message, array<const Byte>^ Trailer,
		array<const Byte>^ Key, array<const Byte>^ Nonce,
		const Byte Rounds, const Byte Parallelism,
		const short TagBitSize,
		[Out] array<Byte>^% Output)
	{
		return NorxManaged::Norx32::Encrypt(Header, Message, 0, Message == nullptr ? 0 : Message->Length, Trailer, 
			Key, Nonce, Rounds, Parallelism, TagBitSize, Output);
	}

	int NorxManaged::Norx32::DecryptVerify(
		array<const Byte>^ Header,
		array<const Byte>^ Message, int Index, int Length,
		array<const Byte>^ Trailer,
		array<const Byte>^ Key, array<const Byte>^ Nonce,
		const Byte Rounds, const Byte Parallelism,
		const short TagBitSize,
		[Out] array<Byte>^% Output)
	{
		if (Key == nullptr || Key->Length != NORX32_KEYBYTES)
			throw gcnew InvalidOperationException("Key not optional.  Must be " + NORX32_KEYBYTES + " bytes.");
		if (Nonce == nullptr && Nonce->Length != NORX32_NONCEBYTES)
			throw gcnew InvalidOperationException("Nonce is optional, but must be " + NORX32_NONCEBYTES + " when specified.");
		if (Rounds < 4)
			throw gcnew ArgumentOutOfRangeException("Rounds", "Rounds are limited to a minimum of 4.");
		if (Parallelism == 0)
			throw gcnew NotImplementedException("Parallelism must be > 0.");
		if (TagBitSize == 0 || TagBitSize % 8 != 0)
			throw gcnew ArgumentOutOfRangeException("TagBitSize", "Tag length must be specified as a multiple of 8 bits, and (0 < x <= " + NORX32_TAGBYTES * 8 + ").");
		if (Message == nullptr || Message->LongLength < TagBitSize / 8 || Length < TagBitSize / 8)
			throw gcnew ArgumentNullException("Message", "Message cannot be NULL, and must be at least (TagBitSize / 8) bytes in length.");
		array<const UInt32>^ kt = reinterpret_cast<array<const UInt32>^>(Key); // prevent unneccessary memory copying of the key!
		//array<UInt32>^ kt = gcnew array<UInt32>(NORX32_KEYWORDS);
		//Buffer::BlockCopy(Key, 0, kt, 0, NORX32_KEYBYTES);
		array<UInt32>^ state = NorxCore32::_init(Nonce, kt, Rounds, Parallelism, TagBitSize);
		NorxCore32::_absorb(state, Header, HEADER_TAG_32, Rounds);
		Output = nullptr;
		if (Message != nullptr)
			Output = (Length >= NORX32_TAGBYTES) ? gcnew array<Byte>(Length - NORX32_TAGBYTES) : nullptr;
		if (Parallelism == 1)
		{
			NorxCore32::_decrypt_p1(state, Message, Index, Length, PAYLOAD_TAG_32, Rounds, TagBitSize / 8, Output, 0);
		}
		else if (Parallelism > 1)
		{
			array<array<UInt32>^>^ lanes = gcnew array<array<UInt32>^>(Parallelism);
			// all branches get the same base state value operated on by a delimiter and then _F
			state[15] ^= BRANCH_TAG_32;
			NorxCore32::_F(state, Rounds);
			// the Lane ID is XOR'd in after _F
			for (Byte i = 0; i < Parallelism; i++)
			{
				lanes[i] = (array<UInt32>^)state->Clone();
				for (Byte j = 0; j < NORX32_RATEWORDS; ++j)
				{
					lanes[i][j] ^= i;
				}
			}
			NorxCore32::_burn(state);
			NorxCore32::_decrypt_p2(lanes, Message, Index, Length, PAYLOAD_TAG_32, Rounds, Parallelism, TagBitSize / 8, Output, 0);
			for (Byte i = 0; i < Parallelism; i++)
			{
				NorxCore32::_merge(state, lanes[i], Rounds); // merge back into the main state and destroy the lane
			}
		}
		//else // Infinite parallelism not implemented (p=0)
		//{ }
		NorxCore32::_absorb(state, Trailer, TRAILER_TAG_32, Rounds);
		array<Byte>^ tempTag = gcnew array<Byte>(TagBitSize / 8);
		array<Byte>^ inputTag = gcnew array<Byte>(TagBitSize / 8);
		Buffer::BlockCopy(Message, Length - (TagBitSize / 8), inputTag, 0, TagBitSize / 8);
		NorxCore32::_finalize(state, kt, Rounds, TagBitSize, tempTag);
		int returnValue = -1; // assume failure
		if (Norx32::norx_verify_tag32((array<const Byte>^)inputTag, (array<const Byte>^)tempTag) == 0)
			returnValue = 0; // OK
		else
		{
			NorxCore32::_burn(Output);
			Threading::Thread::MemoryBarrier();
			Output = nullptr;
		}
		NorxCore32::_burn(inputTag);
		NorxCore32::_burn(tempTag);
		kt = nullptr;
		return returnValue;
	}

	int NorxManaged::Norx32::DecryptVerify(
		array<const Byte>^ Header, array<const Byte>^ Message, array<const Byte>^ Trailer,
		array<const Byte>^ Key, array<const Byte>^ Nonce,
		const Byte Rounds, const Byte Parallelism,
		const short TagBitSize,
		[Out] array<Byte>^% Output)
	{
		return NorxManaged::Norx32::DecryptVerify(Header, Message, 0, Message == nullptr ? 0 : Message->Length, Trailer, 
			Key, Nonce, Rounds, Parallelism, TagBitSize, Output);
	}

	int NorxManaged::Norx32::Encrypt_Detached(
		array<const Byte>^ Header,
		array<const Byte>^ Message, int Index, int Length,
		array<const Byte>^ Trailer,
		array<const Byte>^ Key, array<const Byte>^ Nonce,
		const Byte Rounds, const Byte Parallelism,
		const short TagBitSize,
		[Out] array<Byte>^% Output,
		[Out] array<Byte>^% Tag)
	{
		if (Key == nullptr || Key->Length != NORX32_KEYBYTES)
			throw gcnew InvalidOperationException("Key not optional. Must be " + NORX32_KEYBYTES + " bytes.");
		if (Nonce == nullptr && Nonce->Length != NORX32_NONCEBYTES)
			throw gcnew InvalidOperationException("Nonce is optional, but must be " + NORX32_NONCEBYTES + " when specified.");
		if (Rounds < 4)
			throw gcnew ArgumentOutOfRangeException("Rounds", "Rounds are limited to a minimum of 4.");
		if (Parallelism == 0)
			throw gcnew NotImplementedException("Parallelism must be > 0.");
		if (TagBitSize == 0 || TagBitSize % 8 != 0)
			throw gcnew ArgumentOutOfRangeException("TagBitSize", "Tag length must be specified as a multiple of 8 bits, and (0 < x <= " + NORX32_TAGBYTES * 8 + ").");
		//if (output_tag == nullptr || output_tag->Length * 8 != TagBitSize)
		//	throw gcnew ArgumentOutOfRangeException("output_tag", "The Tag buffer must be allocated by the caller, and be the same bit size as specified in TagBitSize.");
		Tag = gcnew array<Byte>(NORX32_TAGBYTES);
		Output = Message != nullptr ? gcnew array<Byte>(Message->Length) : nullptr;
		array<const UInt32>^ kt = reinterpret_cast<array<const UInt32>^>(Key); // prevent unneccessary memory copying of the key!
		//array<UInt32>^ kt = gcnew array<UInt32>(NORX32_KEYWORDS);
		//Buffer::BlockCopy(Key, 0, kt, 0, NORX32_KEYBYTES);
		array<UInt32>^ state = NorxCore32::_init(Nonce, kt, Rounds, Parallelism, TagBitSize);
		NorxCore32::_absorb(state, Header, HEADER_TAG_32, Rounds);
		if (Message != nullptr)
			Output = gcnew array<Byte>(Length);
		if (Parallelism == 1)
		{
			NorxCore32::_encrypt_p1(state, Message, Index, Length, PAYLOAD_TAG_32, Rounds, Output, 0);
		}
		else if (Parallelism > 1)
		{
			array<array<UInt32>^>^ lanes = gcnew array<array<UInt32>^>(Parallelism);
			// all branches get the same base state value operated on by a delimiter and then _F
			state[15] ^= BRANCH_TAG_32;
			NorxCore32::_F(state, Rounds);
			// the Lane ID is XOR'd in after _F
			for (Byte i = 0; i < Parallelism; i++)
			{
				lanes[i] = (array<UInt32>^)state->Clone();
				for (Byte j = 0; j < NORX32_RATEWORDS; ++j)
				{
					lanes[i][j] ^= i;
				}
			}
			NorxCore32::_burn(state);
			NorxCore32::_encrypt_p2(lanes, Message, Index, Length, PAYLOAD_TAG_32, Rounds, Parallelism, Output, 0);
			for (Byte i = 0; i < Parallelism; i++)
			{
				NorxCore32::_merge(state, lanes[i], Rounds); // merge back into the main state and destroy the lane
			}
		}
		//else // Infinite parallelism not implemented (p=0)
		//{ }
		NorxCore32::_absorb(state, Trailer, TRAILER_TAG_32, Rounds);
		NorxCore32::_finalize(state, kt, Rounds, TagBitSize, Tag);
		return 0; // OK
	}

	int NorxManaged::Norx32::Encrypt_Detached(
		array<const Byte>^ Header, array<const Byte>^ Message, array<const Byte>^ Trailer,
		array<const Byte>^ Key,	array<const Byte>^ Nonce,
		const Byte Rounds, const Byte Parallelism,
		const short TagBitSize,
		[Out] array<Byte>^% Output,
		[Out] array<Byte>^% Tag)
	{
		return NorxManaged::Norx32::Encrypt_Detached(Header, Message, 0, Message == nullptr ? 0 : Message->Length, Trailer, 
			Key, Nonce, Rounds, Parallelism, TagBitSize, Output, Tag);
	}

	int NorxManaged::Norx32::DecryptVerify_Detached(
		array<const Byte>^ Header,
		array<const Byte>^ Message, int Index, int Length,
		array<const Byte>^ Trailer,
		array<const Byte>^ Key,	array<const Byte>^ Nonce,
		const Byte Rounds, const Byte Parallelism,
		array<const Byte>^ Tag,
		[Out] array<Byte>^% Output)
	{
		if (Key == nullptr || Key->Length != NORX32_KEYBYTES)
			throw gcnew InvalidOperationException("Key not optional.  Must be " + NORX32_KEYBYTES + " bytes.");
		if (Nonce == nullptr && Nonce->Length != NORX32_NONCEBYTES)
			throw gcnew InvalidOperationException("Nonce is optional, but must be " + NORX32_NONCEBYTES + " when specified.");
		if (Rounds < 4)
			throw gcnew ArgumentOutOfRangeException("Rounds", "Rounds are limited to a minimum of 4.");
		if (Parallelism == 0)
			throw gcnew NotImplementedException("Parallelism must be > 0.");
		if (Tag == nullptr || Tag->Length > NORX32_TAGBYTES)
			throw gcnew ArgumentNullException("Tag", "Tag cannot be NULL or 0 length, and must be less than or equal to " + NORX32_TAGBYTES + " bytes in length.");
		array<const UInt32>^ kt = reinterpret_cast<array<const UInt32>^>(Key); // prevent unneccessary memory copying of the key!
		//array<UInt32>^ kt = gcnew array<UInt32>(NORX32_KEYWORDS);
		//Buffer::BlockCopy(Key, 0, kt, 0, NORX32_KEYBYTES);
		array<UInt32>^ state = NorxCore32::_init(Nonce, kt, Rounds, Parallelism, Tag->Length * 8);
		NorxCore32::_absorb(state, Header, HEADER_TAG_32, Rounds);
		Output = nullptr;
		if (Message != nullptr)
			Output = gcnew array<Byte>(Length);

		if (Parallelism == 1)
		{
			NorxCore32::_decrypt_p1(state, Message, Index, Length, PAYLOAD_TAG_32, Rounds, 0, Output, 0);
		}
		else if (Parallelism > 1)
		{
			array<array<UInt32>^>^ lanes = gcnew array<array<UInt32>^>(Parallelism);
			// all branches get the same base state value operated on by a delimiter and then _F
			state[15] ^= BRANCH_TAG_32;
			NorxCore32::_F(state, Rounds);
			// the Lane ID is XOR'd in after _F
			for (Byte i = 0; i < Parallelism; i++)
			{
				lanes[i] = (array<UInt32>^)state->Clone();
				for (Byte j = 0; j < NORX32_RATEWORDS; ++j)
				{
					lanes[i][j] ^= i;
				}
			}
			NorxCore32::_burn(state);
			NorxCore32::_decrypt_p2(lanes, Message, Index, Length, PAYLOAD_TAG_32, Rounds, Parallelism, 0, Output, 0);
			for (Byte i = 0; i < Parallelism; i++)
			{
				NorxCore32::_merge(state, lanes[i], Rounds); // merge back into the main state and destroy the lane
			}
		}
		//else // Infinite parallelism not implemented (p=0)
		//{ }
		NorxCore32::_absorb(state, Trailer, TRAILER_TAG_32, Rounds);
		array<Byte>^ temptag = gcnew array<Byte>(Tag->Length);
		NorxCore32::_finalize(state, kt, Rounds, Tag->Length * 8, temptag);
		if (Norx32::norx_verify_tag32(Tag, (array<const Byte>^)temptag) == 0)
		{
			return 0; // ok
		}
		else
		{
			NorxCore32::_burn(Output);
			Output = nullptr;
			return -1;
		}
	}

	int NorxManaged::Norx32::DecryptVerify_Detached(
		array<const Byte>^ Header, array<const Byte>^ Message, array<const Byte>^ Trailer,
		array<const Byte>^ Key, array<const Byte>^ Nonce,
		const Byte Rounds, const Byte Parallelism,
		array<const Byte>^ Tag,
		[Out] array<Byte>^% Output)
	{
		return NorxManaged::Norx32::DecryptVerify_Detached(Header, Message, 0, Message == nullptr ? 0 : Message->Length, Trailer, 
			Key, Nonce, Rounds, Parallelism, Tag, Output);
	}
}
