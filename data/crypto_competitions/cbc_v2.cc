/*
Copyright (c) 2019 Stuart Steven Calder
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <cstdio>

#include <ssc/general/print.hh>
#include <ssc/general/symbols.hh>
#include <ssc/memory/os_memory_locking.hh>

#include "cbc_v2.hh"
#include "input_abstraction.hh"

#ifdef __OpenBSD__
#	include <unistd.h> // Include so we may use unveil() and pledge().
#endif

namespace threecrypt::cbc_v2 {

	static size_t
	calculate_encrypted_size	(size_t const pre_encryption_size) {
		/* CBC_V2 encrypted files */
		constexpr auto const File_Metadata_Size = CBC_V2_Header_t::Total_Size + MAC_Bytes;
		auto s = pre_encryption_size;
		if ( s < Block_Bytes )
			s = Block_Bytes;
		else
			s += (Block_Bytes - (s % Block_Bytes));
		return s + File_Metadata_Size;
	}

	void
	encrypt	(Input_Abstraction const & input_abstr) {
		using namespace std;

		ssc::OS_Map input_map, output_map;

/* OpenBSD-specific security enhancing systemcalls */
#ifdef __OpenBSD__
		// Allow reading and executing everything under /usr.
		if (unveil( "/usr", "rx" ) != 0) {
			fputs( "Error: Failed to unveil() /usr\n", stderr );
			exit( EXIT_FAILURE );
		}
		// Allow reading the input file.
		if (unveil( input_abstr.input_filename.c_str(), "r" ) != 0) {
			fputs( "Error: Failed to unveil() the input file...\n", stderr );
			exit( EXIT_FAILURE );
		}
		// Allow reading, writing, and creating the output file.
		if (unveil( input_abstr.output_filename.c_str(), "rwc" ) != 0) {
			fputs( "Error: Failed to unveil() the output file...\n", stderr );
			exit( EXIT_FAILURE );
		}
		// Disallow further unveil() calls.
		if (unveil( NULL, NULL ) != 0) {
			fputs( "Error: Failed to finalize unveil()\n", stderr );
		}
		puts( "Successfully restricted filesystem visibility with unveil()" );
#endif

		puts( "Opening input and output files..." );
		// Open input file
		input_map.os_file = ssc::open_existing_os_file( input_abstr.input_filename.c_str(), true );
		// Create output file
		output_map.os_file = ssc::create_os_file( input_abstr.output_filename.c_str() );
		// Determine input file size
		input_map.size = ssc::get_file_size( input_map.os_file );
		// Determine output file size
                output_map.size = calculate_encrypted_size( input_map.size );
		// Extend or shrink the output file to match calculated file size
		puts( "Resizing output file..." );
		ssc::set_os_file_size( output_map.os_file, output_map.size );
		// Memory-Map the files
		puts( "Mapping input file..." );
		ssc::map_file( input_map, true );
		puts( "Mapping output file..." );
		ssc::map_file( output_map, false );
		// Get the password
		constexpr auto const Password_Buffer_Bytes = Max_Password_Length + 1;
		char password [Password_Buffer_Bytes];
		int password_length;
		{
			ssc::Terminal term;
			char pwcheck [Password_Buffer_Bytes];
#ifdef __SSC_memlocking__
			ssc::lock_os_memory( password, Password_Buffer_Bytes );
			ssc::lock_os_memory( pwcheck , Password_Buffer_Bytes );
			puts( "Successfully locked the password buffer(s)..." );
#endif
			bool repeat = true;
			do {
				static_assert (sizeof(password) == Password_Buffer_Bytes);
				static_assert (sizeof(pwcheck)  == Password_Buffer_Bytes);
				memset( password, 0, Password_Buffer_Bytes );
				memset( pwcheck , 0, Password_Buffer_Bytes );
				password_length = term.get_pw( password, Max_Password_Length, 1 );
				static_cast<void>(term.get_pw( pwcheck , Max_Password_Length, 1 ));
				if (memcmp( password, pwcheck, Password_Buffer_Bytes ) == 0)
					repeat = false;
				else
					term.notify( "Passwords don't match." );
			} while (repeat);
			ssc::zero_sensitive( pwcheck, Password_Buffer_Bytes );
#ifdef __SSC_memlocking__
			ssc::unlock_os_memory( pwcheck, Password_Buffer_Bytes );
#endif
		}
		// Create a header
		CBC_V2_Header_t header;
		static_assert (sizeof(header.id) == sizeof(CBC_V2_ID));
		memcpy( header.id, CBC_V2_ID, sizeof(header.id) );
		header.total_size = static_cast<decltype(header.total_size)>(output_map.size);
		puts( "Getting entropy from the OS..." );
		ssc::obtain_os_entropy( header.tweak      , sizeof(header.tweak)       );
		ssc::obtain_os_entropy( header.sspkdf_salt, sizeof(header.sspkdf_salt) );
		ssc::obtain_os_entropy( header.cbc_iv     , sizeof(header.cbc_iv)      );
		header.num_iter   = input_abstr.number_iterations;
		header.num_concat = input_abstr.number_concatenations;
		// Copy header into the file, field at a time, advancing the pointer
		u8_t * out = output_map.ptr;
		{
			puts( "Copying header into the output file..." );
			memcpy( out, header.id, sizeof(header.id) );
			out += sizeof(header.id);

			memcpy( out, &header.total_size, sizeof(header.total_size) );
			out += sizeof(header.total_size);

			memcpy( out, header.tweak, sizeof(header.tweak) );
			out += sizeof(header.tweak);

			memcpy( out, header.sspkdf_salt, sizeof(header.sspkdf_salt) );
			out += sizeof(header.sspkdf_salt);

			memcpy( out, header.cbc_iv, sizeof(header.cbc_iv) );
			out += sizeof(header.cbc_iv);

			memcpy( out, &header.num_iter, sizeof(header.num_iter) );
			out += sizeof(header.num_iter);

			memcpy( out, &header.num_concat, sizeof(header.num_concat) );
			out += sizeof(header.num_concat);
		}

		// Generate a 512-bit symmetric key using the password we got earlier as input
		puts( "Generating symmetric key using sspkdf..." );
		u8_t derived_key [Block_Bytes];
#ifdef __SSC_memlocking__
		ssc::lock_os_memory( derived_key, sizeof(derived_key) );
		puts( "Successfully locked the derived key..." );
#endif
		ssc::sspkdf( derived_key, password, password_length, header.sspkdf_salt, header.num_iter, header.num_concat );
		// Securely zero over the password buffer after we've used it to generate the symmetric key
		ssc::zero_sensitive( password, sizeof(password) );
#ifdef __SSC_memlocking__
		ssc::unlock_os_memory( password, sizeof(password) );
#endif
		{
			// Encrypt the input file, writing the ciphertext into the memory-mapped output file
			puts( "Encrypting..." );
			CBC_t cbc{ Threefish_t{ derived_key, header.tweak } };
			out += cbc.encrypt( input_map.ptr, out, input_map.size, header.cbc_iv );
		}
		{	/* Create a 512-bit Message Authentication Code of the ciphertext, using the derived key and the ciphertext with Skein's native MAC
		 	   then append the MAC to the end of the ciphertext */
			puts( "Generating 512-bit Message Authentication Code..." );
			Skein_t skein;
			skein.message_auth_code( out, output_map.ptr, derived_key, output_map.size - MAC_Bytes, sizeof(derived_key), MAC_Bytes );
		}
		// Securely zero over the derived key
		ssc::zero_sensitive( derived_key, sizeof(derived_key) );
#ifdef __SSC_memlocking__
		ssc::unlock_os_memory( derived_key, sizeof(derived_key) );
#endif
		// Synchronize everything written to the output file
		puts( "Synchronizing output mapping..." );
		ssc::sync_map( output_map );
		// Unmap the input and output files
		puts( "Unmapping files..." );
		ssc::unmap_file( input_map );
		ssc::unmap_file( output_map );
		// Close the input and output files
		puts( "Closing files..." );
		ssc::close_os_file( input_map.os_file );
		ssc::close_os_file( output_map.os_file );
	}/* encrypt */
	void
	decrypt	(char const *__restrict input_filename,
		 char const *__restrict output_filename) {
		using namespace std;

		ssc::OS_Map input_map, output_map;
#ifdef __OpenBSD__
		// Allow reading and executing everything under /usr.
		if (unveil( "/usr", "rx" ) != 0) {
			fputs( "Error: Failed to unveil() /usr\n", stderr );
			exit( EXIT_FAILURE );
		}
		// Allow reading the input file.
		if (unveil( input_filename, "r" ) != 0) {
			fputs( "Error: Failed to unveil() the input file...\n", stderr );
			exit( EXIT_FAILURE );
		}
		// Allow reading, writing, and creating the output file.
		if (unveil( output_filename, "rwc" ) != 0) {
			fputs( "Error: Failed to unveil() the output file...\n", stderr );
			exit( EXIT_FAILURE );
		}
		// Disallow further unveil() calls.
		if (unveil( NULL, NULL ) != 0) {
			fputs( "Error: Failed to finalize unveil()\n", stderr );
		}
		puts( "Successfully restricted filesystem visibility with unveil()" );
#endif
		puts( "Opening input and output files..." );
		input_map.os_file  = ssc::open_existing_os_file( input_filename, true );
		output_map.os_file = ssc::create_os_file( output_filename );
		// Get the size fo the input file
		input_map.size = ssc::get_file_size( input_map.os_file );
		// For now, assume the size of the output file will be the same size as the input file
		output_map.size = input_map.size;
		// Check to see if the input file is too small to have possibly been 3crypt encrypted, using any supported means
		static constexpr auto const Minimum_Possible_File_Size = CBC_V2_Header_t::Total_Size + Block_Bytes + MAC_Bytes;
		puts( "Checking input file size..." );
		if (input_map.size < Minimum_Possible_File_Size) {
			fprintf( stderr, "Error: Input file doesn't appear to be large enough to be a %s encrypted file\n", CBC_V2_ID );
			ssc::close_os_file( input_map.os_file );
			ssc::close_os_file( output_map.os_file );
			remove( output_filename );
			exit( EXIT_FAILURE );
		}
		// Set the output file to be `f_data.output_filesize` bytes
		puts( "Resizing output file..." );
		ssc::set_os_file_size( output_map.os_file, output_map.size );
		// Memory-map the input and output files
		puts( "Mapping input file..." );
		ssc::map_file( input_map, true );
		puts( "Mapping output file..." );
		ssc::map_file( output_map, false );
		// `in` pointer used for reading from the input files, and incremented as it's used to read
		u8_t const * in = input_map.ptr;
		CBC_V2_Header_t header;
		/* Copy all the fields of CBC_V2_Header_t from the memory-mapped file
		   into the header struct */
		{
			puts( "Copying header from the input file into memory..." );
			memcpy( header.id, in, sizeof(header.id) );
			in += sizeof(header.id);

			memcpy( &header.total_size, in, sizeof(header.total_size) );
			in += sizeof(header.total_size);

			memcpy( header.tweak, in, sizeof(header.tweak) );
			in += sizeof(header.tweak);

			memcpy( header.sspkdf_salt, in, sizeof(header.sspkdf_salt) );
			in += sizeof(header.sspkdf_salt);

			memcpy( header.cbc_iv, in, sizeof(header.cbc_iv) );
			in += sizeof(header.cbc_iv);

			memcpy( &header.num_iter, in, sizeof(header.num_iter) );
			in += sizeof(header.num_iter);

			memcpy( &header.num_concat, in, sizeof(header.num_concat) );
			in += sizeof(header.num_concat);
		}
		// Check for the magic "3CRYPT_CBC_V2" at the beginning of the file header
		static_assert (sizeof(header.id) == sizeof(CBC_V2_ID));
		puts( "Checking header ID..." );
		if (memcmp( header.id, CBC_V2_ID, sizeof(CBC_V2_ID) ) != 0) {
			fprintf( stderr, "Error: The input file doesn't appear to be a %s encrypted file.\n", CBC_V2_ID );
			ssc::unmap_file( input_map );
			ssc::unmap_file( output_map );
			ssc::close_os_file( input_map.os_file );
			ssc::close_os_file( output_map.os_file );
			remove( output_filename );
			exit( EXIT_FAILURE );
		}
		// Check that the input file is the same size as specified by the file header
		puts( "Comparing file size with size in the input file header..." );
		if (header.total_size != static_cast<decltype(header.total_size)>(input_map.size)) {
			fprintf( stderr, "Error: Input file size (%zu) does not equal file size in the file header of the input file (%zu)\n",
				 input_map.size, header.total_size );
			ssc::unmap_file( input_map );
			ssc::unmap_file( output_map );
			ssc::close_os_file( input_map.os_file );
			ssc::close_os_file( output_map.os_file );
			remove( output_filename );
			exit( EXIT_FAILURE );
		}
		// Get the password
		char password [Max_Password_Length + 1] = { 0 };
#ifdef __SSC_memlocking__
		ssc::lock_os_memory( password, sizeof(password) );
		puts( "Successfully locked the password buffer(s)..." );
#endif
		int password_length;
		{
			ssc::Terminal term;
			password_length = term.get_pw( password, Max_Password_Length, 1 );
		}
		// Generate a 512-bit symmetric key from the given password
		u8_t derived_key [Block_Bytes];
#ifdef __SSC_memlocking__
		ssc::lock_os_memory( derived_key, sizeof(derived_key) );
		puts( "Successfully locked the derived key..." );
#endif
		puts( "Generating symmetric key using sspkdf..." );
		ssc::sspkdf( derived_key, password, password_length, header.sspkdf_salt, header.num_iter, header.num_concat );
		// Securely zero over the password now that we have the derived key
		ssc::zero_sensitive( password, sizeof(password) );
#ifdef __SSC_memlocking__
		ssc::unlock_os_memory( password, sizeof(password) );
#endif
		{
			// Generate a MAC using the ciphertext and the derived key, and compare it to the MAC at the end of the input file
			u8_t gen_mac [MAC_Bytes];
			{
				puts( "Generating 512-bit Message Authentication Code..." );
				Skein_t skein;
				skein.message_auth_code( gen_mac,
							 input_map.ptr,
							 derived_key,
							 input_map.size - MAC_Bytes,
							 sizeof(derived_key),
							 sizeof(gen_mac) );
			}
			puts( "Comparing generated MAC with the MAC at the end of the input file..." );
			if (memcmp( gen_mac, (input_map.ptr + input_map.size - MAC_Bytes), MAC_Bytes) != 0) {
				ssc::zero_sensitive( derived_key, sizeof(derived_key) );
#ifdef __SSC_memlocking__
				ssc::unlock_os_memory( derived_key, sizeof(derived_key) );
#endif
				fputs( "Error: Authentication failed.\n"
				       "Possibilities: Wrong password, the file is corrupted, or it has been somehow tampered with.\n", stderr );
				ssc::unmap_file( input_map );
				ssc::unmap_file( output_map );
				ssc::close_os_file( input_map.os_file );
				ssc::close_os_file( output_map.os_file );
				remove( output_filename );
				exit( EXIT_FAILURE );
			}
		}
		size_t plaintext_size;
		{
			// Decrypt the input file's ciphertext into the output file, recording the number of bytes of plaintext in `plaintext_size`
			puts( "Decrypting..." );
			CBC_t cbc{ Threefish_t{ derived_key, header.tweak } };
			// Securely zero over the derived key now that we're done with it
			ssc::zero_sensitive( derived_key, sizeof(derived_key) );
#ifdef __SSC_memlocking__
			ssc::unlock_os_memory( derived_key, sizeof(derived_key) );
#endif
			static constexpr auto const File_Metadata_Size = CBC_V2_Header_t::Total_Size + MAC_Bytes;
			plaintext_size = cbc.decrypt( in,
						      output_map.ptr,
						      input_map.size - File_Metadata_Size,
						      header.cbc_iv );
		}
		// Synchronize the output file
		puts( "Synchronizing output mapping..." );
		ssc::sync_map( output_map );
		// Unmap the memory-mapped input and output files
		puts( "Unmapping files..." );
		ssc::unmap_file( input_map );
		ssc::unmap_file( output_map );
		// Truncate the output file to the number of plaintext bytes
		puts( "Truncating output file to the number of plaintext bytes..." );
		ssc::set_os_file_size( output_map.os_file, plaintext_size );
		// Close the input and output files
		puts( "Closing files..." );
		ssc::close_os_file( input_map.os_file );
		ssc::close_os_file( output_map.os_file );
	}/* decrypt */
	
	void
	dump_header	(char const * filename) {
		using std::memcpy, std::fprintf, std::fputs, std::putchar, std::exit;
		using ssc::OS_Map, ssc::OS_File_t;

		OS_Map os_map;
		os_map.os_file = ssc::open_existing_os_file( filename, true );
		os_map.size    = ssc::get_file_size( os_map.os_file );
		static constexpr auto const Minimum_Size = CBC_V2_Header_t::Total_Size + Block_Bytes + MAC_Bytes;
		if (os_map.size < Minimum_Size) {
			ssc::close_os_file( os_map.os_file );
			fprintf( stderr, "File %s looks too small to be CBC_V2 encrypted\n", filename );
			exit( EXIT_FAILURE );
		}
		ssc::map_file( os_map, true );

		CBC_V2_Header_t header;
		u8_t mac [MAC_Bytes];
		{
			u8_t const * p = os_map.ptr;

			memcpy( header.id, p, sizeof(header.id) );
			p += sizeof(header.id);

			memcpy( &header.total_size, p, sizeof(header.total_size) );
			p += sizeof(header.total_size);

			memcpy( header.tweak, p, sizeof(header.tweak) );
			p += sizeof(header.tweak);

			memcpy( header.sspkdf_salt, p, sizeof(header.sspkdf_salt) );
			p += sizeof(header.sspkdf_salt);

			memcpy( header.cbc_iv, p, sizeof(header.cbc_iv) );
			p += sizeof(header.cbc_iv);

			memcpy( &header.num_iter, p, sizeof(header.num_iter) );
			p += sizeof(header.num_iter);

			memcpy( &header.num_concat, p, sizeof(header.num_concat) );

			p = os_map.ptr + os_map.size - MAC_Bytes;
			memcpy( mac, p, sizeof(mac) );
		}
		ssc::unmap_file( os_map );
		ssc::close_os_file( os_map.os_file );

		fprintf( stdout,   "File Header ID             : %s\n", header.id );
		fprintf( stdout,   "File Size                  : %zu\n", header.total_size );
		fputs  (           "Threefish Tweak            : ", stdout );
		ssc::print_integral_buffer<u8_t>( header.tweak, sizeof(header.tweak) );
		fputs  (         "\nSSPKDF Salt                : ", stdout );
		ssc::print_integral_buffer<u8_t>( header.sspkdf_salt, sizeof(header.sspkdf_salt) );
		fputs  (         "\nCBC Initialization Vector  : ", stdout );
		ssc::print_integral_buffer<u8_t>( header.cbc_iv, sizeof(header.cbc_iv) );
		fprintf( stdout, "\nNumber Iterations          : %u\n", header.num_iter );
		fprintf( stdout,   "Number Concatenations      : %u\n", header.num_concat );
		fputs(             "Message Authentication Code: ", stdout );
		ssc::print_integral_buffer<u8_t>( mac, sizeof(mac) );
		std::putchar( '\n' );
	}/* ! dump_header */
} /* ! namespace threecrypt::cbc_v2 */
