using namespace System;

#pragma once

#ifndef NORX_NORX_CORE32
#define NORX_NORX_CORE32
#include <stdlib.h> // only needed for _rotr64 and _lotr

namespace NorxManaged
{
	namespace NorxCore32
	{
#define NORX32_WORDBYTES	4
#define NORX32_STATEWORDS	16
#define NORX32_STATEBYTES	64
#define NORX32_NONCEBYTES	16
#define NORX32_NONCEWORDS	4
#define NORX32_KEYBYTES		16
#define NORX32_KEYWORDS		4
#define NORX32_RATEBYTES	48
#define NORX32_RATEWORDS	12
#define NORX32_CAPBYTES		16
#define NORX32_CAPWORDS		4
#define NORX32_TAGBYTES		16
#define NORX32_TAGWORDS		4
#define ROTR32(x, y) _lrotr(x, y)

// The nonlinear primitive 
#define _H(A, B) ( A ^ B ^ (( (A) & (B) ) << 1) )

// The quarter-round 
#define _G32(A, B, C, D)                               \
{                                                   \
    (A) = _H(A, B); (D) ^= (A); (D) = ROTR32((D), 8); \
    (C) = _H(C, D); (B) ^= (C); (B) = ROTR32((B), 11); \
    (A) = _H(A, B); (D) ^= (A); (D) = ROTR32((D), 16); \
    (C) = _H(C, D); (B) ^= (C); (B) = ROTR32((B), 31); \
}
		//These BURN methods should survive compiler optimization, they are only used at the end of a processes
		//Just in case, reference the source array in the calling function in some meaningful way to keep the references alive before the garbage collecter gets them
		static void _burn(array<UInt32>^ thing)
		{
			if (thing == nullptr) return;
			try {
				for (Byte i = 0; i < thing->Length; i++)
				{
					thing[i] = 0;
				}
			}
			catch (...) { throw; }
		}
		static void _burn(array<Byte>^ thing)
		{
			if (thing == nullptr) return;
			try {
				for (int i = 0; i < thing->Length; i++)
				{
					thing[i] = 0;
				}
			}
			catch (...) { throw; }
		}

		static __inline void _F(array<UInt32>^ state, const Byte rounds)
		{
			// normally this would be unrolled for performance, but with variable round amounts, that would be tedious to implement
			for (Byte i = 0; i < rounds; i++)
			{
				// Column step 
				_G32(state[0], state[4], state[8], state[12]);
				_G32(state[1], state[5], state[9], state[13]);
				_G32(state[2], state[6], state[10], state[14]);
				_G32(state[3], state[7], state[11], state[15]);
				// Diagonal step 
				_G32(state[0], state[5], state[10], state[15]);
				_G32(state[1], state[6], state[11], state[12]);
				_G32(state[2], state[7], state[8], state[13]);
				_G32(state[3], state[4], state[9], state[14]);
			}
		}

		// Low-level operations 
		static __inline array<UInt32>^ _init(array<const Byte>^ n, array<const UInt32>^ k, 
			Byte rounds, Byte lanes, short tagSizeBits)
		{
			array<UInt32>^ state;
#ifndef USE_COMPUTED_CONSTANTS
			state = gcnew array<UInt32>(NORX32_STATEWORDS);
			if (n != nullptr) // nonce is optional, so check for null
				Buffer::BlockCopy(n, 0, state, 0, NORX32_NONCEBYTES);
			Buffer::BlockCopy(k, 0, state, NORX32_NONCEBYTES, NORX32_KEYBYTES);

			//instead of using the _F construction twice with 0,1,2...15 as the initial state, use pre-computed constants
			state[8] = 0xA3D8D930;
			state[9] = 0x3FA8B72C;
			state[10] = 0xED84EB49;
			state[11] = 0xEDCA4787;

			//while using pre-computed constants, merge in the parameters to the operation
			state[12] = 0x335463EB ^ 32L;
			state[13] = 0xF994220B ^ (UInt32)rounds;
			state[14] = 0xBE0BF5C9 ^ (UInt32)lanes;
			state[15] = 0xD7C49104 ^ (UInt32)tagSizeBits;
#else
			state = gcnew array<UInt32>{
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
			};
			_F(state, 2);
			if (n != nullptr) // nonce is optional, so check for null
				Buffer::BlockCopy(n, 0, state, 0, NORX32_NONCEBYTES);
			Buffer::BlockCopy(k, 0, state, NORX32_NONCEBYTES, NORX32_KEYBYTES);
			state[12] ^= 32L;
			state[13] ^= (UInt32)rounds;
			state[14] ^= (UInt32)lanes;
			state[15] ^= (UInt32)tagSizeBits;
#endif
			_F(state, rounds);

			state[12] ^= k[0];
			state[13] ^= k[1];
			state[14] ^= k[2];
			state[15] ^= k[3];

			return state;
		}

		static void _absorb(
			array<UInt32>^ state,
			array<const Byte>^ in,
			const _domain_separator_32 tag,
			const Byte rounds)
		{
			// Used for P=1, and Header and Footer/Trailer (using appropriate domain separation constant)
			if (in == nullptr || in->Length == 0) return;
			Int64 outptr = 0;
			array<UInt32>^ state_buffer = gcnew array<UInt32>(NORX32_RATEWORDS);
			for (int i = 0; i < in->Length; i += NORX32_RATEBYTES)
			{
				if (i + NORX32_RATEBYTES >= in->Length)
				{
					array<Byte>^ LastBlock = gcnew array<Byte>(NORX32_RATEBYTES);
					if (i < in->Length)
						Buffer::BlockCopy(in, i, LastBlock, 0, in->Length % NORX32_RATEBYTES);
					LastBlock[in->Length % NORX32_RATEBYTES] = 0x01;
					LastBlock[LastBlock->Length - 1] |= 0x80;
					Buffer::BlockCopy(LastBlock, 0, state_buffer, 0, NORX32_RATEBYTES);
					_burn(LastBlock);
				}
				else
				{
					Buffer::BlockCopy(in, i, state_buffer, 0, NORX32_RATEBYTES);
				}
				state[15] ^= (UInt32)tag;
				_F(state, rounds);
				for (int j = 0; j < state_buffer->Length; j++)
				{
					state[j] ^= state_buffer[j];
				};
			}
			_burn(state_buffer);
			state_buffer[0] ^= state_buffer[1] ^ NORX32_CAPBYTES;
		}

		static void _encrypt_p1(
			array<UInt32>^ state, 
			array<const Byte>^ in, int index, int length,
			const _domain_separator_32 tag, 
			const Byte rounds, 
			array<Byte>^ out, int outIndex) // the input and output length will be the same (the TAG is added in another step)
		{
			// Used only for Payload of Parallelism = 1 (not P=0)
			if (in == nullptr || in->Length == 0 || length == 0) return;
			int apparentLength = index + length;
			if (apparentLength > in->Length)
				throw gcnew System::IndexOutOfRangeException("NorxCore32::_encrypt_p1->The given index and length goes beyond the bounds of the supplied input array.");
			if (outIndex + length > out->Length)
				throw gcnew System::IndexOutOfRangeException("NorxCore32::_encrypt_p1->The given index and length goes beyond the bounds of the supplied output array.");
			int outptr = outIndex;
			array<UInt32>^ state_buffer = gcnew array<UInt32>(NORX32_RATEWORDS);
			for (int i = index; i < apparentLength; i += NORX32_RATEBYTES)
			{
				bool last = i + NORX32_RATEBYTES >= apparentLength;
				if (last)
				{
					array<Byte>^ LastBlock = gcnew array<Byte>(NORX32_RATEBYTES);
					if (i < apparentLength)
						Buffer::BlockCopy(in, i, LastBlock, 0, apparentLength % NORX32_RATEBYTES);
					LastBlock[apparentLength % NORX32_RATEBYTES] = 0x01;
					LastBlock[LastBlock->Length - 1] |= 0x80;
					Buffer::BlockCopy(LastBlock, 0, state_buffer, 0, NORX32_RATEBYTES);
					_burn(LastBlock);
				}
				else
				{
					Buffer::BlockCopy(in, i, state_buffer, 0, NORX32_RATEBYTES);
				}
				state[15] ^= (UInt32)tag;
				_F(state, rounds);
				for (int j = 0; j < state_buffer->Length; j++)
				{
					state[j] ^= state_buffer[j];
				};
				if (last)
					Buffer::BlockCopy(state, 0, out, outptr, apparentLength % NORX32_RATEBYTES);
				else
					Buffer::BlockCopy(state, 0, out, outptr, NORX32_RATEBYTES);
				outptr += NORX32_RATEBYTES;
			}
			_burn(state_buffer);
			state_buffer[0] ^= state_buffer[1] ^ NORX32_CAPBYTES;
		}

		static void _decrypt_p1(
			array<UInt32>^ state,
			array<const Byte>^ in, int index, int length, 
			const _domain_separator_32 tag, 
			const Byte rounds, 
			int tagBytesInMessage, 
			array<Byte>^% out, 
			int outIndex)
		{
			// Used only for Payload of Parallelism = 1 (not P=0)
			if (in == nullptr || in->Length == 0 || length == 0) return;
			int apparentLength = index + length;
			if (apparentLength > in->Length)
				throw gcnew System::IndexOutOfRangeException("NorxCore32::_decrypt_p1->The given index and length goes beyond the bounds of the supplied input array.");
			if (outIndex + length - tagBytesInMessage > out->Length)
				throw gcnew System::IndexOutOfRangeException("NorxCore32::_decrypt_p1->The given index and length goes beyond the bounds of the supplied output array.");

			int outptr = outIndex;
			int actualLength = apparentLength - tagBytesInMessage; // the tag is processed in another step
			array<UInt32>^ state_buffer = gcnew array<UInt32>(NORX32_RATEWORDS);
			for (int i = 0; i < actualLength; i += NORX32_RATEBYTES)
			{
				bool last = i + NORX32_RATEBYTES >= actualLength;
				state[15] ^= (UInt32)tag;
				_F(state, rounds);
				if (last)
				{
					array<Byte>^ LastBlock = gcnew array<Byte>(NORX32_RATEBYTES);
					Buffer::BlockCopy(state, 0, LastBlock, 0, NORX32_RATEBYTES); // !!store state in last block, then overwrite with ciphertext
					if (i < actualLength)
						Buffer::BlockCopy(in, i, LastBlock, 0, actualLength % NORX32_RATEBYTES);
					LastBlock[actualLength % NORX32_RATEBYTES] ^= 0x01; // remove the padding
					LastBlock[LastBlock->Length - 1] ^= 0x80;
					Buffer::BlockCopy(LastBlock, 0, state_buffer, 0, NORX32_RATEBYTES);
					_burn(LastBlock);
				}
				else
				{
					Buffer::BlockCopy(in, i, state_buffer, 0, NORX32_RATEBYTES);
				}			
				for (int j = 0; j < state_buffer->Length; j++)
				{
					UInt32 c = state_buffer[j];
					state_buffer[j] ^= state[j];
					state[j] = c;
				};
				if (last)
					Buffer::BlockCopy(state_buffer, 0, out, outptr, actualLength % NORX32_RATEBYTES);
				else
					Buffer::BlockCopy(state_buffer, 0, out, outptr, NORX32_RATEBYTES);
				outptr += NORX32_RATEBYTES;
			}
			_burn(state_buffer);
			state_buffer[0] ^= state_buffer[1] ^ NORX32_CAPBYTES;
		}

		static void _encrypt_p2(
			array<array<UInt32>^>^ states, 
			array<const Byte>^ in, int index, int length,
			const _domain_separator_32 tag, 
			const Byte rounds, 
			const Byte lanes, 
			array<Byte>^% out, int outIndex)
		{
			// Used only for Payload of Parallelism > 1 (not P=0)
			if (in == nullptr || in->Length == 0 || length == 0) return;
			int apparentLength = index + length;
			if (apparentLength > in->Length)
				throw gcnew System::IndexOutOfRangeException("NorxCore32::_encrypt_p2->The given index and length goes beyond the bounds of the supplied input array.");
			if (outIndex + length > out->Length)
				throw gcnew System::IndexOutOfRangeException("NorxCore32::_encrypt_p2->The given index and length goes beyond the bounds of the supplied output array.");
			int outptr = outIndex;
			Byte laneptr = 0;
			array<UInt32>^ state_buffer = gcnew array<UInt32>(NORX32_RATEWORDS);
			for (int i = index; i < in->Length; i += NORX32_RATEBYTES)
			{
				bool last = i + NORX32_RATEBYTES >= in->Length;
				if (last)
				{
					array<Byte>^ LastBlock = gcnew array<Byte>(NORX32_RATEBYTES);
					if (i < apparentLength)
						Buffer::BlockCopy(in, i, LastBlock, 0, apparentLength % NORX32_RATEBYTES);
					LastBlock[apparentLength % NORX32_RATEBYTES] = 0x01;
					LastBlock[LastBlock->Length - 1] |= 0x80;
					Buffer::BlockCopy(LastBlock, 0, state_buffer, 0, LastBlock->Length);
					_burn(LastBlock);
				}
				else
				{
					Buffer::BlockCopy(in, i, state_buffer, 0, NORX32_RATEBYTES);
				}
				states[laneptr][15] ^= (UInt32)tag;
				_F(states[laneptr], rounds);
				for (int j = 0; j < state_buffer->Length; j++)
				{
					// STATE = STATE ^ P = C
					states[laneptr][j] ^= state_buffer[j];
				};
				if (out != nullptr)
				{
					if (last)
						Buffer::BlockCopy(states[laneptr], 0, out, outptr, apparentLength % NORX32_RATEBYTES);
					else
						Buffer::BlockCopy(states[laneptr], 0, out, outptr, NORX32_RATEBYTES);
					outptr += NORX32_RATEBYTES;
				}
				laneptr = ++laneptr % lanes;
			}
			_burn(state_buffer);
			state_buffer[0] ^= state_buffer[1] ^ NORX32_CAPBYTES;
		}

		static void _decrypt_p2(
			array<array<UInt32>^>^ states, 
			array<const Byte>^ in, int index, int length,
			const _domain_separator_32 tag, 
			const Byte rounds, 
			const Byte lanes, 
			int tagBytesInMessage, 
			array<Byte>^% out, int outIndex)
		{
			// Used only for Payload of Parallelism > 1 (not P=0)
			if (in == nullptr || in->Length == 0 || length == 0) return;
			int apparentLength = index + length;
			if (apparentLength > in->Length)
				throw gcnew System::IndexOutOfRangeException("NorxCore32::_decrypt_p2->The given index and length goes beyond the bounds of the supplied input array.");
			if (outIndex + length - tagBytesInMessage > out->Length)
				throw gcnew System::IndexOutOfRangeException("NorxCore32::_decrypt_p2->The given index and length goes beyond the bounds of the supplied output array.");
			int outptr = outIndex;
			int actualLength = apparentLength - tagBytesInMessage; // the tag is processed in another step
			Byte laneptr = 0;
			array<UInt32>^ state_buffer = gcnew array<UInt32>(NORX32_RATEWORDS);
			for (int i = index; i < actualLength; i += NORX32_RATEBYTES)
			{
				bool last = i + NORX32_RATEBYTES >= actualLength;
				states[laneptr][15] ^= (UInt32)tag;
				_F(states[laneptr], rounds);
				if (last)
				{
					array<Byte>^ LastBlock = gcnew array<Byte>(NORX32_RATEBYTES);
					Buffer::BlockCopy(states[laneptr], 0, LastBlock, 0, NORX32_RATEBYTES); // !!store state in last block, then overwrite with ciphertext
					if (i < actualLength)
						Buffer::BlockCopy(in, i, LastBlock, 0, actualLength % NORX32_RATEBYTES);
					LastBlock[actualLength % NORX32_RATEBYTES] ^= 0x01;
					LastBlock[LastBlock->Length - 1] ^= 0x80;
					Buffer::BlockCopy(LastBlock, 0, state_buffer, 0, LastBlock->Length);
					_burn(LastBlock);
				}
				else
				{
					Buffer::BlockCopy(in, i, state_buffer, 0, NORX32_RATEBYTES);
				}
				for (int j = 0; j < state_buffer->Length; j++)
				{
					// C = (from encryption) P ^ STATE, to recover P then C ^ STATE = P, to recover STATE, then P ^ C
					UInt32 c = state_buffer[j];
					state_buffer[j] ^= states[laneptr][j];
					states[laneptr][j] = c;
				};
				if (last)
					Buffer::BlockCopy(state_buffer, 0, out, outptr, actualLength % NORX32_RATEBYTES);
				else
					Buffer::BlockCopy(state_buffer, 0, out, outptr, NORX32_RATEBYTES);
				outptr += NORX32_RATEBYTES;
				laneptr = ++laneptr % lanes;
			}
			_burn(state_buffer);
			state_buffer[0] ^= state_buffer[1] ^ NORX32_CAPBYTES;
		}

		//static __inline void _branch(array<UInt32>^ state, Byte lane, const Byte rounds)
		//{
		//	state[15] ^= _domain_separator_32::BRANCH_TAG_32;
		//	_F(state, rounds);
		//	// Inject lane ID 
		//	for (Byte i = 0; i < NORX32_RATEWORDS; ++i) {
		//		state[i] ^= lane;
		//	}
		//}

		// stateA ^= stateB, then destroy stateB
		static __inline void _merge(array<UInt32>^ stateA, array<UInt32>^ stateB, const Byte rounds)
		{
			stateB[15] ^= _domain_separator_32::MERGE_TAG_32;
			_F(stateB, rounds);
			for (Byte i = 0; i < 16; ++i) {
				stateA[i] ^= stateB[i];
				stateB[i] |= UInt32::MaxValue; // destroy contents of old state
			}
		}

		static __inline void _finalize(array<UInt32>^ state, array<const UInt32>^ k, const Byte rounds, const short tagsizebits, array<Byte>^% outTag)
		{
			state[15] ^= _domain_separator_32::FINAL_TAG_32;

			_F(state, rounds);
			state[12] ^= k[0];
			state[13] ^= k[1];
			state[14] ^= k[2];
			state[15] ^= k[3];
			_F(state, rounds);
			state[12] ^= k[0];
			state[13] ^= k[1];
			state[14] ^= k[2];
			state[15] ^= k[3];

			Buffer::BlockCopy(state, NORX32_RATEWORDS * NORX32_WORDBYTES, outTag, 0, tagsizebits / 8); // extract Tag
			_burn(state); // at this point we can burn the state 
			state[0] ^= state[state[15] % 7] ^ NORX32_CAPBYTES; // prevent compiler optimizations?
		}
	}
}
#endif // !NORX_NORX_CORE32