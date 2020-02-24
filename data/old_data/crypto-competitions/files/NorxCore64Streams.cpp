#pragma once

#ifndef NORX_NORX_CORE64_STREAMS
#define NORX_NORX_CORE64_STREAMS
#include <stdlib.h> // only needed for _rotr64 and _lotr
#include "NorxCore64.cpp"

namespace NorxManaged
{
	namespace NorxCore64
	{
		// Low-level operations 
		static void _absorb(array<UInt64>^ state, MemoryStream^ in, const _domain_separator_64 tag, const Byte rounds)
		{
			// Used for P=1, and Header and Footer/Trailer (using appropriate domain separation constant)
			if (in == nullptr || in->Length == 0) return;
			Int64 outptr = 0;
			array<UInt64>^ state_buffer = gcnew array<UInt64>(NORX64_RATEWORDS);
			array<Byte>^ block = gcnew array<Byte>(NORX64_RATEBYTES);
			for (Int64 i = 0; i < in->Length; i += NORX64_RATEBYTES)
			{
				bool last = i + NORX64_RATEBYTES >= in->Length;
				if (last)
				{
					if (i < in->Length)
						in->Read(block, 0, in->Length % NORX64_RATEBYTES);
					block[in->Length % NORX64_RATEBYTES] = 0x01;
					block[block->Length - 1] |= 0x80;
				}
				else
				{
					in->Read(block, 0, NORX64_RATEBYTES);
				}
				Buffer::BlockCopy(block, 0, state_buffer, 0, NORX64_RATEBYTES);
				state[15] ^= (UInt64)tag;
				NorxCore64::_F(state, rounds);
				for (int j = 0; j < state_buffer->Length; j++)
				{
					state[j] ^= state_buffer[j];
				};
				NorxCore64::_burn(state_buffer);
				NorxCore64::_burn(block);
			}
			state_buffer[0] ^= (state_buffer[1] ^ block[0]) + NORX64_CAPBYTES;
		}

		static void _encrypt_p1(array<UInt64>^ state, MemoryStream^ in, const _domain_separator_64 tag, const Byte rounds, MemoryStream^ out)
		{
			// Used only for Payload of Parallelism = 1 (not P=0)
			if (in == nullptr || in->Length == 0) return;
			array<UInt64>^ state_buffer = gcnew array<UInt64>(NORX64_RATEWORDS);
			array<Byte>^ block = gcnew array<Byte>(NORX64_RATEBYTES);
			for (Int64 i = 0; i < in->Length; i += NORX64_RATEBYTES)
			{
				bool last = i + NORX64_RATEBYTES >= in->Length;
				if (last)
				{
					if (i < in->Length)
						in->Read(block, 0, in->Length % NORX64_RATEBYTES);
					block[in->Length % NORX64_RATEBYTES] = 0x01;
					block[block->Length - 1] |= 0x80;
				}
				else
				{
					in->Read(block, 0, NORX64_RATEBYTES);
				}
				Buffer::BlockCopy(block, 0, state_buffer, 0, NORX64_RATEBYTES);
				state[15] ^= (UInt64)tag;
				NorxCore64::_F(state, rounds);
				for (int j = 0; j < state_buffer->Length; j++)
				{
					state[j] ^= state_buffer[j];
				};
				Buffer::BlockCopy(state, 0, block, 0, NORX64_RATEBYTES);
				if (last)
					out->Write(block, 0, in->Length % NORX64_RATEBYTES);
				else
					out->Write(block, 0, NORX64_RATEBYTES);
				NorxCore64::_burn(block);
				NorxCore64::_burn(state_buffer);
			}
			state_buffer[0] ^= (state_buffer[1] ^ block[0]) + NORX64_CAPBYTES;
		}

		static void _decrypt_p1(array<UInt64>^ state, MemoryStream^ in, const _domain_separator_64 tag, const Byte rounds, int tagBytesInMessage, MemoryStream^ out)
		{
			// Used only for Payload of Parallelism = 1 (not P=0)
			if (in == nullptr || in->Length == 0) return;
			Int64 actualLength = in->Length - tagBytesInMessage;
			array<UInt64>^ state_buffer = gcnew array<UInt64>(NORX64_RATEWORDS);
			array<Byte>^ block = gcnew array<Byte>(NORX64_RATEBYTES);
			for (Int64 i = 0; i < actualLength; i += NORX64_RATEBYTES)
			{
				bool last = i + NORX64_RATEBYTES >= actualLength;
				state[15] ^= (UInt64)tag;
				NorxCore64::_F(state, rounds);
				if (last)
				{
					Buffer::BlockCopy(state, 0, block, 0, NORX64_RATEBYTES); // !!store state in last block, then overwrite with ciphertext
					if (i < actualLength)
						in->Read(block, 0, actualLength % NORX64_RATEBYTES);
					block[actualLength % NORX64_RATEBYTES] ^= 0x01; // remove the padding
					block[block->Length - 1] ^= 0x80;
				}
				else
				{
					in->Read(block, 0, NORX64_RATEBYTES);
				}
				Buffer::BlockCopy(block, 0, state_buffer, 0, NORX64_RATEBYTES);
				for (int j = 0; j < state_buffer->Length; j++)
				{
					UInt64 c = state_buffer[j];
					state_buffer[j] ^= state[j];
					state[j] = c;
				};
				Buffer::BlockCopy(state_buffer, 0, block, 0, NORX64_RATEBYTES);
				if (last)
					out->Write(block, 0, actualLength % NORX64_RATEBYTES);
				else
					out->Write(block, 0, NORX64_RATEBYTES);
				NorxCore64::_burn(state_buffer);
				NorxCore64::_burn(block);
			}
			state_buffer[0] ^= (state_buffer[1] ^ block[0]) + NORX64_CAPBYTES;
		}

		static void _encrypt_p2(array<array<UInt64>^>^ states, MemoryStream^ in, const _domain_separator_64 tag, const Byte rounds, const Byte lanes, MemoryStream^ out)
		{
			// Used only for Payload of Parallelism > 1 (not P=0)
			if (in == nullptr || in->Length == 0) return;
			Byte laneptr = 0;
			array<UInt64>^ state_buffer = gcnew array<UInt64>(NORX64_RATEWORDS);
			array<Byte>^ block = gcnew array<Byte>(NORX64_RATEBYTES);
			for (Int64 i = 0; i < in->Length; i += NORX64_RATEBYTES)
			{
				bool last = i + NORX64_RATEBYTES >= in->Length;
				if (last)
				{
					if (i < in->Length)
						in->Read(block, 0, in->Length % NORX64_RATEBYTES);
					block[in->Length % NORX64_RATEBYTES] = 0x01;
					block[block->Length - 1] |= 0x80;
				}
				else
				{
					in->Read(block, 0, NORX64_RATEBYTES);
				}
				Buffer::BlockCopy(block, 0, state_buffer, 0, NORX64_RATEBYTES);
				states[laneptr][15] ^= (UInt64)tag;
				NorxCore64::_F(states[laneptr], rounds);
				for (int j = 0; j < state_buffer->Length; j++)
				{
					// STATE = STATE ^ P = C
					states[laneptr][j] ^= state_buffer[j];
				};
				if (out != nullptr)
				{
					Buffer::BlockCopy(states[laneptr], 0, block, 0, NORX64_RATEBYTES);
					if (last)
						out->Write(block, 0, in->Length % NORX64_RATEBYTES);
					else
						out->Write(block, 0, NORX64_RATEBYTES);
				}
				laneptr = ++laneptr % lanes;
				NorxCore64::_burn(state_buffer);
				NorxCore64::_burn(block);
			}
			state_buffer[0] ^= (state_buffer[1] ^ block[0]) + NORX64_CAPBYTES;
		}

		static void _decrypt_p2(array<array<UInt64>^>^ states, MemoryStream^ in, const _domain_separator_64 tag, const Byte rounds, const Byte lanes, int tagBytesInMessage, MemoryStream^ out)
		{
			// Used only for Payload of Parallelism > 1 (not P=0)
			if (in == nullptr || in->Length == 0) return;
			Int64 actualLength = in->Length - tagBytesInMessage;
			Byte laneptr = 0;
			array<UInt64>^ state_buffer = gcnew array<UInt64>(NORX64_RATEWORDS);
			array<Byte>^ block = gcnew array<Byte>(NORX64_RATEBYTES);
			for (Int64 i = 0; i < actualLength; i += NORX64_RATEBYTES)
			{
				bool last = i + NORX64_RATEBYTES >= actualLength;
				states[laneptr][15] ^= (UInt64)tag;
				_F(states[laneptr], rounds);
				if (last)
				{
					Buffer::BlockCopy(states[laneptr], 0, block, 0, NORX64_RATEBYTES); // !!store state in last block, then overwrite with ciphertext
					if (i < actualLength)
						in->Read(block, 0, actualLength % NORX64_RATEBYTES);
					block[actualLength % NORX64_RATEBYTES] ^= 0x01;
					block[block->Length - 1] ^= 0x80;
				}
				else
				{
					in->Read(block, 0, NORX64_RATEBYTES);
				}
				Buffer::BlockCopy(block, 0, state_buffer, 0, NORX64_RATEBYTES);
				for (int j = 0; j < state_buffer->Length; j++)
				{
					// C = (from encryption) P ^ STATE, to recover P then C ^ STATE = P, to recover STATE, then P ^ C
					UInt64 c = state_buffer[j];
					state_buffer[j] ^= states[laneptr][j];
					states[laneptr][j] = c;
				};
				Buffer::BlockCopy(state_buffer, 0, block, 0, NORX64_RATEBYTES);
				if (last)
					out->Write(block, 0, actualLength % NORX64_RATEBYTES);
				else
					out->Write(block, 0, NORX64_RATEBYTES);
				laneptr = ++laneptr % lanes;
				NorxCore64::_burn(state_buffer);
				NorxCore64::_burn(block);
			}
			state_buffer[0] ^= (state_buffer[1] ^ block[0]) + NORX64_CAPBYTES;
		}

		static __inline void _finalize_toStream(
			array<UInt64>^ state,
			array<const UInt64>^ k,
			const Byte rounds,
			const short tagsizebits,
			MemoryStream^ outTag)
		{
			state[15] ^= _domain_separator_64::FINAL_TAG_64;

			NorxCore64::_F(state, rounds);
			state[12] ^= k[0];
			state[13] ^= k[1];
			state[14] ^= k[2];
			state[15] ^= k[3];
			NorxCore64::_F(state, rounds);
			state[12] ^= k[0];
			state[13] ^= k[1];
			state[14] ^= k[2];
			state[15] ^= k[3];

			array<Byte>^ tempTag = gcnew array<Byte>(tagsizebits / 8);
			Buffer::BlockCopy(state, NORX64_RATEWORDS * NORX64_WORDBYTES, tempTag, 0, tempTag->Length); // extract Tag
			outTag->Write(tempTag, 0, tempTag->Length);
			NorxCore64::_burn(tempTag);
			NorxCore64::_burn(state); // at this point we can burn the state 
			state[0] ^= (state[1] ^ tempTag[0]) + NORX64_CAPBYTES;
		}
	}
}
#endif // !NORX_NORX_CORE64_STREAMS