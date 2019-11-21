/*
Copyright 2019 (c) Stuart Steven Calder
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef DETERMINE_DECRYPT_METHOD_HH
#define DETERMINE_DECRYPT_METHOD_HH

#include <vector>

#include <ssc/files/files.hh>
#include <ssc/general/static_string.hh>

#include "3crypt.hh"
#include "cbc_v2.hh"

namespace threecrypt {
	static constexpr size_t
	determine_biggest_identifier_string_size (void) {
		size_t s = 0;
#ifdef CBC_V2_HH
		if ( sizeof(cbc_v2::CBC_V2_ID) > s )
			s = sizeof(cbc_v2::CBC_V2_ID);
#endif
		return s;
	}/* ! threecrypt::determine_biggest_identifier_string_size() */
	constexpr size_t const Biggest_Identifier_String_Size = determine_biggest_identifier_string_size();

	static constexpr size_t
	determine_smallest_identifier_string_size (void) {
		size_t s = Biggest_Identifier_String_Size;
#ifdef CBC_V2_HH
		if ( sizeof(cbc_v2::CBC_V2_ID) < s )
			s = sizeof(cbc_v2::CBC_V2_ID);
#endif
		return s;
	}/* ! threecrypt::determine_smallest_identifier_string_size() */
	constexpr size_t const Smallest_Identifier_String_Size = determine_smallest_identifier_string_size();

	enum class Decryption_Method_e {
		None,
#ifdef CBC_V2_HH
		CBC_V2,
#endif
		Terminating_Enum
	};
	Decryption_Method_e determine_decrypt_method(char const * filename);
}/* ! namespace threecrypt */

#endif
