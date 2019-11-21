#include <stdio.h>
#include <string.h>

void icepole_main();

int main() {


	volatile uint32_t count;
	// addresses of registers
	volatile uint32_t *DWT_CONTROL = (uint32_t *)0xE0001000;
	volatile uint32_t *DWT_CYCCNT = (uint32_t *)0xE0001004;
	volatile uint32_t *DEMCR = (uint32_t *)0xE000EDFC;
	     // enable the use DWT
	     *DEMCR = *DEMCR | 0x01000000;
	     count=0;
	     // Reset cycle counter
	     *DWT_CYCCNT = 0;
	     // enable cycle counter
	     *DWT_CONTROL = *DWT_CONTROL | 1 ;




/*----------------------------------------- USER INPUT -----------------------------------------------------*/
	     char message[] = "This is a test message for ICEPOLE testing";


	     // Copy paste the cipher text below if already having the cipher text and change executionMode = 1 (i.e. Decryption only)
	     //Else it the encrypted text of the plain message will be used as a cipher text automatically.
	     char cipherText[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

	     char key[] = "\x41\x4e\x45\x57\x4b\x45\x59\x46\x4f\x52\x54\x45\x53\x54\x49\x4e";
	     char nonce[] = "\x63\x73\x6f\x6d\x65\x67\x68\x65\x78\x0d\x0a\x69\x5A\x4B\x1E\x0F";
		 char secret[] = "\x74\x65\x73\x74\x69\x6e\x67\x63\x69\x70\x68\x65\x72\xB4\x86\x57";
		 char ad[] = "\xbb\x70\x50\x86\x3b\x50\x40\x89\xAB\xCD\xEF\xF0\xE1\xD2\xC3\xC5";
		 int adlen = strlen(ad)+1;
		 int mlen = strlen(message);
		 int clen = 0;

		 int executionMode = 2;			 // 0 : Encryption only
		 	 	 	 	 	 // 1 : Decryption only
		 	 	 	 	 	 // 2 : Both Encryption and Decryption

/*----------------------------------------- USER INPUT -----------------------------------------------------*/




		char decryptedMessage[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
 		char tag[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";



		 if (executionMode==0) {
			   	char *encryptionParameters[] = {key, message, nonce,	ad, secret, mlen, adlen};
			    icepole_main(encryptionParameters, cipherText, 0);
			 }
		 else if (executionMode==1) {
			    clen = strlen(cipherText);
			    char *decryptionParameters[] = {key, cipherText, nonce, ad, secret, clen, adlen};
			   	icepole_main(decryptionParameters, decryptedMessage, 1, tag);
		}
		else if (executionMode==2) {
				char *encryptionParameters[] = {key, message, nonce,	ad, secret, mlen, adlen};
				icepole_main(encryptionParameters, cipherText, 0);
				clen = strlen(cipherText);
				char *decryptionParameters[] = {key, cipherText, nonce, ad, secret, clen, adlen};
				icepole_main(decryptionParameters, decryptedMessage, 1, tag);
		}



	     // number of cycles stored in count variable
	     count = *DWT_CYCCNT;
	     //you can view the count variable while debugging by selecting
	     //View Menu -> "variables" tab

return 0;
}




