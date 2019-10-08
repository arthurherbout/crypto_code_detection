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



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gmp.h"
#include "damgard_jurik.h"


//The following functions are taken directly from or slightly modified from libpaillier

void init_rand( gmp_randstate_t rand, dj_get_rand_t get_rand, int bytes )
{
	void* buf;
	mpz_t s;

	buf = malloc(bytes);
	get_rand(buf, bytes);

	gmp_randinit_default(rand);
	mpz_init(s);
	mpz_import(s, bytes, 1, 1, 0, 0, buf);
	gmp_randseed(rand, s);
	mpz_clear(s);

	free(buf);
}

void complete_pubkey(dj_pubkey_t* pub)
{
	int i;
	mpz_add_ui(pub->n_plusone, pub->npow[1], 1);
	mpz_set_ui(pub->npow[0],1);
	for(i=2;i<pub->s+2;i++)
		mpz_mul(pub->npow[i],pub->npow[i-1],pub->npow[1]);
}

dj_plaintext_t* dj_plaintext_from_ui(unsigned int i)
{
	dj_plaintext_t* ret = (dj_plaintext_t*) malloc(sizeof(dj_plaintext_t));
	mpz_init_set_ui(ret->m,i);
	return ret;
}

void dj_freepubkey(dj_pubkey_t* pub)
{
	int i;
	for(i=0;i<pub->s+2;i++)
		mpz_clear(pub->npow[i]);
	free(pub->npow);
	mpz_clear(pub->n_plusone);
	free(pub);
}

void dj_freeprvkey(dj_prvkey_t* prv)
{
	mpz_clear(prv->key);
	free(prv);
}

void dj_freeplaintext(dj_plaintext_t* pt)
{
	mpz_clear(pt->m);
	free(pt);
}

void dj_freeciphertext(dj_ciphertext_t* ct)
{
	mpz_clear(ct->c);
	free(ct);
}

void dj_free_partial_prvkey(dj_partial_prvkey_t* keytofree)
{
	mpz_clear(keytofree->s_i);
	free(keytofree);
}

void dj_free_decshare(dj_decshare_t* sharetofree)
{
	mpz_clear(sharetofree->c_i);
	free(sharetofree);
}

void dj_get_rand_file( void* buf, int len, char* file )
{
	FILE* fp;
	void* p;

	fp = fopen(file, "r");

	p = buf;
	while( len )
	{
		size_t s;
		s = fread(p, 1, len, fp);
		p += s;
		len -= s;
	}

	fclose(fp);
}

void dj_get_rand_devrandom( void* buf, int len )
{
	dj_get_rand_file(buf, len, "/dev/random");
}

void dj_get_rand_devurandom( void* buf, int len )
{
	dj_get_rand_file(buf, len, "/dev/urandom");
}


/*************Start of new code******************/

dj_ciphertext_t* dj_homomorphic_add(dj_ciphertext_t* res, dj_ciphertext_t* c1, dj_ciphertext_t* c2, dj_pubkey_t* pub)
{
	if(!res)
	{
		res = (dj_ciphertext_t*) malloc(sizeof(dj_ciphertext_t));
		mpz_init(res->c);
	}
	mpz_mul(res->c,c1->c,c2->c);
	mpz_mod(res->c,res->c,pub->npow[pub->s+1]);
	return res;
}

dj_ciphertext_t* dj_homomorphic_mul(dj_ciphertext_t* res, dj_ciphertext_t* c1, dj_plaintext_t* m1, dj_pubkey_t* pub)
{
	if(!res)
	{
		res = (dj_ciphertext_t*) malloc(sizeof(dj_ciphertext_t));
		mpz_init(res->c);
	}
	mpz_powm(res->c, c1->c, m1->m, pub->npow[pub->s+1]);
	return res;
}

//(this one is almost identical to the original, except that r is picked from Z*_{n^{s+1}} instead of Z_n)
dj_ciphertext_t* dj_encrypt(dj_ciphertext_t* res, dj_pubkey_t* pub, dj_plaintext_t* pt, dj_get_rand_t get_rand)
{
	mpz_t r,temp;
	gmp_randstate_t rand;
	
	if(!res)
	{
		res = (dj_ciphertext_t*) malloc(sizeof(dj_ciphertext_t));
		mpz_init(res->c);
	}
	
	mpz_init(r);
	mpz_init(temp);
	
	init_rand(rand,get_rand, (pub->bits / 8) * pub->s + 1);
	
	//pick a random r from Z*_{n^{s+1}}
	do
	{
		do mpz_urandomm(r,rand,pub->npow[pub->s+1]); while (mpz_cmp_ui(r,0) == 0);
		//ensure our r is in Z*_{n^{s+1}}, i.e., r and n^{s+1} are coprime
		mpz_gcd(temp,r,pub->npow[pub->s+1]);
	} while(mpz_cmp_ui(temp,1) != 0);
	
	
	//use g = n+1 as suggested in the DJ paper
	mpz_powm(r,r,pub->npow[pub->s],pub->npow[pub->s+1]);
	mpz_powm(temp,pub->n_plusone,pt->m,pub->npow[pub->s+1]);
	mpz_mul(res->c,r,temp);
	mpz_mod(res->c,res->c,pub->npow[pub->s+1]);
	
	mpz_clear(r);
	mpz_clear(temp);
	
	gmp_randclear(rand);
	return res;
}

dj_plaintext_t* dj_decrypt(dj_plaintext_t* res,	dj_pubkey_t* pub, dj_prvkey_t* prv,	dj_ciphertext_t* ct)
{
	int i,j,k;
	mpz_t cprime, temp, tempDivisor, i_jCur, i_jPrev;

	if(!res)
	{
		res = (dj_plaintext_t*) malloc(sizeof(dj_plaintext_t));
		mpz_init(res->m);
	}
	
	mpz_init(cprime);
	
	//cprime = c^key mod n^{s+1}
	mpz_powm(cprime,ct->c,prv->key,pub->npow[pub->s+1]);
	
	
	
	//the algorithm from extracting i from (1+n)^i described in the paper.
	
	//the algorithm iteratively extracts i mod n, i mod n^2, etc. i mod n starts us off, and is just input - 1 / n.
	mpz_init(temp);
	mpz_init(tempDivisor);
	mpz_init(i_jCur);
	mpz_init_set(i_jPrev,cprime);
	mpz_sub_ui(i_jPrev,i_jPrev,1);
	mpz_divexact(i_jPrev,i_jPrev,pub->npow[1]);
	mpz_mod(i_jPrev,i_jPrev,pub->npow[2]);
	
	//just in case s=1; in that case we need this line to actually set i_jcur
	if(pub->s==1)
		mpz_set(i_jCur,i_jPrev);
	
	//extract i_j = i mod n^j given i_{j-1}. this is done by taking (input - 1 mod n^{j+1}) / n , and subtracting
	//from that: ((  (i_{j-1} choose 2)*n  + (i_{j-1} choose 3)*n^2  + ... +  (i_{j-1} choose j)*n^{j-1}  ))   mod n^j
	for(j=2;j<=pub->s;j++)
	{
		//L((1+n)^i) as they call it 
		mpz_mod(i_jCur,cprime,pub->npow[j+1]);
		mpz_sub_ui(i_jCur,i_jCur,1);
		mpz_divexact(i_jCur,i_jCur,pub->npow[1]);
		
		mpz_mod(i_jCur,i_jCur,pub->npow[j]);
		
		//subtract each of the binomial things
		for(k=2;k<=j;k++)
		{
			mpz_bin_ui(temp,i_jPrev,k);
			mpz_mul(temp,temp,pub->npow[k-1]);
			mpz_mod(temp,temp,pub->npow[j]);
			mpz_sub(i_jCur,i_jCur,temp);
			mpz_mod(i_jCur,i_jCur,pub->npow[j]);
		}
		mpz_set(i_jPrev,i_jCur);
	}
	
	//i_jCur is currently the message times the private key.
	mpz_invert(temp, prv->key, pub->npow[pub->s]);
	mpz_mul(res->m, i_jCur, temp);
	mpz_mod(res->m, res->m, pub->npow[pub->s]);
	
	//cleanup and return
	mpz_clear(cprime);
	mpz_clear(i_jPrev);
	mpz_clear(i_jCur);
	mpz_clear(temp);
	mpz_clear(tempDivisor);
	
	return res;
}


dj_prvkey_t* dj_keygen(dj_prvkey_t* reskey, dj_pubkey_t** resPub, unsigned short int s, int modulusbits, dj_get_rand_t get_rand)
{
	mpz_t p,q,pprime,qprime,m,minv,n,mnpows,temp;
	
	gmp_randstate_t rand;
	
	int i,j,x;
	
	if(!reskey)
	{
		reskey = (dj_prvkey_t*) malloc(sizeof(dj_prvkey_t));
		mpz_init(reskey->key);
	}
	
	//initialization
	mpz_init(p);
	mpz_init(q);
	mpz_init(pprime);
	mpz_init(qprime);
	mpz_init(m);
	mpz_init(minv);
	mpz_init(mnpows);
	mpz_init(temp);
	
	(*resPub) = (dj_pubkey_t*) malloc(sizeof(dj_pubkey_t));
	mpz_init((*resPub)->n_plusone);
	(*resPub)->npow = (mpz_t*) malloc((s+2)*sizeof(mpz_t));
	for(i=0;i<s+2;i++)
		mpz_init((*resPub)->npow[i]);
	
	init_rand(rand, get_rand, modulusbits / 8 + 1);
	
	//first, pick our secret primes: p and q that are of the form p=2p'+1, q=2q'+1, for p' and q' also prime
	//public key is n = pq
	do
	{
		do
		{
			mpz_urandomb(p,rand,modulusbits / 2);
			mpz_sub_ui(pprime, p, 1);
			mpz_divexact_ui(pprime, pprime, 2);
		} while(!mpz_probab_prime_p(p, 10) || !mpz_probab_prime_p(pprime, 10));

		do
		{
			mpz_urandomb(q,rand,modulusbits / 2);
			mpz_sub_ui(qprime, q, 1);
			mpz_divexact_ui(qprime, qprime, 2);
		} while(!mpz_probab_prime_p(q, 10) || !mpz_probab_prime_p(qprime, 10));

		/* compute the public modulus n = p q */
		mpz_mul((*resPub)->npow[1],p,q);

	} while(!mpz_tstbit((*resPub)->npow[1],modulusbits - 1) || mpz_cmp(p, q) == 0 || mpz_cmp(pprime, q) == 0 || mpz_cmp(p, qprime) == 0);
	(*resPub)->s=s;
	complete_pubkey(*resPub);
	(*resPub)->bits = modulusbits;
	
	
	//now, generate the actual secret key: just lcm(p-1, q-1)
	mpz_sub_ui(p,p,1);
	mpz_sub_ui(q,q,1);
	mpz_lcm(reskey->key, p, q);
	
	
	//cleanup and return
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(pprime);
	mpz_clear(qprime);
	mpz_clear(m);
	mpz_clear(minv);
	mpz_clear(mnpows);
	mpz_clear(temp);
	
	gmp_randclear(rand);
	
	return reskey;
}


void dj_thresh_centralized_keygen(	dj_partial_prvkey_t** keyshares,
									dj_pubkey_t** resPub,
									unsigned short int k,
									unsigned short int l,
									unsigned short int s,
									int modulusbits,
									dj_get_rand_t get_rand)
{
	mpz_t p,q,pprime,qprime,m,minv,n,mnpows,temp,computeDelta;
	mpz_t a[k];
	
	gmp_randstate_t rand;
	
	int i,j,x;
	
	//initialization
	mpz_init(p);
	mpz_init(q);
	mpz_init(pprime);
	mpz_init(qprime);
	mpz_init(m);
	mpz_init(minv);
	mpz_init(mnpows);
	mpz_init(temp);
	for(i=0;i<k;i++)
		mpz_init(a[i]);
	
	(*resPub) = (dj_pubkey_t*) malloc(sizeof(dj_pubkey_t));
	mpz_init((*resPub)->n_plusone);
	(*resPub)->npow = (mpz_t*) malloc((s+2)*sizeof(mpz_t));
	for(i=0;i<s+2;i++)
		mpz_init((*resPub)->npow[i]);

	mpz_init_set_ui(computeDelta, l);
	for(i=l-1;i>1;i--)
		mpz_mul_ui(computeDelta, computeDelta, i);
	
	for(i=0;i<l;i++)
	{
		keyshares[i] = (dj_partial_prvkey_t*) malloc(sizeof(dj_partial_prvkey_t));
		mpz_init(keyshares[i]->s_i);
		mpz_init_set(keyshares[i]->delta, computeDelta);
	}
	
	mpz_clear(computeDelta);
	
	init_rand(rand, get_rand, modulusbits / 8 + 1);
	
	
	//first, pick our secret primes: p and q that are of the form p=2p'+1, q=2q'+1, for p' and q' also prime
	//public key is n = pq
	do
	{
		do
		{
			mpz_urandomb(p,rand,modulusbits / 2);
			mpz_sub_ui(pprime, p, 1);
			mpz_divexact_ui(pprime, pprime, 2);
		} while(!mpz_probab_prime_p(p, 10) || !mpz_probab_prime_p(pprime, 10));

		do
		{
			mpz_urandomb(q,rand,modulusbits / 2);
			mpz_sub_ui(qprime, q, 1);
			mpz_divexact_ui(qprime, qprime, 2);
		} while(!mpz_probab_prime_p(q, 10) || !mpz_probab_prime_p(qprime, 10));

		/* compute the public modulus n = p q */
		mpz_mul((*resPub)->npow[1],p,q);

	} while(!mpz_tstbit((*resPub)->npow[1],modulusbits - 1) || mpz_cmp(p, q) == 0 || mpz_cmp(pprime, q) == 0 || mpz_cmp(p, qprime) == 0);
	(*resPub)->s=s;
	complete_pubkey(*resPub);
	(*resPub)->bits = modulusbits;
	
	
	mpz_mul(m,pprime,qprime);
	mpz_mul(mnpows,m,(*resPub)->npow[s]);
	
	
	//now, generate the secret partial keys.
	//generate a polynomial (mod mn^s) with coeffs a_0 to a_{k-1}.
	//a_0 should be 0 mod m, 1 mod n^s, so set a_0 = (m^-1 mod n^s)(m).
	//the other a_i are random from 0 to (mn^s)-1.
	
	//key i (i starts at 1) is that polynomial evaluated at i.
	
	mpz_invert(minv,m,(*resPub)->npow[s]);
	mpz_mul(a[0],minv,m);
	
	for(i=1;i<k;i++)
		mpz_urandomm(a[i],rand,mnpows);
	
	//generate l keys as requested. note that i starts at 1, in order to represent the i used in the paper.
	for(i=1;i<=l;i++)
	{
		keyshares[i-1]->i=i;
		keyshares[i-1]->numShares=l;
		keyshares[i-1]->threshold=k;
		
		mpz_set(keyshares[i-1]->s_i,a[0]);
		for(j=1;j<k;j++)
		{
			mpz_ui_pow_ui(temp,i,j);
			mpz_mul(temp,temp,a[j]);
			mpz_add(keyshares[i-1]->s_i,keyshares[i-1]->s_i,temp);
			mpz_mod(keyshares[i-1]->s_i,keyshares[i-1]->s_i,mnpows);
		}
	}
	
	//cleanup and return
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(pprime);
	mpz_clear(qprime);
	mpz_clear(m);
	mpz_clear(minv);
	mpz_clear(mnpows);
	mpz_clear(temp);
	for(i=0;i<k;i++)
		mpz_clear(a[i]);
	
	gmp_randclear(rand);
}



dj_decshare_t* dj_partial_decrypt(dj_decshare_t* res, dj_pubkey_t* pub, dj_partial_prvkey_t* prv, dj_ciphertext_t* ct)
{
	int i;

	if(!res)
	{
		res = (dj_decshare_t*) malloc(sizeof(dj_decshare_t));
		mpz_init(res->c_i);
		mpz_init(res->delta);
	}
	
	res->i = prv->i;
	res->threshold = prv->threshold;
	res->numShares = prv->numShares;
	mpz_set(res->delta,prv->delta);
	
	//c_i = c^{2 delta s_i} mod n^{s+1}
	mpz_mul_ui(res->c_i,res->delta,2);
	mpz_mul(res->c_i,res->c_i,prv->s_i);
	mpz_powm(res->c_i,ct->c,res->c_i,pub->npow[pub->s+1]);
	
	return res;
}


dj_plaintext_t* dj_combine_shares(dj_plaintext_t* res, dj_pubkey_t* pub, unsigned short int nSharesPassed, dj_decshare_t** passedshares)
{
	int i,j,k;
	unsigned short int sharesPassed=nSharesPassed;
	dj_decshare_t* shares[sharesPassed];
	mpz_t twoLambdaS0[sharesPassed];
	mpz_t cprime, temp, tempDivisor, i_jCur, i_jPrev;
	int dupCount=0;
	
	if(!res)
	{
		res = (dj_plaintext_t*) malloc(sizeof(dj_plaintext_t));
		mpz_init(res->m);
	}
	
	//first, delete duplicate shares
	for(i=0;i<sharesPassed;i++)
		shares[i]=passedshares[i];
	
	for(i=0;i<sharesPassed;i++)
		for(j=0;j<sharesPassed;j++)
			if(i!=j && shares[i]->i == shares[j]->i)
			{
				int delthis = i>j?i:j;
				for(k=delthis;k<sharesPassed-1;k++)
					shares[k]=shares[k+1];
				shares[nSharesPassed-1] = 0;
				sharesPassed--;
				dupCount++;
			}
	
	//now that duplicates are gone, decryption is doable iff thresh <= # shares we were given
	if(shares[0]->threshold > sharesPassed)
	{
		printf("Error! The shares passed to dj_combine_shares have a threshold of %d, but only %d were passed!\n",
				shares[0]->threshold, sharesPassed);
		if(dupCount>0)
			printf("Furthermore, %d of the shares were duplicates of others, and so were discarded before checking the # of shares.\n",dupCount);
		return 0;
	}
	
	for(i=0;i<sharesPassed;i++)
		mpz_init(twoLambdaS0[i]);
	mpz_init(cprime);
	mpz_init(temp);
	mpz_init(tempDivisor);
	
	//here i is just an index, unlike in the keygen function. Instead, it's the i member of the dec share structure
	//that corresponds to the i in the paper.
	//generate the exponents that each decryption share will be raised to
	for(i=0;i<sharesPassed;i++)
	{
		mpz_set(twoLambdaS0[i],passedshares[0]->delta);
		mpz_mul_ui(twoLambdaS0[i],twoLambdaS0[i],2);
		
		for(j=0;j<sharesPassed;j++)
			if(i!=j)
			{
				mpz_mul_si(twoLambdaS0[i],twoLambdaS0[i],-shares[j]->i);
				mpz_set_si(tempDivisor,shares[i]->i - shares[j]->i);
				mpz_divexact(twoLambdaS0[i],twoLambdaS0[i],tempDivisor);
			}
	}
	
	//now raise the shares to the powers and multiply them
	mpz_set_ui(cprime,1);
	for(i=0;i<sharesPassed;i++)
	{
		mpz_powm(temp,shares[i]->c_i,twoLambdaS0[i],pub->npow[pub->s+1]);
		mpz_mul(cprime,cprime,temp);
		mpz_mod(cprime,cprime,pub->npow[pub->s+1]);
	}
	
	
	
	
	//the algorithm from extracting i from (1+n)^i described in the paper.
	
	//the algorithm iteratively extracts i mod n, i mod n^2, etc. i mod n starts us off, and is just input - 1 / n.
	mpz_init(i_jCur);
	mpz_init_set(i_jPrev,cprime);
	mpz_sub_ui(i_jPrev,i_jPrev,1);
	mpz_divexact(i_jPrev,i_jPrev,pub->npow[1]);
	mpz_mod(i_jPrev,i_jPrev,pub->npow[2]);
	
	//just in case s=1; in that case we need this line to actually set i_jcur
	if(pub->s==1)
		mpz_set(i_jCur,i_jPrev);
	
	//extract i_j = i mod n^j given i_{j-1}. this is done by taking (input - 1 mod n^{j+1}) / n , and subtracting
	//from that: ((  (i_{j-1} choose 2)*n  + (i_{j-1} choose 3)*n^2  + ... +  (i_{j-1} choose j)*n^{j-1}  ))   mod n^j
	for(j=2;j<=pub->s;j++)
	{
		//L((1+n)^i) as they call it 
		mpz_mod(i_jCur,cprime,pub->npow[j+1]);
		mpz_sub_ui(i_jCur,i_jCur,1);
		mpz_divexact(i_jCur,i_jCur,pub->npow[1]);
		
		mpz_mod(i_jCur,i_jCur,pub->npow[j]);
		
		//subtract each of the binomial things
		for(k=2;k<=j;k++)
		{
			mpz_bin_ui(temp,i_jPrev,k);
			mpz_mul(temp,temp,pub->npow[k-1]);
			mpz_mod(temp,temp,pub->npow[j]);
			mpz_sub(i_jCur,i_jCur,temp);
			mpz_mod(i_jCur,i_jCur,pub->npow[j]);
		}
		mpz_set(i_jPrev,i_jCur);
	}
	
	
	
	
	//now knock out the 4delta^2
	mpz_set_ui(temp,4);
	mpz_mul(temp,temp,passedshares[0]->delta);
	mpz_mul(temp,temp,passedshares[0]->delta);
	mpz_invert(temp,temp,pub->npow[pub->s]);
	
	mpz_mul(res->m,i_jCur,temp);
	mpz_mod(res->m,res->m,pub->npow[pub->s]);
	
	//cleanup and return
	for(i=0;i<sharesPassed;i++)
		mpz_clear(twoLambdaS0[i]);
	
	mpz_clear(cprime);
	mpz_clear(i_jPrev);
	mpz_clear(i_jCur);
	mpz_clear(temp);
	mpz_clear(tempDivisor);
	return res;
}


void dj_set_s(dj_pubkey_t* pub, unsigned short int newS)
{
	int i;
	
	if(newS == pub->s)
		return;
	else if(newS < pub->s)
	{
		for(i=newS+2;i<pub->s+2;i++)
			mpz_clear(pub->npow[i]);
		pub->npow = (mpz_t*)realloc(pub->npow, (newS+2)*sizeof(mpz_t));
	}
	else
	{
		pub->npow = (mpz_t*)realloc(pub->npow, (newS+2)*sizeof(mpz_t));
		for(i=pub->s+2;i<newS+2;i++)
		{
			mpz_init(pub->npow[i]);
			mpz_mul(pub->npow[i], pub->npow[i-1], pub->npow[1]);
		}
	}
	
	pub->s = newS;
}

