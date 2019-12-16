#include <stdio.h>
#include <string.h>
#include "grain.h"
 
void init(void){}
/*
 * Function: grain_keystream
 *
 * Synopsis
 *  Generates a new bit and updates the internal state of the cipher.
 */
 
int grain_keystream(grain* mygrain) {
	int i,NBit,LBit,outbit;
	/* Calculate feedback and output bits */
	outbit = N(79)^N(78)^N(76)^N(70)^N(49)^N(37)^N(24) ^ X1 ^ X4 ^ X0&X3 ^ X2&X3 ^ X3&X4 ^ X0&X1&X2 ^ X0&X2&X3 ^ X0&X2&X4 ^ X1&X2&X4 ^ X2&X3&X4;
 
	NBit=L(80)^N(18)^N(20)^N(28)^N(35)^N(43)^N(47)^N(52)^N(59)^N(66)^N(71)^N(80)^
			N(17)&N(20) ^ N(43)&N(47) ^ N(65)&N(71) ^ N(20)&N(28)&N(35) ^ N(47)&N(52)&N(59) ^ N(17)&N(35)&N(52)&N(71)^
			N(20)&N(28)&N(43)&N(47) ^ N(17)&N(20)&N(59)&N(65) ^ N(17)&N(20)&N(28)&N(35)&N(43) ^ N(47)&N(52)&N(59)&N(65)&N(71)^
			N(28)&N(35)&N(43)&N(47)&N(52)&N(59);
	LBit=L(18)^L(29)^L(42)^L(57)^L(67)^L(80);
	/* Update registers */
	for (i=1;i<(mygrain->keysize);++i) {
		mygrain->NFSR[i-1]=mygrain->NFSR[i];
		mygrain->LFSR[i-1]=mygrain->LFSR[i];
	}
	mygrain->NFSR[(mygrain->keysize)-1]=NBit;
	mygrain->LFSR[(mygrain->keysize)-1]=LBit;
	return outbit;	
}
 
void keysetup(
  grain* mygrain, 
  const int* key, 
  int keysize,                /* Key size in bits. */ 
  int ivsize)				  /* IV size in bits. */ 
{
	mygrain->p_key=key;
	mygrain->keysize=keysize;
	mygrain->ivsize=ivsize;
}
 
/*
 * Function: ivsetup
 *
 * Synopsis
 *  Load the key and perform initial clockings.
 *
 * Assumptions
 *  The key is 10 bytes and the IV is 8 bytes. The
 *  registers are loaded in the following way:
 *  
 *  NFSR[0] = lsb of key[0]
 *  ...
 *  NFSR[7] = msb of key[0]
 *  ...
 *  ...
 *  NFSR[72] = lsb of key[9]
 *  ...
 *  NFSR[79] = msb of key[9]
 *  LFSR[0] = lsb of IV[0]
 *  ...
 *  LFSR[7] = msb of IV[0]
 *  ...
 *  ...
 *  LFSR[56] = lsb of IV[7]
 *  ...
 *  LFSR[63] = msb of IV[7]
 */
void ivsetup(
  grain* mygrain, 
  const int* iv)
{
	int i,j;
	int outbit;
	/* load registers */
	for (i=0;i<(mygrain->ivsize)/8;++i) {
		for (j=0;j<8;++j) {
			mygrain->NFSR[i*8+j]=((mygrain->p_key[i]>>j)&1);  
			mygrain->LFSR[i*8+j]=((iv[i]>>j)&1);
		}
	}
	for (i=(mygrain->ivsize)/8;i<(mygrain->keysize)/8;++i) {
		for (j=0;j<8;++j) {
			mygrain->NFSR[i*8+j]=((mygrain->p_key[i]>>j)&1);
			mygrain->LFSR[i*8+j]=1;
		}
	}
	/* do initial clockings */
	for (i=0;i<INITCLOCKS;++i) {
		outbit=grain_keystream(mygrain);
		mygrain->LFSR[79]^=outbit;
		mygrain->NFSR[79]^=outbit;             
	}
}
 
/*
 * Function: keystream_bytes
 *
 * Synopsis
 *  Generate keystream in bytes.
 *
 * Assumptions
 *  Bits are generated in order z0,z1,z2,...
 *  The bits are stored in a byte in order:
 *  
 *  lsb of keystream[0] = z0
 *  ...
 *  msb of keystream[0] = z7
 *  ...
 *  lsb of keystream[1] = z8
 *  ...
 *  msb of keystream[1] = z15
 *  ...
 *  ...
 *  ...
 *  Example: The bit keystream: 10011100 10110011 ..
 *  corresponds to the byte keystream: 39 cd ..
 */
void keystream_bytes(
  grain* mygrain, 
  int* keystream, 
  int msglen)
{
	int i,j;
	for (i = 0; i < msglen; ++i) {
		keystream[i]=0;
		for (j = 0; j < 8; ++j) {
			keystream[i]|=(grain_keystream(mygrain)<<j);
		}
	}
}
void encrypt_bytes(
  grain* mygrain, 
  const int* plaintext, 
  int* ciphertext, 
  int msglen)
{
	int i,j;
	int k;
	for (i = 0; i < msglen; ++i) {
		k=0;
		for (j = 0; j < 8; ++j) {	
			k|=(grain_keystream(mygrain)<<j);
		}
		ciphertext[i]=plaintext[i]^k;
	}
}
 
void decrypt_bytes(
  grain* mygrain, 
  const int* ciphertext, 
  int* plaintext, 
  int msglen)
{
	int i,j;
	int k=0;
	for (i = 0; i < msglen; ++i) {
		k=0;
		for (j = 0; j < 8; ++j) {
			k|=(grain_keystream(mygrain)<<j);
		}
		plaintext[i]=ciphertext[i]^k;
	}
}
 
 
/*  GENERATE TEST VECTORS  */
 
void printData(int *key, int *IV, int *ks, int *pt, int *et, int *dt, int sizeOfPlaintext) {
	int i;
	printf("key:        ");
	for (i=0;i<10;++i) printf("%02x",(int)key[i]);
	printf("\nIV :        ");
	for (i=0;i<8;++i) printf("%02x",(int)IV[i]);
	printf("\nkeystream:  ");
	for (i=0;i<10;++i) printf("%02x",(int)ks[i]);
	printf("\nplaintext:  ");
	for (i=0;i<sizeOfPlaintext;i++) printf("%02x",(int)pt[i]);
	printf("\nencrypted text:  ");
	for (i=0;i<sizeOfPlaintext;i++) printf("%02x",(int)et[i]);
	printf("\ndecrypted text:  ");
	for (i=0;i<sizeOfPlaintext;i++) printf("%02x",(int)dt[i]);
}
 
int main(int argc, char **argv) {
 
	int plaintext[10]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
	int plaintext2[512]; //fill this to encrypt
	int sizeOfPlaintext=10; //fill size here
	int encrypted_text[10];
	int decrypted_text[10];
 
	grain mygrain;
	int key1[10] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
		IV1[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	    ks[10];
 
	int key2[10] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0x12,0x34},
		IV2[8] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
 
	int key3[10] = {0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
		IV3[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
 
	printf("\nTestCase: 1\n");
	keysetup(&mygrain,key1,80,64);
	ivsetup(&mygrain,IV1);
	keystream_bytes(&mygrain,ks,10);
	grain mygrain2 = mygrain;
	encrypt_bytes(&mygrain,plaintext,encrypted_text,10);
	decrypt_bytes(&mygrain2,encrypted_text,decrypted_text,10);
	printData(key1,IV1,ks, plaintext2, encrypted_text, decrypted_text, 10);
 
 
	printf("\n\nTestCase: 2\n");
	keysetup(&mygrain,key2,80,64);
	ivsetup(&mygrain,IV2);
	keystream_bytes(&mygrain,ks,10);
	mygrain2 = mygrain;
	encrypt_bytes(&mygrain,plaintext,encrypted_text,10);
	decrypt_bytes(&mygrain2,encrypted_text,decrypted_text,10);
	printData(key2,IV2,ks, plaintext, encrypted_text, decrypted_text, 10);
 
	printf("\n");
 
	printf("\n\nTestCase: 3\n");
	keysetup(&mygrain,key3,80,64);
	ivsetup(&mygrain,IV3);
	keystream_bytes(&mygrain,ks,10);
	mygrain2 = mygrain;
	encrypt_bytes(&mygrain,plaintext2,encrypted_text,sizeOfPlaintext);
	decrypt_bytes(&mygrain2,encrypted_text,decrypted_text,sizeOfPlaintext);
	printData(key3,IV3,ks, plaintext2, encrypted_text, decrypted_text, sizeOfPlaintext);
 
	printf("\n");
 
	return 0;
}