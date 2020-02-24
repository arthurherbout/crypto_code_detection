#pragma once

#ifndef MORUS_MANAGED_CORE_640
#define MORUS_MANAGED_CORE_640

using namespace System;
using namespace System::IO;

namespace MorusManaged
{
	namespace MorusCore640
	{
		// fibonacci sequence MOD 256, LITTLE ENDIAN
#define CONST32_0_0	0x02010100
#define CONST32_0_1	0x0d080503
#define CONST32_0_2 0x59372215
#define CONST32_0_3 0x6279e990
#define CONST32_1_0	0x55183ddb 
#define CONST32_1_1	0xf12fc26d
#define CONST32_1_2 0x42311120
#define CONST32_1_3 0xdd28b573

#define ROTL32C1 5 
#define ROTL32C2 31
#define ROTL32C3 7
#define ROTL32C4 22
#define ROTL32C5 13

#define RotL_32(x,n)   ((UInt32(x) << n) | (UInt32(x) >> (32-n)))  

		static void _updateState(array<UInt32>^ state, array<const UInt32>^ msgblk)
		{
			UInt32 temp;
			// ROUND 1, Row0 ^= Row3 ^ (Row1 BITWISE-AND Row2)
			state[0] ^= state[12] ^ (state[4] & state[8]);
			state[1] ^= state[13] ^ (state[5] & state[9]);
			state[2] ^= state[14] ^ (state[6] & state[10]);
			state[3] ^= state[15] ^ (state[7] & state[11]);

			state[0] = RotL_32(state[0], ROTL32C1); // Rotl_xxx_yy(S0, b0)
			state[1] = RotL_32(state[1], ROTL32C1);
			state[2] = RotL_32(state[2], ROTL32C1);
			state[3] = RotL_32(state[3], ROTL32C1);

			temp = state[15]; // rotate whole state row 32 bits right
			state[15] = state[14];
			state[14] = state[13];
			state[13] = state[12];
			state[12] = temp;

			// ROUND 2, Row1 ^= Row4 ^ (Row2 BITWISE-AND Row3) ^ MSG
			state[4] ^= state[16] ^ (state[8] & state[12]) ^ msgblk[0];
			state[5] ^= state[17] ^ (state[9] & state[13]) ^ msgblk[1];
			state[6] ^= state[18] ^ (state[10] & state[14]) ^ msgblk[2];
			state[7] ^= state[19] ^ (state[11] & state[15]) ^ msgblk[3];

			state[4] = RotL_32(state[4], ROTL32C2); // Rotl_xxx_yy(S1, b1)
			state[5] = RotL_32(state[5], ROTL32C2);
			state[6] = RotL_32(state[6], ROTL32C2);
			state[7] = RotL_32(state[7], ROTL32C2);

			temp = state[19]; // rotate whole state row 64 bits
			state[19] = state[17];
			state[17] = temp;
			temp = state[18];
			state[18] = state[16];
			state[16] = temp;

			// ROUND 3, Row2 ^= Row0 ^ (Row3 BITWISE-AND Row4) ^ MSG
			state[8] ^= state[0] ^ (state[12] & state[16]) ^ msgblk[0];
			state[9] ^= state[1] ^ (state[13] & state[17]) ^ msgblk[1];
			state[10] ^= state[2] ^ (state[14] & state[18]) ^ msgblk[2];
			state[11] ^= state[3] ^ (state[15] & state[19]) ^ msgblk[3];

			state[8] = RotL_32(state[8], ROTL32C3); // Rotl_xxx_yy(S2, b2)
			state[9] = RotL_32(state[9], ROTL32C3);
			state[10] = RotL_32(state[10], ROTL32C3);
			state[11] = RotL_32(state[11], ROTL32C3);

			temp = state[0]; // rotate whole state row 32 bits left
			state[0] = state[1];
			state[1] = state[2];
			state[2] = state[3];
			state[3] = temp;

			// ROUND 4, Row3 ^= Row1 ^ (Row4 BITWISE-AND Row0) ^ MSG
			state[12] ^= state[4] ^ (state[16] & state[0]) ^ msgblk[0];
			state[13] ^= state[5] ^ (state[17] & state[1]) ^ msgblk[1];
			state[14] ^= state[6] ^ (state[18] & state[2]) ^ msgblk[2];
			state[15] ^= state[7] ^ (state[19] & state[3]) ^ msgblk[3];

			state[12] = RotL_32(state[12], ROTL32C4); // Rotl_xxx_yy(S3, b3)
			state[13] = RotL_32(state[13], ROTL32C4);
			state[14] = RotL_32(state[14], ROTL32C4);
			state[15] = RotL_32(state[15], ROTL32C4);

			temp = state[7]; // rotate whole state row 64 bits
			state[7] = state[5];
			state[5] = temp;
			temp = state[6];
			state[6] = state[4];
			state[4] = temp;

			// ROUND 5, Row4 ^= Row2 ^ (Row0 BITWISE-AND Row1) ^ MSG
			state[16] ^= state[8] ^ (state[0] & state[4]) ^ msgblk[0];
			state[17] ^= state[9] ^ (state[1] & state[5]) ^ msgblk[1];
			state[18] ^= state[10] ^ (state[2] & state[6]) ^ msgblk[2];
			state[19] ^= state[11] ^ (state[3] & state[7]) ^ msgblk[3];

			state[16] = RotL_32(state[16], ROTL32C5); // Rotl_xxx_yy(S4, b4)
			state[17] = RotL_32(state[17], ROTL32C5);
			state[18] = RotL_32(state[18], ROTL32C5);
			state[19] = RotL_32(state[19], ROTL32C5);

			temp = state[11]; // rotate whole state row 32 bits right
			state[11] = state[10];
			state[10] = state[9];
			state[9] = state[8];
			state[8] = temp;
		}

		static array<UInt32>^ _init(array<const Byte>^ key, array<const Byte>^ iv)
		{
			array<UInt32>^ state = gcnew array<UInt32>{ // 5 rows of 4 words
				0,0,0,0, //iv goes here
				0,0,0,0, // key goes here
				UInt32::MaxValue, UInt32::MaxValue, UInt32::MaxValue, UInt32::MaxValue, // all 1's
				CONST32_0_0, CONST32_0_1, CONST32_0_2, CONST32_0_3, // low constant goes here
				CONST32_1_0, CONST32_1_1, CONST32_1_2, CONST32_1_3  // high constant goes here
			}; 
			array<const UInt32>^ temp = gcnew array<const UInt32>(4); // all 0's

			Buffer::BlockCopy(iv, 0, state, 0, iv->Length); // IV fills 16 bytes, full row
			Buffer::BlockCopy(key, 0, state, 4 * sizeof(UInt32), key->Length); // key fills 16 bytes

			array<const UInt32>^ tempKey = gcnew array<const UInt32>{ state[4], state[5], state[6], state[7] }; // need temp key down below
			Byte i;
			for (i = 0; i < 16; i++) MorusCore640::_updateState(state, temp); // update state 16 times
			for (i = 0; i < 4; i++)	state[i + 4] ^= tempKey[i]; // XOR key in a second time after mixing state	
			Buffer::BlockCopy(temp, 0, tempKey, 0, temp->Length * sizeof(UInt32)); // clear tempkey
			// prevent compiler optimizations; these should all be 0 and thus have no mathematical effect
			for (i = 0; i < 4; i++) state[i + 4] ^= tempKey[i];
			tempKey = nullptr;
			temp = nullptr;
			return state;
		}

		//the finalization state of MORUS
		static __inline void _finalize_state(array<UInt32>^ state, const UInt64 adlen, const UInt64 msglen)
		{
			// XOR the first row into the fifth
			state[16] ^= state[0]; 
			state[17] ^= state[1]; 
			state[18] ^= state[2]; 
			state[19] ^= state[3];

			// help prevent forgeries/extension attacks by incorporating plaintext attributes
			// ad & msg lens are always expressed as 64 bits
			{ // C# USING t32, t64
				array<const UInt32>^ t32 = gcnew array<const UInt32>(4);
				array<const UInt64>^ t64 = gcnew array<const UInt64>{adlen << 3, msglen << 3};
				Buffer::BlockCopy(t64, 0, t32, 0, 16);
				for (Byte i = 0; i < 10; i++) _updateState(state, t32); // update state 10 times
			}
			// Row0 ^= (Row1 >>> 32 bits) ^ (Row2 BITWISE-AND Row3), this is output as the tag after this step
			state[0] ^= state[5] ^ (state[8] & state[12]);
			state[1] ^= state[6] ^ (state[9] & state[13]);
			state[2] ^= state[7] ^ (state[10] & state[14]);
			state[3] ^= state[4] ^ (state[11] & state[15]);
		}

		static int _encryptStream(
			MemoryStream^ AdditionalData,
			MemoryStream^ Plaintext,
			array<const Byte>^ Key,
			array<const Byte>^ Nonce,
			MemoryStream^ Ciphertext,
			MemoryStream^ Tag)
		{
			UInt64 MSGLen = Plaintext != nullptr ? Plaintext->Length : 0;
			array<UInt32>^ state = MorusCore640::_init(Key, Nonce);
			//Ciphertext = gcnew array<Byte>(MSGLen + 16);
			UInt64 i;
			UInt64 ADLen = AdditionalData != nullptr ? AdditionalData->Length : 0;
			if (ADLen > 0)
			{	// C# USING: br, buf, NO OUTPUT DURING THIS PHASE! ONLY STATE UPDATES!
				BinaryReader^ br = gcnew BinaryReader(AdditionalData);
				array<UInt32>^ buf = gcnew array<UInt32>(4);
				for (i = 0; (i + 16) <= ADLen; i += 16)
				{
					Buffer::BlockCopy(br->ReadBytes(16), 0, buf, 0, 16);
					MorusCore640::_updateState(state, (array<const UInt32>^)buf);
				}
				Array::Clear(buf, 0, buf->Length);
				if (0 != ADLen % 16)
				{
					Buffer::BlockCopy(br->ReadBytes(ADLen % 16), 0, buf, 0, ADLen % 16);
					MorusCore640::_updateState(state, (array<const UInt32>^)buf);
					Array::Clear(buf, 0, buf->Length);
					state[0] ^= buf[0]; // does nothing (0), but prevents compiler from optimizing out the previous statement 
				}
				else
					state[0] ^= buf[3]; // does nothing (0), but prevents compiler from optimizing out the Clear statement 
			} // end USING
			BinaryWriter^ bw = gcnew BinaryWriter(Ciphertext);
			if (MSGLen > 0)
			{	// C# USING: br, buf, inputBlock
				BinaryReader^ br = gcnew BinaryReader(Plaintext);
				array<UInt32>^ inputBlock = gcnew array<UInt32>(4);
				for (i = 0; (i + 16) <= MSGLen; i += 16)
				{
					Buffer::BlockCopy(br->ReadBytes(16), 0, inputBlock, 0, 16);
					bw->Write(inputBlock[0] ^ state[0] ^ state[5] ^ (state[8] & state[12]));
					bw->Write(inputBlock[1] ^ state[1] ^ state[6] ^ (state[9] & state[13]));
					bw->Write(inputBlock[2] ^ state[2] ^ state[7] ^ (state[10] & state[14]));
					bw->Write(inputBlock[3] ^ state[3] ^ state[4] ^ (state[11] & state[15]));
					MorusCore640::_updateState(state, (array<const UInt32>^)inputBlock);
				}
				Array::Clear(inputBlock, 0, inputBlock->Length);
				if (0 != MSGLen % 16)
				{
					Buffer::BlockCopy(br->ReadBytes(MSGLen % 16), 0, inputBlock, 0, MSGLen % 16);
					array<UInt32>^ outputBlock = gcnew array<UInt32>(4);
					outputBlock[0] = inputBlock[0] ^ state[0] ^ state[5] ^ (state[8] & state[12]);
					outputBlock[1] = inputBlock[1] ^ state[1] ^ state[6] ^ (state[9] & state[13]);
					outputBlock[2] = inputBlock[2] ^ state[2] ^ state[7] ^ (state[10] & state[14]);
					outputBlock[3] = inputBlock[3] ^ state[3] ^ state[4] ^ (state[11] & state[15]);
					for (Byte i = 0; i < MSGLen % 16; i++) // timing attack????
						bw->Write(Buffer::GetByte(outputBlock, i));
					MorusCore640::_updateState(state, (array<const UInt32>^)inputBlock);
					Array::Clear(inputBlock, 0, 4);
					state[0] ^= inputBlock[0]; // does nothing (0), but prevents compiler from optimizing out the previous statement
				}
				else
					state[0] ^= inputBlock[3]; // does nothing (0), but prevents compiler from optimizing out the Clear statement 
			} // end USING
			bw->Flush();
			// TAG GENERATION
			MorusCore640::_finalize_state(state, ADLen, MSGLen); // 1280 128 and 256 do this the same way
			BinaryWriter^ tw = gcnew BinaryWriter(Tag);
			tw->Write(state[0]);
			tw->Write(state[1]);
			tw->Write(state[2]);
			tw->Write(state[3]);
			tw->Flush();
			try {
				Array::Clear(state, 0, state->Length);
				state = gcnew array<UInt32>{state[0], state[1]};
			}
			catch (...) {}
			return (int)state[0]; // should be zero, but used to prevent compiler optimization of Clear
		}

		static int _decryptStream(
			MemoryStream^ AdditionalData,
			MemoryStream^ Ciphertext,
			MemoryStream^ Tag,
			array<const Byte>^ Key,
			array<const Byte>^ Nonce,
			MemoryStream^% Plaintext) // this is assigned by _decryptStream in the event the Plaintext has to be wiped for security
		{
			Int64 MSGLen = 0;
			if (Ciphertext != nullptr)
			{
				// if we have a cipher text BUT NO SEPARATE TAG, then the overall length = C.Len - 16
				// ELSE if we have a separate tag, the length is the length (tag is already accounted for)
				MSGLen = Tag == nullptr ? Ciphertext->Length - 16 : MSGLen = Ciphertext->Length;
			}
			if (MSGLen < 0)
				throw gcnew ArgumentOutOfRangeException("Ciphertext", "Ciphertext cannot be shorter than 16 bytes (length of Tag)");
			array<UInt32>^ state = MorusCore640::_init(Key, Nonce);
			Int64 i;
			Int64 ADLen = AdditionalData != nullptr ? AdditionalData->Length : 0;
			if (ADLen > 0)
			{	// C# USING: br, buf, NO OUTPUT DURING THIS PHASE! ONLY STATE UPDATES!
				BinaryReader^ br = gcnew BinaryReader(AdditionalData);
				array<UInt32>^ buf = gcnew array<UInt32>(4);
				for (i = 0; (i + 16) <= ADLen; i += 16)
				{
					Buffer::BlockCopy(br->ReadBytes(16), 0, buf, 0, 16);
					MorusCore640::_updateState(state, (array<const UInt32>^)buf);
				}
				Array::Clear(buf, 0, buf->Length);
				if (0 != ADLen % 16)
				{
					Buffer::BlockCopy(br->ReadBytes(ADLen % 16), 0, buf, 0, ADLen % 16);
					MorusCore640::_updateState(state, (array<const UInt32>^)buf);
					Array::Clear(buf, 0, buf->Length);
					state[0] ^= buf[0]; // does nothing (0), but prevents compiler from optimizing out the previous statement 
				}
				else
					state[0] ^= buf[3]; // does nothing (0), but prevents compiler from optimizing out the Clear statement 
			} // end USING
			BinaryReader^ br = gcnew BinaryReader(Ciphertext);
			BinaryWriter^ bw = gcnew BinaryWriter(Plaintext);
			if (MSGLen > 0)
			{	// C# USING: buf, inputBlock

				array<UInt32>^ inputBlock = gcnew array<UInt32>(4);
				array<UInt32>^ outputBlock = gcnew array<UInt32>(4);
				for (i = 0; (i + 16) <= MSGLen; i += 16)
				{
					Buffer::BlockCopy(br->ReadBytes(16), 0, inputBlock, 0, 16);
					outputBlock[0] = inputBlock[0] ^ state[0] ^ state[5] ^ (state[8] & state[12]);
					outputBlock[1] = inputBlock[1] ^ state[1] ^ state[6] ^ (state[9] & state[13]);
					outputBlock[2] = inputBlock[2] ^ state[2] ^ state[7] ^ (state[10] & state[14]);
					outputBlock[3] = inputBlock[3] ^ state[3] ^ state[4] ^ (state[11] & state[15]);
					bw->Write(outputBlock[0]);
					bw->Write(outputBlock[1]);
					bw->Write(outputBlock[2]);
					bw->Write(outputBlock[3]);
					MorusCore640::_updateState(state, (array<const UInt32>^)outputBlock);
				}
				Array::Clear(inputBlock, 0, inputBlock->Length); // zero out array
				Array::Clear(outputBlock, 0, outputBlock->Length); // zero out array
				if (0 != MSGLen % 16)
				{
					Buffer::BlockCopy(br->ReadBytes(MSGLen % 16), 0, inputBlock, 0, MSGLen % 16);
					outputBlock[0] = inputBlock[0] ^ state[0] ^ state[5] ^ (state[8] & state[12]);
					outputBlock[1] = inputBlock[1] ^ state[1] ^ state[6] ^ (state[9] & state[13]);
					outputBlock[2] = inputBlock[2] ^ state[2] ^ state[7] ^ (state[10] & state[14]);
					outputBlock[3] = inputBlock[3] ^ state[3] ^ state[4] ^ (state[11] & state[15]);
					for (Byte i = 0; i < 16; i++) // timing attack during the IF statement?? analyse to be sure
						if (i < MSGLen % 16)
							bw->Write(Buffer::GetByte(outputBlock, i)); // output the partial block as plaintext
						else
							Buffer::SetByte(outputBlock, i, 0); // recreate the zero whitespace at the end of the plaintext
					MorusCore640::_updateState(state, (array<const UInt32>^)outputBlock);
					Array::Clear(inputBlock, 0, inputBlock->Length);
					Array::Clear(outputBlock, 0, outputBlock->Length);
					state[0] ^= inputBlock[0] | outputBlock[0]; // does nothing (0), but prevents compiler from optimizing out the previous statement
				}
				else
					state[0] ^= inputBlock[3] | outputBlock[3]; // does nothing (0), but prevents compiler from optimizing out the Clear statement 
			} // end USING
			bw->Flush();
			//bw->Close();
			// TAG GENERATION
			MorusCore640::_finalize_state(state, ADLen, MSGLen); // 1280 128 and 256 do this the same way
			BinaryReader^ tr = gcnew BinaryReader(Tag != nullptr ? Tag : Ciphertext); // if no separate tag, it's in the Ciphertext buffer
			UInt32 a = tr->ReadUInt32(), b = tr->ReadUInt32(), c = tr->ReadUInt32(), d = tr->ReadUInt32();
			UInt32 compare = (a ^ state[0]) | (b ^ state[1]) | (c ^ state[2]) | (d ^ state[3]);
			Array::Clear(state, 0, state->Length); // zero the array
			for (Byte i = 0; i < 5; i++) // go through the rows to zero them
			{
				state[16] |= (state[(i*4)] | UInt32::MaxValue) + compare;
			}
			state = gcnew array<UInt32>{0, compare | UInt32::MaxValue};
			//br->Close();
			if (compare == 0)
			{
				return (int)(state[0]); // prevent compiler optimization of Clear operation above; this value should be 0
			}
			else
			{
				try {
					if (Plaintext != nullptr && Plaintext->Length > 0)
					{
						Array::Clear(Plaintext->GetBuffer(), 0, MSGLen);
						// no matter what, state gets bitwiped to 1 or 0, and then shifted to 0, but this prevents the compiler from optimizing out the Clear
						Plaintext->Seek(0, SeekOrigin::Begin); // if we can't seek, then the exception gets swallowed
						Plaintext->Write(Plaintext->GetBuffer(), 0, 4);
					}
				}
				catch (...)
				{
					System::Diagnostics::Debug::Print("EXCEPTION!");
				}
				return -1;
			}
		}

		/*
		static int _encrypt(
			array<const Byte>^ AdditionalData,
			array<const Byte>^ Plaintext,
			array<const Byte>^ Key,
			array<const Byte>^ Nonce,
			[Out] array<Byte>^% Ciphertext)
		{
			int MSGLen = Plaintext != nullptr ? Plaintext->Length : 0;
			array<array<UInt32>^>^ state = MorusCore640::_init(Key, Nonce);
			Ciphertext = gcnew array<Byte>(MSGLen + 16);
			int i;
			int ADLen = AdditionalData != nullptr ? AdditionalData->Length : 0;
			if (ADLen > 0)
			{	// C# USING: br, buf, NO OUTPUT DURING THIS PHASE! ONLY STATE UPDATES!
				BinaryReader^ br = gcnew BinaryReader(gcnew MemoryStream((array<Byte>^)AdditionalData, false));
				array<UInt32>^ buf = gcnew array<UInt32>(4);
				for (i = 0; (i + 16) <= ADLen; i += 16)
				{
					Buffer::BlockCopy(AdditionalData, i, buf, 0, 16);
					MorusCore640::_updateState(state, (array<const UInt32>^)buf);
				}
				Array::Clear(buf, 0, buf->Length);
				if (0 != ADLen % 16)
				{
					Buffer::BlockCopy(AdditionalData, i, buf, 0, ADLen % 16);
					MorusCore640::_updateState(state, (array<const UInt32>^)buf);
					Array::Clear(buf, 0, buf->Length);
					state[0][0] ^= buf[0]; // does nothing (0), but prevents compiler from optimizing out the previous statement 
				}
				else
					state[0][0] ^= buf[3]; // does nothing (0), but prevents compiler from optimizing out the Clear statement 
			} // end USING
			BinaryWriter^ bw = gcnew BinaryWriter(gcnew MemoryStream(Ciphertext, true));
			if (MSGLen > 0)
			{	// C# USING: br, buf, inputBlock
				BinaryReader^ br = gcnew BinaryReader(gcnew MemoryStream((array<Byte>^)Plaintext, false));
				array<UInt32>^ inputBlock = gcnew array<UInt32>(4);
				for (i = 0; (i + 16) <= MSGLen; i += 16)
				{
					inputBlock[0] = br->ReadUInt32(); inputBlock[1] = br->ReadUInt32();
					inputBlock[2] = br->ReadUInt32(); inputBlock[3] = br->ReadUInt32();
					bw->Write(inputBlock[0] ^ state[0][0] ^ state[5] ^ (state[8] & state[12]));
					bw->Write(inputBlock[1] ^ state[0][1] ^ state[6] ^ (state[9] & state[13]));
					bw->Write(inputBlock[2] ^ state[0][2] ^ state[7] ^ (state[10] & state[14]));
					bw->Write(inputBlock[3] ^ state[0][3] ^ state[4] ^ (state[11] & state[15]));
					MorusCore640::_updateState(state, (array<const UInt32>^)inputBlock);
				}
				Array::Clear(inputBlock, 0, inputBlock->Length);
				if (0 != MSGLen % 16)
				{
					Buffer::BlockCopy(br->ReadBytes(MSGLen % 16), 0, inputBlock, 0, MSGLen % 16);
					array<UInt32>^ outputBlock = gcnew array<UInt32>(4);
					outputBlock[0] = inputBlock[0] ^ state[0][0] ^ state[5] ^ (state[8] & state[12]);
					outputBlock[1] = inputBlock[1] ^ state[0][1] ^ state[6] ^ (state[9] & state[13]);
					outputBlock[2] = inputBlock[2] ^ state[0][2] ^ state[7] ^ (state[10] & state[14]);
					outputBlock[3] = inputBlock[3] ^ state[0][3] ^ state[4] ^ (state[11] & state[15]);
					for (Byte i = 0; i < MSGLen % 16; i++) // timing attack????
						bw->Write(Buffer::GetByte(outputBlock, i));
					MorusCore640::_updateState(state, (array<const UInt32>^)inputBlock);
					Array::Clear(inputBlock, 0, 4);
					state[0][0] ^= inputBlock[0]; // does nothing (0), but prevents compiler from optimizing out the previous statement
				}
				else
					state[0][0] ^= inputBlock[3]; // does nothing (0), but prevents compiler from optimizing out the Clear statement 
			} // end USING

			// TAG GENERATION
			MorusCore640::_finalize_state(state, ADLen, MSGLen); // 1280 128 and 256 do this the same way
			bw->Write(state[0][0]);
			bw->Write(state[0][1]);
			bw->Write(state[0][2]);
			bw->Write(state[0][3]);
			bw->Flush();
			bw->Close();
			try {
				Array::Clear(state, 0, state->Length);
				state[0] = gcnew array<UInt32>(1);
			}
			catch (...) {}
			return (int)state[0][0]; // should be zero, but used to prevent compiler optimization of Clear
		}

		static int _decrypt(
			array<const Byte>^ AdditionalData,
			array<const Byte>^ Ciphertext,
			array<const Byte>^ Key,
			array<const Byte>^ Nonce,
			array<Byte>^% Plaintext)
		{
			int MSGLen = Ciphertext != nullptr ? Ciphertext->Length - 16 : 16; // full length - tag, or just tag
			if (MSGLen < 0)
				throw gcnew ArgumentOutOfRangeException("Ciphertext", "Ciphertext cannot be shorter than 16 bytes (length of Tag)");
			array<array<UInt32>^>^ state = MorusCore640::_init(Key, Nonce);
			int i;
			int ADLen = AdditionalData != nullptr ? AdditionalData->Length : 0;
			if (ADLen > 0)
			{	// C# USING: br, buf, NO OUTPUT DURING THIS PHASE! ONLY STATE UPDATES!
				BinaryReader^ br = gcnew BinaryReader(gcnew MemoryStream((array<Byte>^)AdditionalData, false));
				array<UInt32>^ buf = gcnew array<UInt32>(4);
				for (i = 0; (i + 16) <= ADLen; i += 16)
				{
					Buffer::BlockCopy(AdditionalData, i, buf, 0, 16);
					MorusCore640::_updateState(state, (array<const UInt32>^)buf);
				}
				Array::Clear(buf, 0, buf->Length);
				if (0 != ADLen % 16)
				{
					Buffer::BlockCopy(AdditionalData, i, buf, 0, ADLen % 16);
					MorusCore640::_updateState(state, (array<const UInt32>^)buf);
					Array::Clear(buf, 0, buf->Length);
					state[0][0] ^= buf[0]; // does nothing (0), but prevents compiler from optimizing out the previous statement 
				}
				else
					state[0][0] ^= buf[3]; // does nothing (0), but prevents compiler from optimizing out the Clear statement 
			} // end USING
			//array<Byte>^ tempPlaintext = gcnew array<Byte>(MSGLen);
			BinaryReader^ br = gcnew BinaryReader(gcnew MemoryStream((array<Byte>^)Ciphertext, false));
			BinaryWriter^ bw = gcnew BinaryWriter(gcnew MemoryStream(Plaintext, true));
			if (MSGLen > 0)
			{	// C# USING: buf, inputBlock

				array<UInt32>^ inputBlock = gcnew array<UInt32>(4);
				array<UInt32>^ outputBlock = gcnew array<UInt32>(4);
				for (i = 0; (i + 16) <= MSGLen; i += 16)
				{
					inputBlock[0] = br->ReadUInt32(); inputBlock[1] = br->ReadUInt32();
					inputBlock[2] = br->ReadUInt32(); inputBlock[3] = br->ReadUInt32();
					outputBlock[0] = inputBlock[0] ^ state[0][0] ^ state[5] ^ (state[8] & state[12]);
					outputBlock[1] = inputBlock[1] ^ state[0][1] ^ state[6] ^ (state[9] & state[13]);
					outputBlock[2] = inputBlock[2] ^ state[0][2] ^ state[7] ^ (state[10] & state[14]);
					outputBlock[3] = inputBlock[3] ^ state[0][3] ^ state[4] ^ (state[11] & state[15]);
					bw->Write(outputBlock[0]);
					bw->Write(outputBlock[1]);
					bw->Write(outputBlock[2]);
					bw->Write(outputBlock[3]);
					MorusCore640::_updateState(state, (array<const UInt32>^)outputBlock);
				}
				Array::Clear(inputBlock, 0, inputBlock->Length); // zero out array
				Array::Clear(outputBlock, 0, outputBlock->Length); // zero out array
				if (0 != MSGLen % 16)
				{
					Buffer::BlockCopy(br->ReadBytes(MSGLen % 16), 0, inputBlock, 0, MSGLen % 16);
					outputBlock[0] = inputBlock[0] ^ state[0][0] ^ state[5] ^ (state[8] & state[12]);
					outputBlock[1] = inputBlock[1] ^ state[0][1] ^ state[6] ^ (state[9] & state[13]);
					outputBlock[2] = inputBlock[2] ^ state[0][2] ^ state[7] ^ (state[10] & state[14]);
					outputBlock[3] = inputBlock[3] ^ state[0][3] ^ state[4] ^ (state[11] & state[15]);
					for (Byte i = 0; i < 16; i++) // timing attack during the IF statement?? analyse to be sure
						if (i < MSGLen % 16)
							bw->Write(Buffer::GetByte(outputBlock, i)); // output the partial block as plaintext
						else
							Buffer::SetByte(outputBlock, i, 0); // recreate the zero whitespace at the end of the plaintext
					MorusCore640::_updateState(state, (array<const UInt32>^)outputBlock);
					Array::Clear(inputBlock, 0, inputBlock->Length);
					Array::Clear(outputBlock, 0, outputBlock->Length);
					state[0][0] ^= inputBlock[0] | outputBlock[0]; // does nothing (0), but prevents compiler from optimizing out the previous statement
				}
				else
					state[0][0] ^= inputBlock[3] | outputBlock[3]; // does nothing (0), but prevents compiler from optimizing out the Clear statement 
			} // end USING
			bw->Flush();
			bw->Close();
			// TAG GENERATION
			MorusCore640::_finalize_state(state, ADLen, MSGLen); // 1280 128 and 256 do this the same way
			UInt32 a, b, c, d;
			a = br->ReadUInt32();
			b = br->ReadUInt32();
			c = br->ReadUInt32();
			d = br->ReadUInt32();
			UInt32 compare = (a ^ state[0][0]) | (b ^ state[0][1]) | (c ^ state[0][2]) | (d ^ state[0][3]);
			for (Byte i = 0; i < 5; i++) // go through the rows to zero them
			{
				Array::Clear(state[i], 0, state[i]->Length); // zero the row
				state[16] |= (state[i][0] | UInt32::MaxValue) + compare;
			}
			try // try to prevent compiler optimization
			{
				Array::Clear(state, 0, state->Length); // blow away the individual row array references
				state[0] = gcnew array<UInt32>{compare | UInt32::MaxValue};
			}
			catch (...)
			{
				System::Diagnostics::Debug::Print("EXCEPTION!");
			}
			br->Close();
			if (compare == 0)
			{
				//Plaintext = tempPlaintext;
				return 0;
			}
			else
			{
				try {
					if (Plaintext != nullptr && Plaintext->Length > 0)
					{
						Array::Clear(Plaintext, 0, MSGLen);
						// no matter what, state gets bitwiped to 1 or 0, and then shifted to 0, but this prevents the compiler from optimizing out the Clear
						Plaintext[0] = (Byte)(state[0][0] & (UInt32)compare) >> 1;
					}
				}
				catch (...)
				{
					System::Diagnostics::Debug::Print("EXCEPTION!");
				}
				return -1; 
			}
		}*/
	}
}
#endif // !MORUS_MANAGED_CORE_640
