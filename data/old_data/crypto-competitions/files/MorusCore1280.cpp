#pragma once

#ifndef MORUS_MANAGED_CORE_1280
#define MORUS_MANAGED_CORE_1280

using namespace System;
using namespace System::IO;

namespace MorusManaged
{
	namespace MorusCore1280
	{
		// fibonacci sequence MOD 256, LITTLE ENDIAN
#define CONST64_0_0	0x0d08050302010100
#define CONST64_0_1	0x6279e99059372215
#define CONST64_1_0	0xf12fc26d55183ddb 
#define CONST64_1_1	0xdd28b57342311120

#define ROTL64C1 13
#define ROTL64C2 46
#define ROTL64C3 38
#define ROTL64C4 7
#define ROTL64C5 4

#define RotL_64(x,n)   ((UInt64(x) << (n)) | (UInt64(x) >> (64-n)))
#define MORUS_1280_TAG_LEN_BYTES	16
#define MORUS_1280_TAG_LEN_WORDS	2

		static void _updateState(array<UInt64>^ state, array<const UInt64>^ msgblk)
		{
			UInt64 temp;
			// ROUND 1, Row0 ^= Row3 ^ (Row1 BITWISE-AND Row2)
			state[0] ^= state[12] ^ (state[4] & state[8]);
			state[1] ^= state[13] ^ (state[5] & state[9]);
			state[2] ^= state[14] ^ (state[6] & state[10]); 
			state[3] ^= state[15] ^ (state[7] & state[11]);
			
			state[0] = RotL_64(state[0], ROTL64C1); // Rotl_xxx_yy(S0, b0)
			state[1] = RotL_64(state[1], ROTL64C1);       
			state[2] = RotL_64(state[2], ROTL64C1);       
			state[3] = RotL_64(state[3], ROTL64C1);
			
			temp = state[15]; // rotate whole state row 64 bits right
			state[15] = state[14];  
			state[14] = state[13];  
			state[13] = state[12];  
			state[12] = temp;

			// ROUND 2, Row1 ^= Row4 ^ (Row2 BITWISE-AND Row3) ^ MSG
			state[4] ^= state[16] ^ (state[8] & state[12]) ^ msgblk[0];   
			state[5] ^= state[17] ^ (state[9] & state[13]) ^ msgblk[1];   
			state[6] ^= state[18] ^ (state[10] & state[14]) ^ msgblk[2];   
			state[7] ^= state[19] ^ (state[11] & state[15]) ^ msgblk[3];

			state[4] = RotL_64(state[4], ROTL64C2); // Rotl_xxx_yy(S1, b1)
			state[5] = RotL_64(state[5], ROTL64C2);
			state[6] = RotL_64(state[6], ROTL64C2);
			state[7] = RotL_64(state[7], ROTL64C2);
			
			temp = state[19]; // rotate whole state row 128 bits
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
			
			state[8] = RotL_64(state[8], ROTL64C3); // Rotl_xxx_yy(S2, b2)
			state[9] = RotL_64(state[9], ROTL64C3);       
			state[10] = RotL_64(state[10], ROTL64C3);       
			state[11] = RotL_64(state[11], ROTL64C3);
			
			temp = state[0]; // rotate whole state row 64 bits left
			state[0] = state[1];
			state[1] = state[2];
			state[2] = state[3];
			state[3] = temp;

			// ROUND 4, Row3 ^= Row1 ^ (Row4 BITWISE-AND Row0) ^ MSG
			state[12] ^= state[4] ^ (state[16] & state[0]) ^ msgblk[0];
			state[13] ^= state[5] ^ (state[17] & state[1]) ^ msgblk[1];
			state[14] ^= state[6] ^ (state[18] & state[2]) ^ msgblk[2];
			state[15] ^= state[7] ^ (state[19] & state[3]) ^ msgblk[3];
			
			state[12] = RotL_64(state[12], ROTL64C4); // Rotl_xxx_yy(S3, b3)
			state[13] = RotL_64(state[13], ROTL64C4);       
			state[14] = RotL_64(state[14], ROTL64C4);       
			state[15] = RotL_64(state[15], ROTL64C4);
			
			temp = state[7]; // rotate whole state row 128 bits
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
				
			state[16] = RotL_64(state[16], ROTL64C5); // Rotl_xxx_yy(S4, b4)
			state[17] = RotL_64(state[17], ROTL64C5);
			state[18] = RotL_64(state[18], ROTL64C5);
			state[19] = RotL_64(state[19], ROTL64C5);
			
			temp = state[11]; // rotate whole state row 64 bits right
			state[11] = state[10];  
			state[10] = state[9];  
			state[9] = state[8];  
			state[8] = temp;
		}

		static array<UInt64>^ _init(array<const Byte>^ key, array<const Byte>^ iv)
		{
			array<UInt64>^ state = gcnew array<UInt64>{ // 5 rows * 4 words
				0,0,0,0, // iv goes in front half here
				0,0,0,0, // key goes here (twice if 16 bytes (128 bits), once if 32 bytes (256 bits))
				UInt64::MaxValue, UInt64::MaxValue, UInt64::MaxValue, UInt64::MaxValue, // all 1's
				0,0,0,0, // all 0's
				CONST64_0_0, CONST64_0_1, CONST64_1_0, CONST64_1_1}; // constant goes here
			array<UInt64>^ temp = gcnew array<UInt64>(4); // all 0's

			Buffer::BlockCopy(iv, 0, state, 0, iv->Length); // IV fills first 16 bytes, next 16 bytes are 0
			if (key->Length == 16)
			{
				Buffer::BlockCopy(key, 0, state, 4 * sizeof(UInt64), key->Length); // key fills first 16 bytes...
				Buffer::BlockCopy(key, 0, state, 6 * sizeof(UInt64), key->Length); // ...repeated in second 16 bytes
			}
			else if(key->Length == 32)
				Buffer::BlockCopy(key, 0, state, 4 * sizeof(UInt64), key->Length); // key fills all 32 bytes (different from 1280-128)

			array<UInt64>^ tempKey = gcnew array<UInt64>{ state[4], state[5], state[6], state[7] }; // need temp key down below
			Byte i;
			for (i = 0; i < 16; i++) _updateState(state, (array<const UInt64>^)temp); // update state 16 times
			for (i = 0; i < 4; i++) state[i + 4] ^= tempKey[i]; // XOR key in a second time after mixing state
			Buffer::BlockCopy(temp, 0, tempKey, 0, temp->Length * sizeof(UInt64)); // clear tempkey
			// prevent compiler optimizations; these should all be 0 and thus have no mathematical effect
			for (i = 0; i < 4; i++) state[i + 4] ^= tempKey[i];
			tempKey = nullptr;
			temp = nullptr;
			return state;
		}

		//the finalization state of MORUS
		static __inline void _finalize_state(array<UInt64>^ state, const UInt64 adlen, const UInt64 msglen)
		{
			// XOR the first row into the fifth
			state[16] ^= state[0]; 
			state[17] ^= state[1]; 
			state[18] ^= state[2]; 
			state[19] ^= state[3];

			// help prevent forgeries/extension attacks by incorporating plaintext attributes
			{// C# USING t64
				array<const UInt64>^ t64 = { adlen << 3, msglen << 3, 0, 0 };
				for (Byte i = 0; i < 10; i++) _updateState(state, t64); // update state 10 times
			}
			// Row0 ^= (Row1 >>> 64 bits) ^ (Row2 BITWISE-AND Row3), this is output as the tag after this step
			state[0] ^= state[5] ^ (state[8] & state[12]);
			state[1] ^= state[6] ^ (state[9] & state[13]);
			//state[2] ^= state[7] ^ (state[10] & state[14]); // these aren't needed, we only need 128 bits, half a state row
			//state[3] ^= state[4] ^ (state[11] & state[15]);
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
			array<UInt64>^ state = _init(Key, Nonce);
			//Ciphertext = gcnew array<Byte>(MSGLen + 16);
			UInt64 i;
			UInt64 ADLen = AdditionalData != nullptr ? AdditionalData->Length : 0;
			if (ADLen > 0)
			{	// C# USING: br, buf, NO OUTPUT DURING THIS PHASE! ONLY STATE UPDATES!
				BinaryReader^ br = gcnew BinaryReader(AdditionalData);
				array<UInt64>^ buf = gcnew array<UInt64>(4);
				for (i = 0; (i + 32) <= ADLen; i += 32)
				{
					Buffer::BlockCopy(br->ReadBytes(32), 0, buf, 0, 32);
					MorusCore1280::_updateState(state, (array<const UInt64>^)buf);
				}
				Array::Clear(buf, 0, buf->Length);
				if (0 != ADLen % 32)
				{
					Buffer::BlockCopy(br->ReadBytes(ADLen % 32), 0, buf, 0, ADLen % 32);
					MorusCore1280::_updateState(state, (array<const UInt64>^)buf);
					Array::Clear(buf, 0, buf->Length);
					state[0] ^= buf[0]; // does nothing (0), but prevents compiler from optimizing out the previous statement 
				}
				else
					state[0] ^= buf[3]; // does nothing (0), but prevents compiler from optimizing out the Clear statement 
			} // end USING
			{ // C# USING: bw
				BinaryWriter^ bw = gcnew BinaryWriter(Ciphertext);
				if (MSGLen > 0)
				{	// C# USING: br, buf, inputBlock
					BinaryReader^ br = gcnew BinaryReader(Plaintext);
					array<UInt64>^ inputBlock = gcnew array<UInt64>(4);
					for (i = 0; (i + 32) <= MSGLen; i += 32)
					{
						inputBlock[0] = br->ReadUInt64(); inputBlock[1] = br->ReadUInt64();
						inputBlock[2] = br->ReadUInt64(); inputBlock[3] = br->ReadUInt64();
						bw->Write(inputBlock[0] ^ state[0] ^ state[5] ^ (state[8] & state[12]));
						bw->Write(inputBlock[1] ^ state[1] ^ state[6] ^ (state[9] & state[13]));
						bw->Write(inputBlock[2] ^ state[2] ^ state[7] ^ (state[10] & state[14]));
						bw->Write(inputBlock[3] ^ state[3] ^ state[4] ^ (state[11] & state[15]));
						MorusCore1280::_updateState(state, (array<const UInt64>^)inputBlock);
					}
					Array::Clear(inputBlock, 0, inputBlock->Length);
					if (0 != MSGLen % 32)
					{
						Buffer::BlockCopy(br->ReadBytes(MSGLen % 32), 0, inputBlock, 0, MSGLen % 32);
						array<UInt64>^ outputBlock = gcnew array<UInt64>(4);
						outputBlock[0] = inputBlock[0] ^ state[0] ^ state[5] ^ (state[8] & state[12]);
						outputBlock[1] = inputBlock[1] ^ state[1] ^ state[6] ^ (state[9] & state[13]);
						outputBlock[2] = inputBlock[2] ^ state[2] ^ state[7] ^ (state[10] & state[14]);
						outputBlock[3] = inputBlock[3] ^ state[3] ^ state[4] ^ (state[11] & state[15]);
						for (Byte i = 0; i < MSGLen % 32; i++) // timing attack????
							bw->Write(Buffer::GetByte(outputBlock, i));
						MorusCore1280::_updateState(state, (array<const UInt64>^)inputBlock);
						Array::Clear(inputBlock, 0, 4);
						state[0] ^= inputBlock[0]; // does nothing (0), but prevents compiler from optimizing out the previous statement
					}
					else
						state[0] ^= inputBlock[3]; // does nothing (0), but prevents compiler from optimizing out the Clear statement 
				} // end USING
				bw->Flush();
				bw = nullptr;
			} // end USING
			// TAG GENERATION
			MorusCore1280::_finalize_state(state, ADLen, MSGLen); // 1280 128 and 256 do this the same way
			BinaryWriter^ tw = gcnew BinaryWriter(Tag);
			tw->Write(state[0]);
			tw->Write(state[1]);
			tw->Flush();
			tw = nullptr;
			try {
				Array::Clear(state, 0, state->Length);
				state = gcnew array<UInt64>{state[0], state[1]};
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
			MemoryStream^ Plaintext) 
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
			array<UInt64>^ state = MorusCore1280::_init(Key, Nonce);
			Int64 i;
			Int64 ADLen = AdditionalData != nullptr ? AdditionalData->Length : 0;
			if (ADLen > 0)
			{	// C# USING: br, buf, NO OUTPUT DURING THIS PHASE! ONLY STATE UPDATES!
				BinaryReader^ br = gcnew BinaryReader(AdditionalData);
				array<UInt64>^ buf = gcnew array<UInt64>(4);
				for (i = 0; (i + 32) <= ADLen; i += 32)
				{
					//Buffer::BlockCopy(AdditionalData, i, buf, 0, 32);
					buf[0] = br->ReadUInt64(); buf[1] = br->ReadUInt64(); buf[2] = br->ReadUInt64(); buf[3] = br->ReadUInt64();
					MorusCore1280::_updateState(state, (array<const UInt64>^)buf);
				}
				Array::Clear(buf, 0, buf->Length);
				if (0 != ADLen % 32)
				{
					//Buffer::BlockCopy(AdditionalData, i, buf, 0, ADLen % 32);
					Buffer::BlockCopy(br->ReadBytes(ADLen % 32), 0, buf, 0, ADLen % 32);
					MorusCore1280::_updateState(state, (array<const UInt64>^)buf);
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

				array<UInt64>^ inputBlock = gcnew array<UInt64>(4);
				array<UInt64>^ outputBlock = gcnew array<UInt64>(4);
				for (i = 0; (i + 32) <= MSGLen; i += 32)
				{
					inputBlock[0] = br->ReadUInt64(); inputBlock[1] = br->ReadUInt64();
					inputBlock[2] = br->ReadUInt64(); inputBlock[3] = br->ReadUInt64();
					outputBlock[0] = inputBlock[0] ^ state[0] ^ state[5] ^ (state[8] & state[12]);
					outputBlock[1] = inputBlock[1] ^ state[1] ^ state[6] ^ (state[9] & state[13]);
					outputBlock[2] = inputBlock[2] ^ state[2] ^ state[7] ^ (state[10] & state[14]);
					outputBlock[3] = inputBlock[3] ^ state[3] ^ state[4] ^ (state[11] & state[15]);
					bw->Write(outputBlock[0]);
					bw->Write(outputBlock[1]);
					bw->Write(outputBlock[2]);
					bw->Write(outputBlock[3]);
					MorusCore1280::_updateState(state, (array<const UInt64>^)outputBlock);
				}
				Array::Clear(inputBlock, 0, inputBlock->Length); // zero out array
				Array::Clear(outputBlock, 0, outputBlock->Length); // zero out array
				if (0 != MSGLen % 32)
				{
					Buffer::BlockCopy(br->ReadBytes(MSGLen % 32), 0, inputBlock, 0, MSGLen % 32);
					outputBlock[0] = inputBlock[0] ^ state[0] ^ state[5] ^ (state[8] & state[12]);
					outputBlock[1] = inputBlock[1] ^ state[1] ^ state[6] ^ (state[9] & state[13]);
					outputBlock[2] = inputBlock[2] ^ state[2] ^ state[7] ^ (state[10] & state[14]);
					outputBlock[3] = inputBlock[3] ^ state[3] ^ state[4] ^ (state[11] & state[15]);
					for (Byte i = 0; i < 32; i++) // timing attack during the IF statement?? analyse to be sure
						if (i < MSGLen % 32)
							bw->Write(Buffer::GetByte(outputBlock, i)); // output the partial block as plaintext
						else
							Buffer::SetByte(outputBlock, i, 0); // recreate the zero whitespace at the end of the plaintext
					MorusCore1280::_updateState(state, (array<const UInt64>^)outputBlock);
					Array::Clear(inputBlock, 0, inputBlock->Length);
					Array::Clear(outputBlock, 0, outputBlock->Length);
					state[0] ^= inputBlock[0] | outputBlock[0]; // does nothing (0), but prevents compiler from optimizing out the previous statement
				}
				else
					state[0] ^= inputBlock[3] | outputBlock[3]; // does nothing (0), but prevents compiler from optimizing out the Clear statement 
			} // end USING
			bw->Flush();
			// TAG GENERATION
			MorusCore1280::_finalize_state(state, ADLen, MSGLen); // 1280 128 and 256 do this the same way
			BinaryReader^ tr = gcnew BinaryReader(Tag != nullptr ? Tag : Ciphertext); // if no separate tag, it's in the Ciphertext buffer);
			UInt64 a = tr->ReadUInt64(), b = tr->ReadUInt64();
			UInt64 compare = (a ^ state[0]) | (b ^ state[1]);
			Array::Clear(state, 0, state->Length); // zero the state
			for (Byte i = 0; i < 5; i++)
			{
				state[16] |= (state[i*4] | UInt64::MaxValue) + compare;
			}
			state = gcnew array<UInt64>{state[0], compare | UInt64::MaxValue};
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
			array<Byte>^% Ciphertext,
			array<Byte>^% Tag)
		{
			int MSGLen = Plaintext != nullptr ? Plaintext->Length : 0;
			array<array<UInt64>^>^ state = _init(Key, Nonce);
			//Ciphertext = gcnew array<Byte>(MSGLen + 16);
			int i;
			int ADLen = AdditionalData != nullptr ? AdditionalData->Length : 0;
			if (ADLen > 0)
			{	// C# USING: br, buf, NO OUTPUT DURING THIS PHASE! ONLY STATE UPDATES!
				BinaryReader^ br = gcnew BinaryReader(gcnew MemoryStream((array<Byte>^)AdditionalData, false));
				array<UInt64>^ buf = gcnew array<UInt64>(4);
				for (i = 0; (i + 32) <= ADLen; i += 32)
				{
					Buffer::BlockCopy(AdditionalData, i, buf, 0, 32);
					MorusCore1280::_updateState(state, (array<const UInt64>^)buf);
				}
				Array::Clear(buf, 0, buf->Length);
				if (0 != ADLen % 32)
				{
					Buffer::BlockCopy(AdditionalData, i, buf, 0, ADLen % 32);
					MorusCore1280::_updateState(state, (array<const UInt64>^)buf);
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
				array<UInt64>^ inputBlock = gcnew array<UInt64>(4);
				for (i = 0; (i + 32) <= MSGLen; i += 32)
				{
					inputBlock[0] = br->ReadUInt64(); inputBlock[1] = br->ReadUInt64();
					inputBlock[2] = br->ReadUInt64(); inputBlock[3] = br->ReadUInt64();
					bw->Write(inputBlock[0] ^ state[0][0] ^ state[5] ^ (state[8] & state[12]));
					bw->Write(inputBlock[1] ^ state[0][1] ^ state[6] ^ (state[9] & state[13]));
					bw->Write(inputBlock[2] ^ state[0][2] ^ state[7] ^ (state[10] & state[14]));
					bw->Write(inputBlock[3] ^ state[0][3] ^ state[4] ^ (state[11] & state[15]));
					MorusCore1280::_updateState(state, (array<const UInt64>^)inputBlock);
				}
				Array::Clear(inputBlock, 0, inputBlock->Length);
				if (0 != MSGLen % 32)
				{
					Buffer::BlockCopy(br->ReadBytes(MSGLen % 32), 0, inputBlock, 0, MSGLen % 32);
					array<UInt64>^ outputBlock = gcnew array<UInt64>(4);
					outputBlock[0] = inputBlock[0] ^ state[0][0] ^ state[5] ^ (state[8] & state[12]);
					outputBlock[1] = inputBlock[1] ^ state[0][1] ^ state[6] ^ (state[9] & state[13]);
					outputBlock[2] = inputBlock[2] ^ state[0][2] ^ state[7] ^ (state[10] & state[14]);
					outputBlock[3] = inputBlock[3] ^ state[0][3] ^ state[4] ^ (state[11] & state[15]);
					for (Byte i = 0; i < MSGLen % 32; i++) // timing attack????
						bw->Write(Buffer::GetByte(outputBlock, i));
					MorusCore1280::_updateState(state, (array<const UInt64>^)inputBlock);
					Array::Clear(inputBlock, 0, 4);
					state[0][0] ^= inputBlock[0]; // does nothing (0), but prevents compiler from optimizing out the previous statement
				}
				else
					state[0][0] ^= inputBlock[3]; // does nothing (0), but prevents compiler from optimizing out the Clear statement 
			} // end USING
			bw->Flush();
			bw->Close();
			// TAG GENERATION
			MorusCore1280::_finalize_state(state, ADLen, MSGLen); // 1280 128 and 256 do this the same way
			BinaryWriter^ tw = gcnew BinaryWriter(gcnew MemoryStream(Tag, true));
			tw->Write(state[0][0]);
			tw->Write(state[0][1]);
			tw->Flush();
			try {
				Array::Clear(state, 0, state->Length);
				state[0] = gcnew array<UInt64>(1);
			}
			catch (...) {}
			return (int)state[0][0]; // should be zero, but used to prevent compiler optimization of Clear
		}

		static int _decrypt(
			array<const Byte>^ AdditionalData,
			array<const Byte>^ Ciphertext,
			array<const Byte>^ Tag,
			array<const Byte>^ Key,
			array<const Byte>^ Nonce,
			array<Byte>^ Plaintext)
		{
			int MSGLen = Ciphertext != nullptr ? Ciphertext->Length - 16 : 16; // full length - tag, or just tag
			if (MSGLen < 0)
				throw gcnew ArgumentOutOfRangeException("Ciphertext", "Ciphertext cannot be shorter than 16 bytes (length of Tag)");
			array<array<UInt64>^>^ state = MorusCore1280::_init(Key, Nonce);
			int i;
			int ADLen = AdditionalData != nullptr ? AdditionalData->Length : 0;
			if (ADLen > 0)
			{	// C# USING: br, buf, NO OUTPUT DURING THIS PHASE! ONLY STATE UPDATES!
				BinaryReader^ br = gcnew BinaryReader(gcnew MemoryStream((array<Byte>^)AdditionalData, false));
				array<UInt64>^ buf = gcnew array<UInt64>(4);
				for (i = 0; (i + 32) <= ADLen; i += 32)
				{
					Buffer::BlockCopy(AdditionalData, i, buf, 0, 32);
					MorusCore1280::_updateState(state, (array<const UInt64>^)buf);
				}
				Array::Clear(buf, 0, buf->Length);
				if (0 != ADLen % 32)
				{
					Buffer::BlockCopy(AdditionalData, i, buf, 0, ADLen % 32);
					MorusCore1280::_updateState(state, (array<const UInt64>^)buf);
					Array::Clear(buf, 0, buf->Length);
					state[0][0] ^= buf[0]; // does nothing (0), but prevents compiler from optimizing out the previous statement 
				}
				else
					state[0][0] ^= buf[3]; // does nothing (0), but prevents compiler from optimizing out the Clear statement 
			} // end USING
			//array<Byte>^ tempPlaintext = gcnew array<Byte>(MSGLen);
			{ // C# USING: br, bw
				BinaryReader^ br = gcnew BinaryReader(gcnew MemoryStream((array<Byte>^)Ciphertext, false));
				BinaryWriter^ bw = gcnew BinaryWriter(gcnew MemoryStream(Plaintext, true));
				if (MSGLen > 0)
				{	// C# USING: buf, inputBlock

					array<UInt64>^ inputBlock = gcnew array<UInt64>(4);
					array<UInt64>^ outputBlock = gcnew array<UInt64>(4);
					for (i = 0; (i + 32) <= MSGLen; i += 32)
					{
						inputBlock[0] = br->ReadUInt64(); inputBlock[1] = br->ReadUInt64();
						inputBlock[2] = br->ReadUInt64(); inputBlock[3] = br->ReadUInt64();
						outputBlock[0] = inputBlock[0] ^ state[0][0] ^ state[5] ^ (state[8] & state[12]);
						outputBlock[1] = inputBlock[1] ^ state[0][1] ^ state[6] ^ (state[9] & state[13]);
						outputBlock[2] = inputBlock[2] ^ state[0][2] ^ state[7] ^ (state[10] & state[14]);
						outputBlock[3] = inputBlock[3] ^ state[0][3] ^ state[4] ^ (state[11] & state[15]);
						bw->Write(outputBlock[0]);
						bw->Write(outputBlock[1]);
						bw->Write(outputBlock[2]);
						bw->Write(outputBlock[3]);
						MorusCore1280::_updateState(state, (array<const UInt64>^)outputBlock);
					}
					Array::Clear(inputBlock, 0, inputBlock->Length); // zero out array
					Array::Clear(outputBlock, 0, outputBlock->Length); // zero out array
					if (0 != MSGLen % 32)
					{
						Buffer::BlockCopy(br->ReadBytes(MSGLen % 32), 0, inputBlock, 0, MSGLen % 32);
						outputBlock[0] = inputBlock[0] ^ state[0][0] ^ state[5] ^ (state[8] & state[12]);
						outputBlock[1] = inputBlock[1] ^ state[0][1] ^ state[6] ^ (state[9] & state[13]);
						outputBlock[2] = inputBlock[2] ^ state[0][2] ^ state[7] ^ (state[10] & state[14]);
						outputBlock[3] = inputBlock[3] ^ state[0][3] ^ state[4] ^ (state[11] & state[15]);
						for (Byte i = 0; i < 32; i++) // timing attack during the IF statement?? analyse to be sure
							if (i < MSGLen % 32)
								bw->Write(Buffer::GetByte(outputBlock, i)); // output the partial block as plaintext
							else
								Buffer::SetByte(outputBlock, i, 0); // recreate the zero whitespace at the end of the plaintext
						MorusCore1280::_updateState(state, (array<const UInt64>^)outputBlock);
						Array::Clear(inputBlock, 0, inputBlock->Length);
						Array::Clear(outputBlock, 0, outputBlock->Length);
						state[0][0] ^= inputBlock[0] | outputBlock[0]; // does nothing (0), but prevents compiler from optimizing out the previous statement
					}
					else
						state[0][0] ^= inputBlock[3] | outputBlock[3]; // does nothing (0), but prevents compiler from optimizing out the Clear statement 
				} // end USING
				bw->Flush();
				bw->Close();
				br->Close();
			} // end USING
			// TAG GENERATION
			MorusCore1280::_finalize_state(state, ADLen, MSGLen); // 1280 128 and 256 do this the same way
			BinaryReader^ tr = gcnew BinaryReader(gcnew MemoryStream((array<Byte>^)Tag, false));
			UInt64 a = tr->ReadUInt64(), b = tr->ReadUInt64();
			UInt64 compare = (a ^ state[0][0]) | (b ^ state[0][1]);
			for (Byte i = 0; i < 5; i++)
			{
				Array::Clear(state[i], 0, state[i]->Length); // zero the row
				state[16] |= (state[i][0] | UInt64::MaxValue) + compare;
			}
			try // try to prevent compiler optimization
			{
				Array::Clear(state, 0, state->Length); // blow away the individual row array references
				state[0] = gcnew array<UInt64>{compare | UInt64::MaxValue};
			}
			catch (...)
			{
				System::Diagnostics::Debug::Print("EXCEPTION!");
			}		
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
						Plaintext[0] = (Byte)(state[0][0] & (UInt64)compare) >> 1;
					}
					//else tempPlaintext = gcnew array<Byte>(1);
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
#endif // !MORUS_MANAGED_CORE_128
