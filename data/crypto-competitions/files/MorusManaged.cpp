/* ====================================================================================== 
* From the source morusv2.pdf:
* 
* The Authenticated Cipher MORUS (v2)
* 15 Sep, 2016
* Designers: Hongjun Wu, Tao Huang
* Submitters: Hongjun Wu, Tao Huang
* Division of Mathematical Sciences
* Nanyang Technological University, Singapore
* 
* Section 9:
* "MORUS is not patented and it is free of intellectual property restrictions."
* ========================================================================================
* This implementation taken direct (and modified for .Net C++/CLR) from the reference C
* implemention by the authors, and is released into the Public Domain under CC(0) License.
* You should have received a copy of the CC0 Public Domain Dedication along with
* this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
* Dustin Sparks, March 2019
* ========================================================================================
*/

#pragma once

#ifndef MORUS_MANAGED_CPP
#define MORUS_MANAGED_CPP

#include "MorusManaged.h"
#include "MorusCore640.cpp"
#include "MorusCore1280.cpp"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace MorusManaged
{
	int MorusManaged::Morus_640_128::Encrypt(
		array<const Byte>^ AdditionalData,
		array<const Byte>^ Plaintext,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		[Out] array<Byte>^% Ciphertext)
	{
		//TODO Gaurd clauses!
		if (Key == nullptr || Key->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 16 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		MemoryStream^ AD = AdditionalData != nullptr ? gcnew MemoryStream((array<Byte>^)AdditionalData, false) : nullptr;
		MemoryStream^ P = Plaintext != nullptr ? gcnew MemoryStream((array<Byte>^)Plaintext, false) : nullptr;
		MemoryStream^ tempCiphertext = gcnew MemoryStream(P != nullptr ? Plaintext->Length + 16 : 16);
		int result = MorusCore640::_encryptStream(AD, P, Key, Nonce, tempCiphertext, tempCiphertext);
		if (result == 0) Ciphertext = tempCiphertext->ToArray();
		return result;
	}

	int MorusManaged::Morus_640_128::Decrypt(
		array<const Byte>^ AdditionalData,
		array<const Byte>^ Ciphertext,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		[Out] array<Byte>^% Plaintext)
	{
		//TODO Gaurd clauses!
		if (Key == nullptr || Key->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 16 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		if (Ciphertext == nullptr || Ciphertext->Length < 16)
			throw gcnew ArgumentOutOfRangeException("Ciphertext", "Ciphertext must at least contain the 16 byte tag and cannot be null.");
		int apparentLength = Ciphertext->Length - 16;
		MemoryStream^ AD = AdditionalData != nullptr ? gcnew MemoryStream((array<Byte>^)AdditionalData, false) : nullptr;
		MemoryStream^ C = gcnew MemoryStream((array<Byte>^)Ciphertext, false);
		MemoryStream^ tempPlaintext = gcnew MemoryStream(apparentLength);
		int result = MorusCore640::_decryptStream(AD, C, nullptr, Key, Nonce, tempPlaintext);
		if (result == -1) tempPlaintext = nullptr; // already wiped within _decrypt
		Plaintext = tempPlaintext->ToArray(); // do the assignment method to prevent leakage of partial plaintext before verification!
		return result;
	}

	int MorusManaged::Morus_640_128::Encrypt_Detached(
		array<const Byte>^ AdditionalData,
		array<const Byte>^ Plaintext,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		[Out] array<Byte>^% Ciphertext,
		[Out] array<Byte>^% Tag)
	{
		//TODO Gaurd clauses!
		if (Key == nullptr || Key->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 16 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		MemoryStream^ AD = AdditionalData != nullptr ? gcnew MemoryStream((array<Byte>^)AdditionalData, false) : nullptr;
		MemoryStream^ P = Plaintext != nullptr ? gcnew MemoryStream((array<Byte>^)Plaintext, false) : nullptr;
		MemoryStream^ tempCiphertext = gcnew MemoryStream(P != nullptr ? Plaintext->Length : 0);
		MemoryStream^ tempTag = gcnew MemoryStream(16);
		int result = MorusCore640::_encryptStream(AD, P, Key, Nonce, tempCiphertext, tempTag);
		if (result == 0)
		{
			Ciphertext = tempCiphertext->ToArray();
			Tag = tempTag->ToArray();
		}
		return result;
	}

	int MorusManaged::Morus_640_128::Decrypt_Detached(
		array<const Byte>^ AdditionalData,
		array<const Byte>^ Ciphertext,
		array<const Byte>^ Tag,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		[Out] array<Byte>^% Plaintext)
	{
		//TODO Gaurd clauses!
		if (Key == nullptr || Key->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 16 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		if (Tag == nullptr || Tag->Length < 16)
			throw gcnew ArgumentOutOfRangeException("Tag", "Must at least contain the 16-byte tag (cannot be null).");
		MemoryStream^ AD = AdditionalData != nullptr ? gcnew MemoryStream((array<Byte>^)AdditionalData, false) : nullptr;
		MemoryStream^ C = Ciphertext != nullptr ? gcnew MemoryStream((array<Byte>^)Ciphertext, false) : nullptr;
		MemoryStream^ T = gcnew MemoryStream((array<Byte>^)Tag, false);
		MemoryStream^ tempPlaintext = gcnew MemoryStream(C != nullptr ? Ciphertext->Length : 0);
		int result = MorusCore640::_decryptStream(AD, C, T, Key, Nonce, tempPlaintext);
		if (result == 0) Plaintext = tempPlaintext->ToArray(); // do the assignment method to prevent leakage of partial plaintext before verification!
		return result;
	}

	int MorusManaged::Morus_640_128::EncryptStream(
		MemoryStream^ AdditionalData,
		MemoryStream^ Plaintext,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		MemoryStream^ Ciphertext)
	{
		//TODO  Gaurd clauses!
		if (Key == nullptr || Key->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 16 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		if (!Ciphertext->CanWrite)
			throw gcnew InvalidOperationException("Ciphertext MemoryStream is READ ONLY. Operation aborted.");
		return MorusCore640::_encryptStream(AdditionalData, Plaintext, Key, Nonce, Ciphertext, Ciphertext); // write tag to same stream
	}

	int MorusManaged::Morus_640_128::DecryptStream(
		MemoryStream^ AdditionalData,
		MemoryStream^ Ciphertext,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		MemoryStream^ Plaintext)
	{
		//TODO Gaurd clauses!
		if (Key == nullptr || Key->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 16 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		if (Ciphertext == nullptr || Ciphertext->Length < 16)
			throw gcnew ArgumentOutOfRangeException("Ciphertext", "Ciphertext Stream must at least contain the 16 byte tag and cannot be null.");
		if (!Plaintext->CanWrite)
			throw gcnew InvalidOperationException("Plaintext MemoryStream is READ ONLY. Operation aborted.");
		return MorusCore640::_decryptStream(AdditionalData, Ciphertext, nullptr, Key, Nonce, Plaintext); // read tag from same stream
	}

	// the only different between 1280128 and 1280256 are the Guard Clauses (and then in only one regard), and the initialization
	// (which is handled in that function, otherwise the two are identical!)
	int MorusManaged::Morus_1280_128::Encrypt(
		array<const Byte>^ AdditionalData,
		array<const Byte>^ Plaintext,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		[Out] array<Byte>^% Ciphertext)
	{
		//TODO Gaurd clauses!
		if (Key == nullptr || Key->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 16 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		MemoryStream^ AD = AdditionalData != nullptr ? gcnew MemoryStream((array<Byte>^)AdditionalData, false) : nullptr;
		MemoryStream^ P = Plaintext != nullptr ? gcnew MemoryStream((array<Byte>^)Plaintext, false) : nullptr;
		MemoryStream^ tempCiphertext = gcnew MemoryStream(P != nullptr ? Plaintext->Length + 16 : 16);
		int result = MorusCore1280::_encryptStream(AD, P, Key, Nonce, tempCiphertext, tempCiphertext);
		if (result == 0) Ciphertext = tempCiphertext->ToArray();
		return result;
	}
	   
	int MorusManaged::Morus_1280_128::Decrypt(
		array<const Byte>^ AdditionalData,
		array<const Byte>^ Ciphertext,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		[Out] array<Byte>^% Plaintext)
	{
		//TODO Gaurd clauses!
		if (Key == nullptr || Key->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 16 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		if (Ciphertext == nullptr || Ciphertext->Length < 16)
			throw gcnew ArgumentOutOfRangeException("Ciphertext", "Ciphertext must at least contain the 16 byte tag and cannot be null.");
		MemoryStream^ AD = AdditionalData != nullptr ? gcnew MemoryStream((array<Byte>^)AdditionalData, false) : nullptr;
		MemoryStream^ C = gcnew MemoryStream((array<Byte>^)Ciphertext, false);
		MemoryStream^ tempPlaintext = gcnew MemoryStream(Ciphertext->Length - 16);
		int result = MorusCore1280::_decryptStream(AD, C, nullptr, Key, Nonce, tempPlaintext);
		if (result == 0) Plaintext = tempPlaintext->ToArray();
		return result;
	}

	int MorusManaged::Morus_1280_128::Encrypt_Detached(
		array<const Byte>^ AdditionalData,
		array<const Byte>^ Plaintext,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		[Out] array<Byte>^% Ciphertext,
		[Out] array<Byte>^% Tag)
	{
		//TODO Gaurd clauses!
		if (Key == nullptr || Key->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 16 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		MemoryStream^ AD = AdditionalData != nullptr ? gcnew MemoryStream((array<Byte>^)AdditionalData, false) : nullptr;
		MemoryStream^ P = Plaintext != nullptr ? gcnew MemoryStream((array<Byte>^)Plaintext, false) : nullptr;
		MemoryStream^ tempCiphertext = gcnew MemoryStream(P != nullptr ? Plaintext->Length : 0);
		MemoryStream^ tempTag = gcnew MemoryStream(16);
		int result = MorusCore1280::_encryptStream(AD, P, Key, Nonce, tempCiphertext, tempTag);
		if (result == 0)
		{
			Ciphertext = tempCiphertext->ToArray();
			Tag = tempTag->ToArray();
		}
		return result;
	}

	int MorusManaged::Morus_1280_128::Decrypt_Detached(
		array<const Byte>^ AdditionalData,
		array<const Byte>^ Ciphertext,
		array<const Byte>^ Tag,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		[Out] array<Byte>^% Plaintext)
	{
		//TODO Gaurd clauses!
		if (Key == nullptr || Key->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 16 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		if (Tag == nullptr || Tag->Length < 16)
			throw gcnew ArgumentOutOfRangeException("Tag", "Must at least contain the 16-byte tag (cannot be null).");
		MemoryStream^ AD = AdditionalData != nullptr ? gcnew MemoryStream((array<Byte>^)AdditionalData, false) : nullptr;
		MemoryStream^ C = Ciphertext != nullptr ? gcnew MemoryStream((array<Byte>^)Ciphertext, false) : nullptr;
		MemoryStream^ T = gcnew MemoryStream((array<Byte>^)Tag, false);
		MemoryStream^ tempPlaintext = gcnew MemoryStream(C != nullptr ? C->Length : 0);
		int result = MorusCore1280::_decryptStream(AD, C, T, Key, Nonce, tempPlaintext);
		if (result == 0) Plaintext = tempPlaintext->ToArray(); // do the assignment method to prevent leakage of partial plaintext before verification!
		return result;
	}

	int MorusManaged::Morus_1280_128::EncryptStream(
		MemoryStream^ AdditionalData,
		MemoryStream^ Plaintext,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		MemoryStream^ Ciphertext)
	{
		//TODO  Gaurd clauses!
		if (Key == nullptr || Key->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 16 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		if (!Ciphertext->CanWrite)
			throw gcnew InvalidOperationException("Ciphertext MemoryStream is READ ONLY. Operation aborted.");
		return MorusCore1280::_encryptStream(AdditionalData, Plaintext, Key, Nonce, Ciphertext, Ciphertext); // write tag to same stream
	}

	int MorusManaged::Morus_1280_128::DecryptStream(
		MemoryStream^ AdditionalData,
		MemoryStream^ Ciphertext,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		MemoryStream^ Plaintext)
	{
		//TODO Gaurd clauses!
		if (Key == nullptr || Key->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 16 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		if (Ciphertext == nullptr || Ciphertext->Length < 16)
			throw gcnew ArgumentOutOfRangeException("Ciphertext", "Ciphertext Stream must at least contain the 16 byte tag and cannot be null.");
		if (!Plaintext->CanWrite)
			throw gcnew InvalidOperationException("Plaintext MemoryStream is READ ONLY. Operation aborted.");
		return MorusCore1280::_decryptStream(AdditionalData, Ciphertext, nullptr, Key, Nonce, Plaintext); // read tag from same stream
	}

	// 1280_256

	int MorusManaged::Morus_1280_256::Encrypt(
		array<const Byte>^ AdditionalData,
		array<const Byte>^ Plaintext,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		[Out] array<Byte>^% Ciphertext)
	{
		//TODO  Gaurd clauses!
		if (Key == nullptr || Key->Length != 32)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 32 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		MemoryStream^ AD = AdditionalData != nullptr ? gcnew MemoryStream((array<Byte>^)AdditionalData, false) : nullptr;
		MemoryStream^ P = Plaintext != nullptr ? gcnew MemoryStream((array<Byte>^)Plaintext, false) : nullptr;
		MemoryStream^ tempCiphertext = gcnew MemoryStream(P != nullptr ? Plaintext->Length : 16);
		int result = MorusCore1280::_encryptStream(AD, P, Key, Nonce, tempCiphertext, tempCiphertext);
		if (result == 0) Ciphertext = tempCiphertext->ToArray();
		return result;
	}

	int MorusManaged::Morus_1280_256::Decrypt(
		array<const Byte>^ AdditionalData,
		array<const Byte>^ Ciphertext,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		[Out] array<Byte>^% Plaintext)
	{
		//TODO Gaurd clauses!
		if (Key == nullptr || Key->Length != 32)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 32 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		if (Ciphertext == nullptr || Ciphertext->Length < 16)
			throw gcnew ArgumentOutOfRangeException("Ciphertext", "Ciphertext must at least contain the 16 byte tag and cannot be null.");
		MemoryStream^ AD = AdditionalData != nullptr ? gcnew MemoryStream((array<Byte>^)AdditionalData, false) : nullptr;
		MemoryStream^ C = gcnew MemoryStream((array<Byte>^)Ciphertext, false);
		MemoryStream^ tempPlaintext = gcnew MemoryStream(C->Length - 16);
		int result = MorusCore1280::_decryptStream(AD, C, nullptr, Key, Nonce, tempPlaintext);
		if (result == 0) Plaintext = tempPlaintext->ToArray();
		return result;
	}

	int MorusManaged::Morus_1280_256::Encrypt_Detached(
		array<const Byte>^ AdditionalData,
		array<const Byte>^ Plaintext,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		[Out] array<Byte>^% Ciphertext,
		[Out] array<Byte>^% Tag)
	{
		//TODO Gaurd clauses!
		if (Key == nullptr || Key->Length != 32)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 32 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		MemoryStream^ AD = AdditionalData != nullptr ? gcnew MemoryStream((array<Byte>^)AdditionalData, false) : nullptr;
		MemoryStream^ P = Plaintext != nullptr ? gcnew MemoryStream((array<Byte>^)Plaintext, false) : nullptr;
		MemoryStream^ tempCiphertext = gcnew MemoryStream(P != nullptr ? Plaintext->Length : 0);
		MemoryStream^ tempTag = gcnew MemoryStream(16);
		int result = MorusCore1280::_encryptStream(AD, P, Key, Nonce, tempCiphertext, tempTag);
		if (result == 0)
		{
			Ciphertext = tempCiphertext->ToArray();
			Tag = tempTag->ToArray();
		}
		return result;
	}

	int MorusManaged::Morus_1280_256::Decrypt_Detached(
		array<const Byte>^ AdditionalData,
		array<const Byte>^ Ciphertext,
		array<const Byte>^ Tag,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		[Out] array<Byte>^% Plaintext)
	{
		//TODO Gaurd clauses!
		if (Key == nullptr || Key->Length != 32)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 32 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		if (Tag == nullptr || Tag->Length < 16)
			throw gcnew ArgumentOutOfRangeException("Tag", "Must at least contain the 16-byte tag (cannot be null).");
		MemoryStream^ AD = AdditionalData != nullptr ? gcnew MemoryStream((array<Byte>^)AdditionalData, false) : nullptr;
		MemoryStream^ C = Ciphertext != nullptr ? gcnew MemoryStream((array<Byte>^)Ciphertext, false) : nullptr;
		MemoryStream^ T = gcnew MemoryStream((array<Byte>^)Tag, false);
		MemoryStream^ tempPlaintext = gcnew MemoryStream(C != nullptr ? C->Length : 0);
		int result = MorusCore1280::_decryptStream(AD, C, T, Key, Nonce, tempPlaintext);
		if (result == 0) Plaintext = tempPlaintext->ToArray(); // do the assignment method to prevent leakage of partial plaintext before verification!
		return result;
	}

	int MorusManaged::Morus_1280_256::EncryptStream(
		MemoryStream^ AdditionalData,
		MemoryStream^ Plaintext,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		MemoryStream^ Ciphertext)
	{
		//TODO  Gaurd clauses!
		if (Key == nullptr || Key->Length != 32)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 32 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		if (!Ciphertext->CanWrite)
			throw gcnew InvalidOperationException("Ciphertext MemoryStream is READ ONLY. Operation aborted.");
		return MorusCore1280::_encryptStream(AdditionalData, Plaintext, Key, Nonce, Ciphertext, Ciphertext); // write tag to same stream
	}

	int MorusManaged::Morus_1280_256::DecryptStream(
		MemoryStream^ AdditionalData,
		MemoryStream^ Ciphertext,
		array<const Byte>^ Key,
		array<const Byte>^ Nonce,
		MemoryStream^ Plaintext)
	{
		//TODO Gaurd clauses!
		if (Key == nullptr || Key->Length != 32)
			throw gcnew ArgumentOutOfRangeException("Key", "Key must not be null and be 32 bytes in length.");
		if (Nonce == nullptr || Nonce->Length != 16)
			throw gcnew ArgumentOutOfRangeException("Nonce", "Nonce must not be null and be 16 bytes in length.");
		if (Ciphertext == nullptr || Ciphertext->Length < 16)
			throw gcnew ArgumentOutOfRangeException("Ciphertext", "Ciphertext Stream must at least contain the 16 byte tag and cannot be null.");
		if (!Plaintext->CanWrite)
			throw gcnew InvalidOperationException("Plaintext MemoryStream is READ ONLY. Operation aborted.");
		return MorusCore1280::_decryptStream(AdditionalData, Ciphertext, nullptr, Key, Nonce, Plaintext); // read tag from same stream
	}
}

#endif // !MORUS_MANAGED_CPP