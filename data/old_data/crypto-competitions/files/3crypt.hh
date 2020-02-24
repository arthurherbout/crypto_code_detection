/*
Copyright 2019 (c) Stuart Steven Calder
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once

/* SSC Library Includes */
#include <ssc/general/symbols.hh>	// Useful Macros
#include <ssc/general/arg_mapping.hh>   // Processing Command-Line Arguments
#include <ssc/general/static_string.hh> // Constexpr C-String Functionalities
#include <ssc/general/integers.hh>      // Integer Type Aliases
#include <ssc/crypto/operations.hh>     // Genericized Cryptographic Operations
#include <ssc/crypto/threefish.hh>      // Threefish Tweakable Block Cipher
#include <ssc/crypto/cipherblockchaining.hh>            // Cipher Block Chaining Mode for Threefish
#include <ssc/crypto/skein.hh>          // Skein hash function for its Message Authentication Code Functionality
#include <ssc/crypto/sspkdf.hh>         // SSPKDF Key-Derivation Function
#include <ssc/files/files.hh>           // Genericized File-Related Functions
#include <ssc/files/os_map.hh>          // OS-Abstracted memory-mapping functionalities
#include <ssc/interface/terminal.hh>    // Terminal Interface

/* Platform-Specific Includes */
#if defined(__Unix_Like__)
#	include <sys/types.h> // For Some Types That We Need
#	include <sys/stat.h>  // The stat() Family of Functions
#	include <fcntl.h>     // File Control Options
#	include <sys/mman.h>  // Memory-Mapping Related Functions and Defines
#	include <unistd.h>    // ftruncate() and truncate() Etc. 
#elif defined(_WIN64)
#	include <windows.h>    // Windows Functions, Macros, Etc.
#	include <memoryapi.h>  // Windows Memory-Mapping API
#else
#	error "Currently, only OpenBSD, GNU/Linux, and 64-bit MS Windows are implemented."
#endif

/* Standard Library Includes */
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

namespace threecrypt {
	static_assert (CHAR_BIT == 8);
	static constexpr auto const Salt_Bits   = 128;             // Sufficiently-Big 128-Bit Salt for SSPKDF.
	static constexpr auto const Salt_Bytes  = Salt_Bits / 8;   // Number of SSPKDF Salt Bytes.
	static constexpr auto const Tweak_Bits  = 128;             // Required 128-Bit Tweak for Threefish.
	static constexpr auto const Tweak_Bytes = Tweak_Bits / 8;  // Number of Threefish Tweak Bytes.
	static constexpr auto const Block_Bits  = 512;             // Use the 512-Bit Block Variants for Algorithms Here.
	static constexpr auto const Block_Bytes = Block_Bits / 8;  // Number of Bytes in One Cipher-Block-Chain'd Block; Bytes of State.
	static constexpr auto const MAC_Bytes   = Block_Bytes;     /* Use the same number of bytes of Message Authentication Code
							 	      as is in the block. */
	static constexpr auto const Max_Password_Length = 120;   // 80 as The Longest Legal Password, Arbitrarily.
	static constexpr auto const & Help_String = "Usage: 3crypt Mode [Switches...]\n\n"
						    "Arguments to switches MUST be in seperate words. (i.e. 3crypt -e -i file; NOT 3crypt -e -ifile)\n\n"
						    "Modes:\n"
						    "-e, --encrypt  Symmetric encryption mode; encrypt a file using a passphrase.\n"
						    "-d, --decrypt  Symmetric decryption mode; decrypt a file using a passphrase.\n"
						    "--dump-header  Dump information on a 3crypt encrypted file; must specify an input-file.\n\n"
						    "Switches:\n"
						    "-i, --input-file  Input file ; Must be specified for symmetric encryption and decryption modes.\n"
						    "-o, --output-file Output file; For symmetric encryption and decryption modes. Optional for encryption.\n"
						    "--iter-count      Iteration Count (Default: 1,000,000); Higher takes more time. May only be specified for encryption.\n"
						    "--concat-count    Concatenation Count (Default: 1,000,000); Higher takes more time. May only be specified for encryption.\n";

	static constexpr auto const & Help_Suggestion = "( Use 3crypt --help for more information )\n";

	// Abstractly Define Cryptographic Primitives
	using Threefish_t   = ssc::Threefish<Block_Bits>;				/*Use Threefish<Block_Bits> as Default Block-Cipher in 3crypt.*/
	using Skein_t       = ssc::Skein    <Block_Bits>;				/*Use Skein<Block_Bits> as Default Cryptographic Hash-Function in 3crypt.*/
	using CBC_t         = ssc::CipherBlockChaining<Threefish_t, Block_Bits>;	/*Use Threefish block cipher in Cipher-Block-Chaining Mode.*/
	// Abstractly Define Standard Types
	using Arg_Map_t   = typename ssc::Arg_Mapping::Arg_Map_t;   // Use std::vector<std::pair<std::string, std::string>> to Store Passed-Arguments.
	using namespace ssc::ints;                                  // Import ssc defined integers... like u8_t, u32_t, u64_t, etc.

	/* Structure Describing a File-Header: The Beginning Metadata of
	 * 3crypt-Related files that use SSPKDF as their key-derivation function ( i.e. 3CRYPT_CBC_V2 ).
	 */
	template <size_t ID_Bytes>
	struct SSPKDF_Header {
		char  id          [ID_Bytes];   // i.e. "3CRYPT_CBC_V2"
		u64_t total_size;               // i.e. 1'000'000, for a 3crypt File 1'000'000 Bytes Large
		u8_t  tweak       [Tweak_Bytes];// i.e. (128 / 8) Bytes
		u8_t  sspkdf_salt [Salt_Bytes]; // i.e. (128 / 8) Bytes
		u8_t  cbc_iv      [Block_Bytes];// i.e. (512 / 8) Bytes
		u32_t num_iter;                 // i.e. 1'000'000 Iterations
		u32_t num_concat;               // i.e. 1'000'000 Concatenations
		// Compile-Time-Constant, Describing the Combined Size in Bytes
		static constexpr auto const Total_Size = sizeof(id) + sizeof(total_size) + sizeof(tweak) +
							 sizeof(sspkdf_salt) + sizeof(cbc_iv) + sizeof(num_iter) +
							 sizeof(num_concat);
	};
} /* ! namespace threecrypt */
