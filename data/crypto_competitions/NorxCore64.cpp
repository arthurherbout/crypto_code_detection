using namespace System;
using namespace System::Runtime::InteropServices;

#pragma once

#ifndef NORX_NORX_CORE64
#define NORX_NORX_CORE64
#include <stdlib.h> // only needed for _rotr64 and _lotr

namespace NorxManaged
{
	namespace NorxCore64
	{
#define NORX64_WORDBYTES	8
#define NORX64_STATEWORDS	16
#define NORX64_STATEBYTES	128
#define NORX64_NONCEBYTES	32
#define NORX64_NONCEWORDS	4
#define NORX64_KEYBYTES		32
#define NORX64_KEYWORDS		4
#define NORX64_RATEBYTES	96
#define NORX64_RATEWORDS	12
#define NORX64_CAPBYTES		32
#define NORX64_CAPWORDS		4
#define NORX64_TAGBYTES		32
#define NORX64_TAGWORDS		4
#define ROTR64(x, y) _rotr64(x, y)

// The nonlinear primitive 
#define _H(A, B) ( A ^ B ^ (( (A) & (B) ) << 1) )

// The quarter-round 
#define _G64(A, B, C, D)                               \
{                                                   \
    (A) = _H(A, B); (D) ^= (A); (D) = ROTR64((D), 8); \
    (C) = _H(C, D); (B) ^= (C); (B) = ROTR64((B), 19); \
    (A) = _H(A, B); (D) ^= (A); (D) = ROTR64((D), 40); \
    (C) = _H(C, D); (B) ^= (C); (B) = ROTR64((B), 63); \
}
		//These BURN methods should survive compiler optimization, they are only used at the end of a process
		//Just in case, reference the source array in the calling function in some meaningful way to keep the references alive before the garbage collecter gets them
		static void _burn(array<UInt64>^ thing)
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

		static __inline void _F(array<UInt64>^ state, const Byte rounds)
		{
			// normally this would be unrolled for performance, but with variable round amounts, that would be tedious to implement
			for (Byte i = 0; i < rounds; i++)
			{
				// Column step 
				_G64(state[0], state[4], state[8], state[12]);
				_G64(state[1], state[5], state[9], state[13]);
				_G64(state[2], state[6], state[10], state[14]);
				_G64(state[3], state[7], state[11], state[15]);
				// Diagonal step 
				_G64(state[0], state[5], state[10], state[15]);
				_G64(state[1], state[6], state[11], state[12]);
				_G64(state[2], state[7], state[8], state[13]);
				_G64(state[3], state[4], state[9], state[14]);
			}
		}

		// Low-level operations 
		static __inline array<UInt64>^ _init(array<const Byte>^ n, array<const UInt64>^ k, 
			Byte rounds, Byte lanes, short tagSizeBits)
		{
			array<UInt64>^ state;
#ifndef USE_COMPUTED_CONSTANTS
			state = gcnew array<UInt64>(NORX64_STATEWORDS);
			if (n != nullptr)  // nonce is optional, so check for null
				Buffer::BlockCopy(n, 0, state, 0, NORX64_NONCEBYTES);
			Buffer::BlockCopy(k, 0, state, NORX64_NONCEBYTES, NORX64_KEYBYTES);

			//instead of using the _F construction twice with 0,1,2...15 as the initial state, use pre-computed constants
			state[8] = 0xB15E641748DE5E6BUL;
			state[9] = 0xAA95E955E10F8410UL;
			state[10] = 0x28D1034441A9DD40UL;
			state[11] = 0x7F31BBF964E93BF5UL;

			//while using pre-computed constants, merge in the parameters to the operation
			state[12] = 0xB5E9E22493DFFB96UL ^ 64UL;
			state[13] = 0xB980C852479FAFBDUL ^ (UInt64)rounds;
			state[14] = 0xDA24516BF55EAFD4UL ^ (UInt64)lanes;
			state[15] = 0x86026AE8536F1501UL ^ (UInt64)tagSizeBits;
#else
			state = gcnew array<UInt64>{
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
			};
			_F(state, 2);
			if (n != nullptr)  // nonce is optional, so check for null
				Buffer::BlockCopy(n, 0, state, 0, NORX64_NONCEBYTES);
			Buffer::BlockCopy(k, 0, state, NORX64_NONCEBYTES, NORX64_KEYBYTES);
			state[12] ^= 64UL;
			state[13] ^= (UInt64)rounds;
			state[14] ^= (UInt64)lanes;
			state[15] ^= (UInt64)tagSizeBits;
#endif
			_F(state, rounds);

			state[12] ^= k[0];
			state[13] ^= k[1];
			state[14] ^= k[2];
			state[15] ^= k[3];
			
			return state;
		}

		static void _absorb(array<UInt64>^ state, array<const Byte>^ in, const _domain_separator_64 tag, const Byte rounds)
		{
			// Used for P=1, and Header and Footer/Trailer (using appropriate domain separation constant)
			if (in == nullptr || in->Length == 0) return;
			Int64 outptr = 0;
			array<UInt64>^ state_buffer = gcnew array<UInt64>(NORX64_RATEWORDS);
			for (int i = 0; i < in->Length; i += NORX64_RATEBYTES)
			{
				bool last = i + NORX64_RATEBYTES >= in->Length;
				if (last)
				{
					array<Byte>^ LastBlock = gcnew array<Byte>(NORX64_RATEBYTES);
					if (i < in->Length)
						Buffer::BlockCopy(in, i, LastBlock, 0, in->Length % NORX64_RATEBYTES);
					LastBlock[in->Length % NORX64_RATEBYTES] = 0x01;
					LastBlock[LastBlock->Length - 1] |= 0x80;
					Buffer::BlockCopy(LastBlock, 0, state_buffer, 0, NORX64_RATEBYTES);
					_burn(LastBlock);
				}
				else
				{
					Buffer::BlockCopy(in, i, state_buffer, 0, NORX64_RATEBYTES);
				}			
				state[15] ^= (UInt64)tag;
				_F(state, rounds);
				for (int j = 0; j < state_buffer->Length; j++)
				{
					state[j] ^= state_buffer[j];
				};
			}
			_burn(state_buffer);
			state_buffer[0] ^= state_buffer[1] ^ NORX64_CAPBYTES;
		}

		static void _encrypt_p1(
			array<UInt64>^ state, 
			array<const Byte>^ in, int index, int length,
			const _domain_separator_64 tag, 
			const Byte rounds, 
			array<Byte>^ out, int outIndex) // the input and output length will be the same (the TAG is added in another step)
		{
			// Used only for Payload of Parallelism = 1 (not P=0)
			if (in == nullptr || in->Length == 0 || length == 0) return;
			int apparentLength = index + length;
			if (apparentLength > in->Length)
				throw gcnew System::IndexOutOfRangeException("NorxCore64::_encrypt_p1->The given index and length goes beyond the bounds of the supplied input array.");
			if (outIndex + length > out->Length)
				throw gcnew System::IndexOutOfRangeException("NorxCore64::_encrypt_p1->The given index and length goes beyond the bounds of the supplied output array.");
			int outptr = outIndex;
			array<UInt64>^ state_buffer = gcnew array<UInt64>(NORX64_RATEWORDS);
			for (int i = index; i < apparentLength; i += NORX64_RATEBYTES)
			{
				bool last = i + NORX64_RATEBYTES >= apparentLength;
				if (last)
				{
					array<Byte>^ LastBlock = gcnew array<Byte>(NORX64_RATEBYTES);
					if (i < apparentLength)
						Buffer::BlockCopy(in, i, LastBlock, 0, apparentLength % NORX64_RATEBYTES);
					LastBlock[apparentLength % NORX64_RATEBYTES] = 0x01;
					LastBlock[LastBlock->Length - 1] |= 0x80;
					Buffer::BlockCopy(LastBlock, 0, state_buffer, 0, NORX64_RATEBYTES);
					_burn(LastBlock);
				}
				else
				{
					Buffer::BlockCopy(in, i, state_buffer, 0, NORX64_RATEBYTES);
				}
				state[15] ^= (UInt64)tag;
				_F(state, rounds);
				for (int j = 0; j < state_buffer->Length; j++)
				{
					state[j] ^= state_buffer[j];
				};
				if (last)
					Buffer::BlockCopy(state, 0, out, outptr, apparentLength % NORX64_RATEBYTES);
				else
					Buffer::BlockCopy(state, 0, out, outptr, NORX64_RATEBYTES);
				outptr += NORX64_RATEBYTES;
			}
			_burn(state_buffer);
			state_buffer[0] ^= state_buffer[1] ^ NORX64_CAPBYTES;
		}

		static void _decrypt_p1(
			array<UInt64>^ state, 
			array<const Byte>^ in, int index, int length, 
			const _domain_separator_64 tag, 
			const Byte rounds, 
			int tagBytesInMessage, 
			array<Byte>^% out, int outIndex)
		{
			// Used only for Payload of Parallelism = 1 (not P=0)
			if (in == nullptr || in->Length == 0 || length == 0) return;
			int apparentLength = index + length;
			if (apparentLength > in->Length)
				throw gcnew System::IndexOutOfRangeException("NorxCore64::_decrypt_p1->The given index and length goes beyond the bounds of the supplied input array.");
			if (outIndex + length - tagBytesInMessage > out->Length)
				throw gcnew System::IndexOutOfRangeException("NorxCore64::_decrypt_p1->The given index and length goes beyond the bounds of the supplied output array.");
			int outptr = outIndex;
			int actualLength = apparentLength - tagBytesInMessage; // the tag is processed in another step
			array<UInt64>^ state_buffer = gcnew array<UInt64>(NORX64_RATEWORDS);
			for (int i = index; i < actualLength; i += NORX64_RATEBYTES)
			{
				bool last = i + NORX64_RATEBYTES >= actualLength;
				state[15] ^= (UInt64)tag;
				_F(state, rounds);
				if (last)
				{
					array<Byte>^ LastBlock = gcnew array<Byte>(NORX64_RATEBYTES);
					Buffer::BlockCopy(state, 0, LastBlock, 0, NORX64_RATEBYTES); // !!store state in last block, then overwrite with ciphertext
					if (i < actualLength)
						Buffer::BlockCopy(in, i, LastBlock, 0, actualLength % NORX64_RATEBYTES);
					LastBlock[actualLength % NORX64_RATEBYTES] ^= 0x01; // remove the padding
					LastBlock[LastBlock->Length - 1] ^= 0x80;
					Buffer::BlockCopy(LastBlock, 0, state_buffer, 0, NORX64_RATEBYTES);
					_burn(LastBlock);
				}
				else
				{
					Buffer::BlockCopy(in, i, state_buffer, 0, NORX64_RATEBYTES);
				}			
				for (int j = 0; j < state_buffer->Length; j++)
				{
					UInt64 c = state_buffer[j];
					state_buffer[j] ^= state[j];
					state[j] = c;
				};
				if (last)
					Buffer::BlockCopy(state_buffer, 0, out, outptr, actualLength % NORX64_RATEBYTES);
				else
					Buffer::BlockCopy(state_buffer, 0, out, outptr, NORX64_RATEBYTES);
				outptr += NORX64_RATEBYTES;
			}
			_burn(state_buffer);
			state_buffer[0] ^= state_buffer[1] ^ NORX64_CAPBYTES;
		}

		static void _encrypt_p2(
			array<array<UInt64>^>^ states, 
			array<const Byte>^ in, int index, int length, 
			const _domain_separator_64 tag, 
			const Byte rounds, 
			const Byte lanes, 
			array<Byte>^% out, int outIndex)
		{
			// Used only for Payload of Parallelism > 1 (not P=0)
			if (in == nullptr || in->Length == 0 || length == 0) return;
			int apparentLength = index + length;
			if (apparentLength > in->Length)
				throw gcnew System::IndexOutOfRangeException("NorxCore64::_encrypt_p2->The given index and length goes beyond the bounds of the supplied input array.");
			if (outIndex + length > out->Length)
				throw gcnew System::IndexOutOfRangeException("NorxCore64::_encrypt_p2->The given index and length goes beyond the bounds of the supplied output array.");
			int outptr = outIndex;
			Byte laneptr = 0;
			array<UInt64>^ state_buffer = gcnew array<UInt64>(NORX64_RATEWORDS);
			for (int i = index; i < apparentLength; i += NORX64_RATEBYTES)
			{
				bool last = i + NORX64_RATEBYTES >= in->Length;
				if (last)
				{
					array<Byte>^ LastBlock = gcnew array<Byte>(NORX64_RATEBYTES);
					if (i < in->Length)
						Buffer::BlockCopy(in, i, LastBlock, 0, apparentLength % NORX64_RATEBYTES);
					LastBlock[in->Length % NORX64_RATEBYTES] = 0x01;
					LastBlock[LastBlock->Length - 1] |= 0x80;
					Buffer::BlockCopy(LastBlock, 0, state_buffer, 0, LastBlock->Length);
					_burn(LastBlock);
				}
				else
				{
					Buffer::BlockCopy(in, i, state_buffer, 0, NORX64_RATEBYTES);
				}
				states[laneptr][15] ^= (UInt64)tag;
				_F(states[laneptr], rounds);
				for (int j = 0; j < state_buffer->Length; j++)
				{
					// STATE = STATE ^ P = C
					states[laneptr][j] ^= state_buffer[j];
				};
				if (out != nullptr)
				{
					if (last)
						Buffer::BlockCopy(states[laneptr], 0, out, outptr, apparentLength % NORX64_RATEBYTES);
					else
						Buffer::BlockCopy(states[laneptr], 0, out, outptr, NORX64_RATEBYTES);
					outptr += NORX64_RATEBYTES;
				}
				laneptr = ++laneptr % lanes;
			}
			_burn(state_buffer);
			state_buffer[0] ^= state_buffer[1] ^ NORX64_CAPBYTES;
		}

		static void _decrypt_p2(
			array<array<UInt64>^>^ states, 
			array<const Byte>^ in, int index, int length,
			const _domain_separator_64 tag, 
			const Byte rounds, 
			const Byte lanes, 
			int tagBytesInMessage, 
			array<Byte>^% out, int outIndex)
		{
			// Used only for Payload of Parallelism > 1 (not P=0)
			if (in == nullptr || in->Length == 0 || length == 0) return;
			int apparentLength = index + length;
			if (apparentLength > in->Length)
				throw gcnew System::IndexOutOfRangeException("NorxCore64::_decrypt_p2->The given index and length goes beyond the bounds of the supplied input array.");
			if (outIndex + length - tagBytesInMessage > out->Length)
				throw gcnew System::IndexOutOfRangeException("NorxCore64::_decrypt_p2->The given index and length goes beyond the bounds of the supplied output array.");

			int outptr = outIndex;
			int actualLength = apparentLength - tagBytesInMessage; // the tag is processed in another step
			Byte laneptr = 0;
			array<UInt64>^ state_buffer = gcnew array<UInt64>(NORX64_RATEWORDS);
			for (int i = index; i < actualLength; i += NORX64_RATEBYTES)
			{
				bool last = i + NORX64_RATEBYTES >= actualLength;
				states[laneptr][15] ^= (UInt64)tag;
				_F(states[laneptr], rounds);
				if (last)
				{
					array<Byte>^ LastBlock = gcnew array<Byte>(NORX64_RATEBYTES);
					Buffer::BlockCopy(states[laneptr], 0, LastBlock, 0, NORX64_RATEBYTES); // !!store state in last block, then overwrite with ciphertext
					if (i < actualLength)
						Buffer::BlockCopy(in, i, LastBlock, 0, actualLength % NORX64_RATEBYTES);
					LastBlock[actualLength % NORX64_RATEBYTES] ^= 0x01;
					LastBlock[LastBlock->Length - 1] ^= 0x80;
					Buffer::BlockCopy(LastBlock, 0, state_buffer, 0, LastBlock->Length);
					_burn(LastBlock);
				}
				else
				{
					Buffer::BlockCopy(in, i, state_buffer, 0, NORX64_RATEBYTES);
				}
				for (int j = 0; j < state_buffer->Length; j++)
				{
					// C = (from encryption) P ^ STATE, to recover P then C ^ STATE = P, to recover STATE, then P ^ C
					UInt64 c = state_buffer[j];
					state_buffer[j] ^= states[laneptr][j];
					states[laneptr][j] = c;
				};
				if (last)
					Buffer::BlockCopy(state_buffer, 0, out, outptr, actualLength % NORX64_RATEBYTES);
				else
					Buffer::BlockCopy(state_buffer, 0, out, outptr, NORX64_RATEBYTES);
				outptr += NORX64_RATEBYTES;
				laneptr = ++laneptr % lanes;
			}
			_burn(state_buffer);
			state_buffer[0] ^= state_buffer[1] ^ NORX64_CAPBYTES;
		}

		//static __inline void _branch(array<UInt64>^ state, Byte lane, const Byte rounds)
		//{
		//	state[15] ^= BRANCH_TAG_64;
		//	_F(state, rounds);
		//	// Inject lane ID 
		//	for (Byte i = 0; i < NORX64_RATEWORDS; ++i) {
		//		state[i] ^= lane;
		//	}
		//}

		// stateDest ^= (processed)stateSource, then overwrite stateSource
		static __inline void _merge(array<UInt64>^ stateDest, array<UInt64>^ stateSource, const Byte rounds)
		{
			stateSource[15] ^= MERGE_TAG_64;
			_F(stateSource, rounds);
			for (Byte i = 0; i < 16; ++i) {
				stateDest[i] ^= stateSource[i];
				stateSource[i] |= UInt64::MaxValue; // destroy contents of old state
			}
		}

		static __inline void _finalize(array<UInt64>^ state, array<const UInt64>^ k, const Byte rounds, const short tagsizebits, array<Byte>^% outTag)
		{
			state[15] ^= FINAL_TAG_64;

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

			Buffer::BlockCopy(state, NORX64_RATEWORDS * NORX64_WORDBYTES, outTag, 0, tagsizebits / 8); // extract Tag
			_burn(state); // at this point we can burn the state 
			state[0] ^= state[state[15] % 7] ^ NORX64_CAPBYTES; // prevent compiler optimizations?
		}
	}
}
#endif // !NORX_NORX_CORE64