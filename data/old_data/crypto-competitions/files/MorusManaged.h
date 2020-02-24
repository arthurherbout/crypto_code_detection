#pragma once

#ifndef MORUS_MANAGED_H
#define MORUS_MANAGED_H

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;

namespace MorusManaged {

	public ref class Morus_640_128
	{
	public:
		static int Encrypt(array<const Byte>^ AdditionalData, array<const Byte>^ Plaintext,
			array<const Byte>^ Key, array<const Byte>^ Nonce, [Out] array<Byte>^% Ciphertext);
		static int Decrypt(array<const Byte>^ AdditionalData, array<const Byte>^ Ciphertext,
			array<const Byte>^ Key, array<const Byte>^ Nonce, [Out] array<Byte>^% Plaintext);
		static int Encrypt_Detached(array<const Byte>^ AdditionalData, array<const Byte>^ Plaintext,
			array<const Byte>^ Key, array<const Byte>^ Nonce, [Out] array<Byte>^% Ciphertext, [Out] array<Byte>^% Tag);
		static int Decrypt_Detached(array<const Byte>^ AdditionalData, array<const Byte>^ Ciphertext, array<const Byte>^ Tag,
			array<const Byte>^ Key, array<const Byte>^ Nonce, [Out] array<Byte>^% Plaintext);
		static int EncryptStream(MemoryStream^ AdditionalData, MemoryStream^ Plaintext,
			array<const Byte>^ Key, array<const Byte>^ Nonce, MemoryStream^ Ciphertext);
		static int DecryptStream(MemoryStream^ AdditionalData, MemoryStream^ Ciphertext,
			array<const Byte>^ Key, array<const Byte>^ Nonce, MemoryStream^ Plaintext);
	};
	
	public ref class Morus_1280_128
	{
	public:
		static int Encrypt(array<const Byte>^ AdditionalData, array<const Byte>^ Plaintext,
			array<const Byte>^ Key, array<const Byte>^ Nonce, [Out] array<Byte>^% Ciphertext);
		static int Decrypt(array<const Byte>^ AdditionalData, array<const Byte>^ Ciphertext,
			array<const Byte>^ Key, array<const Byte>^ Nonce, [Out] array<Byte>^% Plaintext);
		static int Encrypt_Detached(array<const Byte>^ AdditionalData, array<const Byte>^ Plaintext,
			array<const Byte>^ Key, array<const Byte>^ Nonce, [Out] array<Byte>^% Ciphertext, [Out] array<Byte>^% Tag);
		static int Decrypt_Detached(array<const Byte>^ AdditionalData, array<const Byte>^ Ciphertext, array<const Byte>^ Tag,
			array<const Byte>^ Key, array<const Byte>^ Nonce, [Out] array<Byte>^% Plaintext);
		static int EncryptStream(MemoryStream^ AdditionalData, MemoryStream^ Plaintext,
			array<const Byte>^ Key, array<const Byte>^ Nonce, MemoryStream^ Ciphertext);
		static int DecryptStream(MemoryStream^ AdditionalData, MemoryStream^ Ciphertext,
			array<const Byte>^ Key, array<const Byte>^ Nonce, MemoryStream^ Plaintext);
	};

	public ref class Morus_1280_256
	{
	public:
		static int Encrypt(array<const Byte>^ AdditionalData, array<const Byte>^ Plaintext,
			array<const Byte>^ Key, array<const Byte>^ Nonce, [Out] array<Byte>^% Ciphertext);
		static int Decrypt(array<const Byte>^ AdditionalData, array<const Byte>^ Ciphertext,
			array<const Byte>^ Key, array<const Byte>^ Nonce, [Out] array<Byte>^% Plaintext);
		static int Encrypt_Detached(array<const Byte>^ AdditionalData, array<const Byte>^ Plaintext,
			array<const Byte>^ Key, array<const Byte>^ Nonce, [Out] array<Byte>^% Ciphertext, [Out] array<Byte>^% Tag);
		static int Decrypt_Detached(array<const Byte>^ AdditionalData, array<const Byte>^ Ciphertext, array<const Byte>^ Tag,
			array<const Byte>^ Key, array<const Byte>^ Nonce, [Out] array<Byte>^% Plaintext);
		static int EncryptStream(MemoryStream^ AdditionalData, MemoryStream^ Plaintext,
			array<const Byte>^ Key, array<const Byte>^ Nonce, MemoryStream^ Ciphertext);
		static int DecryptStream(MemoryStream^ AdditionalData, MemoryStream^ Ciphertext,
			array<const Byte>^ Key, array<const Byte>^ Nonce, MemoryStream^ Plaintext);
	};
}

#endif // !MORUS_MANAGED_H
