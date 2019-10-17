/*
Copyright (c) 2019 Stuart Steven Calder
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "determine_decrypt_method.hh"

namespace threecrypt {
	/* This function looks at the "magic bytes" at the beginning of a file,
	*  and tries to determine which 3crypt format it was encrypted with, if any.
	*/
	Decryption_Method_e
	determine_decrypt_method	(char const * filename) {
		// Get the size of the file.
		auto const file_size = ssc::get_file_size( filename );
		// Assume that the file is not a valid 3crypt encrypted file to start.
		auto method = Decryption_Method_e::None;

		// Discard files that are smaller than the smallest identifier string.
		if (file_size < Smallest_Identifier_String_Size) {
			std::fprintf( stderr, "Error: The file `%s` is too small to be a 3crypt encrypted file.\n", filename );
			std::exit( EXIT_FAILURE );
		}
		{
			// Open the file
			std::FILE * file_ptr;
			if ((file_ptr = std::fopen( filename, "rb" )) == nullptr) {
				std::fprintf( stderr, "Error: Failed to open `%s` to determine its decryption method.\n", filename );
				std::exit( EXIT_FAILURE );
			}
			// Create a buffer that can fit the biggest possible identifier string.
			u8_t buffer [Biggest_Identifier_String_Size];
			/* If the file is bigger than the biggest possible identifier string, read in Biggest_Identifier_String_Size bytes,
			* otherwise, read in the entire file to check it.
			*/
			size_t const bytes_to_read = (file_size > Biggest_Identifier_String_Size) ? Biggest_Identifier_String_Size : file_size;
			size_t const read_bytes = std::fread( buffer, 1, bytes_to_read, file_ptr );
			// Ensure that all the requested bytes were read in.
			if (read_bytes != bytes_to_read) {
				std::fprintf( stderr, "Error: Failed to read bytes to determine decryption method: (%zu) requsted, (%zu) read.\n",
					      bytes_to_read, read_bytes );
				std::exit( EXIT_FAILURE );
			}
			// If the CBC_V2 header was included, check to see if the file is a CBC_V2 encrypted file.
#ifdef CBC_V2_HH
			{
				using namespace cbc_v2;
				static_assert (sizeof(CBC_V2_Header_t::id) == sizeof(CBC_V2_ID));
				if (method == Decryption_Method_e::None &&
				    memcmp( buffer, CBC_V2_ID, sizeof(CBC_V2_ID) ) == 0)
				{
					method = Decryption_Method_e::CBC_V2;
				}
			}
#endif
			// Close the file.
			if (std::fclose( file_ptr ) != 0) {
				std::fprintf( stderr, "Error: Failed to close `%s` after checking for its decryption method.\n", filename );
				std::exit( EXIT_FAILURE );
			}
		}
		// Return the detected needed decryption method.
		return method;
	}/* determine_decrypt_method */
} /* ! namespace threecrypt */
