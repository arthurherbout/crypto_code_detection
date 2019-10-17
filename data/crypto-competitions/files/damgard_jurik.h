/*
	libdj - A library implementing the Damgård-Jurik cryptosystem.

	Copyright 2012 Fred Douglas (fed2@illinois.edu)
	This library is an extension/modification of libpaillier.
	libpaillier is copyright 2006 SRI International (written by John Bethencourt).

	This file is part of libdj.

    libdj is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libdj is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libdj.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
	Include gmp.h before including this file.
*/

/*
	IMPORTANT SECURITY NOTES:

	Strong primes:

  libpaillier, which this library is based on, doesn't ensure that the
  primes it generates has any special properties, since modern factoring
  algorithms (GNFS) aren't affected by the use of strong primes. This
  library, however, ensures that (p-1)/2 is prime, as this property is
  required for correct operation of the Damgård-Jurik cryptosystem.

  On memory handling: (same as in libpaillier)

  At no point is any special effort made to securely "shred" sensitive
  memory or prevent it from being paged out to disk. This means that
  it is important that functions dealing with private keys and
  plaintexts (e.g., dj_keygen and dj_encrypt) only be run on
  trusted machines. The resulting ciphertexts and public keys,
  however, may of course be handled in an untrusted manner.
  
*/

/*
A note on what libdj lacks:

The threshold version of Damgård-Jurik provides for partial private key
holders generating a NIZK proof that they computed the decryption share
correctly. This implementation doesn't have that. Adding it should be a
pretty reasonable task.

It's also supposed to be possible to generate the threshold keys in a
distributed manner, so that no one ever sees all of the keys at once.
When I looked into this, the information necessary for doing this seemed
to be distributed across 2 papers in addition to the D-J paper. I am
pretty sure that trying to add this would be many many times more work
than what it took to produce this library.
*/


/******
 TYPES
*******/

/*
The structures this code uses. Unless you're doing something moderately
fancy, they'll always be dynamically allocated by the functions that
fill them (the keygen functions for the key structs, the encryption
function for the ciphertext struct, etc.).

Use the provided functions (at the bottom) to free them; they free
their contents as well.
*/


typedef struct
{
	int bits;  /* e.g., 1024 */
	unsigned int s;
	mpz_t* npow;
	mpz_t n_plusone;
} dj_pubkey_t;

typedef struct
{
	unsigned short int i;
	unsigned short int numShares;
	unsigned short int threshold;
	mpz_t s_i;
	mpz_t delta;
} dj_partial_prvkey_t;

typedef struct
{
	mpz_t key;
} dj_prvkey_t;

typedef struct
{
	unsigned short int i;
	unsigned short int numShares;
	unsigned short int threshold;
	mpz_t c_i;
	mpz_t delta;
} dj_decshare_t;

typedef struct
{
	mpz_t m;
} dj_plaintext_t;

typedef struct
{
	mpz_t c;
} dj_ciphertext_t;














/***********
 MISC STUFF
***********/

/*
	These functions may be passed to the dj_keygen,
	dj_thresh_centralized_keygen, and dj_encrypt functions to provide
	a source of random numbers. The	first reads bytes from /dev/random.
	On Linux, this device exclusively returns entropy gathered from
	environmental noise and therefore frequently blocks when not enough
	is available. The second returns bytes from /dev/urandom. On Linux,
	this device also returns environmental noise, but augments it with
	a pseudo-random number generator when not enough is available. The
	latter is probably the better choice unless you have a specific
	reason to believe it is	insufficient.
*/
void dj_get_rand_devrandom(  void* buf, int len );
void dj_get_rand_devurandom( void* buf, int len );

/*
  This is the type of the callback functions used to obtain the
  randomness needed by the probabilistic algorithms. The functions
  dj_get_rand_devrandom and dj_get_rand_devurandom
  (documented later) may be passed to any library function requiring a
  dj_get_rand_t, or you may implement your own. If you implement
  your own such function, it should fill in "len" random bytes in the
  array "buf".
*/
typedef void (*dj_get_rand_t) ( void* buf, int len );











/********
FUNCTIONS
*********/


/*NOTE:

dj_keygen, dj_decrypt, and dj_freeprvkey are used only for non-threshold keys.

dj_thresh_centralized_keygen, dj_combine_shares, dj_partial_decrypt,
dj_free_partial_prvkey, and dj_free_decshare are used only for threshold keys.

All others can be used with both types.
*/





/*
Generates non-threshold public/private key pair.


returns: a pointer to the resulting private key


-------------------------------------------------------------------------------
arguments:
reskey: the location to store the result; a new dj_prvkey_t will be
		allocated if this is null
		
resPub: a pointer to a pointer that will have the location of the
		created public key written into it

s: 	the D-J system's 's' parameter
	(for normal usage - not nesting encryptions, and in general not encrypting
	values larger than the public key's n - set s=1.)

modulusbits: the number of bits the public key's n should have

get_rand: You most likely want to just pass dj_get_rand_devurandom
-------------------------------------------------------------------------------
*/
dj_prvkey_t* dj_keygen(dj_prvkey_t* reskey,
					   dj_pubkey_t** resPub,
					   unsigned short int s,
					   int modulusbits,
					   dj_get_rand_t get_rand);








/*
Generates a public key and threshold private keyshares.


-------------------------------------------------------------------------------
arguments:
resPrvs: an array (should be allocated) of pointers (shouldn't be allocated)
		 that will have the resulting keyshares' locations written into them

resPub: a pointer to a pointer that will have the location of the created
		public key written into it

threshold:  the D-J system's threshold parameter; at least this many decryption
			shares will need to be combined to get the message

numKeyshares: how many keyshares to generate

s: 	what value of the D-J system's 's' parameter this key should start with
	(for normal usage - not nesting encryptions, and in general not encrypting
	values larger than the public key's n - set s=1.)

modulusbits: the number of bits the public key's n should have

get_rand: You most likely want to just pass dj_get_rand_devurandom
-------------------------------------------------------------------------------
*/
void dj_thresh_centralized_keygen(	dj_partial_prvkey_t** resPrvs,
									dj_pubkey_t** resPub,
									unsigned short int threshold,
									unsigned short int numKeyshares,
									unsigned short int s,
									int modulusbits,
									dj_get_rand_t get_rand);







/*
Creates a dj_plaintext_t with message value i.


returns: a pointer to the resulting plaintext


-------------------------------------------------------------------------------
arguments:
i: the value to use
-------------------------------------------------------------------------------
*/
dj_plaintext_t* dj_plaintext_from_ui(unsigned int i);










/*
Encryption.


returns: a pointer to the resulting plaintext


-------------------------------------------------------------------------------
arguments:
res:	the location to store the result; a new dj_ciphertext_t will be
		allocated if this is null

pub:	the public key to encrypt with

pt:		the plaintext to encrypt

get_rand: You most likely want to just pass dj_get_rand_devurandom
-------------------------------------------------------------------------------
*/
dj_ciphertext_t* dj_encrypt(dj_ciphertext_t* res,
							dj_pubkey_t* pub,
							dj_plaintext_t* pt,
							dj_get_rand_t get_rand);









/*
Uses the cryptosystem's homomorphic property to add the contents of
two ciphertexts, and puts the result in a third ciphertext.


returns: a pointer to the resulting ciphertext


-------------------------------------------------------------------------------
arguments:
res:	the location to store the result; a new dj_ciphertext_t will be
		allocated if this is null

c1, c2:	the ciphertexts whose contents will be added

pub:	the public key that encrypted c1 and c2
-------------------------------------------------------------------------------
*/
dj_ciphertext_t* dj_homomorphic_add(dj_ciphertext_t* res,
									dj_ciphertext_t* c1,
									dj_ciphertext_t* c2,
									dj_pubkey_t* pub);










/*
Uses the cryptosystem's homomorphic property to multiply the contents of
a ciphertext with a plaintext, and puts the result in a second ciphertext.


returns: a pointer to the resulting ciphertext


-------------------------------------------------------------------------------
arguments:
res:	the location to store the result; a new dj_ciphertext_t will be
		allocated if this is null

c1:	the ciphertexts whose contents will be multiplied

m1:	the plaintext to multiply c1 by

pub: the public key that encrypted c1
-------------------------------------------------------------------------------
*/
dj_ciphertext_t* dj_homomorphic_mul(dj_ciphertext_t* res,
									dj_ciphertext_t* c1,
									dj_plaintext_t* m1,
									dj_pubkey_t* pub);








/*
(Non-threshold) decryption.


returns: a pointer to the resulting plaintext


-------------------------------------------------------------------------------
arguments:
res:	the location to store the result; a new dj_plaintext_t will be
		allocated if this is null

pub:	the public key that encrypted the ciphertext

prv:	the private key that can decrypt the ciphertext

ct:		the ciphertext
-------------------------------------------------------------------------------
*/
dj_plaintext_t* dj_decrypt(	dj_plaintext_t* res,
							dj_pubkey_t* pub,
							dj_prvkey_t* prv,
							dj_ciphertext_t* ct);








/*
Combines decryption shares (if given enough) to obtain the message.


returns: a pointer to the resulting plaintext


-------------------------------------------------------------------------------
arguments:
res:	the location to store the result; a new dj_plaintext_t will be
		allocated if this is null

pub:	the public key that encrypted the ciphertext that these decryption
		shares are from

nSharesPassed:	the number of shares being passed into the function (i.e. the
				size of the passedshares array), not the threshold. If there
				aren't enough shares, the function will let you know.

passedshares: an array of decryption shares to be combined.
-------------------------------------------------------------------------------
*/
dj_plaintext_t* dj_combine_shares(dj_plaintext_t* res,
								  dj_pubkey_t* pub,
								  unsigned short int nSharesPassed,
								  dj_decshare_t** passedshares);









/*
Using a partial private key, generates that key's decryption share
for a ciphertext.


returns: a pointer to the resulting decryption share


-------------------------------------------------------------------------------
arguments:
res:	the location to store the result; a new dj_decshare_t will be 
		allocated if this is null
		
pub: the public key corresponding to prv

prv: the partial private key to decrypt with

ct:	the ciphertext to generate a partial decryption for
-------------------------------------------------------------------------------
*/
dj_decshare_t* dj_partial_decrypt(dj_decshare_t* res,
								  dj_pubkey_t* pub,
								  dj_partial_prvkey_t* prv,
								  dj_ciphertext_t* ct);










/*
Sets the 's' parameter of a public key: for a public key with modulus n,
and currently using parameter s, the plaintext space is Z_{n^s}, and the
ciphertext space is Z_{n^{s+1}}.


-------------------------------------------------------------------------------
arguments:
pub: the public key

newS: the desired value of the D-J system's 's' parameter
-------------------------------------------------------------------------------
*/
void dj_set_s(dj_pubkey_t* pub,
			  unsigned short int newS);






/********
 CLEANUP
********/

/*
  These free the structures allocated and returned by various
  functions within the library and should be used when the structures are
  no longer needed.
*/
void dj_freepubkey(dj_pubkey_t* pub);
void dj_freeprvkey(dj_prvkey_t* prv);
void dj_freeplaintext(dj_plaintext_t* pt);
void dj_freeciphertext(dj_ciphertext_t* ct);
void dj_free_partial_prvkey(dj_partial_prvkey_t* keytofree);
void dj_free_decshare(dj_decshare_t* sharetofree);
